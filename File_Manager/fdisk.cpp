#include "fdisk.h"

Fdisk::Fdisk(int size, QString unit, QString path, QString type, QString fit, QString t_delete, QString name, int add)
{
    this->tamano = size;
    this->unidad = unit;
    this->path = path;
    this->tipo_particion = type;
    this->ajuste = fit;
    this->tipo_eliminacion = t_delete;
    this->nombre = name;
    this->agregar_espacio = add;
}

void Fdisk::Ejecutar()
{
    // Verificar si viene valores en ADD
    if (this->agregar_espacio != 0){

        this->Modificar_Espacio(this->agregar_espacio, this->unidad, this->nombre, this->path);
        // Si no viene ADD se verifica si viene un DELETE
    }else if (this->tipo_eliminacion != ""){

        this->Eliminar_Particion(this->tipo_eliminacion, this->path, this->nombre);
    }else{

        // De lo contrario se crea una nueva particion
        if (this->tipo_particion == "p") {

            this->Crear_Particion_Primaria();
        } else if(this->tipo_particion == "e"){

            this->Crear_Particion_Extendida();
        } else {

            this->Crear_Particion_Logica();
        }
    }

    this->show_Particiones(this->path);
}

void Fdisk::Modificar_Espacio(int add, QString unit, QString name, QString path)
{
    // Cantidad_add puede ser positivo o negativo
    int cantidad_add = add;
    QString tipo = "";

    if (cantidad_add > 0) {
        tipo = "add";
    }

    if (unit.toLower() == "m"){

        cantidad_add = add * 1048576;
    } else if (unit.toLower() == "k"){

        cantidad_add = add * 1024;
    }

    FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");

    if (disco_actual != NULL){

        MBR mbr_auxiliar;
        // Indice de la particion encotrada
        int index = -1;
        int index_Extendida = -1;
        bool is_Extendida = false;

        rewind(disco_actual);
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        /** Buscando el indice de la particion y si esta es E**/
        for (int i = 0; i < 4; i++){
            // Comparar si el nombre de la particion a buscar existe
            if (strcmp(mbr_auxiliar.mbr_partition[i].part_name, name.toStdString().c_str()) == 0){
                // Se guarda el indice de la particion a modificar
                index = i;
                // Ademas del indice se valida si esta es Extendida
                if(mbr_auxiliar.mbr_partition[i].part_type == 'E')
                    is_Extendida = true;
                break;

            } else if (mbr_auxiliar.mbr_partition[i].part_type == 'E') {
                // Se guarda solamente el indice de la E, siendo esta la que no se busca
                index_Extendida = i;
            }
        }

        /** La particion se encontro dentro de las 4 principales **/
        if(index != -1){

            if(tipo == "add"){//Agregar
                /** Verificar que exista espacio libre a la derecha **/
                // Validacion para las primeras 3 particiones
                if(index != 3){
                    int p1 = mbr_auxiliar.mbr_partition[index].part_start + mbr_auxiliar.mbr_partition[index].part_size;
                    int p2 = mbr_auxiliar.mbr_partition[index+1].part_start;

                    if((p2 - p1) != 0){//Hay fragmentacion (existe espacio entre donde termina una con donde empieza la otra)
                        int fragmentacion = p2-p1;
                        if(fragmentacion >= cantidad_add){
                            mbr_auxiliar.mbr_partition[index].part_size = mbr_auxiliar.mbr_partition[index].part_size + cantidad_add;
                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

                            cout << "Se agrego espacio a la particion de manera exitosa" << endl;
                        }else{
                            cout << "ERROR no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha" << endl;
                        }
                    }else{// Particion contigua no utilizada
                        if(mbr_auxiliar.mbr_partition[index + 1].part_status == '1'){
                            if(mbr_auxiliar.mbr_partition[index + 1].part_size >= cantidad_add){
                                mbr_auxiliar.mbr_partition[index].part_size = mbr_auxiliar.mbr_partition[index].part_size + cantidad_add;
                                mbr_auxiliar.mbr_partition[index + 1].part_size = (mbr_auxiliar.mbr_partition[index + 1].part_size - cantidad_add);
                                mbr_auxiliar.mbr_partition[index + 1].part_start = mbr_auxiliar.mbr_partition[index + 1].part_start + cantidad_add;

                                fseek(disco_actual, 0, SEEK_SET);
                                fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);
                                cout << "Se agrego espacio a la particion de manera exitosa" << endl;
                            }else{
                                cout << "ERROR no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha" << endl;
                            }
                        } else {// No hay espacio entre la particion actual y la contigua
                            cout<<"INFORMACION. Ya no es posible agregar mas espacio a la particion"<<endl;
                        }
                    }

                    // Validacion para la ultima (4ta) particion
                }else{
                    int p = mbr_auxiliar.mbr_partition[index].part_start + mbr_auxiliar.mbr_partition[index].part_size;
                    int total = mbr_auxiliar.mbr_size + (int)sizeof(MBR);

                    // Si al final de la ultima particion hay espacio entre ella y el disco
                    if((total-p) != 0){
                        int fragmentacion = total - p;
                        if(fragmentacion >= cantidad_add){
                            mbr_auxiliar.mbr_partition[index].part_size = mbr_auxiliar.mbr_partition[index].part_size + cantidad_add;

                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);
                            cout << "Se agrego espacio a la particion de manera exitosa" << endl;
                        }else{
                            cout << "ERROR no es posible agregar espacio a la particion porque no hay suficiente espacio disponible a su derecha" << endl;
                        }
                        // El disco esta lleno...
                    }else{
                        cout << "ERROR no es posible agregar espacio a la particion porque no hay espacio disponible a su derecha" << endl;
                    }
                }

            }else{// Quitar espacio
                /** Si la particion es una Primaria **/
                if(!is_Extendida){
                    // ERROR: La cantidad a quitar sobrepasa el tamaño mismo de la particion
                    if((cantidad_add * -1) >= mbr_auxiliar.mbr_partition[index].part_size){
                        cout << "ERROR no es posible quitarle esta cantidad de espacio a la particion porque la borraria" << endl;
                    }else{
                        // Se suma la cantidad pues el numero que trae en si es negativo
                        mbr_auxiliar.mbr_partition[index].part_size = mbr_auxiliar.mbr_partition[index].part_size + cantidad_add;
                        fseek(disco_actual, 0, SEEK_SET);
                        fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);
                        cout << "Se quito espacio a la particion de manera exitosa" << endl;
                    }
                    /** La particion encontrada es Extendida **/
                }else{

                    // ERROR: La cantidad a quitar sobrepasa el tamaño mismo de la particion
                    if((cantidad_add * -1) >= mbr_auxiliar.mbr_partition[index_Extendida].part_size){
                        cout << "ERROR. No es posible quitarle esta cantidad de espacio a la particion porque la borraria" << endl;
                    }else{
                        EBR extendedBoot;
                        fseek(disco_actual, mbr_auxiliar.mbr_partition[index_Extendida].part_start, SEEK_SET);
                        fread(&extendedBoot, sizeof(EBR), 1, disco_actual);

                        while((extendedBoot.part_next != -1) && (ftell(disco_actual) < (mbr_auxiliar.mbr_partition[index_Extendida].part_size + mbr_auxiliar.mbr_partition[index_Extendida].part_start))){
                            fseek(disco_actual, extendedBoot.part_next, SEEK_SET);
                            fread(&extendedBoot, sizeof(EBR), 1, disco_actual);
                        }

                        int ultimaLogica = extendedBoot.part_start + extendedBoot.part_size;
                        int aux = (mbr_auxiliar.mbr_partition[index_Extendida].part_start + mbr_auxiliar.mbr_partition[index_Extendida].part_size) + cantidad_add;
                        if(aux > ultimaLogica){//No toca ninguna logica
                            mbr_auxiliar.mbr_partition[index_Extendida].part_size = mbr_auxiliar.mbr_partition[index_Extendida].part_size + cantidad_add;
                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

                            cout << "Se quito espacio a la particion de manera exitosa" << endl;
                        }else{
                            cout << "ERROR. Si quita ese espacio dañaria una logica" << endl;
                        }
                    }
                }
            }

            // No se encontro entre las principales, se busca en las Logicas
        }else{
            if(index_Extendida != -1){

                int logica = buscarParticion_L(path, name);
                if(logica != -1){
                    if(tipo == "add"){
                        //Verificar que exista espacio libre a su derecha
                        /** NOTA: AGREGAR ESPACIO A UNA LOGICA ¡NO TERMINADO!**/
                        EBR extendedBoot;
                        fseek(disco_actual, logica, SEEK_SET);
                        fread(&extendedBoot, sizeof(EBR), 1, disco_actual);

                    }else{//Quitar
                        //Verificar que no la elimine
                        EBR extendedBoot;
                        fseek(disco_actual, logica, SEEK_SET);
                        fread(&extendedBoot, sizeof(EBR), 1, disco_actual);

                        if((cantidad_add * -1) >= extendedBoot.part_size){
                            cout << "ERROR si quita ese espacio eliminaria la logica" << endl;
                        }else{
                            extendedBoot.part_size = extendedBoot.part_size + cantidad_add;
                            fseek(disco_actual, logica, SEEK_SET);
                            fwrite(&extendedBoot, sizeof(EBR), 1, disco_actual);
                            cout << "Se quito espacio a la particion logica de manera exitosa" << endl;
                        }
                    }
                }else{
                    cout << "ERROR. No se encuentro la particion logica" << endl;
                }
            }else{
                cout << "ERROR. No se cuenta con particion Extendida para redimensionar" << endl;
            }
        }
        fclose(disco_actual);

    } else{

        cout<<"Error. El disco no existe"<<endl;
        return;
    }
}

