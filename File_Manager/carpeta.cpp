#include "carpeta.h"

carpeta::carpeta()
{

}

void carpeta::mkDir(QString path, int p, QString id, Mount mount, bool is_bitacora)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nameParticion;

    // Se obtienen datos sobre la particion montada en donde se creara el directorio
    this->getDatosParticionMontada(id, mount, &pathDisco_Particion,&startParticion, &sizeParticion, &nameParticion, &error);

    if (error == 1) {
        return;
    }

    Structs::SuperBloque SBloque_Temporal;
    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    // Se obtiene el SuperBloque de la particion
    fseek(disco_actual, startParticion, SEEK_SET);
    fread(&SBloque_Temporal, sizeof(Structs::SuperBloque), 1, disco_actual);

    char uno = '1';
    // LA CARPETA SE CREARA EN LA RAIZ
    if(path == "/"){

        Structs::arbolVirtual carpeta;
        strcpy(carpeta.nombre_directorio, "raiz");
        strcpy(carpeta.date_creacion, this->getFecha().c_str());
        carpeta.detalle_directorio = SBloque_Temporal.first_free_dd;
        // Escribimos el avd raiz
        fseek(disco_actual, SBloque_Temporal.start_arbol_directorio, SEEK_SET);
        fwrite(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
        // Ocupamos su posicion en bitmap, escribiendo un "1"
        fseek(disco_actual, SBloque_Temporal.start_bm_arbol_directorio, SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);
        // Se reduce en 1 el contador de AVFree y el primer AVDFree sera el siguiente
        SBloque_Temporal.arbol_virtual_free--;
        SBloque_Temporal.first_free_avd++;

        // Escribiendo el detalle de directorio
        Structs::detalleDirectorio detalle;
        fseek(disco_actual, SBloque_Temporal.start_detalle_directorio, SEEK_SET);
        fwrite(&detalle, sizeof(Structs::detalleDirectorio), 1, disco_actual);
        // Ocupamos la posicion en el bitmap de DD, escribiendo un "1"
        fseek(disco_actual, SBloque_Temporal.start_bm_detalle_directorio, SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);
        // Se reduce en 1 el contador de DDFree y el primer DDFree sera el siguiente
        SBloque_Temporal.detalle_directorio_free--;
        SBloque_Temporal.first_free_dd++;
        // Con los nuevos cambios, se escribe de nuevo el Super Bloque en el disco
        fseek(disco_actual, startParticion, SEEK_SET);
        fwrite(&SBloque_Temporal, sizeof(Structs::SuperBloque), 1, disco_actual);
        fclose(disco_actual);
    }else{ // LA CARPETA SE CREA EN UN DIRECTORIO DIFERENTE A RAIZ

        vector<string> array_directorios;
        stringstream total_path(path.toStdString());
        string dir_tmp;

        // Se fragmenta (split, "/") el path en directorios y se enlistan en el vector
        while (getline(total_path, dir_tmp, '/'))
        {
            if(dir_tmp != ""){
                array_directorios.push_back(dir_tmp);
            }
        }

        Structs::arbolVirtual carpeta_raiz;

        fseek(disco_actual, SBloque_Temporal.start_arbol_directorio, SEEK_SET);
        fread(&carpeta_raiz, sizeof(Structs::arbolVirtual), 1, disco_actual);

        fclose(disco_actual);

        this->crearCarpeta(carpeta_raiz, array_directorios, pathDisco_Particion, SBloque_Temporal, 0);
        //VALIDAR P
    }

    if(!is_bitacora){
        string fechaActual = this->getFecha();

        Structs::logBitacora bitacora;
        strcpy(bitacora.path, path.toStdString().c_str());
        bitacora.tipo = '0';
        strcpy(bitacora.tipo_operacion, "mkdir");
        bitacora.size = p;
        strcpy(bitacora.log_fecha, fechaActual.c_str());


        FILE *disco_actual1 = fopen(pathDisco_Particion.c_str(), "rb+");

        Structs::logBitacora BitacoraRaiz;
        fseek(disco_actual1, SBloque_Temporal.start_log, SEEK_SET);
        fread(&BitacoraRaiz, sizeof(Structs::logBitacora), 1, disco_actual1);

        //Lo escribimos en el first free de la bitacora.
        fseek(disco_actual1, SBloque_Temporal.start_log + (BitacoraRaiz.size * sizeof(Structs::logBitacora)), SEEK_SET);
        fwrite(&bitacora, sizeof(Structs::logBitacora), 1, disco_actual1);

        BitacoraRaiz.size++;
        fseek(disco_actual1, SBloque_Temporal.start_log, SEEK_SET);
        fwrite(&BitacoraRaiz, sizeof(Structs::logBitacora), 1, disco_actual1);

        fclose(disco_actual1);
    }
}

