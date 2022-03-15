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

#endif // TIPO_ESTRUCTURA_H