/**
  * Funcion que devuelve el byte donde inicia la particion logica si esta existe
  * return -1, si no se encuentra la particion logica
**/
int Fdisk::buscarParticion_L(QString path, QString name)
{
    FILE *disco_actual;
    if((disco_actual = fopen(path.toStdString().c_str(),"rb+"))){

        int index_Extendida = -1;
        MBR masterboot;
        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);

        for(int i = 0; i < 4; i++){
            if(masterboot.mbr_partition[i].part_type == 'E'){
                index_Extendida = i;
                break;
            }
        }

        if(index_Extendida != -1){
            EBR extendedBoot;
            fseek(disco_actual, masterboot.mbr_partition[index_Extendida].part_start, SEEK_SET);
            while(fread(&extendedBoot, sizeof(EBR), 1, disco_actual) != 0 && (ftell(disco_actual) < masterboot.mbr_partition[index_Extendida].part_start + masterboot.mbr_partition[index_Extendida].part_size)){
                if(strcmp(extendedBoot.part_name, name.toStdString().c_str()) == 0){
                    return (ftell(disco_actual) - sizeof(EBR));
                }
            }
        }
        fclose(disco_actual);
    }
    return -1;
}

void Fdisk::Eliminar_Particion(QString t_delete, QString path, QString name)
{
    string eleccion;
    cout << "Confirmar eliminacion de particion: "<< this->path.toStdString() << " (Y/N)? \n";
    cin >> eleccion;

    // Se valida si se confirma la eliminacion
    if (eleccion == "Y" || eleccion == "y"){

        FILE *disco_actual;
        if((disco_actual = fopen(path.toStdString().c_str(), "rb+"))){

            MBR masterboot;
            fseek(disco_actual, 0, SEEK_SET);
            fread(&masterboot, sizeof (MBR), 1, disco_actual);

            int index = -1;
            int index_Extendida = -1;
            bool isExtendida = false;
            char buffer = '\0';

            // Buscamos la particion primaria/extendida
            for(int i = 0; i < 4; i++){
                if((strcmp(masterboot.mbr_partition[i].part_name, name.toStdString().c_str())) == 0){
                    index = i;
                    if(masterboot.mbr_partition[i].part_type == 'E')
                        isExtendida = true;
                    break;
                }else if(masterboot.mbr_partition[i].part_type == 'E'){
                    index_Extendida = i;
                }
            }

            /** La particion se encontro en las principales **/
            if(index != -1){

                // La eliminacion puede ser para una Primaria como para la Extendida
                if(t_delete == "full"){

                    // part_status = '1', inactiva pero ya ocupa espacio
                    masterboot.mbr_partition[index].part_status = '1';
                    strcpy(masterboot.mbr_partition[index].part_name, "");
                    fseek(disco_actual, 0, SEEK_SET);
                    fwrite(&masterboot, sizeof(MBR), 1, disco_actual);

                    fseek(disco_actual, masterboot.mbr_partition[index].part_start, SEEK_SET);
                    // Rellena el espacio con el caracter \0
                    fwrite(&buffer,1, masterboot.mbr_partition[index].part_size, disco_actual);

                }else{//fast
                    //masterboot.mbr_partition[index].part_status = '1';
                    //strcpy(masterboot.mbr_partition[index].part_name,"");
                    //fseek(disco_actual, 0, SEEK_SET);
                    //fwrite(&masterboot, sizeof(MBR), 1, disco_actual);
                }

                if (!isExtendida) {// Mensaje por eliminacion Primaria
                    cout << "Particion primaria eliminada con exito" << endl;
                } else { // Mensaje por eliminacion Extendida
                    cout << "Particion extendida eliminada con exito" << endl;
                }

                /** La particion es una posible Logica**/
            }else{

                if(index_Extendida != -1){
                    bool isLogica = false;//Bandera para saber si existe
                    EBR extendedBoot;

                    fseek(disco_actual, masterboot.mbr_partition[index_Extendida].part_start, SEEK_SET);
                    fread(&extendedBoot, sizeof(EBR), 1, disco_actual);

                    if(extendedBoot.part_size != 0){

                        fseek(disco_actual, masterboot.mbr_partition[index_Extendida].part_start, SEEK_SET);

                        while((fread(&extendedBoot, sizeof(EBR), 1, disco_actual)) != 0 && (ftell(disco_actual) < (masterboot.mbr_partition[index_Extendida].part_start + masterboot.mbr_partition[index_Extendida].part_size))) {
                            if(strcmp(extendedBoot.part_name, name.toStdString().c_str()) == 0 && extendedBoot.part_status != '1'){
                                isLogica = true;
                                break;
                            }else if(extendedBoot.part_next == -1){//Ya es la ultima y no se encontro
                                break;
                            }
                        }

                    }

                    // Se encontro una particion logica
                    if(isLogica){

                        if(t_delete == "fast"){
                            //extendedBoot.part_status = '1';
                            //strcpy(extendedBoot.part_name, "");
                            //fseek(disco_actual, ftell(disco_actual) - sizeof(EBR), SEEK_SET);
                            //fwrite(&extendedBoot, sizeof(EBR), 1, disco_actual);
                            //cout << "Particion logica eliminada con exito" << endl;

                        }else{// La opcion es full
                            extendedBoot.part_status = '1';
                            strcpy(extendedBoot.part_name, "");
                            fseek(disco_actual, ftell(disco_actual) - sizeof(EBR), SEEK_SET);
                            fwrite(&extendedBoot, sizeof(EBR), 1, disco_actual);
                            fwrite(&buffer, 1, extendedBoot.part_size, disco_actual);
                            cout << "Particion logica eliminada con exito" << endl;
                        }

                    }else{
                        cout << "ERROR. No se encuentra la particion Logica a eliminar" << endl;
                    }
                }else{
                    cout << "ERROR. No se cuenta con Extendida para eliminar Logica" << endl;
                }
            }

            fclose(disco_actual);
        }else{
            cout << "ERROR. El disco donde se va a eliminar no existe" << endl;
        }

    }else if (eleccion == "N" || eleccion == "n"){
        cout << "Particion no eliminada!" << endl;
    }else cout << "Opcion incorrecta, particion no eliminada!" << endl;
}

