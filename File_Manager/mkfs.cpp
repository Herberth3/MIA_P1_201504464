#include "mkfs.h"

Mkfs::Mkfs()
{

}

void Mkfs::Ejecutar(QString id, QString type, QString fs, Mount mount)
{
    this->identificador = id;
    this->tipo_formateo = type;
    this->tipo_sistema = fs;
    this->montaje = mount;

    int num_Estructuras = -1;
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathD= "";
    string nombreParticion;

    this->getDatosID(this->identificador.toStdString(), this->montaje, &pathD, &startParticion, &sizeParticion, &nombreParticion, &error);

    if(error == 1){
        return;
    }

    char pathDisco[100] = "";
    strcpy(pathDisco,  pathD.c_str());

    // Formula
    num_Estructuras = (sizeParticion - (2*sizeof(SuperBloque)))/
            (27+sizeof(arbolVirtual)+ sizeof(detalleDirectorio) +
             5*(sizeof(InodoArchivo)) + 20*(sizeof(bloqueDatos)) + sizeof(logBitacora));

    int noAVD = num_Estructuras;
    int noDD = num_Estructuras;
    int noInodos = 5 * num_Estructuras;
    int noBloques = 20 * num_Estructuras;

    string fechaActual = this->getFecha();

    //FORMATEAR, ASIGNAR ESPACIO A CADA SECTOR DE LA PARTICION
    //ASIGNAR SUPER BLOQUE.
    SuperBloque superBloque;
    strcpy(superBloque.nombre_hd, nombreParticion.c_str());
    superBloque.arbol_virtual_count = noAVD;
    superBloque.detalle_directorio_count = noDD;
    superBloque.inodos_count = noInodos;
    superBloque.bloques_count = noBloques;
    superBloque.arbol_virtual_free = noAVD; //Se crea carpeta raiz.
    superBloque.detalle_directorio_free = noDD; //Detalle de directorio de la carpeta raiz
    superBloque.inodos_free = noInodos;
    superBloque.bloques_free = noBloques;
    strcpy(superBloque.date_creacion, fechaActual.c_str());
    strcpy(superBloque.date_ultimo_montaje, fechaActual.c_str());
    superBloque.montajes_count = 0;
    superBloque.start_bm_arbol_directorio = startParticion + sizeof(SuperBloque);
    superBloque.start_arbol_directorio = superBloque.start_bm_arbol_directorio + num_Estructuras;
    superBloque.start_bm_detalle_directorio = superBloque.start_arbol_directorio + (num_Estructuras*sizeof(arbolVirtual));
    superBloque.start_detalle_directorio = superBloque.start_bm_detalle_directorio + num_Estructuras;
    superBloque.start_bm_inodos = superBloque.start_detalle_directorio + (num_Estructuras * sizeof(detalleDirectorio));
    superBloque.start_inodos = superBloque.start_bm_inodos + (5*num_Estructuras);
    superBloque.start_bm_bloques = superBloque.start_inodos + (5*num_Estructuras*sizeof(InodoArchivo));
    superBloque.start_bloques = superBloque.start_bm_bloques + (20*num_Estructuras);
    superBloque.start_log = superBloque.start_bloques + (20*num_Estructuras*sizeof(bloqueDatos)); //Bitacora.
    superBloque.first_free_avd = 0;
    superBloque.first_free_dd = 0;
    superBloque.first_free_inodo = 0;
    superBloque.first_free_bloque = 0;

    FILE *disco_actual = fopen(pathDisco, "rb+");
    fseek(disco_actual, startParticion, SEEK_SET);
    fwrite(&superBloque, sizeof(SuperBloque), 1, disco_actual); //Escribo el superbloque al inicio de la particion

    //FORMATEO.
    //fseek(bfile, superBloque.start_bm_arbol_directorio, SEEK_SET); //bitmap de avd
    char cero = '0';
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_arbol_directorio + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    arbolVirtual avd;
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_arbol_directorio + (i * sizeof(arbolVirtual))), SEEK_SET);//avd
        fwrite(&avd, sizeof(arbolVirtual), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_bm_detalle_directorio, SEEK_SET); //bitmap dd
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_detalle_directorio + i), SEEK_SET);//avd
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_detalle_directorio, SEEK_SET); // dd
    detalleDirectorio dd;
    for(int i = 0; i<num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_detalle_directorio + (i * sizeof(detalleDirectorio))), SEEK_SET);
        fwrite(&dd, sizeof(detalleDirectorio), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_bm_inodos, SEEK_SET); //bitmap inodos
    for(int i = 0; i < (5*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bm_inodos + i), SEEK_SET);//avd
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_inodos, SEEK_SET); //inodos
    InodoArchivo inodo;
    for(int i = 0; i < (5*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_inodos + (i * sizeof(InodoArchivo))), SEEK_SET);
        fwrite(&inodo, sizeof(InodoArchivo), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_bm_bloques, SEEK_SET); //bitmap bloques
    for(int i = 0; i < (20*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bm_bloques + i), SEEK_SET);//avd
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_bloques, SEEK_SET); //bloques
    bloqueDatos bloque;
    for(int i = 0; i < (20*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bloques + (i * sizeof(bloqueDatos))), SEEK_SET);
        fwrite(&bloque, sizeof(bloqueDatos), 1, disco_actual);
    }

    //fseek(bfile, superBloque.start_log, SEEK_SET); //bitacora
    logBitacora log;
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_log + (i * sizeof(logBitacora))), SEEK_SET);
        fwrite(&log, sizeof(logBitacora), 1, disco_actual);
    }

    //SET LA RAIZ DE LAS BITACORAS.
    log.size = 1;
    fseek(disco_actual, (superBloque.start_log), SEEK_SET);
    fwrite(&log, sizeof(logBitacora), 1, disco_actual);

    int start_copia_sb = superBloque.start_log + (num_Estructuras * sizeof(logBitacora));
    fseek(disco_actual, start_copia_sb, SEEK_SET); //copia super bloque
    fwrite(&superBloque, sizeof(SuperBloque), 1, disco_actual);

    fclose(disco_actual);

    // Crear carpeta Raiz
    //carpeta folder;
    //folder.makeDirectory("/", 0, this->identificador, this->montaje, false);
}

