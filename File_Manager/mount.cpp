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
    int posicionPath = 0;
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
        if(!is_Particion && part_startExtendida !=0){

            EBR logicaM;

            fseek(disco_actual, part_startExtendida, SEEK_SET);
            fread(&logicaM, sizeof(EBR), 1, disco_actual);

            while(logicaM.part_status != '0' && logicaM.part_next != -1){

                if(logicaM.part_name == this->nombre){

                    is_Particion = true;
                }

                fseek(disco_actual, logicaM.part_next, SEEK_SET);
                fread(&logicaM, sizeof(EBR), 1, disco_actual);
            }
        }

        if(!is_Particion){

            cout<<"Error. La particion no existente."<<endl;
            fclose(disco_actual);
            return;
        }

        fclose(disco_actual);
    } else{

        cout<<"Error. El disco no existe"<<endl;
        return;
    }

    for (int i = 0; i < 26; i++){

        // El path valida si el disco esta montado, pues se ha guardado en la estructura
        if (this->discos[i].path == this->path.toStdString()){

            existePath = true;
            posicionPath = i;
        }
    }

    // Si no existe el disco montado, se monta
    if (!existePath){

        for (int i = 0; i < 99; i++){

            if (this->discos[i].estado == 0){

                this->discos[i].estado = 1;
                this->discos[i].letra = i + 97;
                strcpy(this->discos[i].path, this->path.toStdString().c_str());
                posicionPath = i;
                existePath = true;
                break;
            }
        }
    }

    // Disco ya montado, solo se agrega la particion
    if (existePath){

        for (int i = 0; i < 99; i++){

            if (this->discos[posicionPath].particiones[i].estado == 0)
            {
                this->discos[posicionPath].particiones[i].estado = 1;
                this->discos[posicionPath].particiones[i].numero = i + 1;
                strcpy(this->discos[posicionPath].particiones[i].nombre, this->nombre.toStdString().c_str());

                cout<<"Particion montada exitosamente, id: vd"<<this->discos[posicionPath].letra << this->discos[posicionPath].particiones[i].numero<<endl;
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

                cout<<"vd"<<this->discos[i].letra<<this->discos[i].particiones[j].numero<<endl;
            }
        }
    }
}