void Fdisk::Crear_Particion_Primaria()
{
    // Caracter en mayuscula para el fit de la particion
    char auxFit = this->ajuste.toUpper().toStdString()[0];
    string auxPath = this->path.toStdString();
    // Almacena el espacio que ocupara la particion
    int size_bytes = 1024;
    // Servira para llenar de 1's el espacio que ocupa cada particion en el disco
    char buffer = '1';

    if (this->unidad == "m"){

        size_bytes = this->tamano * 1048576;
    } else if (this->unidad == "k"){

        size_bytes = this->tamano * 1024;
    } else {
        // Tamaño en bytes
        size_bytes = this->tamano;
    }

    FILE *disco_actual;
    MBR masterboot;

    // Si el disco es abierto con exito, se empieza a verificar si se puede crear la particion
    if((disco_actual = fopen(auxPath.c_str(), "rb+"))){

        bool flagParticion = false;//Flag para ver si hay una particion disponible
        int numParticion = 0;//Que numero de particion es

        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);
        //Verificar si existe una particion disponible
        // Esta puede no estar ocupada o estar inactiva/vacia y que su tamaño sea el suficiente para almacenar la nueva particion
        for(int i = 0; i < 4; i++){
            if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                flagParticion = true;
                numParticion = i;
                break;
            }
        }

        // Si existe una particion disponible...
        if(flagParticion){
            //Verificar el espacio libre del disco
            int espacioUsado = 0;
            for(int i = 0; i < 4; i++){
                if(masterboot.mbr_partition[i].part_status != '1'){
                    espacioUsado += masterboot.mbr_partition[i].part_size;
                }
            }

            // Reporte del espacio necesario para crear la particion
            cout << "Espacio disponible: " << (masterboot.mbr_size - espacioUsado) << " Bytes" << endl;
            cout << "Espacio necesario:  " << size_bytes << " Bytes" << endl;

            //Verificar que haya espacio suficiente para crear la particion
            if((masterboot.mbr_size - espacioUsado) >= size_bytes){

                // Si, se puede crear la particion
                // Verificar en las P y en las L de la E, si existe el nombre
                if(!this->existeParticion(this->path, this->nombre)){

                    /** Se crea la particion implementando el primer ajuste **/
                    if(masterboot.mbr_disk_fit == 'F'){

                        masterboot.mbr_partition[numParticion].part_type = 'P';
                        masterboot.mbr_partition[numParticion].part_fit = auxFit;
                        // La primera posicion esta disponible
                        if(numParticion == 0){
                            // part_start = se posiciona al final del tamaño del MBR
                            masterboot.mbr_partition[numParticion].part_start = sizeof(masterboot);

                        }else{
                            // Se crea en la primera posicion (diferente a la primera) que haya encontrado disponible
                            // part_start = se posiciona al final del tamaño de la particion anterior
                            masterboot.mbr_partition[numParticion].part_start = masterboot.mbr_partition[numParticion-1].part_start + masterboot.mbr_partition[numParticion-1].part_size;
                        }

                        masterboot.mbr_partition[numParticion].part_size = size_bytes;
                        masterboot.mbr_partition[numParticion].part_status = '0'; // '0' = indica que la particion esta activa
                        strcpy(masterboot.mbr_partition[numParticion].part_name, this->nombre.toStdString().c_str());
                        //Se guarda el MBR actualizado
                        fseek(disco_actual, 0, SEEK_SET);
                        fwrite(&masterboot, sizeof(MBR), 1, disco_actual);

                        // Se posiciona en el part_start de la nueva particion en el disco
                        fseek(disco_actual,masterboot.mbr_partition[numParticion].part_start,SEEK_SET);
                        // Se rellena de 1's el tamaño que ocupara la particion en el disco
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,disco_actual);
                        }

                        cout << "...\n" << "Particion primaria creada con exito" <<  endl;

                        /** Se crea la particion implementando el mejor ajuste **/
                    }else if(masterboot.mbr_disk_fit == 'B'){
                        // Se busca la mejor posicion para almacenar la particion
                        int bestIndex = numParticion;
                        for(int i = 0; i < 4; i++){
                            if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                                if(i != numParticion){
                                    if(masterboot.mbr_partition[bestIndex].part_size > masterboot.mbr_partition[i].part_size){
                                        bestIndex = i;
                                        break;
                                    }
                                }
                            }
                        }

                        masterboot.mbr_partition[bestIndex].part_type = 'P';
                        masterboot.mbr_partition[bestIndex].part_fit = auxFit;

                        if(bestIndex == 0){
                            masterboot.mbr_partition[bestIndex].part_start = sizeof(masterboot);
                        }else{
                            masterboot.mbr_partition[bestIndex].part_start = masterboot.mbr_partition[bestIndex-1].part_start + masterboot.mbr_partition[bestIndex-1].part_size;
                        }

                        masterboot.mbr_partition[bestIndex].part_size = size_bytes;
                        masterboot.mbr_partition[bestIndex].part_status = '0';
                        strcpy(masterboot.mbr_partition[bestIndex].part_name, this->nombre.toStdString().c_str());
                        //Se guarda el MBR actualizado
                        fseek(disco_actual, 0, SEEK_SET);
                        fwrite(&masterboot, sizeof(MBR), 1, disco_actual);
                        //Se guardan los bytes del tamaño de la particion
                        fseek(disco_actual, masterboot.mbr_partition[bestIndex].part_start, SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,disco_actual);
                        }

                        cout << "...\n" << "Particion primaria creada con exito" <<  endl;

                        /** Se crea la particion implementando el peor ajuste **/
                    }else if(masterboot.mbr_disk_fit == 'W'){
                        // Se busca el peor ajuste
                        int  worstIndex= numParticion;
                        for(int i = 0; i < 4; i++){
                            if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                                if(i != numParticion){
                                    if(masterboot.mbr_partition[worstIndex].part_size < masterboot.mbr_partition[i].part_size){
                                        worstIndex = i;
                                        break;
                                    }
                                }
                            }
                        }

                        masterboot.mbr_partition[worstIndex].part_type = 'P';
                        masterboot.mbr_partition[worstIndex].part_fit = auxFit;

                        if(worstIndex == 0){
                            masterboot.mbr_partition[worstIndex].part_start = sizeof(masterboot);
                        }else{
                            masterboot.mbr_partition[worstIndex].part_start = masterboot.mbr_partition[worstIndex-1].part_start + masterboot.mbr_partition[worstIndex-1].part_size;
                        }

                        masterboot.mbr_partition[worstIndex].part_size = size_bytes;
                        masterboot.mbr_partition[worstIndex].part_status = '0';
                        strcpy(masterboot.mbr_partition[worstIndex].part_name, this->nombre.toStdString().c_str());
                        //Se guarda el MBR actualizado
                        fseek(disco_actual,0,SEEK_SET);
                        fwrite(&masterboot,sizeof(MBR),1,disco_actual);
                        //Se guardan los bytes del tamaño de la particion
                        fseek(disco_actual,masterboot.mbr_partition[worstIndex].part_start,SEEK_SET);
                        for(int i = 0; i < size_bytes; i++){
                            fwrite(&buffer,1,1,disco_actual);
                        }

                        cout << "...\n" << "Particion primaria creada con exito" <<  endl;
                    }
                }else{
                    cout << "ERROR: ya existe una particion con ese nombre" << endl;
                }

            }else{
                cout << "ERROR: la particion a crear excede el espacio libre" << endl;
            }
        }else{
            cout << "ERROR: Ya existen 4 particiones, no se puede crear otra" << endl;
            cout << "Elimine alguna para poder crear" << endl;
        }
        fclose(disco_actual);
    }else{
        cout << "ERROR: no existe el disco" << endl;
    }
}

