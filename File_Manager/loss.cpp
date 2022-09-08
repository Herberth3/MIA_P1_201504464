#include "loss.h"

Loss::Loss()
{

}

void Loss::Ejecutar(QString id, Mount mount)
{
    int sizeParticion;
    int startParticion;
    int error = 0;
    string pathDisco_Particion= "";
    string nombreParticion;

    this->getDatosParticionMontada(id, mount, &pathDisco_Particion, &startParticion, &sizeParticion, &nombreParticion, &error);

    int num_Estructuras = -1;
    num_Estructuras = (sizeParticion - (2*sizeof(Structs::SuperBloque)))/
            (27+sizeof(Structs::arbolVirtual)+ sizeof(Structs::detalleDirectorio) +
             5*(sizeof(Structs::InodoArchivo)) + 20*(sizeof(Structs::bloqueDatos)) + sizeof(Structs::logBitacora));

    if(error == 1){
        return;
    }

    FILE *disco_actual = fopen(pathDisco_Particion.c_str(), "rb+");
    Structs::SuperBloque superBloque;

    fseek(disco_actual, startParticion, SEEK_SET);
    // Extraer el superbloque del inicio de la particion
    fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    // FORMATEO.
    char cero = '0';
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_arbol_directorio + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    Structs::arbolVirtual avd;
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_arbol_directorio + (i * sizeof(Structs::arbolVirtual))), SEEK_SET);
        fwrite(&avd, sizeof(Structs::arbolVirtual), 1, disco_actual);
    }

    // Bitmap DD
    for(int i = 0; i < num_Estructuras; i++){

        fseek(disco_actual, (superBloque.start_bm_detalle_directorio + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // DD
    Structs::detalleDirectorio dd;
    for(int i = 0; i < num_Estructuras; i++){
        fseek(disco_actual, (superBloque.start_detalle_directorio + (i * sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fwrite(&dd, sizeof(Structs::detalleDirectorio), 1, disco_actual);
    }

    // Bitmap de inodos
    for(int i = 0; i < (5*num_Estructuras); i++){

        fseek(disco_actual, (superBloque.start_bm_inodos + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Inodos
    Structs::InodoArchivo inodo;
    for(int i = 0; i < (5*num_Estructuras); i++){
        fseek(disco_actual, (superBloque.start_inodos + (i * sizeof(Structs::InodoArchivo))), SEEK_SET);
        fwrite(&inodo, sizeof(Structs::InodoArchivo), 1, disco_actual);
    }

    // Bitmap bloques
    for(int i = 0; i < (20*num_Estructuras); i++){
        fseek(disco_actual, (superBloque.start_bm_bloques + i), SEEK_SET);
        fwrite(&cero, sizeof(char), 1, disco_actual);
    }

    // Bloques
    Structs::bloqueDatos bloque;
    for(int i = 0; i < (20*num_Estructuras); i++){
        fseek(disco_actual, (superBloque.start_bloques + (i * sizeof(Structs::bloqueDatos))), SEEK_SET);
        fwrite(&bloque, sizeof(Structs::bloqueDatos), 1, disco_actual);
    }

    // Actualizo la copia del superbloque
    int start_copia_SB = superBloque.start_log + (superBloque.arbol_virtual_count * sizeof(Structs::logBitacora));

    fseek(disco_actual, start_copia_SB, SEEK_SET); //copia super bloque
    fwrite(&superBloque, sizeof(Structs::SuperBloque), 1, disco_actual);

    // LIMPIAR EL SUPERBLOQUE.
    Structs::SuperBloque SuperBloque_Vacio;
    SuperBloque_Vacio.start_log = superBloque.start_log;
    SuperBloque_Vacio.start_arbol_directorio = superBloque.start_arbol_directorio;
    SuperBloque_Vacio.start_bm_arbol_directorio = superBloque.start_bm_arbol_directorio;
    SuperBloque_Vacio.start_bloques = superBloque.start_bloques;
    SuperBloque_Vacio.start_bm_bloques =superBloque.start_bm_bloques;
    SuperBloque_Vacio.start_bm_detalle_directorio = superBloque.start_bm_detalle_directorio;
    SuperBloque_Vacio.start_bm_inodos = superBloque.start_bm_inodos;
    SuperBloque_Vacio.start_detalle_directorio = superBloque.start_detalle_directorio;
    SuperBloque_Vacio.start_inodos = superBloque.start_inodos;
    SuperBloque_Vacio.inodos_count = superBloque.inodos_count;
    SuperBloque_Vacio.bloques_count = superBloque.bloques_count;
    SuperBloque_Vacio.arbol_virtual_count = superBloque.arbol_virtual_count;
    SuperBloque_Vacio.detalle_directorio_count = superBloque.detalle_directorio_count;

    fseek(disco_actual, startParticion, SEEK_SET);
    // Escribo el superbloque al inicio de la particion
    fwrite(&SuperBloque_Vacio, sizeof(Structs::SuperBloque), 1, disco_actual);

    fclose(disco_actual);

    cout << "LOSS ejecutado con exito." << endl;
}

void Loss::getDatosParticionMontada(QString id, Mount mount, string *path, int *part_starParticion, int *part_sizeParticion, string *part_nameParticion, int *error)
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

                if (mount.discos[i].particiones[j].numero == j && mount.discos[i].particiones[j].estado == 1){

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