void carpeta::getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int *error)
{
    QString identificador = id; // Ejemplo: 6413f
    identificador.remove(0, 2); // Ejemplo: 13f

    int id_size = identificador.size();
    char letra = identificador.toStdString().at(id_size - 1); // Ejemplo: f
    QString numero = identificador.remove(id_size - 1, 1); // Ejemplo: "13"
    int numeroInt =  numero.toInt(); // Ejemplo: 13
    bool existePath = false;
    string nombreParticion = "";
    char pathDisco[100] = "";
    int sizeParticion = -1;
    int startParticion = -1;

    //Obtener el nombre de la particion y el path del disco.
    for (int i = 0; i < 26; i++){

        if (mount.discos[i].nombre == "nombre"){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].numero == 1 && mount.discos[i].particiones[j].estado == 1){

                    strcpy(pathDisco, mount.discos[i].path);
                    nombreParticion = mount.discos[i].particiones[j].nombre;
                    existePath = true;
                    break;
                }
            }
        }
    }

    if (!existePath)
    {
        cout << "Error: ID no reconocido o la particion no esta montada" << endl;
        *error = 1;
        return;
    }

    // Obtener el part_start de la particion y su tamaño
    FILE *disco_actual = fopen(pathDisco, "rb+");
    if(disco_actual != NULL){

        rewind(disco_actual);
        MBR mbr_auxiliar;
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        // Solo se busca en las particiones primarias
        // NOTA: no se implementa la creacion del sistema de archivos en las logicas
        for (Partition particion : mbr_auxiliar.mbr_partition) {

            if (particion.part_name == nombreParticion) {

                sizeParticion = particion.part_size;
                startParticion = particion.part_start;
            }
        }

        fclose(disco_actual);
    }else{
        cout << "Error. El PATH no fue encontrado." << endl;
        *error = 1;
        return;
    }

    *part_nameParticion = string(nombreParticion);
    *error = 0;
    *path = string(pathDisco);
    *part_starParticion = startParticion;
    *part_sizeParticion = sizeParticion;
}

