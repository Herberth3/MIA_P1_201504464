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

        this->show_Particiones(this->path);
    }
}

void Fdisk::Modificar_Espacio(int add, QString unit, QString name, QString path)
{
    // Cantidad_add puede ser positivo o negativo
    int cantidad_add = add;

    if (unit.toLower() == "m"){

        cantidad_add = add * 1048576;
    } else if (unit.toLower() == "k"){

        cantidad_add = add * 1024;
    }

    MBR mbr_auxiliar;
    FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");

    if (disco_actual != NULL){

        rewind(disco_actual);
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        for (int i = 0; i < 4; i++){
            // Comparar si el nombre de la particion a buscar existe
            if (strcmp(mbr_auxiliar.mbr_partition[i].part_name, name.toStdString().c_str()) == 0){
                // Se verifica si lo agregado no resta mas al tamaño de la particion
                if ((cantidad_add + mbr_auxiliar.mbr_partition[i].part_size) > 0){

                    if (mbr_auxiliar.mbr_partition[i + 1].part_start != 0){

                        if ((cantidad_add + mbr_auxiliar.mbr_partition[i].part_size + mbr_auxiliar.mbr_partition[i].part_start) <= mbr_auxiliar.mbr_partition[i + 1].part_start){

                            mbr_auxiliar.mbr_partition[i].part_size = mbr_auxiliar.mbr_partition[i].part_size + (cantidad_add);
                            cout<<"Part_size modificado correctamente"<<endl;
                            break;
                        } else{

                            cout<<"Error: ADD sobrepasa el espacio en la particion." << endl;
                        }
                    } else{

                        if ((cantidad_add + mbr_auxiliar.mbr_partition[i].part_size + mbr_auxiliar.mbr_partition[i].part_start) <= mbr_auxiliar.mbr_size){

                            mbr_auxiliar.mbr_partition[i].part_size = mbr_auxiliar.mbr_partition[i].part_size + (cantidad_add);
                            cout<<"Part_size modificado correctamente"<<endl;
                            break;
                        } else{

                            cout<<"Error: ADD sobrepasa el espacio en disco."<<endl;
                        }
                    }
                } else{

                    cout<<"Error: Tamaño a quitar es mayor al de la particion"<<endl;
                }
            }
        }
        rewind(disco_actual);
        fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);
    } else{

        cout<<"Error. El disco no existe"<<endl;
        return;
    }
    fclose(disco_actual);
}

void Fdisk::Eliminar_Particion(QString t_delete, QString path, QString name)
{
    string eleccion;
    int part_startExtendida = -1;
    cout << "Confirmar eliminacion de particion: "<< this->path.toStdString() << " (Y/N)? \n";
    cin >> eleccion;

    if (eleccion == "Y" || eleccion == "y"){

        // *************** ELIMINAR SI LA PARTICION ES LOGICA ***********************
        EBR ebr_auxiliar;
        FILE *disco_actual =fopen(path.toStdString().c_str(), "rb+");

        if (disco_actual != NULL){

            MBR mbr_auxiliar;
            rewind(disco_actual);
            fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

            for (Partition particion  : mbr_auxiliar.mbr_partition) {

                if (particion.part_type == 'E') {

                    part_startExtendida = particion.part_start;
                }
            }

            // Si existe una particion Extendida, se ingresa al if
            if(part_startExtendida != -1){

                fseek(disco_actual, part_startExtendida, SEEK_SET);
                fread(&ebr_auxiliar, sizeof(EBR), 1, disco_actual);

                while(ebr_auxiliar.part_next != -1){

                    // Se compara el nombre de la particion en el ebr y el nombre que nos envian de parametro
                    if(strcmp(ebr_auxiliar.part_name, name.toStdString().c_str()) == 0){

                        ebr_auxiliar.part_status = '0'; // Elimino la particion logica.
                        fseek(disco_actual, ebr_auxiliar.part_start, SEEK_SET);
                        fwrite(&ebr_auxiliar, sizeof(EBR), 1, disco_actual);

                        cout<<"Particion logica eliminada exitosamente."<<endl;
                        fclose(disco_actual);
                        return;
                    }

                    fseek(disco_actual, ebr_auxiliar.part_next, SEEK_SET);
                    fread(&ebr_auxiliar, sizeof(EBR), 1, disco_actual);
                }
            }
            fclose(disco_actual);
        }else{
            cout<<"Error. El disco no existe"<<endl;
            return;
        }

        // *************** ELIMINAR SI LA PARTICION ES PRIMARIA O EXTENDIDA ****************
        if (t_delete.toLower() == "fast"){

            MBR mbr_auxiliar;
            FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");
            rewind(disco_actual);
            fread(&mbr_auxiliar, sizeof(mbr_auxiliar), 1, disco_actual);

            // Eliminacion via Fast
            for (int i = 0; i < 4; i++){

                if (strcmp(mbr_auxiliar.mbr_partition[i].part_name, name.toStdString().c_str()) == 0){

                    mbr_auxiliar.mbr_partition[i].part_status = '0';

                    cout<<"Particion eliminada correctamente via Fast"<<endl;

                    int j = i;
                    Partition particion_auxiliar;

                    // Re-ordenando las particiones
                    while (j != 3){

                        if (mbr_auxiliar.mbr_partition[j + 1].part_status != '0'){

                            particion_auxiliar = mbr_auxiliar.mbr_partition[j];
                            mbr_auxiliar.mbr_partition[j] = mbr_auxiliar.mbr_partition[j + 1];
                            mbr_auxiliar.mbr_partition[j + 1] = particion_auxiliar;
                        }
                        j++;
                    }
                    break;
                }
            }

            rewind(disco_actual);
            fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

            fclose(disco_actual);
        } else if (t_delete.toLower() == "full"){

            MBR mbr_auxiliar;
            FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");
            rewind(disco_actual);
            fread(&mbr_auxiliar, sizeof(mbr_auxiliar), 1, disco_actual);

            // Eliminacion via full
            for (int i = 0; i < 4; i++){

                if (strcmp(mbr_auxiliar.mbr_partition[i].part_name, name.toStdString().c_str()) == 0){

                    mbr_auxiliar.mbr_partition[i].part_status = '0';
                    mbr_auxiliar.mbr_partition[i].part_fit = ' ';
                    strcpy(mbr_auxiliar.mbr_partition[i].part_name, "");
                    mbr_auxiliar.mbr_partition[i].part_start = 0;
                    mbr_auxiliar.mbr_partition[i].part_size = 0;
                    mbr_auxiliar.mbr_partition[i].part_type = ' ';

                    cout<<"Particion eliminada correctamente via Full"<<endl;

                    int j = i;
                    Partition particion_auxiliar;

                    // Re-ordenando las particiones
                    while (j != 3){

                        if (mbr_auxiliar.mbr_partition[j + 1].part_status != '0'){

                            particion_auxiliar = mbr_auxiliar.mbr_partition[j];
                            mbr_auxiliar.mbr_partition[j] = mbr_auxiliar.mbr_partition[j + 1];
                            mbr_auxiliar.mbr_partition[j + 1] = particion_auxiliar;
                        }
                        j++;
                    }
                    break;
                }
            }

            rewind(disco_actual);
            fwrite(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

            fclose(disco_actual);
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