void Fdisk::Crear_Particion_Extendida()
{
    char auxFit = this->ajuste.toUpper().toStdString()[0];
    string auxPath = this->path.toStdString();
    int size_bytes = 1024;
    char buffer = '1';

    if (this->unidad == "m"){

        size_bytes = this->tamano * 1048576;
    } else if (this->unidad == "k"){

        size_bytes = this->tamano * 1024;
    } else {
        // Tamaño en bytes
        size_bytes = this->tamano;
    }

    FILE *disco_actual;
    MBR masterboot;

    if((disco_actual = fopen(auxPath.c_str(), "rb+"))){

        bool flagParticion = false;//Flag para ver si hay una particion disponible
        bool flagExtendida = false;//Flag para ver si ya hay una particion extendida
        int numParticion = 0;//Que numero de particion es

        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);

        for(int i = 0; i < 4; i++){
            if (masterboot.mbr_partition[i].part_type == 'E'){
                flagExtendida = true;
                break;
            }
        }

        // Si no existe una particion extendida
        if(!flagExtendida){
            //Verificar si existe una particion disponible
            for(int i = 0; i < 4; i++){
                if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                    flagParticion = true;
                    numParticion = i;
                    break;
                }
            }

            if(flagParticion){
                //Verificar el espacio libre del disco
                int espacioUsado = 0;
                for(int i = 0; i < 4; i++){
                    if(masterboot.mbr_partition[i].part_status != '1'){
                        espacioUsado += masterboot.mbr_partition[i].part_size;
                    }
                }

                cout << "Espacio disponible: " << (masterboot.mbr_size - espacioUsado) <<" Bytes"<< endl;
                cout << "Espacio necesario:  " << size_bytes << " Bytes" << endl;

                //Verificar que haya espacio suficiente para crear la particion
                if((masterboot.mbr_size - espacioUsado) >= size_bytes){

                    // Si, se puede crear la particion
                    // Verificar en las P y en las L de la E, si existe el nombre
                    if(!(existeParticion(this->path, this->nombre))){

                        /** Se crea la particion implementando el primer ajuste **/
                        if(masterboot.mbr_disk_fit == 'F'){

                            masterboot.mbr_partition[numParticion].part_type = 'E';
                            masterboot.mbr_partition[numParticion].part_fit = auxFit;

                            if(numParticion == 0){
                                masterboot.mbr_partition[numParticion].part_start = sizeof(masterboot);
                            }else{
                                masterboot.mbr_partition[numParticion].part_start =  masterboot.mbr_partition[numParticion-1].part_start + masterboot.mbr_partition[numParticion-1].part_size;
                            }

                            masterboot.mbr_partition[numParticion].part_size = size_bytes;
                            masterboot.mbr_partition[numParticion].part_status = '0';
                            strcpy(masterboot.mbr_partition[numParticion].part_name, this->nombre.toStdString().c_str());

                            //Se guarda el MBR actualizado
                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&masterboot, sizeof(MBR), 1, disco_actual);

                            // Se posiciona el puntero en el disco en el part_start de la nueva particion
                            fseek(disco_actual, masterboot.mbr_partition[numParticion].part_start, SEEK_SET);
                            //Se guarda la particion extendida
                            EBR extendedBoot; // Nuevo EBR
                            extendedBoot.part_fit = auxFit;
                            extendedBoot.part_status = '0';
                            extendedBoot.part_start = masterboot.mbr_partition[numParticion].part_start;
                            extendedBoot.part_size = 0;
                            extendedBoot.part_next = -1;
                            strcpy(extendedBoot.part_name, "");

                            // Se escribe el EBR en el disco, en el part_start de la particion ubicado ya con fseek
                            fwrite(&extendedBoot, sizeof (EBR), 1, disco_actual);

                            // Se escriben 1's despues del EBR llenando el espacio del tamaño de la particion
                            for(int i = 0; i < (size_bytes - (int)sizeof(EBR)); i++){
                                fwrite(&buffer,1,1,disco_actual);
                            }

                            cout << "...\n" << "Particion extendida creada con exito"<< endl;

                            /** Se crea la particion implementando el mejor ajuste **/
                        }else if(masterboot.mbr_disk_fit == 'B'){

                            int bestIndex = numParticion;
                            for(int i = 0; i < 4; i++){

                                if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                                    if(i != numParticion){
                                        if(masterboot.mbr_partition[bestIndex].part_size > masterboot.mbr_partition[i].part_size){
                                            bestIndex = i;
                                            break;
                                        }
                                    }
                                }
                            }

                            masterboot.mbr_partition[bestIndex].part_type = 'E';
                            masterboot.mbr_partition[bestIndex].part_fit = auxFit;

                            if(bestIndex == 0){
                                masterboot.mbr_partition[bestIndex].part_start = sizeof(masterboot);
                            }else{
                                masterboot.mbr_partition[bestIndex].part_start =  masterboot.mbr_partition[bestIndex-1].part_start + masterboot.mbr_partition[bestIndex-1].part_size;
                            }

                            masterboot.mbr_partition[bestIndex].part_size = size_bytes;
                            masterboot.mbr_partition[bestIndex].part_status = '0';
                            strcpy(masterboot.mbr_partition[bestIndex].part_name, this->nombre.toStdString().c_str());

                            //Se guarda el MBR actualizado
                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&masterboot, sizeof(MBR), 1, disco_actual);

                            // Se posiciona el puntero en el disco en el part_start de la nueva particion
                            fseek(disco_actual, masterboot.mbr_partition[bestIndex].part_start, SEEK_SET);
                            //Se guarda la particion extendida
                            EBR extendedBoot; // Nuevo EBR
                            extendedBoot.part_fit = auxFit;
                            extendedBoot.part_status = '0';
                            extendedBoot.part_start = masterboot.mbr_partition[bestIndex].part_start;
                            extendedBoot.part_size = 0;
                            extendedBoot.part_next = -1;
                            strcpy(extendedBoot.part_name, "");

                            fwrite(&extendedBoot, sizeof (EBR), 1, disco_actual);
                            for(int i = 0; i < (size_bytes - (int)sizeof(EBR)); i++){
                                fwrite(&buffer, 1, 1, disco_actual);
                            }

                            cout << "...\n" << "Particion extendida creada con exito"<< endl;

                            /** Se crea la particion implementando el peor ajuste **/
                        }else if(masterboot.mbr_disk_fit == 'W'){

                            int  worstIndex= numParticion;
                            for(int i = 0; i < 4; i++){
                                if(masterboot.mbr_partition[i].part_start == -1 || (masterboot.mbr_partition[i].part_status == '1' && masterboot.mbr_partition[i].part_size >= size_bytes)){
                                    if(i != numParticion){
                                        if(masterboot.mbr_partition[worstIndex].part_size < masterboot.mbr_partition[i].part_size){
                                            worstIndex = i;
                                            break;
                                        }
                                    }
                                }
                            }

                            masterboot.mbr_partition[worstIndex].part_type = 'E';
                            masterboot.mbr_partition[worstIndex].part_fit = auxFit;

                            if(worstIndex == 0){
                                masterboot.mbr_partition[worstIndex].part_start = sizeof(masterboot);
                            }else{
                                masterboot.mbr_partition[worstIndex].part_start =  masterboot.mbr_partition[worstIndex-1].part_start + masterboot.mbr_partition[worstIndex-1].part_size;
                            }

                            masterboot.mbr_partition[worstIndex].part_size = size_bytes;
                            masterboot.mbr_partition[worstIndex].part_status = '0';
                            strcpy(masterboot.mbr_partition[worstIndex].part_name, this->nombre.toStdString().c_str());

                            //Se guarda el MBR actualizado
                            fseek(disco_actual, 0, SEEK_SET);
                            fwrite(&masterboot, sizeof(MBR), 1, disco_actual);

                            // Se posiciona el puntero en el disco en el part_start de la nueva particion
                            fseek(disco_actual, masterboot.mbr_partition[worstIndex].part_start, SEEK_SET);
                            //Se guarda la particion extendida
                            EBR extendedBoot;
                            extendedBoot.part_fit = auxFit;
                            extendedBoot.part_status = '0';
                            extendedBoot.part_start = masterboot.mbr_partition[worstIndex].part_start;
                            extendedBoot.part_size = 0;
                            extendedBoot.part_next = -1;
                            strcpy(extendedBoot.part_name, "");

                            fwrite(&extendedBoot, sizeof (EBR), 1, disco_actual);

                            for(int i = 0; i < (size_bytes - (int)sizeof(EBR)); i++){
                                fwrite(&buffer, 1, 1, disco_actual);
                            }

                            cout << "...\n" << "Particion extendida creada con exito"<< endl;

                        }
                    }else{
                        cout << "ERROR: ya existe una particion con ese nombre" << endl;
                    }
                }else{
                    cout << "ERROR: la particion a crear excede el tamano libre" << endl;
                }
            }else{
                cout << "ERROR: Ya existen 4 particiones, no se puede crear otra" << endl;
                cout << "Elimine alguna para poder crear una" << endl;
            }
        }else{
            cout << "ERROR: ya existe una particion extendida en este disco" << endl;
        }
        fclose(disco_actual);
    }else{
        cout << "ERROR: no existe el disco" << endl;
    }
}

