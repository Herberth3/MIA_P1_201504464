#ifndef STRUCTS_H
#define STRUCTS_H

class Structs
{
public:
    Structs();

    typedef struct{
        char nombre_particion[16] = "";
        int arbol_virtual_count = 0;
        int detalle_directorio_count = 0;
        int inodos_count = 0; // Guarda el numero total de inodos
        int bloques_count = 0; // Guarda el numero total de bloques
        int arbol_virtual_free = 0;
        int detalle_directorio_free = 0;
        int inodos_free = 0; // Contiene el numero de inodos libres
        int bloques_free = 0; // Contiene elnumero de bloques libres
        char date_creacion[20] = ""; // Ultima fecha en el que el sistema fue montado
        char date_ultimo_montaje[20] = ""; // Ultima fecha en que el sistema fue desmontado
        int montajes_count = 0; // Indica cuantas veces se ha montado el sistema
        int start_bm_arbol_directorio = 0;
        int start_arbol_directorio = 0;
        int start_bm_detalle_directorio = 0;
        int start_detalle_directorio = 0;
        int start_bm_inodos = 0; // Guardara el inicio del bitmap de inodos
        int start_inodos = 0; // Guardara el inicio de la tabla de inodos
        int start_bm_bloques = 0; // Guardara el inicio del bitmap de bloques
        int start_bloques = 0; // Guardara el inicio de la tabla de bloques
        int start_log; //Bitacora.
        int size_struct_avd = sizeof(arbolVirtual);
        int size_struct_dd = sizeof(detalleDirectorio);
        int size_struct_inodo = sizeof(InodoArchivo); // Tamaño del inodo
        int size_struct_bloque = sizeof(bloqueDatos); // Tamaño del bloque
        int first_free_avd = 0;
        int first_free_dd = 0;
        int first_free_inodo = 0; // Primer inodo libre
        int first_free_bloque = 0; // Primer bloque libre
        int magic_num = 0xEF53; // Valor que identifica al sistema de archivos
    } SuperBloque;

    typedef struct{
        char date_creacion[20] = "";
        char nombre_directorio[20] = "";
        int array_subdirectorios[6] = {-1, -1, -1, -1, -1, -1};
        int detalle_directorio = -1;
        int avd_siguiente = -1;
        int avd_owner = -1;
    }arbolVirtual;

    typedef struct{
        char nombre_directorio[20] = "";
        int noInodo = -1;
        char date_creacion[20] = "";
        char date_modificacion[20] = "";
    }detalleArchivo;

    typedef struct{
        detalleArchivo archivos[5];
        int dd_siguiente = -1;
    }detalleDirectorio;

    typedef struct{
        int count_inodo;
        int size_archivo;
        int count_bloques_asignados;
        int array_bloques[4] = {-1, -1, -1, -1};
        int ap_indirecto = -1;
        int id_owner = -1;
    }InodoArchivo;

    typedef struct{
        char db_data[25] = "";
    }bloqueDatos;

    typedef struct{
        char tipo_operacion[10] = "";
        char tipo = '-1';
        char path[40] = "";
        char contenido[50] = "";
        char log_fecha[20] = "";
        int size = 0;
    }logBitacora;
};

#endif // STRUCTS_H
