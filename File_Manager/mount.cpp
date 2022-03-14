#include "mount.h"

Mount::Mount()
{

}

void Mount::Ejecutar(QString path, QString name)
{
    this->path = path;
    this->nombre = name;

    int part_startExtendida = 0;
    bool existePath = false;
    int disk_mount_pos = 0;
    FILE *disco_actual = fopen(this->path.toStdString().c_str(), "rb+");

    if (disco_actual != NULL){

        MBR mbr_disco;
        bool is_Particion = false;

        rewind(disco_actual);
        fread(&mbr_disco, sizeof(MBR), 1, disco_actual);

        for (Partition particion : mbr_disco.mbr_partition) {

            if (particion.part_type == 'E') {

                part_startExtendida = particion.part_start;
            }

            if (particion.part_name == this->nombre.toStdString()) {

                is_Particion = true;
            }
        }

        // Verificar si es una particion logica
        // Se busca dentro de la particion Extendida
        if(!is_Particion && part_startExtendida !=0){

            EBR ebr_temporal;

            // Se posiciona en el partstart del primer EBR en la Extendida
            fseek(disco_actual, part_startExtendida, SEEK_SET);
            fread(&ebr_temporal, sizeof(EBR), 1, disco_actual);

            while(ebr_temporal.part_status != '0' && ebr_temporal.part_next != -1){

                if(ebr_temporal.part_name == this->nombre.toStdString()){

                    is_Particion = true;
                }

                fseek(disco_actual, ebr_temporal.part_next, SEEK_SET);
                fread(&ebr_temporal, sizeof(EBR), 1, disco_actual);
            }
        }

        if(!is_Particion){

            cout<<"Error. La particion no existe."<<endl;
            fclose(disco_actual);
            return;
        }

        fclose(disco_actual);
    } else{

        cout<<"Error. El disco no existe"<<endl;
        fclose(disco_actual);
        return;
    }

    // SE VERIFICA SI LA PARTICION YA HA SIDO MONTADA CON ANTERIORIDAD
    for (int i = 0; i < 26; i++){

        if (this->discos[i].path == this->path.toStdString()){

            for (int j = 0; j < 99; j++){

                if (this->discos[i].particiones[j].nombre == this->nombre.toStdString() && this->discos[i].particiones[j].estado == 1){

                    cout<<"Error: Particion ya montada: 64"<<this->discos[i].numero<<this->discos[i].particiones[j].letra<<endl;
                    return;
                }
            }
        }
    }

    // SI LA PARTICION EXISTE ENTONCES SE MONTA LA PARTICION
    for (int i = 0; i < 26; i++){

        // El path valida si el disco esta montado, pues se ha guardado en la estructura
        if (this->discos[i].path == this->path.toStdString()){

            existePath = true;
            disk_mount_pos = i;
        }
    }

    // Si no existe el disco montado, se monta el disco; aun no se monta la particion
    if (!existePath){

        for (int i = 0; i < 26; i++){

            if (this->discos[i].estado == 0){

                this->discos[i].estado = 1;
                // NUMERO identifica al disco al que pertenecen las particiones
                this->discos[i].numero = i + 1;
                strcpy(this->discos[i].path, this->path.toStdString().c_str());
                disk_mount_pos = i;
                existePath = true;
                break;
            }
        }
    }

    // Disco ya montado, solo se agrega la particion
    if (existePath){

        for (int i = 0; i < 99; i++){

            if (this->discos[disk_mount_pos].particiones[i].estado == 0)
            {
                this->discos[disk_mount_pos].particiones[i].estado = 1;
                // Se le asigna a LETRA el caracter en codigo ASCII empezando con el 97 = a, para identificar las particiones
                this->discos[disk_mount_pos].particiones[i].letra = i + 97;
                strcpy(this->discos[disk_mount_pos].particiones[i].nombre, this->nombre.toStdString().c_str());

                cout<<"Particion montada exitosamente, id: 64"<<this->discos[disk_mount_pos].numero<<this->discos[disk_mount_pos].particiones[i].letra<<endl;
                break;
            }
        }
    }

    /********E L I M I N A R***********/
    this->Show_Montajes();
    /********E L I M I N A R***********/
}

void Mount::Show_Montajes()
{
    cout << "<<<-------------------------- MONTAJES ---------------------->>>\n"<< endl;
    for (int i = 0; i < 26; i++){

        for (int j = 0; j < 99; j++){

            if (this->discos[i].particiones[j].estado == 1){

                cout<<"64"<<this->discos[i].numero<<this->discos[i].particiones[j].letra<<endl;
            }
        }
    }
}