void carpeta::crearCarpeta(Structs::arbolVirtual avd, vector<string> array_directorios, string pathDisco, Structs::SuperBloque superBloque, int pointer)
{
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");
    char uno = '1';
    int apuntador = 0;
    int pointerAvd = 0;

    if(array_directorios.empty()){
        fclose(disco_actual);
        return;
    }

    for(int i = 0; i < 6; i++){
        apuntador = avd.array_subdirectorios[i];

        // Se crea una nueva carpeta en donde el arreglo de subdirectorios tenga -1
        if(apuntador == -1){

            Structs::arbolVirtual carpeta;
            strcpy(carpeta.nombre_directorio, array_directorios[0].c_str());
            strcpy(carpeta.date_creacion, this->getFecha().c_str());
            carpeta.detalle_directorio = superBloque.first_free_dd;
            avd.array_subdirectorios[i] = superBloque.first_free_avd;
            pointerAvd = superBloque.first_free_avd;

            int posicion = superBloque.start_arbol_directorio + (superBloque.first_free_avd * sizeof(Structs::arbolVirtual));
            // Escribimos el avd
            fseek(disco_actual, posicion, SEEK_SET);
            fwrite(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
            // Ocupamos su posicion en bitmap de AVD
            fseek(disco_actual, (superBloque.start_bm_arbol_directorio + superBloque.first_free_avd), SEEK_SET);
            fwrite(&uno, sizeof(char), 1, disco_actual);
            superBloque.arbol_virtual_free--;
            superBloque.first_free_avd++;

            // Reescribo AVD actual - *
            // Escribimos el AVD
            fseek(disco_actual, superBloque.start_arbol_directorio + (pointer * sizeof(Structs::arbolVirtual)), SEEK_SET);
            fwrite(&avd, sizeof(Structs::arbolVirtual), 1, disco_actual);

            // Escribiendo el detalle de directorio.
            fseek(disco_actual, (superBloque.start_bm_detalle_directorio + superBloque.first_free_dd), SEEK_SET); //Ocupamos su posicion en bitmap.
            fwrite(&uno, sizeof(char), 1, disco_actual);
            superBloque.detalle_directorio_free--;
            superBloque.first_free_dd++;

            int posSB= superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque);
            fseek(disco_actual, posSB, SEEK_SET);
            fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

            array_directorios.erase(array_directorios.begin());
            fclose(disco_actual);

            this->crearCarpeta(carpeta, array_directorios, pathDisco, superBloque, pointerAvd);
            return;
        }else{

            Structs::arbolVirtual carpetaHijo;
            // Nos posicionamos en la carpeta hija
            fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
            fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);

            if(carpetaHijo.nombre_directorio == array_directorios[0]){
                array_directorios.erase(array_directorios.begin());
                fclose(disco_actual);

                this->crearCarpeta(carpetaHijo, array_directorios, pathDisco, superBloque, apuntador);
                return;
            }
        }
    }

    // NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;

    if(apuntador == -1){

        Structs::arbolVirtual carpeta;
        strcpy(carpeta.nombre_directorio, avd.nombre_directorio);
        strcpy(carpeta.date_creacion, this->getFecha().c_str());
        carpeta.detalle_directorio = superBloque.first_free_dd;
        avd.avd_siguiente = superBloque.first_free_avd;
        pointerAvd = superBloque.first_free_avd;

        int posicion = superBloque.start_arbol_directorio + (superBloque.first_free_avd * sizeof(Structs::arbolVirtual));
        // Escribimos el AVD
        fseek(disco_actual, posicion, SEEK_SET);
        fwrite(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
        // Ocupamos su posicion en bitmap.
        fseek(disco_actual, (superBloque.start_bm_arbol_directorio + superBloque.first_free_avd), SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);
        superBloque.arbol_virtual_free--;
        superBloque.first_free_avd++;

        // Reescribo AVD actual
        fseek(disco_actual, superBloque.start_arbol_directorio + (pointer * sizeof(Structs::arbolVirtual)), SEEK_SET);
        fwrite(&avd, sizeof(Structs::arbolVirtual), 1, disco_actual);

        // Escribiendo el detalle de directorio.
        // Ocupamos su posicion en bitmap.
        fseek(disco_actual, (superBloque.start_bm_detalle_directorio + superBloque.first_free_dd), SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);
        superBloque.detalle_directorio_free--;
        superBloque.first_free_dd++;

        fseek(disco_actual, (superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque)), SEEK_SET);
        fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

        fclose(disco_actual);

        this->crearCarpeta(carpeta, array_directorios, pathDisco, superBloque, pointerAvd);
        return;
    }else{

        Structs::arbolVirtual carpetaIndirecta;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_actual);

        fclose(disco_actual);
        this->crearCarpeta(carpetaIndirecta, array_directorios, pathDisco, superBloque, apuntador);
        return;
    }
}

string carpeta::getFecha()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    string fechaActual(buffer);
    return fechaActual;
}

