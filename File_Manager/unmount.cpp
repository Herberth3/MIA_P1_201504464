#include "unmount.h"

Unmount::Unmount()
{

}

void Unmount::Ejecutar(QString id, Mount &mount)
{
    this->identificador = id; // Ejemplo: 6413f
    this->identificador.remove(0, 2); // Ejemplo: 13f

    int id_size = this->identificador.size();
    char letra = this->identificador.toStdString().at(id_size - 1); // Ejemplo: f
    QString numero = this->identificador.remove(id_size - 1, 1); // Ejemplo: 13
    int numeroInt =  numero.toInt();
    bool is_desmontado = false;

    for (int i = 0; i < 26; i++){

        if (mount.discos[i].numero == numeroInt){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].letra == letra){

                    cout<<"Particion desmontada: 64"<<mount.discos[i].numero<<mount.discos[i].particiones[j].letra<<endl;
                    mount.discos[i].particiones[j].estado = 0;
                    is_desmontado = true;
                }
            }
        }
    }

    if(!is_desmontado){
        cout << "Error: No fue posible desmontar la particion. No existe el ID." <<endl;
    }

    /*******E L I M I N A R***********/
    mount.Show_Montajes();
    /*******E L I M I N A R***********/
}
