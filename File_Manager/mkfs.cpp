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
    //carpeta folder;
    //folder.makeDirectory("/", 0, this->identificador, this->montaje, false);
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