void carpeta::renameFile(QString id, QString path, QString name, Mount montaje)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nombreParticion;

    this->getDatosParticionMontada(id, montaje, &pathDisco_Particion, &startParticion, &sizeParticion, &nombreParticion, &error);

    if(error == 1){
        return;
    }

    vector<string> array_directorios;
    stringstream total_path(path.toStdString());
    string dir_tmp;

    while (getline(total_path, dir_tmp, '/'))
    {
        if(dir_tmp != ""){
            array_directorios.push_back(dir_tmp);
        }
    }

    Structs::SuperBloque superBloque;
    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");

    fseek(disco_actual, startParticion, SEEK_SET);
    //Leo el superbloque al inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    Structs::arbolVirtual carpeta_raiz;
    fseek(disco_actual, superBloque.start_arbol_directorio, SEEK_SET);
    fread(&carpeta_raiz, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual);

    this->recorrerRuta(carpeta_raiz, array_directorios, pathDisco_Particion, superBloque, name.toStdString(), 0);
}

void carpeta::recorrerRuta(Structs::arbolVirtual avd, vector<string> array_directorios, string pathDisco, Structs::SuperBloque superBloque, string nombre, int pointer)
{
    bool esArchivo = false;
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");
    int apuntador = 0;

    for(int i = 0; i < 6; i++){
        apuntador = avd.array_subdirectorios[i];
        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);

        if(carpetaHijo.nombre_directorio == array_directorios[0]){

            string name = array_directorios[0];
            array_directorios.erase(array_directorios.begin());
            // O es archivo o es carpeta.
            if(array_directorios.size() == 1){

                apuntador = carpetaHijo.detalle_directorio;
                Structs::detalleDirectorio Archivos;
                //nos posicionamos en el detalle de directorio.
                fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

                for(int j = 0; j < 5; j++){

                    if(Archivos.archivos[j].nombre_directorio == array_directorios[0]){

                        cout<< "Archivo Renombrado"<< endl;
                        esArchivo = true;
                        strcpy(Archivos.archivos[j].nombre_directorio, nombre.c_str());
                        // Nos posicionamos en el detalle de directorio
                        fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                        fwrite(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);
                        fclose(disco_actual);
                        return;
                    }
                }

                //ES CARPETA.
                if(!esArchivo){

                    for(int k = 0; k < 6; k++){

                        int pointer = carpetaHijo.array_subdirectorios[k];
                        Structs::arbolVirtual carpeta;
                        // Nos posicionamos en la carpeta hija
                        fseek(disco_actual, (superBloque.start_arbol_directorio + (pointer * sizeof(Structs::arbolVirtual))), SEEK_SET);
                        fread(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
                        fclose(disco_actual);

                        if(carpeta.nombre_directorio == array_directorios[0]){

                            cout<< "Carpeta Renombrada"<< endl;
                            strcpy(carpeta.nombre_directorio, nombre.c_str());
                            // Nos posicionamos en la carpeta hija
                            fseek(disco_actual, (superBloque.start_arbol_directorio + (pointer * sizeof(Structs::arbolVirtual))), SEEK_SET);
                            fread(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
                            fclose(disco_actual);
                            return;
                        }
                    }
                }

                cout<< "Archivo No encontrado"<< endl;
                fclose(disco_actual);
                return;

            }else if(array_directorios.size() == 0){

                if(carpetaHijo.nombre_directorio == name){

                    cout<< "Carpeta Renombrada"<< endl;
                    strcpy(carpetaHijo.nombre_directorio, nombre.c_str());
                    // Nos posicionamos en la carpeta hija
                    fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
                    fwrite(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);
                    fclose(disco_actual);
                    return;
                }else{

                    apuntador = avd.detalle_directorio;
                    Structs::detalleDirectorio Archivos;
                    // Nos posicionamos en el detalle de directorio
                    fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                    fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

                    for(int j=0;j<5;j++){

                        if(Archivos.archivos[j].nombre_directorio == array_directorios[0]){

                            cout<< "Archivo Renombrado"<< endl;
                            esArchivo = true;
                            strcpy(Archivos.archivos[j].nombre_directorio, nombre.c_str());
                            // Nos posicionamos en el detalle de directorio
                            fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                            fwrite(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);
                            fclose(disco_actual);
                            return;
                        }
                    }
                }
         }else{
                fclose(disco_actual);
                this->recorrerRuta(carpetaHijo, array_directorios, pathDisco, superBloque, nombre, apuntador);
                return;
            }
        }
    }

    //NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;
    Structs::arbolVirtual carpetaIndirecta;
    // Nos posicionamos en la carpeta hija
    fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual);
    this->recorrerRuta(carpetaIndirecta, array_directorios, pathDisco, superBloque, nombre, apuntador);
    return;
}

