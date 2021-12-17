#include "comando.h"

Comando::Comando()
{

}

void Comando::Ejecutar(QString command, QList<Parametro *> parameters)
{
    this->comando = command.toLower();
    this->parametros = parameters;

    this->size_valor = -1;
    this->add_valor = 0;
    this->unit_valor = "";
    this->fit_valor = "";
    this->path_valor = "";
    this->type_valor = "";
    this->delete_valor = "";
    this->name_valor = "";
    this->id_valor = "";
    this->fs_valor = "";

    this->size_flag = 0;
    this->unit_flag = 0;
    this->fit_flag = 0;
    this->path_flag = 0;
    this->type_flag = 0;
    this->delete_flag = 0;
    this->name_flag = 0;
    this->add_flag = 0;
    this->id_flag = 0;
    this->fs_flag = 0;

    for(int i = 0; i<this->parametros.size(); i++){
        QString nombre = this->parametros[i]->getNombre();
        QString valor = this->parametros[i]->getValor();

        // Se hace match con el comando actual y se hacen validaciones
        // Aun no se ejecuta la accion de cada comando
        // ********************** C O M A N D O   M K D I S K *************************
        if (this->comando == "mkdisk") {

            if (nombre.toLower() == "size") {
                if(this->size_flag == 0){
                    this->size_valor = valor.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "unit") {
                if(this->unit_flag == 0){
                    this->unit_valor = valor;
                    this->unit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "fit") {
                if(this->fit_flag == 0){
                    this->fit_valor = valor;
                    this->fit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en mkdisk: "<<nombre.toStdString()<<endl;
                return;
            }

            // ********************** C O M A N D O   R M D I S K *************************
        } else if (this->comando == "rmdisk") {

            // La validacion esta demas, pues ya en el sintactico se envia solo un parametro
            // En si solo se deberia asignar el valor a la variable path_valor
            if (nombre.toLower() == "path") {

                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else {
                cout<<"Error. Parametro no permitido en rmdisk: "<<nombre.toStdString()<<endl;
                return;
            }

            // ********************** C O M A N D O   F D I S K *************************
        } else if(this->comando == "fdisk"){
            // Obligatorios: size, path, name
            // Opcionales: unit, type, fit, delete, add
            if (nombre.toLower() == "size") {
                if(this->size_flag == 0){
                    this->size_valor = valor.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "unit") {
                if(this->unit_flag == 0){
                    this->unit_valor = valor;
                    this->unit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "type") {
                if(this->type_flag == 0){
                    this->type_valor = valor;
                    this->type_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "fit") {
                if(this->fit_flag == 0){
                    this->fit_valor = valor;
                    this->fit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "delete") {
                if(this->delete_flag == 0){
                    this->delete_valor = valor;
                    this->delete_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }

                if (this->add_flag == 1) {
                    cout<<"Error. Parametro DELETE definido junto con ADD"<<endl;
                    return;
                }
            }else if (nombre.toLower() == "name") {
                if(this->name_flag == 0){
                    this->name_valor = valor;
                    this->name_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "add") {
                if(this->add_flag == 0){
                    this->add_valor = valor.toInt();
                    this->add_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }

                if (this->delete_flag == 1) {
                    cout<<"Error. Parametro ADD definido junto con DELETE"<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en fdisk: "<<nombre.toStdString()<<endl;
                return;
            }
            // ********************** C O M A N D O   M O U N T *************************
        } else if (this->comando == "mount") {

            if (nombre.toLower() == "name") {
                if(this->name_flag == 0){
                    this->name_valor = valor;
                    this->name_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            }else if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en mount: "<<nombre.toStdString()<<endl;
                return;
            }
            // ********************** C O M A N D O   U N M O U N T *************************
        } else if (this->comando == "unmount") {

            this->id_valor = valor;
            this->id_flag = 1;

            // ********************** C O M A N D O   M K F S *************************
        } else if (this->comando == "mkfs") {

            if (nombre.toLower() == "id") {
                if(this->id_flag == 0){
                    this->id_valor = valor;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "type") {
                if(this->type_flag == 0){
                    this->type_valor = valor;
                    this->type_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "fs") {
                if(this->fs_flag == 0){
                    this->fs_valor = valor;
                    this->fs_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en mkfs: "<<nombre.toStdString()<<endl;
                return;
            }

            // ********************** C O M A N D O   M K F I L E *************************
        } else if (this->comando == "mkfile") {

            if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "p") {
                if(this->p_flag == 0){
                    this->p_valor = 1;
                    this->p_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "size") {
                if(this->size_flag == 0){
                    this->size_valor = valor.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "cont") {
                if(this->cont_flag == 0){
                    this->cont_valor = valor;
                    this->cont_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "id") {
                if(this->id_flag == 0){
                    this->id_valor = valor;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en mkfile: "<<nombre.toStdString()<<endl;
                return;
            }

            // ********************** C O M A N D O   M K D I R *************************
        } else if (this->comando == "mkdir") {

            if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "p") {
                if(this->p_flag == 0){
                    this->p_valor = 1;
                    this->p_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "id") {
                if(this->id_flag == 0){
                    this->id_valor = valor;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en mkdir: "<<nombre.toStdString()<<endl;
                return;
            }

            // ********************** C O M A N D O   C P *************************
        } else if (this->comando == "cp") {

            if (nombre.toLower() == "path") {
                if(this->path_flag == 0){
                    this->path_valor = valor;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "dest") {
                if(this->dest_flag == 0){
                    this->dest_valor = 1;
                    this->dest_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else if (nombre.toLower() == "id") {
                if(this->id_flag == 0){
                    this->id_valor = valor;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en cp: "<<nombre.toStdString()<<endl;
                return;
            }

        } else {
            cout<<"Comando no encontrado"<<endl;
            return;
        }

    }

    // Si la ruta viene con " se le remueven
    if (this->path_valor.startsWith("\"")) {
        this->path_valor.replace("\"", "");
    }
    if (this->cont_valor.startsWith("\"")) {
        this->cont_valor.replace("\"", "");
    }

    // Se hace match con el comando actual, se hacen algunas validaciones
    // Se ejecuta la accion de cada comando
    // ********************** C O M A N D O   M K D I S K *************************
    if (this->comando == "mkdisk") {
        if (this->size_valor == -1) {
            cout<<"Error. Parametro Size no establecido"<<endl;
            return;
        }else if (this->size_valor == 0) {
            cout<<"Error. Parametro Size debe ser mayor a 0"<<endl;
            return;
        }

        if (this->unit_valor != "") {

            if (this->unit_valor.toLower() == "k" || this->unit_valor.toLower() == "m") {
            }else {
                cout<<"Error. Valor no permitido en Unit: "<<this->unit_valor.toStdString()<<endl;
                return;
            }
        }

        if (this->fit_valor != "") {

            if (this->fit_valor.toLower() == "ff" || this->fit_valor.toLower() == "bf" || this->fit_valor.toLower() == "wf") {
            }else {
                cout<<"Error. Valor no permitido en Fit: "<<this->fit_valor.toStdString()<<endl;
                return;
            }
        }

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        Mkdisk *nuevoDisco = new Mkdisk(this->size_valor, this->unit_valor, this->fit_valor, this->path_valor);
        nuevoDisco->Ejecutar();

        // ********************** C O M A N D O   R M D I S K *************************
    } else if (this->comando == "rmdisk") {

        Rmdisk *removerDisco = new Rmdisk(this->path_valor);
        removerDisco->Ejecutar();

        // ********************** C O M A N D O   F D I S K *************************
    } else if(this->comando == "fdisk"){

        if (this->size_valor == -1) {
            cout<<"Error. Parametro Size no establecido"<<endl;
            return;
        }else if (this->size_valor == 0) {
            cout<<"Error. Parametro Size debe ser mayor a 0"<<endl;
            return;
        }

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->name_flag == 0) {
            cout<<"Error. Parametro Name no establecido"<<endl;
            return;
        }

        // La validacion para saber si un parametro existe puede ser comparando si la cadena de valor no viene vacia
        // O corroborar que la bandera del parametro este en 1
        if (this->unit_flag == 1) {

            if (this->unit_valor.toLower() == "k" || this->unit_valor.toLower() == "m" || this->unit_valor.toLower() == "b") {
            }else {
                cout<<"Error. Valor no permitido en Unit: "<<this->unit_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro UNIT no fue declarado como parametro, se toma por defecto el valor K (Kilobytes).
            this->unit_valor = "K";
            this->unit_flag = 1;
        }

        if (this->fit_flag == 1) {

            if (this->fit_valor.toLower() == "ff" || this->fit_valor.toLower() == "bf" || this->fit_valor.toLower() == "wf") {
            }else {
                cout<<"Error. Valor no permitido en Fit: "<<this->fit_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro FIT no fue declarado como parametro, se toma por defecto el valor WF (worst fit).
            this->fit_valor = "WF";
            this->fit_flag = 1;
        }

        if (this->type_flag == 1) {

            if (this->type_valor.toLower() == "p" || this->type_valor.toLower() == "e" || this->type_valor.toLower() == "l") {
            }else {
                cout<<"Error. Valor no permitido en Type: "<<this->type_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro TYPE no fue declarado como parametro, se toma por defecto el valor P (primaria).
            this->type_valor = "P";
            this->type_flag = 1;
        }

        if (this->delete_flag == 1) {

            if (this->delete_valor.toLower() == "fast" || this->delete_valor.toLower() == "full") {
            } else {
                cout<<"Error. Valor no permitido en Delete: "<<this->delete_valor.toStdString()<<endl;
                return;
            }
        }

        Fdisk *fixDisco = new Fdisk(this->size_valor, this->unit_valor, this->path_valor, this->type_valor, this->fit_valor, this->delete_valor, this->name_valor, this->add_valor);
        fixDisco->Ejecutar();

        // ********************** C O M A N D O   M O U N T *************************
    } else if (this->comando == "mount") {

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->name_flag == 0) {
            cout<<"Error. Parametro Name no establecido"<<endl;
            return;
        }

        // Instancia de MOUNT
        this->montaje.Ejecutar(this->path_valor, this->name_valor);

        // ********************** C O M A N D O   U N M O U N T *************************
    } else if (this->comando == "unmount") {

        Unmount desmontarDisco;
        desmontarDisco.Ejecutar(this->id_valor.toLower(), this->montaje);

        // ********************** C O M A N D O   M K F S *************************
    } else if (this->comando == "mkfs") {

        if (this->id_flag == 0) {
            cout<<"Error. Parametro ID no establecido"<<endl;
            return;
        }

        if (this->type_flag == 1) {

            if (this->type_valor.toLower() == "fast" || this->type_valor.toLower() == "full") {
            }else {
                cout<<"Error. Valor no permitido en Type: "<<this->type_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro TYPE no fue declarado como parametro, se toma por defecto el valor Full.
            this->type_valor = "full";
            this->type_flag = 1;
        }

        if (this->fs_flag == 1) {

            if (this->fs_valor.toLower() == "2fs" || this->fs_valor.toLower() == "3fs") {
            }else {
                cout<<"Error. Valor no permitido en Fs: "<<this->fs_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro FS no fue declarado como parametro, se toma por defecto el valor 2fs.
            this->fs_valor = "2fs";
            this->fs_flag = 1;
        }

        Mkfs sistema_archivos;
        sistema_archivos.Ejecutar(this->id_valor, this->type_valor, this->fs_valor, this->montaje);

        // ********************** C O M A N D O   M K F I L E *************************
    } else if (this->comando == "mkfile") {

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->size_flag == 0) {

            this->size_valor = 0;
            this->size_flag = 1;
        }

        if (this->p_flag == 0) {

            this->p_valor = 1;
            this->p_flag = 1;
        }

        //archivo nuevo_archivo;
        //nuevo_archivo.Mkfile(this->id_valor, this->path_valor, this->p_valor, this->size_valor, this->cont_valor, this->montaje, false);

        // ********************** C O M A N D O   M K D I R *************************
    } else if (this->comando == "mkdir") {

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->p_flag == 0) {

            this->p_valor = 1;
            this->p_flag = 1;
        }

        //carpeta nueva_carpeta;
        //nueva_carpeta.mkdir(this->path_valor, this->p_valor, this->id_valor, this->montaje, false);

        // ********************** C O M A N D O   C P *************************
    } else if (this->comando == "cp") {

        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->dest_flag == 0) {
            cout<<"Error. Parametro Dest no establecido"<<endl;
            return;
        }

        //carpeta directorio;
        //directorio.copyFile(this->path_valor.toStdString(), this->dest_valor.toStdString(), this->montaje, this->id_valor.toStdString());

        // ********************** C O M A N D O   P A U S E *************************
    } else if (this->comando == "pause") {

        cout << "Comando pause: Ingresa un texto para continuar..." << endl;
        string respuesta;
        cin >> respuesta;
    }

}
