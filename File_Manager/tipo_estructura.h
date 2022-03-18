#ifndef TIPO_ESTRUCTURA_H
#define TIPO_ESTRUCTURA_H
#include "QString"

typedef struct {
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}Partition;

typedef struct {
    int mbr_tamano;
    char mbr_fecha_creacion[20];
    int mbr_disk_signature;
    char disk_fit;
    Partition mbr_partition[4];
}MBR;

typedef struct{
    char part_status;
    char part_fit;
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

/*
// STRUCTS PARA EL SISTEMA DE ARCHIVOS

typedef struct{
    int s_filesystem_type; //Guarda el numero que identifica al sistea de archivos utilizados
    int s_inodes_count; //Guarda el numero total de inodos
    int s_blocks_count; //Guarda el numero total de bloques
    int s_free_blocks_count; //Contiene el numero de bloques libres
    int s_free_inodes_count; //Contiene el numero inodos libres
    char s_mtime[20]; //Ultima fecha en el que el sistema fue montado
    char s_umtime[20]; //Ultima fecha en que el sistema fue desmontado
    int s_mnt_count; //Indica cuantas veces se ha montado el sistema
    int s_magic; //Valor que identifica al sistema de archivos 0xEF53
    int s_inode_size; //Tamano del inodo
    int s_block_size; //Tamano del bloque
    int s_first_ino; //Primer inodo libre
    int s_first_blo; //Primero bloque libre
    int s_bm_inode_start; //Guardara el inicio del bitmap de inodos
    int s_bm_block_start; //Guardara el inicio del bitmap de bloques
    int s_inode_start; //Guarada el inicio de la tabla de inodos
    int s_block_start; //Guardara el inicio de la tabla de bloques
}SuperBloque;*/

typedef struct{
    int i_uid; //UID del usuario propiertario del archivo/carpeta
    int i_gid; //GID del grupo al que pertenece el archivo/carpeta
    int i_size; //Tamano del archivo en bytes
    int i_block[15]; //Array de bloques
    char i_type; //Indica si es archivo o carpeta
    int i_perm; //Guarada los permisos del archivo/carpeta
    char i_atime[20]; //Ultima fecha en que se leyo el inodo sin modificarlo
    char i_ctime[20]; //Fecha en que se creo el el inodo
    char i_mtime[20]; //Ultima fecha en la que se modifco
}InodoTable;

typedef struct{
    char b_name[12];//Nombre carpeta/archivo
    int b_inodo;//Apuntador hacia un inodo asociado al archivo o carpeta
}Content;

typedef struct{
    Content b_content[4];//Array con el contenido de la carpeta
}BloqueCarpeta;

typedef struct{
    char b_content[64];
}BloqueArchivo;

typedef struct{
    int b_pointer[16];//Array con los apuntadores hacia bloques
}BloqueApuntadores;

// Struct para guardar un registro de las operaciones que se hacen el sistema
// de archivos ejemplo: creacion de carpetas o archivos
typedef struct{
    char journal_operation_type[10];
    int journal_type;//Archivo/Carpeta
    char journal_name[100];
    char journal_content[100];
    char journal_date[20];
    int journal_owner;
    int journal_permissions;
}Journal;

//Struct para guardar los datos del usuario loggeado
typedef struct{
    int id_user;
    int id_grp;
    int inicioSuper;
    int inicioJournal;
    int tipo_sistema;
    QString direccion;
    char fit;
}Sesion;

typedef struct{
    int id_usr;
    int id_grp;
    char username[12];
    char password[12];
    char group[12];
}Usuario;

#endif // TIPO_ESTRUCTURA_H