void carpeta::copyFile(QString path, QString dest, Mount montaje, QString id)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nombreParticion;

    this->getDatosParticionMontada(id, montaje, &pathDisco_Particion, &startParticion, &sizeParticion, &nombreParticion, &error);

    if(error == 1){
        return;
    }

    vector<string> array_directorios;
    stringstream total_path(path.toStdString());
    string dir_tmp;

    while (getline(total_path, dir_tmp, '/'))
    {
        if(dir_tmp != ""){
            array_directorios.push_back(dir_tmp);
        }
    }

    vector<string> destinoArray;
    stringstream total_path2(dest.toStdString());
    string dir_tmp2;

    while (getline(total_path2, dir_tmp2, '/'))
    {
        if(dir_tmp2 != ""){
            destinoArray.push_back(dir_tmp2);
        }
    }

    Structs::SuperBloque superBloque;

    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    fseek(disco_actual, startParticion, SEEK_SET);
    // Leo el superbloque al inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    Structs::arbolVirtual carpeta_raiz;
    fseek(disco_actual, superBloque.start_arbol_directorio, SEEK_SET);
    fread(&carpeta_raiz, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual);

    Structs::arbolVirtual avdDest = getAVD(carpeta_raiz, destinoArray, pathDisco_Particion, superBloque, 0);
    int avdDestNo = getintAVD(carpeta_raiz, destinoArray, pathDisco_Particion, superBloque, 0);
    int avdRuta = getintAVD(carpeta_raiz, array_directorios, pathDisco_Particion, superBloque, 0);

    FILE *disco_actual1 = fopen(pathDisco_Particion.c_str(), "rb+");

    if(this->copiarArchivo){

        int apuntador = avdDest.detalle_directorio;
        Structs::detalleDirectorio Archivos;
        // Nos posicionamos en el detalle de directorio
        fseek(disco_actual1, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual1);

        for(int j=0;j<5;j++){

            if(Archivos.archivos[j].noInodo == -1){

                Archivos.archivos[j].noInodo = avdRuta;
                // Nos posicionamos en el detalle de directorio.
                fseek(disco_actual1, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fwrite(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual1);
                this->copiarArchivo = false;
                break;
            }
        }
    }else{

        for(int j = 0; j < 6; j++){

            if(avdDest.array_subdirectorios[j] == -1){

                avdDest.array_subdirectorios[j] = avdRuta;
                fseek(disco_actual1, superBloque.start_arbol_directorio + (avdDestNo * sizeof(Structs::arbolVirtual)), SEEK_SET);
                fwrite(&avdDest, sizeof(Structs::arbolVirtual), 1, disco_actual1);
                break;
            }
        }
    }
    fclose(disco_actual1);
}

