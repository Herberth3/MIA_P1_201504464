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

    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";

    // Obtener todos los datos por referencia relacionados con la particion montanda en la cual se formateara el sistema de archivos
    this->getDatosID(this->identificador, this->montaje, &pathDisco_Particion, &startParticion, &sizeParticion, &error);

    if(error == 1){
        return;
    }

    /** SE FORMATEAN SOLO PRIMARIAS, LAS LOGICAS NO SE IMPLEMENTARON **/

    if (this->tipo_sistema == "3fs") {

        this->formatoEXT3(startParticion, sizeParticion, pathDisco_Particion);
    } else {

        this->formatoEXT2(startParticion, sizeParticion, pathDisco_Particion);
    }

}

void Mkfs::getDatosID(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, int *error)
{
    QString identificador = id; // Ejemplo: 6413Disco1
    identificador.remove(0, 2); // Ejemplo: 13Disco1

    QRegularExpressionMatch match;
    identificador.indexOf(QRegularExpression("[0-9]+"), 0, &match); // Captura 13 en match
    int indice = identificador.indexOf(QRegularExpression("[A-Za-z]"), 0); // Indice en donde empieza el nombre (una letra)
    int numeroInt =  match.captured().toInt(); // 13
    identificador.remove(0, indice); // Disco1
    string nombre_disco = identificador.toStdString(); // Disco1

    bool existePath = false;
    char pathDisco[100] = "";
    int sizeParticion = -1;
    int startParticion = -1;
    string nombreParticion = "";

     //Obtener el nombre de la particion y el path del disco.
    for (int i = 0; i < 26; i++){

        if (strcmp(mount.discos[i].nombre, nombre_disco.c_str()) == 0){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].numero == numeroInt && mount.discos[i].particiones[j].estado == 1){

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

            if (strcmp(particion.part_name, nombreParticion.c_str()) == 0) {

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

    *error = 0;
    *path = string(pathDisco);
    *part_starParticion = startParticion;
    *part_sizeParticion = sizeParticion;
}

/** Metodo encargado de formatear una particion con formato EXT2
 * @param int part_start: Byte donde inicia la particion en el disco
 * @param int part_size: Tamano de la particion
 * @param string path: ruta del disco
**/
void Mkfs::formatoEXT2(int part_start, int part_size, string path)
{
    double n = (part_size - static_cast<int>(sizeof(SuperBloque)))/(4 + static_cast<int>(sizeof(InodoTable)) +3*static_cast<int>(sizeof(BloqueArchivo)));
    int num_estructuras = static_cast<int>(floor(n));//Numero de inodos
    int num_bloques = 3 * num_estructuras;

    string fechaActual = this->getFecha();

    SuperBloque super;
    super.s_filesystem_type = 2;
    super.s_inodes_count = num_estructuras;
    super.s_blocks_count = num_bloques;
    super.s_free_blocks_count = num_bloques -2;
    super.s_free_inodes_count = num_estructuras -2;
    strcpy(super.s_mtime, fechaActual.c_str());
    strcpy(super.s_umtime, fechaActual.c_str());
    //super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(InodoTable);
    super.s_block_size = sizeof(BloqueArchivo);
    super.s_first_ino = 2;
    super.s_first_blo = 2;
    super.s_bm_inode_start = part_start + static_cast<int>(sizeof(SuperBloque));
    super.s_bm_block_start = part_start + static_cast<int>(sizeof(SuperBloque)) + num_estructuras;
    super.s_inode_start = part_start + static_cast<int>(sizeof (SuperBloque)) + num_estructuras + num_bloques;
    super.s_block_start = part_start + static_cast<int>(sizeof(SuperBloque)) + num_estructuras + num_bloques + (static_cast<int>(sizeof(InodoTable)) * num_estructuras);

    InodoTable inodo;
    BloqueCarpeta bloque;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *disco_actual = fopen(path.c_str(),"rb+");

    /*-------------------SUPERBLOQUE------------------*/
    // Se escribe el SuperBloque al inicio de la particion
    fseek(disco_actual, part_start, SEEK_SET);
    fwrite(&super, sizeof(SuperBloque), 1, disco_actual);
    /*----------------BITMAP DE INODOS----------------*/
    //SB-BMI  se rellena de 0's
    for(int i = 0; i < num_estructuras; i++){
        fseek(disco_actual, super.s_bm_inode_start + i,SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_actual);
    }
    /*----------bit para / y users.txt en BM----------*/
    fseek(disco_actual, super.s_bm_inode_start, SEEK_SET);
    fwrite(&buffer2, sizeof(char), 1, disco_actual);
    fwrite(&buffer2, sizeof(char), 1, disco_actual);
    /*---------------BITMAP DE BLOQUES----------------*/
    // SB-BMI-BMB  se rellena de 0's
    for(int i = 0; i < num_bloques; i++){
        fseek(disco_actual, super.s_bm_block_start + i, SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_actual);
    }
    /*----------bit para / y users.txt en BM----------*/
    fseek(disco_actual,super.s_bm_block_start,SEEK_SET);
    fwrite(&buffer2,sizeof(char),1,disco_actual);
    fwrite(&buffer3,sizeof(char),1,disco_actual);
    /*------------inodo para carpeta root-------------*/
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

    fseek(disco_actual, super.s_inode_start, SEEK_SET);
    fwrite(&inodo, sizeof(InodoTable), 1, disco_actual);

    /*------------Bloque para carpeta root------------*/
    strcpy(bloque.b_content[0].b_name, ".");//Actual (el mismo)
    bloque.b_content[0].b_inodo = 0;

    strcpy(bloque.b_content[1].b_name, "..");//Padre
    bloque.b_content[1].b_inodo = 0;

    strcpy(bloque.b_content[2].b_name, "users.txt");
    bloque.b_content[2].b_inodo = 1;

    strcpy(bloque.b_content[3].b_name, ".");
    bloque.b_content[3].b_inodo = -1;

    fseek(disco_actual, super.s_block_start, SEEK_SET);
    fwrite(&bloque, sizeof(BloqueCarpeta), 1, disco_actual);

    /*-------------inodo para users.txt-------------*/
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 27;
    strcpy(inodo.i_atime, fechaActual.c_str());
    strcpy(inodo.i_ctime, fechaActual.c_str());
    strcpy(inodo.i_mtime, fechaActual.c_str());
    inodo.i_block[0] = 1;
    for(int i = 1; i < 15;i++){
        inodo.i_block[i] = -1;
    }
    inodo.i_type = '1';
    inodo.i_perm = 755;

    fseek(disco_actual, super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fwrite(&inodo,sizeof(InodoTable),1,disco_actual);

    /*-------------Bloque para users.txt------------*/
    BloqueArchivo archivo;
    memset(archivo.b_content, 0, sizeof(archivo.b_content));
    strcpy(archivo.b_content, "1,G,root\n1,U,root,root,123\n");

    fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)), SEEK_SET);
    fwrite(&archivo, sizeof(BloqueArchivo), 1, disco_actual);

    cout << "EXT2" << endl;
    cout << "..." << endl;
    cout << "Disco formateado con exito" << endl;

    fclose(disco_actual);
}

