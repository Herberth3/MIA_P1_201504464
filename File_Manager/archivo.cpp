#include "archivo.h"

archivo::archivo()
{

}

void archivo::mkFile(QString id, QString path, int r, int size, QString cont, Mount mount, bool is_bitacora)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    int numSize = size;
    string pathDisco_Particion = "";
    string nombreParticion;

    this->getDatosParticionMontada(id, mount, &pathDisco_Particion, &startParticion, &sizeParticion, &nombreParticion, &error);

    if(error == 1){
        return;
    }

    Structs::SuperBloque superBloque;
    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    fseek(disco_actual, startParticion, SEEK_SET);
    //Leo el superbloque al inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    vector<string> array_directorios;
    stringstream total_path(path.toStdString());
    string dir_tmp;

    while (getline(total_path, dir_tmp, '/'))
    {
        if(dir_tmp != ""){
            array_directorios.push_back(dir_tmp);
        }
    }

    Structs::arbolVirtual raiz;
    fseek(disco_actual, superBloque.start_arbol_directorio, SEEK_SET);
    fread(&raiz, sizeof(Structs::arbolVirtual), 1, disco_actual);

    vector<string> pathCarpeta = array_directorios;
    pathCarpeta.pop_back();

    //Nos aseguramos de crear toda la ruta hacia el archivo.
    fclose(disco_actual);

    carpeta folder;
    // Aqui se edita el superbloque.
    folder.crearCarpeta(raiz, pathCarpeta, pathDisco_Particion, superBloque, 0);

    FILE *disco_actual1 = fopen(pathDisco_Particion.c_str(), "rb+");
    fseek(disco_actual1, startParticion, SEEK_SET);
    // Leo el superbloque al inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual1);

    fseek(disco_actual1, superBloque.start_arbol_directorio, SEEK_SET);
    fread(&raiz, sizeof(Structs::arbolVirtual), 1, disco_actual);

    fclose(disco_actual1);

    this->recorrerRuta(raiz, array_directorios, pathDisco_Particion, superBloque, numSize, cont.toStdString());

    if(!is_bitacora){

        string fechaActual = this->getFecha();

        Structs::logBitacora bitacora;
        strcpy(bitacora.path, path.toStdString().c_str());
        bitacora.tipo = '1';
        strcpy(bitacora.tipo_operacion, "mkfile");
        bitacora.size = size;
        strcpy(bitacora.log_fecha, fechaActual.c_str());

        if(cont.toStdString().size() > 50){
            strcpy(bitacora.contenido, cont.toStdString().substr(0, 49).c_str());
        }else{
            strcpy(bitacora.contenido, cont.toStdString().c_str());
        }


        FILE *disco_actual2 = fopen(pathDisco_Particion.c_str(), "rb+");

        Structs::logBitacora BitacoraRaiz;
        fseek(disco_actual2, superBloque.start_log, SEEK_SET);
        fread(&BitacoraRaiz, sizeof(Structs::logBitacora), 1, disco_actual2);

        //Lo escribimos en el first free de la bitacora.
        fseek(disco_actual2, superBloque.start_log + (BitacoraRaiz.size * sizeof(Structs::logBitacora)), SEEK_SET);
        fwrite(&bitacora, sizeof(Structs::logBitacora), 1, disco_actual2);

        BitacoraRaiz.size++;
        fseek(disco_actual2, superBloque.start_log, SEEK_SET);
        fwrite(&BitacoraRaiz, sizeof(Structs::logBitacora), 1, disco_actual2);

        fclose(disco_actual2);
    }
}