Structs::arbolVirtual carpeta::getAVD(Structs::arbolVirtual avd, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int pointer)
{
    bool esArchivo = false;
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");
    int apuntador = 0;

    for(int i = 0; i < 6; i++){

        apuntador = avd.array_subdirectorios[i];
        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);

        if(carpetaHijo.nombre_directorio == path[0]){
            string name = path[0];
            path.erase(path.begin());
            // O es archivo o es carpeta
            if(path.size() == 1){

                apuntador = carpetaHijo.detalle_directorio;
                Structs::detalleDirectorio Archivos;
                //nos posicionamos en el detalle de directorio
                fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

                for(int j = 0; j < 5; j++){

                    if(Archivos.archivos[j].nombre_directorio == path[0]){
                        cout<< "Archivo Encontrado"<< endl;
                        this->copiarArchivo = true;
                        fclose(disco_actual);
                        return carpetaHijo;
                    }
                }

                //ES CARPETA.
                if(!esArchivo){

                    for(int k = 0; k < 6; k++){

                        int pointer = carpetaHijo.array_subdirectorios[k];
                        Structs::arbolVirtual carpeta;
                        // Nos posicionamos en la carpeta hija
                        fseek(disco_actual, (superBloque.start_arbol_directorio + (pointer * sizeof(Structs::arbolVirtual))), SEEK_SET);
                        fread(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);

                        if(carpeta.nombre_directorio == path[0]){
                            cout<< "Carpeta Encontrada"<< endl;
                            fclose(disco_actual);
                            return carpeta;
                        }
                    }
                }
                cout<< "Archivo No encontrado"<< endl;
                fclose(disco_actual);
                return avd;
            }else if(path.size() == 0){

                if(carpetaHijo.nombre_directorio == name){
                    cout<< "Carpeta Encontrada"<< endl;
                    fclose(disco_actual);
                    return carpetaHijo;
                }else{
                    this->copiarArchivo = true;
                    fclose(disco_actual);
                    return avd;
                }
         }else{
                fclose(disco_actual);

                return this->getAVD(carpetaHijo, path, pathDisco, superBloque, apuntador);            }
        }
    }

    // NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;
    Structs::arbolVirtual carpetaIndirecta;
    // Nos posicionamos en la carpeta hija
    fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual);
    return this->getAVD(carpetaIndirecta, path, pathDisco, superBloque, apuntador);
}

int carpeta::getintAVD(Structs::arbolVirtual avd, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int pointer)
{
    bool esArchivo = false;
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");
    int apuntador = 0;

    for(int i = 0; i < 6; i++){

        apuntador = avd.array_subdirectorios[i];
        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);

        if(carpetaHijo.nombre_directorio == path[0]){
            string name = path[0];
            path.erase(path.begin());
            if(path.size() == 1){ // O es archivo o es carpeta.
                apuntador = carpetaHijo.detalle_directorio;
                Structs::detalleDirectorio Archivos;
                // Nos posicionamos en el detalle de directorio
                fseek(disco_actual, (superBloque.start_detalle_directorio+(apuntador*sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

                for(int j=0; j < 5; j++){

                    if(Archivos.archivos[j].nombre_directorio == path[0]){
                        cout<< "Archivo Encontrado"<< endl;
                        this->copiarArchivo = true;
                        fclose(disco_actual);
                        return Archivos.archivos[j].noInodo;
                    }
                }

                //ES CARPETA.
                if(!esArchivo){

                    for(int k = 0; k < 6; k++){

                        int pointer = carpetaHijo.array_subdirectorios[k];
                        Structs::arbolVirtual carpeta;
                        // Nos posicionamos en la carpeta hija
                        fseek(disco_actual, (superBloque.start_arbol_directorio+(pointer*sizeof(Structs::arbolVirtual))), SEEK_SET);
                        fread(&carpeta, sizeof(Structs::arbolVirtual), 1, disco_actual);
                        fclose(disco_actual);

                        if(carpeta.nombre_directorio == path[0]){
                            cout<< "Carpeta Encontrada"<< endl;
                            return pointer;
                        }
                    }
                }
                cout<< "Archivo No encontrado"<< endl;
                fclose(disco_actual);
                return pointer;
            }else if(path.size() == 0){

                if(carpetaHijo.nombre_directorio == name){
                    cout<< "Carpeta Encontrada"<< endl;
                    fclose(disco_actual);
                    return apuntador;
                }else{
                    this->copiarArchivo = true;
                    fclose(disco_actual);
                    return pointer;
                }
         }else{
                fclose(disco_actual);
                return this->getintAVD(carpetaHijo, path, pathDisco, superBloque, apuntador);
            }
        }
    }

    // NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;
    Structs::arbolVirtual carpetaIndirecta;
    // Nos posicionamos en la carpeta hija
    fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual);
    return this->getintAVD(carpetaIndirecta, path, pathDisco, superBloque, apuntador);
}
