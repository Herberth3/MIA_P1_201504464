#include "unmount.h"

Unmount::Unmount()
{

}

void Unmount::Ejecutar(QString id, Mount mount)
{
    this->identificador = id;
    this->montaje = mount;

    char letra = this->identificador.toStdString().c_str()[2];
    char numero = this->identificador.toStdString().c_str()[3];
    bool desmontado = false;
    int numeroInt = (int) numero - 48;

    for (int i = 0; i < 26; i++){

        if (this->montaje.discos[i].letra == letra){

            for (int j = 0; j < 99; j++){

                if (this->montaje.discos[i].particiones[j].numero == numeroInt){

                    cout<<"Particion desmontada: vd"<<this->montaje.discos[i].letra<<this->montaje.discos[i].particiones[j].numero<<endl;
                    this->montaje.discos[i].particiones[j].estado = 0;
                    desmontado = true;
                }
            }
        }
    }

    if(!desmontado){
        cout << "Error: ID no existe, no se desmonto la particion." <<endl;
    }

    /*******E L I M I N A R***********/
    this->montaje.Show_Montajes();
    /*******E L I M I N A R***********/
}