/** Metodo encargado de formatear una particion con formato EXT3
 * @param int part_start: Byte donde inicia la particion en el disco
 * @param int part_size: Tamano de la particion
 * @param string path: ruta del disco
**/
void Mkfs::formatoEXT3(int part_start, int part_size, string path)
{
    double n = (part_size - static_cast<int>(sizeof(SuperBloque)))/(4 + static_cast<int>(sizeof(InodoTable)) +3*static_cast<int>(sizeof(BloqueArchivo)));
    int num_estructuras = static_cast<int>(floor(n));//Bitmap indos
    int num_bloques = 3 * num_estructuras;//Bitmap bloques
    int super_size = static_cast<int>(sizeof(SuperBloque));
    int journal_size = static_cast<int>(sizeof(Journal)) * num_estructuras;

    string fechaActual = this->getFecha();

    SuperBloque super;
    super.s_filesystem_type = 3;
    super.s_inodes_count = num_estructuras;
    super.s_blocks_count = num_bloques;
    super.s_free_blocks_count = num_bloques - 2;
    super.s_free_inodes_count = num_estructuras - 2;
    strcpy(super.s_mtime, fechaActual.c_str());
    strcpy(super.s_umtime, fechaActual.c_str());
    //super.s_umtime = 0;
    super.s_mnt_count = 0;
    super.s_magic = 0xEF53;
    super.s_inode_size = sizeof(InodoTable);
    super.s_block_size = sizeof(BloqueArchivo);
    super.s_first_ino = 2;
    super.s_first_blo = 2;
    super.s_bm_inode_start = part_start + super_size + journal_size;
    super.s_bm_block_start = part_start + super_size + journal_size + num_estructuras;
    super.s_inode_start = part_start + super_size + journal_size + num_estructuras + num_bloques;
    super.s_block_start = part_start + super_size + journal_size + num_estructuras + num_bloques + static_cast<int>(sizeof(InodoTable)) * num_estructuras;

    InodoTable inodo;
    BloqueCarpeta bloque;

    char buffer = '0';
    char buffer2 = '1';
    char buffer3 = '2';

    FILE *disco_actual = fopen(path.c_str(),"rb+");

    /*-------------------SUPERBLOQUE------------------*/
    fseek(disco_actual, part_start, SEEK_SET);
    fwrite(&super, sizeof(SuperBloque), 1, disco_actual);
    /*----------------BITMAP DE INODOS----------------*/
    for(int i = 0; i < num_estructuras; i++){
        fseek(disco_actual, super.s_bm_inode_start + i, SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_actual);
    }
    /*----------bit para / y users.txt en BM----------*/
    fseek(disco_actual, super.s_bm_inode_start, SEEK_SET);
    fwrite(&buffer2, sizeof(char), 1, disco_actual);
    fwrite(&buffer2, sizeof(char), 1, disco_actual);
    /*---------------BITMAP DE BLOQUES----------------*/
    for(int i = 0; i < num_bloques; i++){
        fseek(disco_actual, super.s_bm_block_start + i, SEEK_SET);
        fwrite(&buffer, sizeof(char), 1, disco_actual);
    }
    /*----------bit para / y users.txt en BM----------*/
    fseek(disco_actual, super.s_bm_block_start, SEEK_SET);
    fwrite(&buffer2, sizeof(char), 1, disco_actual);
    fwrite(&buffer3, sizeof(char), 1, disco_actual);
    /*------------inodo para carpeta root-------------*/
    inodo.i_uid = 1;
    inodo.i_gid = 1;
    inodo.i_size = 0;
    strcpy(inodo.i_atime, fechaActual.c_str());
    strcpy(inodo.i_ctime, fechaActual.c_str());
    strcpy(inodo.i_mtime, fechaActual.c_str());
    inodo.i_block[0] = 0;
    for(int i = 1; i < 15; i++){
        inodo.i_block[i] = -1;
    }
    inodo.i_type = '0';
    inodo.i_perm = 664;

    fseek(disco_actual, super.s_inode_start, SEEK_SET);
    fwrite(&inodo, sizeof(InodoTable), 1, disco_actual);

    /*------------Bloque para carpeta root------------*/
    strcpy(bloque.b_content[0].b_name, ".");//Actual
    bloque.b_content[0].b_inodo = 0;

    strcpy(bloque.b_content[1].b_name, "..");//Padre
    bloque.b_content[1].b_inodo = 0;

    strcpy(bloque.b_content[2].b_name, "users.txt");
    bloque.b_content[2].b_inodo = 1;

    strcpy(bloque.b_content[3].b_name, ".");
    bloque.b_content[3].b_inodo = -1;

    fseek(disco_actual, super.s_block_start, SEEK_SET);
    fwrite(&bloque, sizeof(BloqueCarpeta), 1, disco_actual);

    /*-------------inodo para users.txt-------------*/
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

    fseek(disco_actual, super.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fwrite(&inodo, sizeof(InodoTable), 1, disco_actual);

    /*-------------Bloque para users.txt------------*/
    BloqueArchivo archivo;
    memset(archivo.b_content, 0, sizeof(archivo.b_content));
    strcpy(archivo.b_content, "1,G,root\n1,U,root,root,123\n");
    fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)),SEEK_SET);
    fwrite(&archivo, sizeof(BloqueArchivo), 1, disco_actual);

    cout << "EXT3" << endl;
    cout << "..." << endl;
    cout << "Disco formateado con exito" << endl;

    fclose(disco_actual);
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
