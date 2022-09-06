#include "unmount.h"

Unmount::Unmount()
{

}

/**
  * Parametro &mount: se pasa por referencia, por lo que los cambios aca, actualizan el original
**/
void Unmount::Ejecutar(QString id, Mount &mount)
{
    this->identificador = id; // Ejemplo: 6413Disco1
    this->identificador.remove(0, 2); // Ejemplo: 13Disco1

    //int id_size = this->identificador.size();
    //char letra = this->identificador.toStdString().at(id_size - 1); // Ejemplo: f
    //QString numero = this->identificador.remove(id_size - 1, 1); // Ejemplo: 13
    QRegularExpressionMatch match;
    this->identificador.indexOf(QRegularExpression("[0-9]+"), 0, &match); // Captura 13 en match
    int indice = this->identificador.indexOf(QRegularExpression("[A-Za-z]"), 0); // Indice en donde empieza el nombre (una letra)
    int numeroInt =  match.captured().toInt(); // 13
    this->identificador.remove(0, indice); // Disco1
    string nombre_disco = this->identificador.toStdString(); // Disco1

    bool is_desmontado = false;

    for (int i = 0; i < 26; i++){

        if (strcmp(mount.discos[i].nombre, nombre_disco.c_str())  == 0){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].numero == numeroInt){

                    cout<<"Particion desmontada: 64"<<mount.discos[i].particiones[j].numero<<mount.discos[i].nombre<<endl;
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