void Fdisk::Crear_Particion_Logica()
{
    char auxFit = this->ajuste.toUpper().toStdString()[0];
    string auxPath = this->path.toStdString();
    int size_bytes = 1024;

    if (this->unidad == "m"){

        size_bytes = this->tamano * 1048576;
    } else if (this->unidad == "k"){

        size_bytes = this->tamano * 1024;
    } else {
        // Tamaño en bytes
        size_bytes = this->tamano;
    }

    FILE *disco_actual;
    MBR masterboot;

    if((disco_actual = fopen(auxPath.c_str(), "rb+"))){

        int numExtendida = -1;
        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot,sizeof(MBR),1,disco_actual);

        //Verificar si existe una particion extendida
        for(int i = 0; i < 4; i++){
            if(masterboot.mbr_partition[i].part_type == 'E'){
                numExtendida = i;
                break;
            }
        }

        if(!existeParticion(this->path, this->nombre)){

            if(numExtendida != -1){
                EBR extendedBoot;
                int cont = masterboot.mbr_partition[numExtendida].part_start;

                // Se posiciona en el part_start del primer EBR
                fseek(disco_actual, cont, SEEK_SET);
                fread(&extendedBoot, sizeof(EBR), 1, disco_actual);

                // Si es la primera particion logica
                if(extendedBoot.part_size == 0){

                    // Verificacion si la nueva particion L cabe en la E
                    if(masterboot.mbr_partition[numExtendida].part_size < size_bytes){
                        cout << "ERROR. La particion logica a crear excede el espacio disponible de la particion extendida " << endl;
                    }else{
                        extendedBoot.part_status = '0';
                        extendedBoot.part_fit = auxFit;
                        extendedBoot.part_start = ftell(disco_actual) - sizeof(EBR); //Para regresar al inicio de la extendida
                        extendedBoot.part_size = size_bytes;
                        extendedBoot.part_next = -1;
                        strcpy(extendedBoot.part_name, this->nombre.toStdString().c_str());

                        fseek(disco_actual, masterboot.mbr_partition[numExtendida].part_start, SEEK_SET);
                        fwrite(&extendedBoot, sizeof(EBR), 1, disco_actual);
                        cout << "...\nParticion logica creada con exito "<< endl;
                    }

                }else{
                    // Si no es la primera particion Logica
                    while((extendedBoot.part_next != -1) && (ftell(disco_actual) < (masterboot.mbr_partition[numExtendida].part_size + masterboot.mbr_partition[numExtendida].part_start))){
                        fseek(disco_actual, extendedBoot.part_next, SEEK_SET);
                        fread(&extendedBoot, sizeof(EBR), 1, disco_actual);
                    }

                    int espacioNecesario = extendedBoot.part_start + extendedBoot.part_size + size_bytes;
                    if(espacioNecesario <= (masterboot.mbr_partition[numExtendida].part_size + masterboot.mbr_partition[numExtendida].part_start)){

                        extendedBoot.part_next = extendedBoot.part_start + extendedBoot.part_size;
                        //Escribimos el next del ultimo EBR
                        fseek(disco_actual, ftell(disco_actual) - sizeof (EBR), SEEK_SET);
                        fwrite(&extendedBoot, sizeof(EBR), 1 ,disco_actual);

                        //Escribimos el nuevo EBR
                        fseek(disco_actual, extendedBoot.part_start + extendedBoot.part_size, SEEK_SET);
                        extendedBoot.part_status = '0';
                        extendedBoot.part_fit = auxFit;
                        extendedBoot.part_start = ftell(disco_actual);
                        extendedBoot.part_size = size_bytes;
                        extendedBoot.part_next = -1;
                        strcpy(extendedBoot.part_name, this->nombre.toStdString().c_str());
                        fwrite(&extendedBoot,sizeof(EBR),1,disco_actual);

                        cout << "Particion logica creada con exito "<< endl;

                    }else{
                        cout << "ERROR la particion logica a crear excede el" << endl;
                        cout << "espacio disponible de la particion extendida" << endl;
                    }
                }
            }else{
                cout << "ERROR se necesita una particion extendida donde guardar la logica " << endl;
            }
        }else{
            cout << "ERROR ya existe una particion con ese nombre" << endl;
        }

        fclose(disco_actual);
    }else{
        cout << "ERROR no existe el disco" << endl;
    }
}

