#include "recovery.h"

Recovery::Recovery()
{

}

void Recovery::Ejecutar(QString id, Mount mount)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nombreParticion;

    this->getDatosParticionMontada(id, mount, &pathDisco_Particion, &startParticion, &sizeParticion, &nombreParticion, &error);

    if(error == 1){
        return;
    }

    int nEstructuras = (sizeParticion - (2 * sizeof(Structs::SuperBloque))) /
            (27 + sizeof(Structs::arbolVirtual)+ sizeof(Structs::detalleDirectorio) + 5 * (sizeof(Structs::InodoArchivo)) + 20 * (sizeof(Structs::bloqueDatos)) + sizeof(Structs::logBitacora));


    Structs::SuperBloque superBloque;
    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    fseek(disco_actual, startParticion, SEEK_SET);
    // Leo el superbloque al inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    int total_AVD = nEstructuras;
    int total_DD = nEstructuras;
    int total_Inodos = 5*nEstructuras;
    int total_Bloques = 20*nEstructuras;
    string fechaActual = this->getFecha();

    // FORMATEAR, ASIGNAR ESPACIO A CADA SECTOR DE LA PARTICION
    // ASIGNAR SUPER BLOQUE.
    strcpy(superBloque.nombre_particion, nombreParticion.c_str());
    superBloque.arbol_virtual_count = total_AVD;
    superBloque.detalle_directorio_count = total_DD;
    superBloque.inodos_count = total_Inodos;
    superBloque.bloques_count = total_Bloques;
    superBloque.arbol_virtual_free = total_AVD; // Se crea carpeta raiz.
    superBloque.detalle_directorio_free = total_DD; // Detalle de directorio de la carpeta raiz
    superBloque.inodos_free = total_Inodos;
    superBloque.bloques_free = total_Bloques;
    strcpy(superBloque.date_creacion, fechaActual.c_str());
    strcpy(superBloque.date_ultimo_montaje, fechaActual.c_str());
    superBloque.montajes_count = 0;
    superBloque.start_bm_arbol_directorio = startParticion + sizeof(Structs::SuperBloque);
    superBloque.start_arbol_directorio = superBloque.start_bm_arbol_directorio + nEstructuras;
    superBloque.start_bm_detalle_directorio = superBloque.start_arbol_directorio + (nEstructuras*sizeof(Structs::arbolVirtual));
    superBloque.start_detalle_directorio = superBloque.start_bm_detalle_directorio + nEstructuras;
    superBloque.start_bm_inodos = superBloque.start_detalle_directorio + (nEstructuras * sizeof(Structs::detalleDirectorio));
    superBloque.start_inodos = superBloque.start_bm_inodos + (5*nEstructuras);
    superBloque.start_bm_bloques = superBloque.start_inodos + (5*nEstructuras*sizeof(Structs::InodoArchivo));
    superBloque.start_bloques = superBloque.start_bm_bloques + (20*nEstructuras);
    superBloque.start_log = superBloque.start_bloques + (20*nEstructuras*sizeof(Structs::bloqueDatos)); //Bitacora.
    superBloque.first_free_avd = 0;
    superBloque.first_free_dd = 0;
    superBloque.first_free_inodo = 0;
    superBloque.first_free_bloque = 0;

    Structs::logBitacora BitacoraRaiz;
    fseek(disco_actual, superBloque.start_log, SEEK_SET);
    fread(&BitacoraRaiz, sizeof(Structs::logBitacora), 1, disco_actual);

    fseek(disco_actual, startParticion, SEEK_SET);
    // Leo el superbloque al inicio de la particion
    fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    fclose(disco_actual);

    Structs::logBitacora bitacora;

    for(int i = 1; i< BitacoraRaiz.size; i++){

        FILE *disco_actual1 = fopen(pathDisco_Particion.c_str(), "rb+");
        fseek(disco_actual1, superBloque.start_log + (i * sizeof(Structs::logBitacora)), SEEK_SET);
        fread(&bitacora, sizeof(Structs::logBitacora), 1, disco_actual1);
        fclose(disco_actual1);

        if(string(bitacora.tipo_operacion) ==  "mkdir"){
            carpeta folder;
            QString path_bitacora = QString::fromStdString(string(bitacora.path));
            folder.mkDir(path_bitacora, bitacora.size, id, mount, true);

        }else if(string(bitacora.tipo_operacion) ==  "mkfile"){
            archivo file;
            QString path_bitacora = QString::fromStdString(string(bitacora.path));
            QString cont_bitacora = QString::fromStdString(string(bitacora.contenido));
            file.mkFile(id, path_bitacora, 1, bitacora.size, cont_bitacora, mount, true);
        }else{
            cout<<"Comando de operacion no reconocido"<<endl;
        }
    }

    cout << "Recuperacion del sistema exitosa." << endl;
}

void Recovery::getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int *error)
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

string Recovery::getFecha()
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