void archivo::getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int *error)
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

        if (mount.discos[i].numero == numeroInt){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].letra == letra && mount.discos[i].particiones[j].estado == 1){

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

string archivo::getFecha()
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

void archivo::recorrerRuta(Structs::arbolVirtual avd, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int size, string cont)
{
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");

    int apuntador = 0;

    for(int i = 0; i<6; i++){
        apuntador = avd.array_subdirectorios[i];

        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_arbol_directorio + (apuntador * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_actual);

        if(carpetaHijo.nombre_directorio == path[0]){

            path.erase(path.begin());
            if(path.size() == 1){

                apuntador = carpetaHijo.detalle_directorio;
                Structs::detalleDirectorio Archivos;
                // Nos posicionamos en el detalle de directorio.
                fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);
                fclose(disco_actual);

                this->crearArchivo(Archivos, apuntador, path, pathDisco, superBloque, size, cont);
                return;
            }else{
                fclose(disco_actual);
                this->recorrerRuta(carpetaHijo, path, pathDisco, superBloque, size, cont);
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
    this->recorrerRuta(carpetaIndirecta, path, pathDisco, superBloque, size, cont);
    return;
}

void archivo::crearArchivo(Structs::detalleDirectorio Archivos, int apuntador, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, int size, string cont)
{
    char uno = '1';
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");

    for(int i = 0; i<5; i++){

        int pointer = Archivos.archivos[i].noInodo;
        if(pointer == -1){ //Aqui escribo el archivo.

            Structs::detalleArchivo detalle;
            strcpy(detalle.date_creacion, this->getFecha().c_str());
            strcpy(detalle.date_modificacion, this->getFecha().c_str());
            detalle.noInodo = superBloque.first_free_inodo;
            strcpy(detalle.nombre_directorio, path[0].c_str());

            // Asigno el detalle de archivo al detalle de directorio
            Archivos.archivos[i] = detalle;

            // Escribimos el detalle de directorio
            // Nos posicionamos en el detalle de directorio
            fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntador * sizeof(Structs::detalleDirectorio))), SEEK_SET);
            fwrite(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

            int pointInode = superBloque.first_free_inodo;

            //Seteamos el inodo.
            Structs::InodoArchivo inodo;
            inodo.count_inodo = pointInode;
            inodo.size_archivo = size;
            int cantidadBloques = floor((double) size/25 +0.99);
            if(cantidadBloques <= 4){
                inodo.count_bloques_asignados = cantidadBloques;
            }else{
                inodo.count_bloques_asignados = 4;
            }

            superBloque.inodos_free--;
            superBloque.first_free_inodo++;

            // Escribimos en el bitmap de inodo.
            fseek(disco_actual, (superBloque.start_bm_inodos + inodo.count_inodo), SEEK_SET);
            fwrite(&uno, sizeof(char), 1, disco_actual);

            // Escribimos el SB
            fseek(disco_actual, (superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque)), SEEK_SET);
            fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

            fclose(disco_actual);
            this->crearInodo(inodo, path[0], pathDisco, superBloque, cont, cantidadBloques);
            return;
        }
    }

    // Si no lo encuentra se va al apuntador indirecto.
    int apuntadorA = Archivos.dd_siguiente;

    if(apuntadorA != -1){
        Structs::detalleDirectorio detalleIndirecto;
        // Nos posicionamos en la carpeta hija
        fseek(disco_actual, (superBloque.start_detalle_directorio + (apuntadorA * sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fread(&detalleIndirecto, sizeof(Structs::detalleDirectorio), 1, disco_actual);

        fclose(disco_actual);
        this->crearArchivo(detalleIndirecto, apuntadorA, path, pathDisco, superBloque, size, cont);
        return;

    }else{
        Structs::detalleDirectorio nuevoIndirecto;

        int posicion = superBloque.start_detalle_directorio + (superBloque.first_free_dd * sizeof(Structs::detalleDirectorio));
        apuntadorA = superBloque.first_free_dd;
        Archivos.dd_siguiente = superBloque.first_free_dd;

        // Escribimos el DD indirecto
        fseek(disco_actual, posicion, SEEK_SET);
        fwrite(&nuevoIndirecto, sizeof(Structs::detalleDirectorio), 1, disco_actual);
        // Escribimos en el bitmap dd
        // Ocupamos su posicion en bitmap.
        fseek(disco_actual, (superBloque.start_bm_detalle_directorio + superBloque.first_free_dd), SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);

        superBloque.detalle_directorio_free--;
        superBloque.first_free_dd++;

        // Escribimos el detalle de directorio raiz.
        // Nos posicionamos en el detalle de directorio.
        fseek(disco_actual, (superBloque.start_detalle_directorio+(apuntador*sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fwrite(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_actual);

        // Escribimos el SB
        fseek(disco_actual, (superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque)), SEEK_SET);
        fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

        fclose(disco_actual);
        this->crearArchivo(nuevoIndirecto, apuntadorA, path, pathDisco, superBloque, size, cont);
        return;
    }
}

void archivo::crearInodo(Structs::InodoArchivo inodo, string nombre_dir, string pathDisco, Structs::SuperBloque superBloque, string cont, int cantidadBloques)
{
    FILE *disco_actual = fopen(pathDisco.c_str(), "rb+");
    char uno = '1';
    string abc = "abcdefghijklmnopqrstuvwxy";
    for(int i = 0; i < inodo.count_bloques_asignados  ; i++){

        if(cont == ""){
            cont = abc;
        }

        //Creamos nuevo bloque.
        Structs::bloqueDatos block;
        strcpy(block.db_data, cont.substr(0, 25).c_str());

        if(cont.size() > 25){
            cont = cont.substr(25, cont.size());
        }else{
            cont = "";
        }

        inodo.array_bloques[i] = superBloque.first_free_bloque;

        // Escribimos el bloque.
        fseek(disco_actual, (superBloque.start_bloques + (superBloque.first_free_bloque * sizeof(Structs::bloqueDatos))), SEEK_SET);
        fwrite(&block, sizeof(Structs::bloqueDatos), 1, disco_actual);

        // Escribimos en el bitmap de bloques
        fseek(disco_actual, (superBloque.start_bm_bloques + superBloque.first_free_bloque), SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);

        // Escribimos el inodo.
        fseek(disco_actual, (superBloque.start_inodos + (inodo.count_inodo*(sizeof(Structs::InodoArchivo)))), SEEK_SET);
        fwrite(&inodo, sizeof(Structs::InodoArchivo), 1, disco_actual);

        superBloque.bloques_free--;
        superBloque.first_free_bloque++;
        // Escribimos el SB
        fseek(disco_actual, (superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque)), SEEK_SET);
        fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);
    }

    if(cantidadBloques > 4){
        cantidadBloques = cantidadBloques - 4;

        Structs::InodoArchivo inodoIndirecto;
        inodoIndirecto.count_inodo = superBloque.first_free_inodo;
        inodo.ap_indirecto = inodoIndirecto.count_inodo;
        inodoIndirecto.size_archivo = inodo.size_archivo;

        superBloque.inodos_free--;
        superBloque.first_free_inodo++;

        // Escribimos el inodo.
        fseek(disco_actual, (superBloque.start_inodos + (inodo.count_inodo*(sizeof(Structs::InodoArchivo)))), SEEK_SET);
        fwrite(&inodo, sizeof(Structs::InodoArchivo), 1, disco_actual);

        // Escribimos en el bitmap de inodo.
        fseek(disco_actual, (superBloque.start_bm_inodos + inodoIndirecto.count_inodo), SEEK_SET);
        fwrite(&uno, sizeof(char), 1, disco_actual);

        // Escribimos el SB
        fseek(disco_actual, (superBloque.start_bm_arbol_directorio - sizeof(Structs::SuperBloque)), SEEK_SET);
        fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

        if(cantidadBloques <= 4){
            inodoIndirecto.count_bloques_asignados = cantidadBloques;
        }else{
            inodoIndirecto.count_bloques_asignados = 4;
        }

        fclose(disco_actual);
        this->crearInodo(inodoIndirecto, nombre_dir, pathDisco, superBloque, cont, cantidadBloques);
        return;
    }

    fclose(disco_actual);
}