void Fdisk::show_Particiones(QString path)
{
    MBR masterboot;
    FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");
    if (disco_actual != NULL)
    {
        rewind(disco_actual);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);
        cout << "<<------------------------ PARTICIONES ---------------------------->>" << endl;
        for (int i = 0; i < 4; i++)
        {
            if(masterboot.mbr_partition[i].part_status != '1'){
                cout << "<< ------------------- " << i << " ------------------->>" << endl;
                cout << "Estado: " << masterboot.mbr_partition[i].part_status << endl;
                cout << "Nombre: " << masterboot.mbr_partition[i].part_name << endl;
                cout << "Fit: " << masterboot.mbr_partition[i].part_fit << endl;
                cout << "Part_start: " << masterboot.mbr_partition[i].part_start << endl;
                cout << "Size: " << masterboot.mbr_partition[i].part_size << endl;
                cout << "Type: " << masterboot.mbr_partition[i].part_type << endl;

                if (masterboot.mbr_partition[i].part_type == 'E')
                {
                    EBR logicaR;
                    fseek(disco_actual, masterboot.mbr_partition[i].part_start, SEEK_SET);
                    fread(&logicaR, sizeof(EBR), 1, disco_actual);

                    while(logicaR.part_next != -1){
                        cout << "<< -------------------- Particion Logica --------------------->> " << endl;
                        cout << "Nombre: " << logicaR.part_name << endl;
                        cout << "Fit: " << logicaR.part_fit << endl;
                        cout << "Part_start: " << logicaR.part_start << endl;
                        cout << "Size: " << logicaR.part_size << endl;
                        cout << "Part_next: " << logicaR.part_next << endl;
                        cout << "Estado: " << logicaR.part_status << endl;

                        fseek(disco_actual, logicaR.part_next, SEEK_SET);
                        fread(&logicaR, sizeof(EBR), 1, disco_actual);
                    }
                }
            }
        }
        fclose(disco_actual);
    }else{
        cout << "No se pueden mostrar las particiones por path incorrecto.";
    }
}