void Mkfs::getDatosID(string id, Mount mount, string *path, int *part_starParticion, int *part_size, string *part_name, int *error)
{
    //Obtener particion y su size.
    char letra = id.c_str()[2];
    char numero = id.c_str()[3];
    bool existePath = false;
    int numeroInt = (int)numero - 48;
    string nombreParticion = "";

    char pathDisco[100] = "";

    for (int i = 0; i < 26; i++)
    {
        if (mount.discos[i].letra == letra && mount.discos[i].particiones[numeroInt-1].estado == 1)
        {
            strcpy(pathDisco, mount.discos[i].path);
            nombreParticion = mount.discos[i].particiones[numeroInt-1].nombre;
            existePath = true;
            break;
        }
    }

    if (!existePath)
    {
        cout << "Error: ID no existe, path no existente." << endl;
        return;
    }
    int sizeParticion = -1;
    int startParticion = -1;

    FILE *disco_actual = fopen(pathDisco, "rb+");
    if(disco_actual != NULL){

        rewind(disco_actual);
        MBR mbr_auxiliar;
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        for (Partition particion : mbr_auxiliar.mbr_partition) {

            if (particion.part_name == nombreParticion) {

                sizeParticion = particion.part_size;
                startParticion = particion.part_start;
            }
        }

        fclose(disco_actual);
    }else{
        cout << "Error. Path no existente." << endl;
        *error = 1;
        return;
    }

    *part_name = string(nombreParticion);
    *error = 0;
    *path = string(pathDisco);
    *part_starParticion = startParticion;
    *part_size = sizeParticion;
}

string Mkfs::getFecha()
{
    //fecha
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    string fechaActual(buffer);
    return fechaActual;
}
