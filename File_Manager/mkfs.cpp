#include "mkfs.h"

Mkfs::Mkfs()
{

}

void Mkfs::Ejecutar(QString id, QString type, QString fs, Mount mount)
{
    this->identificador = id;
    this->tipo_formateo = type;
    this->tipo_sistema = fs.toLower();
    this->montaje = mount;

    int num_Estructuras = -1;
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nameParticion;

    // Obtener todos los datos por referencia relacionados con la particion montanda en la cual se formateara el sistema de archivos
    this->getDatosID(this->identificador, this->montaje, &pathDisco_Particion, &startParticion, &sizeParticion, &nameParticion, &error);

    if(error == 1){
        return;
    }

    // Formula para el numero de estructuras
    num_Estructuras = (sizeParticion - (2 * sizeof(Structs::SuperBloque))) /
            (27 + sizeof(Structs::arbolVirtual) + sizeof(Structs::detalleDirectorio) + 5 * (sizeof(Structs::InodoArchivo)) + 20 * (sizeof(Structs::bloqueDatos)) + sizeof(Structs::logBitacora));

    // AVD: Arbol virtual directorio; DD: Detalle directorio
    int total_AVD = num_Estructuras;
    int total_DD = num_Estructuras;
    int total_Inodos = 5 * num_Estructuras;
    int total_Bloques = 20 * num_Estructuras;
    string fechaActual = this->getFecha();

    // Inicializar Super Bloque
    // Dar formato, asignando el espacio que ocupara cada estructura en la particion
    Structs::SuperBloque superBloque;

    strcpy(superBloque.nombre_particion, nameParticion.c_str());
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
    //superBloque.montajes_count = 0;
    superBloque.start_bm_arbol_directorio = startParticion + sizeof(Structs::SuperBloque); // SB -> BMAVD
    superBloque.start_arbol_directorio = superBloque.start_bm_arbol_directorio + num_Estructuras; // SB -> BMAVD -> TAVD
    superBloque.start_bm_detalle_directorio = superBloque.start_arbol_directorio + (num_Estructuras*sizeof(Structs::arbolVirtual)); // SB -> BMAVD -> TAVD -> BMDD
    superBloque.start_detalle_directorio = superBloque.start_bm_detalle_directorio + num_Estructuras; // SB -> BMAVD -> TAVD -> BMDD -> TDD
    superBloque.start_bm_inodos = superBloque.start_detalle_directorio + (num_Estructuras * sizeof(Structs::detalleDirectorio)); // SB -> BMAVD -> TAVD -> BMDD -> TDD -> BMI
    superBloque.start_inodos = superBloque.start_bm_inodos + (5*num_Estructuras);
    superBloque.start_bm_bloques = superBloque.start_inodos + (5*num_Estructuras*sizeof(Structs::InodoArchivo));
    superBloque.start_bloques = superBloque.start_bm_bloques + (20*num_Estructuras);
    superBloque.start_log = superBloque.start_bloques + (20*num_Estructuras*sizeof(Structs::bloqueDatos)); //Bitacora.
    //superBloque.first_free_avd = 0;
    //superBloque.first_free_dd = 0;
    //superBloque.first_free_inodo = 0;
    //superBloque.first_free_bloque = 0;

    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    fseek(disco_actual, startParticion, SEEK_SET);
    // Se escribe el superbloque al inicio de la particion: P -> SB
    fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    // DANDO FORMATO AL ESPACIO EN DISCO DESDE EL PART_START DE LA PARTICION
    // Escribiendo el Bit Map de Arbol Directorio, llenandolo de "0"
    char cero = '0';
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_arbol_directorio + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Escribiendo arboles virtuales en el disco
    Structs::arbolVirtual avd;
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_arbol_directorio + (i * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fwrite(&avd, sizeof(Structs::arbolVirtual), 1, disco_actual);
    }

    // Escribiendo el Bit Map de Detalle Directorio, llenandolo de "0"
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_detalle_directorio + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Escribiendo estructuras Detalle Directorio en el disco
    Structs::detalleDirectorio dd;
    for(int i = 0; i< num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_detalle_directorio + (i * sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fwrite(&dd, sizeof(Structs::detalleDirectorio), 1, disco_actual);
    }

    // Escribiendo el BIT MAP de INODOS, llenandolo de "0"
    for(int i = 0; i < (5*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bm_inodos + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Escribiendo tabla de Inodos struct en el disco
    Structs::InodoArchivo inodo;
    for(int i = 0; i < (5*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_inodos + (i * sizeof(Structs::InodoArchivo))), SEEK_SET);
        fwrite(&inodo, sizeof(Structs::InodoArchivo), 1, disco_actual);
    }

    // Escribiendo el BIT MAP de BLOQUES, llenandolo de "0"
    for(int i = 0; i < (20*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bm_bloques + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Escribiendo la tabla de Bloques struct en el disco
    Structs::bloqueDatos bloque;
    for(int i = 0; i < (20*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bloques + (i * sizeof(Structs::bloqueDatos))), SEEK_SET);
        fwrite(&bloque, sizeof(Structs::bloqueDatos), 1, disco_actual);
    }

    // Escribiendo bitacora struct en el disco
    Structs::logBitacora log;
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_log + (i * sizeof(Structs::logBitacora))), SEEK_SET);
        fwrite(&log, sizeof(Structs::logBitacora), 1, disco_actual);
    }

    //SET LA RAIZ DE LAS BITACORAS.
    log.size = 1;
    fseek(disco_actual, (superBloque.start_log), SEEK_SET);
    fwrite(&log, sizeof(Structs::logBitacora), 1, disco_actual);

    int start_copia_sb = superBloque.start_log + (num_Estructuras * sizeof(Structs::logBitacora));
    fseek(disco_actual, start_copia_sb, SEEK_SET); //copia super bloque
    fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    fclose(disco_actual);

    // Crear carpeta Raiz
    carpeta folder;
    folder.mkDir("/", 0, this->identificador, this->montaje, false);

    /*
    // Formula para el numero de estructuras
    // EXT2 y EXT3
    double n = (sizeParticion - static_cast<int>(sizeof(SuperBloque))) / (4 + static_cast<int>(sizeof(InodoTable)) + 3*static_cast<int>(sizeof(BloqueArchivo)));
    int num_estructuras = static_cast<int>(floor(n));//Numero de inodos
    int num_bloques = 3 * num_estructuras;
    // EXT3
    int super_size = static_cast<int>(sizeof(SuperBloque));
    int journal_size = static_cast<int>(sizeof(Journal)) * num_estructuras;
    string fechaActual = this->getFecha();

    // EXT2 y EXT3 Inicializando el Super Bloque
    SuperBloque superBloque;
    superBloque.s_inodes_count = num_estructuras;
    superBloque.s_blocks_count = num_bloques;
    superBloque.s_free_blocks_count = num_bloques -2;
    superBloque.s_free_inodes_count = num_estructuras -2;
    strcpy(superBloque.s_mtime, fechaActual.c_str());
    strcpy(superBloque.s_umtime, "");
    superBloque.s_mnt_count = 0;
    superBloque.s_magic = 0xEF53;
    superBloque.s_inode_size = sizeof(InodoTable);
    superBloque.s_block_size = sizeof(BloqueArchivo);
    superBloque.s_first_ino = 2;
    superBloque.s_first_blo = 2;
    if (this->tipo_sistema == "2fs") {
        // EXT2
        superBloque.s_filesystem_type = 2;
        superBloque.s_bm_inode_start = startParticion + static_cast<int>(sizeof(SuperBloque));
        superBloque.s_bm_block_start = startParticion + static_cast<int>(sizeof(SuperBloque)) + num_estructuras;
        superBloque.s_inode_start = startParticion + static_cast<int>(sizeof (SuperBloque)) + num_estructuras + num_bloques;
        superBloque.s_block_start = startParticion + static_cast<int>(sizeof(SuperBloque)) + num_estructuras + num_bloques + (static_cast<int>(sizeof(InodoTable))*num_estructuras);
    } else {
        // EXT3
        superBloque.s_filesystem_type = 3;
        superBloque.s_bm_inode_start = startParticion + super_size + journal_size;
        superBloque.s_bm_block_start = startParticion + super_size + journal_size + num_estructuras;
        superBloque.s_inode_start = startParticion + super_size + journal_size + num_estructuras + num_bloques;
        superBloque.s_block_start = startParticion + super_size + journal_size + num_estructuras + num_bloques + static_cast<int>(sizeof(InodoTable))*num_estructuras;
    }

    // EXT2 y EXT3
    InodoTable inodo;
    BloqueCarpeta bloque;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *disco_Actual = fopen(pathDisco_Particion.c_str(),"rb+");

    //-------------------SUPERBLOQUE------------------
    fseek(disco_Actual, startParticion, SEEK_SET);
    fwrite(&superBloque, sizeof(SuperBloque), 1, disco_Actual);

    //----------------BITMAP DE INODOS----------------
    for(int i = 0; i < num_estructuras; i++){
        fseek(disco_Actual, superBloque.s_bm_inode_start + i, SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_Actual);
    }
    //----------bit para / y users.txt en BM----------
    fseek(disco_Actual, superBloque.s_bm_inode_start, SEEK_SET);
    fwrite(&buffer2, sizeof(char), 1, disco_Actual);
    fwrite(&buffer2, sizeof(char), 1, disco_Actual);

    //---------------BITMAP DE BLOQUES----------------
    for(int i = 0; i < num_bloques; i++){
        fseek(disco_Actual, superBloque.s_bm_block_start + i, SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_Actual);
    }
    //----------bit para / y users.txt en BM----------
    fseek(disco_Actual, superBloque.s_bm_block_start, SEEK_SET);
    fwrite(&buffer2, sizeof(char), 1, disco_Actual);
    fwrite(&buffer3, sizeof(char), 1, disco_Actual);

    //------------inodo para carpeta root-------------
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 0;
    strcpy(inodo.i_atime, fechaActual.c_str());
    strcpy(inodo.i_ctime, fechaActual.c_str());
    strcpy(inodo.i_mtime, fechaActual.c_str());
    inodo.i_block[0] = 0;
    for(int i = 1; i < 15;i++)
        inodo.i_block[i] = -1;
    inodo.i_type = '0';
    inodo.i_perm = 664;

    fseek(disco_Actual, superBloque.s_inode_start, SEEK_SET);
    fwrite(&inodo, sizeof(InodoTable), 1, disco_Actual);

    //------------Bloque para carpeta root------------
    strcpy(bloque.b_content[0].b_name, ".");//Actual (el mismo)
    bloque.b_content[0].b_inodo = 0;

    strcpy(bloque.b_content[1].b_name, "..");//Padre
    bloque.b_content[1].b_inodo = 0;

    strcpy(bloque.b_content[2].b_name, "users.txt");
    bloque.b_content[2].b_inodo = 1;

    strcpy(bloque.b_content[3].b_name, ".");
    bloque.b_content[3].b_inodo = -1;

    fseek(disco_Actual, superBloque.s_block_start, SEEK_SET);
    fwrite(&bloque, sizeof(BloqueCarpeta), 1, disco_Actual);

    //------------inodo para users.txt-------------
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 27;
    strcpy(inodo.i_atime, fechaActual.c_str());
    strcpy(inodo.i_ctime, fechaActual.c_str());
    strcpy(inodo.i_mtime, fechaActual.c_str());
    inodo.i_block[0] = 1;
    for(int i = 1; i < 15; i++){
        inodo.i_block[i] = -1;
    }
    inodo.i_type = '1';
    inodo.i_perm = 755;
    fseek(disco_Actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fwrite(&inodo, sizeof(InodoTable), 1, disco_Actual);

    //-------------Bloque para users.txt------------
    BloqueArchivo archivo;
    memset(archivo.b_content, 0, sizeof(archivo.b_content));
    strcpy(archivo.b_content, "1,G,root\n1,U,root,root,123\n");
    fseek(disco_Actual, superBloque.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)), SEEK_SET);
    fwrite(&archivo, sizeof(BloqueArchivo), 1, disco_Actual);

    if (this->tipo_sistema == "2fs") {
        cout << "EXT2" << endl;
    } else {
        cout << "EXT3" << endl;
    }
    cout << "..." << endl;
    cout << "Disco formateado con exito" << endl;

    fclose(disco_Actual);*/
}

void Mkfs::getDatosID(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int *error)
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