bool Fdisk::existeParticion(QString path, QString name)
{
    int pos_extendida = -1;
    FILE *disco_actual;

    if((disco_actual = fopen(path.toStdString().c_str(), "rb+"))){

        MBR masterboot;
        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);

        for(int i = 0; i < 4; i++){

            if(strcmp(masterboot.mbr_partition[i].part_name, name.toStdString().c_str()) == 0){
                fclose(disco_actual);
                return true;
            }else if(masterboot.mbr_partition[i].part_type == 'e'){
                pos_extendida = i;
            }
        }

        if(pos_extendida != -1){

            fseek(disco_actual, masterboot.mbr_partition[pos_extendida].part_start, SEEK_SET);

            EBR extendedBoot;

            while((fread(&extendedBoot, sizeof(EBR), 1, disco_actual)) != 0 && (ftell(disco_actual) < (masterboot.mbr_partition[pos_extendida].part_size + masterboot.mbr_partition[pos_extendida].part_start))){
                if(strcmp(extendedBoot.part_name, name.toStdString().c_str()) == 0){
                    fclose(disco_actual);
                    return true;
                }
                if(extendedBoot.part_next == -1){
                    fclose(disco_actual);
                    return false;
                }
            }
        }
        fclose(disco_actual);
    }
    return false;
}
