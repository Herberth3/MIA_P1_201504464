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
        this->Crear_Particion(this->tamano, this->unidad, this->nombre, this->path, this->ajuste, this->tipo_particion);
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

                        if ((cantidad_add + mbr_auxiliar.mbr_partition[i].part_size + mbr_auxiliar.mbr_partition[i].part_start) <= mbr_auxiliar.mbr_tamano){

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

void Fdisk::Crear_Particion(int size, QString unit, QString name, QString path, QString fit, QString type)
{
    // Estructura tipo Particion, en la cual se llevara a cabo la creacion
    Partition nueva_particion;
    int tamanoParticion = size;

    // Se establece el tamaño que tendra la particion ya sea en Megas o Kilo o solo bytes
    if (unit.toLower() == "m") {

        tamanoParticion = size * 1048576;
    } else if (unit.toLower() == "k") {

        tamanoParticion = size * 1024;
    }

    // Se establecen los parametros para la particion a crear
    nueva_particion.part_status = '1';

    //  - Estableciendo el ajuste de la particion
    if (fit.toLower() == "bf") {
        nueva_particion.part_fit = 'B';
    } else if(fit.toLower() == "ff") {
        nueva_particion.part_fit = 'F';
    }else {
        nueva_particion.part_fit = 'W';
    }

    strcpy(nueva_particion.part_name, name.toStdString().c_str());
    nueva_particion.part_start = 0;
    nueva_particion.part_size = tamanoParticion;
    nueva_particion.part_type = type.toUpper().toStdString()[0];

    // Antes de crear una particion P o E, se pregunta si la particion es L (logica)
    if (type.toLower() == "l") {

        // Metodo para crear la particion logica
        this->Crear_Logica(nueva_particion, path);

        /*******E L I M I N A R*********/
        this->show_Particiones(path);
        /*******E L I M I N A R*********/

        return;
    }

    // *************** Crear particion P o E en disco **********************
    MBR mbr_temporal;

    // Se intenta abrir el archivo del disco
    FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");
    // Se verifica si existe el archivo, de lo contrario ERROR
    if (disco_actual != NULL) {

        // Se posiciona el apuntador al inicio del archivo
        rewind(disco_actual);
        // Se captura en mbr_temporal el mbr ya escrito en el disco
        fread(&mbr_temporal, sizeof(mbr_temporal), 1, disco_actual);
    } else {

        cout<<"Error. El disco no existe"<<endl;
        return;
    }
    // Se cierra el archivo del disco
    fclose(disco_actual);

    // Variables para validar la manipulacion de particiones en el MBR
    bool is_discoLleno = true;
    bool is_tamanoMayor = false;
    bool is_ultimaParticion = false;
    int no_Extendidas = 0;
    int part_startExtendida = 0;

    // Validar cuantas particiones extendidas hay en el disco, recorriendo el arreglo de particiones en el MBR
    for (Partition particion : mbr_temporal.mbr_partition) {

        if (particion.part_type == 'E') {

            no_Extendidas++;
        }
    }

    // Verificar si hay particiones disponibles, recorriendo el arreglo de particiones en el MBR
    for (Partition particion : mbr_temporal.mbr_partition) {

        if (particion.part_status == '0') {

            is_discoLleno = false;
            break;
        }
    }

    // ********* POSICIONA EL INICIO DE LA NUEVA PARTICION IMPLEMENTANDO EL PRIMER AJUSTE ****************
    for (int i = 0; i < 4; i++) {

        // Si la particion esta activa o ya existe
        if (mbr_temporal.mbr_partition[i].part_status != '0') {

            // Entra al if: Si no existe Extendida y la nueva particion es Extendida
            // Entra al if: Si existe o no Extendida y la nueva particion es Primaria
            if (!(nueva_particion.part_type == 'E' && no_Extendidas > 0)) {

                // Establecer primer ajuste, posicionando el part_start en el primer espacio vacio que encuentre y en donde quepa el part_size de la nueva particion
                if (i == 0) {

                    // Resta la posicion de inicio de la primera particion ya establecida y le resta el tamaño del MBR, si la diferencia es suficiente para la nueva particion
                    // La nueva particion empieza despues del MBR
                    if (mbr_temporal.mbr_partition[i].part_start - sizeof(MBR) >= nueva_particion.part_size) {
                        nueva_particion.part_start = sizeof(MBR);
                        break;
                    }
                } else {

                    // Resta la posicion de inicio de la particion que tengo indexada entre la suma de la posicion de inicio y su tamaño de la particion anterior
                    // Si existe espacio, ahi inicia la nueva particion
                    if (mbr_temporal.mbr_partition[i].part_start - (mbr_temporal.mbr_partition[i - 1].part_start + mbr_temporal.mbr_partition[i - 1].part_size) >= nueva_particion.part_size) {

                        nueva_particion.part_start = mbr_temporal.mbr_partition[i -1].part_start + mbr_temporal.mbr_partition[i - 1].part_size;
                        break;
                    }
                }
            } else {
                // De lo contrario ya hay particion Extendida. ERROR
                cout<<"Error. Ya existe una particion Extendida, no es posible crear otra"<<endl;
                is_tamanoMayor = true;
                return;
            }

            // De lo contrario hay una libre y se establece la posicion en donde va a ser creada
        } else {
            // La particion se encuentra libre en la primera posicion
            if (i == 0) {

                // El part_start de la particion se posiciona despues del tamaño del MBR en el disco
                nueva_particion.part_start = sizeof(MBR);
                break;

                // La particion libre se valida que sea creada en otra posicion diferente a primera posicion
            } else {

                is_ultimaParticion = true;
                break;
            }
        }
    }

    // Una de las 4 particiones esta libre y es diferente a la primera
    for (int i = 0; i < 4; i++) {

        // Ingresa al if si la particion esta inactiva o no existe
        if (mbr_temporal.mbr_partition[i].part_status != '1') {

            if (is_ultimaParticion) {

                nueva_particion.part_start = mbr_temporal.mbr_partition[i - 1].part_start + mbr_temporal.mbr_partition[i].part_size;
                if (mbr_temporal.mbr_tamano < (nueva_particion.part_start + nueva_particion.part_size)) {

                    is_tamanoMayor = true;
                }
            }

            mbr_temporal.mbr_partition[i] = nueva_particion;

            // Si la nueva particion es de tipo E, hay que crear un EBR
            if (nueva_particion.part_type == 'E') {

                // Por el momento solo se captura la posicion en donde empezara la extendida
                part_startExtendida = nueva_particion.part_start;
            }

            break;
        }
    }

    // Se implementa un ordenamiento tipo burbuja a las particiones en el mbr_temporal
    Partition particion_aux;

    for (int i = 1; i < 4; i++) {

        for (int j = 0; j < 4 - i; j++) {

            if ((mbr_temporal.mbr_partition[j].part_start > mbr_temporal.mbr_partition[j + 1].part_start) && mbr_temporal.mbr_partition[j + 1].part_status != '0') {

                particion_aux = mbr_temporal.mbr_partition[j + 1];
                mbr_temporal.mbr_partition[j + 1] = mbr_temporal.mbr_partition[j];
                mbr_temporal.mbr_partition[j] = particion_aux;
            }
        }
    }

    if (!is_discoLleno && !is_tamanoMayor) {

        FILE *disco_actual2 = fopen(path.toStdString().c_str(), "rb+");

        rewind(disco_actual2);
        fwrite(&mbr_temporal, sizeof(MBR), 1, disco_actual2);

        // Creacion de una particion Extendida junto a un EBR
        if (part_startExtendida != 0) {

            rewind(disco_actual2);
            EBR ebr_logica;

            ebr_logica.part_status = '0';
            ebr_logica.part_fit = ' ';
            ebr_logica.part_start = part_startExtendida;
            ebr_logica.part_size = 0;
            ebr_logica.part_next = -1;
            strcpy(ebr_logica.part_name, "Nothing");

            fseek(disco_actual2, ebr_logica.part_start, SEEK_SET);
            fwrite(&ebr_logica, sizeof(EBR), 1, disco_actual2);
        }


        fclose(disco_actual2);

        /*******E L I M I N A R*********/
        this->show_Particiones(path);
        /*******E L I M I N A R*********/

    } else {

        if (is_discoLleno) {

            cout<<"Error. Numero maximo de particiones ya completado"<<endl;
            return;
        } else {

            cout<<"Error. Tamaño de la nueva particion es mayor a lo disponible"<<endl;
            return;
        }
    }
}

void Fdisk::Crear_Logica(Partition particion, QString path)
{
    EBR nueva_logica;
    // Validar que exista una particion extendida para crear la logica
    bool is_Extendida = false;
    // Posicion en la que se encuentra la particion E en el arreglo del MBR
    int posicion_Extendida = 0;

    nueva_logica.part_status = '1';
    nueva_logica.part_fit = particion.part_fit;
    nueva_logica.part_size = particion.part_size;
    nueva_logica.part_next = -1;
    strcpy(nueva_logica.part_name, particion.part_name);

    MBR mbr_auxiliar;
    FILE *disco_actual = fopen(path.toStdString().c_str(), "rb+");

    if (disco_actual != NULL){

        rewind(disco_actual);
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        for(int i = 0; i < 4; i++){

            if(mbr_auxiliar.mbr_partition[i].part_type == 'E'){
                is_Extendida = true;
                posicion_Extendida = i;
                break;
            }
        }

        if(is_Extendida){

            int part_startExtendida = mbr_auxiliar.mbr_partition[posicion_Extendida].part_start;
            EBR ebr_logica;
            bool finalizar = false;

            fseek(disco_actual, part_startExtendida, SEEK_SET);
            fread(&ebr_logica, sizeof(EBR), 1, disco_actual);

            do{

                if(ebr_logica.part_status == '0' && ebr_logica.part_next == -1){

                    nueva_logica.part_start = ebr_logica.part_start;
                    nueva_logica.part_next = nueva_logica.part_start + nueva_logica.part_size;

                    fseek(disco_actual, nueva_logica.part_start, SEEK_SET);
                    fwrite(&nueva_logica, sizeof(EBR), 1, disco_actual);

                    fseek(disco_actual, nueva_logica.part_next, SEEK_SET);
                    // Siguiente particion logica lista para ser creada la proxima vez que se quiera otra logica
                    EBR addLogic;

                    addLogic.part_status = '0';
                    addLogic.part_fit = ' ';
                    addLogic.part_start = nueva_logica.part_next;
                    addLogic.part_size = 0;
                    addLogic.part_next = -1;
                    strcpy(addLogic.part_name, "Nothing");

                    fseek(disco_actual, addLogic.part_start, SEEK_SET);
                    fwrite(&addLogic, sizeof(EBR), 1, disco_actual);

                    finalizar = true;
                    cout<<" Particion logica creada exitosamente: "<< nueva_logica.part_name<< endl;
                }

                fseek(disco_actual, ebr_logica.part_next, SEEK_SET);
                fread(&ebr_logica, sizeof(EBR), 1, disco_actual);
            }while(!finalizar);

        }else{
            cout<<"Error. No existe particion extendida"<<endl;
        }
    } else{
        cout<<"Error. El disco no existe"<<endl;
        return;
    }

    fclose(disco_actual);
}

void Fdisk::show_Particiones(QString path)
{
    MBR discoEditar;
    FILE *bfilel = fopen(path.toStdString().c_str(), "rb+");
    if (bfilel != NULL)
    {
        rewind(bfilel);
        fread(&discoEditar, sizeof(MBR), 1, bfilel);
        cout << "<<------------------------ PARTICIONES ---------------------------->>" << endl;
        for (int i = 0; i < 4; i++)
        {
            if(discoEditar.mbr_partition[i].part_status != '0'){
                cout << "<< ------------------- " << i << " ------------------->>" << endl;
                cout << "Estado: " << discoEditar.mbr_partition[i].part_status << endl;
                cout << "Nombre: " << discoEditar.mbr_partition[i].part_name << endl;
                cout << "Fit: " << discoEditar.mbr_partition[i].part_fit << endl;
                cout << "Part_start: " << discoEditar.mbr_partition[i].part_start << endl;
                cout << "Size: " << discoEditar.mbr_partition[i].part_size << endl;
                cout << "Type: " << discoEditar.mbr_partition[i].part_type << endl;
                if (discoEditar.mbr_partition[i].part_type == 'E')
                {
                    EBR logicaR;
                    fseek(bfilel, discoEditar.mbr_partition[i].part_start, SEEK_SET);
                    fread(&logicaR, sizeof(EBR), 1, bfilel);

                    while(logicaR.part_next != -1){
                        cout << "<< -------------------- Particion Logica --------------------->> " << endl;
                        cout << "Nombre: " << logicaR.part_name << endl;
                        cout << "Fit: " << logicaR.part_fit << endl;
                        cout << "Part_start: " << logicaR.part_start << endl;
                        cout << "Size: " << logicaR.part_size << endl;
                        cout << "Part_next: " << logicaR.part_next << endl;
                        cout << "Estado: " << logicaR.part_status << endl;

                        fseek(bfilel, logicaR.part_next, SEEK_SET);
                        fread(&logicaR, sizeof(EBR), 1, bfilel);
                    }
                }
            }
        }
        fclose(bfilel);
    }else{
        cout << "No se pueden mostrar las particiones por path incorrecto.";
    }
}
