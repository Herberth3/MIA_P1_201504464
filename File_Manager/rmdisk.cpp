#include "rmdisk.h"

Rmdisk::Rmdisk(QString path)
{
    this->path = path;
}

void Rmdisk::Ejecutar()
{
    ifstream existe(this->path.toUtf8(), ifstream::in);
    if (existe){

        string eleccion;
        cout << "Confirmar eliminacion de disco: "<< this->path.toStdString() << " (Y/N)? \n";
        cin >> eleccion;
        if (eleccion == "Y" || eleccion == "y"){

            remove(this->path.toStdString().c_str());

            cout <<"Disco eliminado exitosamente!" << endl;
        }else if (eleccion == "N" || eleccion == "n"){
            cout << "Disco no eliminado!" << endl;
        }else cout << "Opcion incorrecta, disco no eliminado!" << endl;

    }else cout << "Error: No existe el archivo a eliminar" <<  endl;
}
