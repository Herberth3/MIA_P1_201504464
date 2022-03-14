#include "comando.h"

Comando::Comando()
{

}

enum COMANDO
{
    MKDISK = 1,
    RMDISK =2,
    FDISK = 3,
    MOUNT = 4,
    UNMOUNT = 5,
    MKFS = 6,
    MKFILE = 7,
    MKDIR = 8,
    CP = 9,
    PAUSE = 10,
    EXEC = 11
};

enum PARAMETRO
{
    SIZE = 1,
    UNIT = 2,
    FIT = 3,
    PATH = 4,
    TYPE = 5,
    DELETE = 6,
    NAME = 7,
    ADD = 8,
    ID = 9,
    FS = 10,
    P = 11,
    CONT = 12,
    DEST = 13
};

void Comando::Ejecutar(QString command, QList<Parametro *> parameters)
{
    this->comandoID = this->getComandoID(command.toLower());
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
        int ID_param = this->getParametroID(this->parametros[i]->getNombre());
        QString nombre_param = this->parametros[i]->getNombre();
        QString valor_param = this->parametros[i]->getValor();

        // Se hace match con el comando actual y se hacen validaciones
        // Aun no se ejecuta la accion de cada comando
        switch (this->comandoID) {

        case MKDISK:
        {    // ********************** C O M A N D O   M K D I S K *************************
            if (ID_param == SIZE) {
                if(this->size_flag == 0){
                    this->size_valor = valor_param.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == UNIT) {
                if(this->unit_flag == 0){
                    this->unit_valor = valor_param;
                    this->unit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == FIT) {
                if(this->fit_flag == 0){
                    this->fit_valor = valor_param;
                    this->fit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en MKDISK: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case RMDISK:
        {    // ********************** C O M A N D O   R M D I S K *************************
            // La validacion esta demas, pues ya en el sintactico se envia solo un parametro
            // En si solo se deberia asignar el valor a la variable path_valor
            if (ID_param == PATH) {

                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else {
                cout<<"Error. Parametro no permitido en RMDISK: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case FDISK:
        {    // ********************** C O M A N D O   F D I S K *************************
            // Obligatorios: size, path, name
            // Opcionales: unit, type, fit, delete, add
            if (ID_param == SIZE) {
                if(this->size_flag == 0){
                    this->size_valor = valor_param.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == UNIT) {
                if(this->unit_flag == 0){
                    this->unit_valor = valor_param;
                    this->unit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == TYPE) {
                if(this->type_flag == 0){
                    this->type_valor = valor_param;
                    this->type_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == FIT) {
                if(this->fit_flag == 0){
                    this->fit_valor = valor_param;
                    this->fit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == DELETE) {
                if(this->delete_flag == 0){
                    this->delete_valor = valor_param;
                    this->delete_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }

                if (this->add_flag == 1) {
                    cout<<"Error. Parametro DELETE definido junto con ADD"<<endl;
                    return;
                }
            }else if (ID_param == NAME) {
                if(this->name_flag == 0){
                    this->name_valor = valor_param;
                    this->name_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == ADD) {
                if(this->add_flag == 0){
                    this->add_valor = valor_param.toInt();
                    this->add_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }

                if (this->delete_flag == 1) {
                    cout<<"Error. Parametro ADD definido junto con DELETE"<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en FDISK: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case MOUNT:
        {    // ********************** C O M A N D O   M O U N T *************************
            if (ID_param == NAME) {
                if(this->name_flag == 0){
                    this->name_valor = valor_param;
                    this->name_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en MOUNT: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case UNMOUNT:
        {   // ********************** C O M A N D O   U N M O U N T *************************
            this->id_valor = valor_param;
            this->id_flag = 1;
        }
            break;
        case MKFS:
        {   // ********************** C O M A N D O   M K F S *************************
            if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == TYPE) {
                if(this->type_flag == 0){
                    this->type_valor = valor_param;
                    this->type_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == FS) {
                if(this->fs_flag == 0){
                    this->fs_valor = valor_param;
                    this->fs_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en MKFS: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case MKFILE:
        {   // ********************** C O M A N D O   M K F I L E *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == P) {
                if(this->p_flag == 0){
                    this->p_valor = 1;
                    this->p_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == SIZE) {
                if(this->size_flag == 0){
                    this->size_valor = valor_param.toInt();
                    this->size_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == CONT) {
                if(this->cont_flag == 0){
                    this->cont_valor = valor_param;
                    this->cont_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en MKFILE: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case MKDIR:
        {   // ********************** C O M A N D O   M K D I R *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == P) {
                if(this->p_flag == 0){
                    this->p_valor = 1;
                    this->p_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en MKDIR: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case CP:
        {   // ********************** C O M A N D O   C P *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == DEST) {
                if(this->dest_flag == 0){
                    this->dest_valor = 1;
                    this->dest_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en CP: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case EXEC:
        {   // ********************** C O M A N D O   E X E C *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else {
                cout<<"Error. Parametro no permitido en EXEC: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        default:
            cout<<"Comando no encontrado"<<endl;
            return;
        }
    }

    // Si la ruta viene con " se le remueven
    if (this->path_valor.startsWith("\"")) {
        this->path_valor.replace("\"", "");
    }
    // Si el name en fdisk viene con " se le remueven
    if (this->name_valor.startsWith("\"")) {
        this->name_valor.replace("\"", "");
    }
    // Si el cont viene con " se le remueven
    if (this->cont_valor.startsWith("\"")) {
        this->cont_valor.replace("\"", "");
    }

    // Se hace match con el comando actual, se hacen algunas validaciones
    // Se ejecuta la accion de cada comando
    switch (this->comandoID) {

    case MKDISK:
    {   // ********************** C O M A N D O   M K D I S K *************************
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
    }
        break;
    case RMDISK:
    {   // ********************** C O M A N D O   R M D I S K *************************
        Rmdisk *removerDisco = new Rmdisk(this->path_valor);
        removerDisco->Ejecutar();
    }
        break;
    case FDISK:
    {   // ********************** C O M A N D O   F D I S K *************************
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
    }
        break;
    case MOUNT:
    {   // ********************** C O M A N D O   M O U N T *************************
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
    }
        break;
    case UNMOUNT:
    {   // ********************** C O M A N D O   U N M O U N T *************************
        if (this->id_valor.startsWith("64")) {
            Unmount desmontarDisco;
            desmontarDisco.Ejecutar(this->id_valor.toLower(), this->montaje);
        }else {
            cout<<"Error. El ID no cumple con la estructura requerida"<<endl;
            return;
        }

    }
        break;
    case MKFS:
    {   // ********************** C O M A N D O   M K F S *************************
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
    }
        break;
    case MKFILE:
    {   // ********************** C O M A N D O   M K F I L E *************************
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
    }
        break;
    case MKDIR:
    {   // ********************** C O M A N D O   M K D I R *************************
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
    }
        break;
    case CP:
    {   // ********************** C O M A N D O   C P *************************
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
    }
        break;
    case PAUSE:
    {   // ********************** C O M A N D O   P A U S E *************************
        cout << "Comando pause: Ingresa un texto para continuar..." << endl;
        string respuesta;
        cin >> respuesta;
    }
        break;
    case EXEC:
    {   // ********************** C O M A N D O   R M D I S K *************************
        Exec *script = new Exec(this->path_valor);
        script->Ejecutar();
    }
        break;
    default:
        break;
    }

}

int Comando::getComandoID(QString comando)
{
    if(comando == "mkdisk") return 1;
    if(comando == "rmdisk") return 2;
    if(comando == "fdisk") return 3;
    if(comando == "mount") return 4;
    if(comando == "unmount") return 5;
    if(comando == "mkfs") return 6;
    if(comando == "mkfile") return 7;
    if(comando == "mkdir") return 8;
    if(comando == "cp") return 9;
    if(comando == "pause") return 10;
    if(comando == "exec") return 11;
}

int Comando::getParametroID(QString parametro)
{
    if(parametro.toLower() == "size") return 1;
    if(parametro.toLower() == "unit") return 2;
    if(parametro.toLower() == "fit") return 3;
    if(parametro.toLower() == "path") return 4;
    if(parametro.toLower() == "type") return 5;
    if(parametro.toLower() == "delete") return 6;
    if(parametro.toLower() == "name") return 7;
    if(parametro.toLower() == "add") return 8;
    if(parametro.toLower() == "id") return 9;
    if(parametro.toLower() == "fs") return 10;
    if(parametro.toLower() == "p") return 11;
    if(parametro.toLower() == "cont") return 12;
    if(parametro.toLower() == "dest") return 13;
}
