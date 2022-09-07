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
    COPY = 9,
    PAUSE = 10,
    EXEC = 11,
    LOGIN = 12,
    RENAME = 13,
    RECOVERY = 14,
    LOSS = 15,
    REP = 16
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
    DESTINO = 13,
    USUARIO = 14,
    PASSWORD = 15,
    R = 16,
    RUTA = 17
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
    this->usuario_valor = "";
    this->password_valor = "";
    this->cont_valor = "";
    this->destino_valor = "";
    this->ruta_valor = "";

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
    this->usuario_flag = 0;
    this->password_flag = 0;
    this->p_flag = 0;
    this->r_flag = 0;
    this->cont_flag = 0;
    this->destino_flag = 0;
    this->ruta_flag = 0;

    /**
      * VALIDACIONES DE REPETICION, PARAMETRO NO PERTENECIENTE AL COMANDO
      * SE RECORREN TODOS LOS PARAMETROS CAPTURADOS
      **/
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
                    this->unit_valor = valor_param.toLower();
                    this->unit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == FIT) {
                if(this->fit_flag == 0){
                    this->fit_valor = valor_param.toLower();
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
                    this->unit_valor = valor_param.toLower();
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
                    this->type_valor = valor_param.toLower();
                    this->type_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == FIT) {
                if(this->fit_flag == 0){
                    this->fit_valor = valor_param.toLower();
                    this->fit_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else if (ID_param == DELETE) {
                if(this->delete_flag == 0){
                    this->delete_valor = valor_param.toLower();
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
                    this->name_valor = valor_param.toLower();
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
                    this->name_valor = valor_param.toLower();
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
        case LOGIN:
        {   // ********************** C O M A N D O   L O G I N *************************
            if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == USUARIO) {
                if(this->usuario_flag == 0){
                    this->usuario_valor = valor_param;
                    this->usuario_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == PASSWORD) {
                if(this->password_flag == 0){
                    this->password_valor = valor_param;
                    this->password_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en LOGIN: "<<nombre_param.toStdString()<<endl;
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
            } else if (ID_param == R) {
                if(this->r_flag == 0){
                    this->r_flag = 1;
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
                    this->p_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) { // NOTA: El comando MKDIR solo pide PATH y P
                if(this->id_flag == 0){ // Se pide el parametro ID para quemar el dato, se ignora al tener el LOGIN
                    this->id_valor = valor_param; // QUITAR ID CUANDO SE HAGA LO DE LOGIN
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
        case RENAME:
        {   // ********************** C O M A N D O   R E N A M E *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == NAME) {
                if(this->name_flag == 0){
                    this->name_valor = valor_param;
                    this->name_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) { // NOTA: El comando RENAME solo pide PATH y NAME
                if(this->id_flag == 0){ // Se pide el parametro ID para quemar el dato, se ignora al tener el LOGIN
                    this->id_valor = valor_param; // QUITAR ID CUANDO SE HAGA LO DE LOGIN
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en RENAME: "<<nombre_param.toStdString()<<endl;
                return;
            }

        }
            break;
        case COPY:
        {   // ********************** C O M A N D O   C P *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == DESTINO) {
                if(this->destino_flag == 0){
                    this->destino_valor = valor_param;
                    this->destino_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == ID) { // NOTA: El comando COPY solo pide PATH y DESTINO
                if(this->id_flag == 0){ // Se pide el parametro ID para quemar el dato, se ignora al tener el LOGIN
                    this->id_valor = valor_param; // QUITAR ID CUANDO SE HAGA LO DE LOGIN
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else {
                cout<<"Error. Parametro no permitido en COPY: "<<nombre_param.toStdString()<<endl;
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
        case RECOVERY:
        {   // ********************** C O M A N D O   R E C O V E R Y *************************
            if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else {
                cout<<"Error. Parametro no permitido en RECOVERY: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case LOSS:
        {   // ********************** C O M A N D O   L O S S *************************
            if (ID_param == ID) {
                if(this->id_flag == 0){
                    this->id_valor = valor_param;
                    this->id_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            }else {
                cout<<"Error. Parametro no permitido en LOSS: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        case REP:
        {   // ********************** C O M A N D O   R E P *************************
            if (ID_param == PATH) {
                if(this->path_flag == 0){
                    this->path_valor = valor_param;
                    this->path_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == RUTA) {
                if(this->ruta_flag == 0){
                    this->ruta_valor = valor_param;
                    this->ruta_flag = 1;
                } else {
                    cout<<"Error. Parametro repetido: "<<nombre_param.toStdString()<<endl;
                    return;
                }
            } else if (ID_param == NAME) {
                if(this->name_flag == 0){
                    this->name_valor = valor_param.toLower();
                    this->name_flag = 1;
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
                cout<<"Error. Parametro no permitido en REP: "<<nombre_param.toStdString()<<endl;
                return;
            }
        }
            break;
        default:
            cout<<"Comando no encontrado"<<endl;
            return;
        }
    }

    /**
      * SE QUITAN LAS COMILLAS A LOS VALORES DE ALGUNOS PARAMETROS
    **/
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
    // Si el destino viene con " se le remueven
    if (this->destino_valor.startsWith("\"")) {
        this->destino_valor.replace("\"", "");
    }
    // Si el usuario viene con " se le remueven
    if (this->usuario_valor.startsWith("\"")) {
        this->usuario_valor.replace("\"", "");
    }
    // Si el password viene con " se le remueven
    if (this->password_valor.startsWith("\"")) {
        this->password_valor.replace("\"", "");
    }
    // Si el ruta viene con " se le remueven
    if (this->ruta_valor.startsWith("\"")) {
        this->ruta_valor.replace("\"", "");
    }

    /**
    * VALIDACION DE PARAMETROS OBLIGATORIOS Y ESTABLECIMIENTO DE VALORES
    * A LOS PARAMETROS NO OBLIGATORIOS.
    * EJECUCION DEL COMANDO
    **/

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

        if (this->unit_flag == 1) {

            if (this->unit_valor == "k" || this->unit_valor == "m") {
            }else {
                cout<<"Error. Valor no permitido en Unit: "<<this->unit_valor.toStdString()<<endl;
                return;
            }
        } else {
            this->unit_valor = "m";
            this->unit_flag = 1;
        }

        if (this->fit_flag == 1) {

            if (this->fit_valor.toLower() == "ff" || this->fit_valor.toLower() == "bf" || this->fit_valor.toLower() == "wf") {
            }else {
                cout<<"Error. Valor no permitido en Fit: "<<this->fit_valor.toStdString()<<endl;
                return;
            }
        } else {
            this->fit_valor = "ff";
            this->fit_flag = 1;
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

            if (this->unit_valor == "k" || this->unit_valor == "m" || this->unit_valor == "b") {
            }else {
                cout<<"Error. Valor no permitido en Unit: "<<this->unit_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro UNIT no fue declarado como parametro, se toma por defecto el valor K (Kilobytes).
            this->unit_valor = "k";
            this->unit_flag = 1;
        }

        if (this->fit_flag == 1) {

            if (this->fit_valor == "ff" || this->fit_valor == "bf" || this->fit_valor == "wf") {
            }else {
                cout<<"Error. Valor no permitido en Fit: "<<this->fit_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro FIT no fue declarado como parametro, se toma por defecto el valor WF (worst fit).
            this->fit_valor = "wf";
            this->fit_flag = 1;
        }

        if (this->type_flag == 1) {

            if (this->type_valor == "p" || this->type_valor == "e" || this->type_valor == "l") {
            }else {
                cout<<"Error. Valor no permitido en Type: "<<this->type_valor.toStdString()<<endl;
                return;
            }
        }else {
            // Si el parametro TYPE no fue declarado como parametro, se toma por defecto el valor P (primaria).
            this->type_valor = "p";
            this->type_flag = 1;
        }

        if (this->delete_flag == 1) {

            if (this->delete_valor == "full") {
            } else {
                cout<<"Error. Valor no permitido en Delete: "<<this->delete_valor.toStdString()<<endl;
                return;
            }
        }

        // Comprobar que no este montada la particion para poder ELIMINAR PARTICION O AGREGAR/QUITAR ESPACIO
        if (this->delete_flag == 1 || this->add_flag == 1) {

            bool isMontada = this->existeMontaje(this->path_valor.toStdString(), this->name_valor.toStdString());

            if (isMontada) {
                cout<<"Error. Desmonte primero la particion para poder realizar la accion solicitada"<<endl;
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
            desmontarDisco.Ejecutar(this->id_valor, this->montaje);
        }else {
            cout<<"Error. El ID no cumple con la estructura requerida"<<endl;
            return;
        }

    }
        break;
    case MKFS:
    {   // ********************** C O M A N D O   M K F S *************************
        // Obligatorio: id
        // Opcionales: type, fs
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

        if (this->id_valor.startsWith("64")) {
            Mkfs sistema_archivos;
            sistema_archivos.Ejecutar(this->id_valor.toLower(), this->type_valor, this->fs_valor, this->montaje);
        }else {
            cout<<"Error. El ID no cumple con la estructura requerida"<<endl;
            return;
        }
    }
        break;
    case LOGIN:
    {   // ********************** C O M A N D O   L O G I N *************************
        // Obligatorio: id, usuario, password
        if (this->id_flag == 0) {
            cout<<"Error. Parametro ID no establecido"<<endl;
            return;
        }

        if (this->usuario_flag == 0) {
            cout<<"Error. Parametro USUARIO no establecido"<<endl;
            return;
        }

        if (this->password_flag == 0) {
            cout<<"Error. Parametro PASSWORD no establecido"<<endl;
            return;
        }

        if (this->id_valor.startsWith("64")) {
            Login loguear;
            loguear.Ejecutar(this->id_valor.toLower(), this->usuario_valor, this->password_valor, this->montaje, this->flag_login, this->currentSession);
        }else {
            cout<<"Error. El ID no cumple con la estructura requerida"<<endl;
            return;
        }

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

        // Quitar al tener en marcha LOGIN
        if (this->id_flag == 0) {
            cout<<"Error. Al no tener LOGIN se necesita ID para ejecutar el comando"<<endl;
            return;
        }

        archivo nuevo_archivo;
        nuevo_archivo.mkFile(this->id_valor.toLower(), this->path_valor, this->r_flag, this->size_valor, this->cont_valor, this->montaje, false);
    }
        break;
    case MKDIR:
    {   // ********************** C O M A N D O   M K D I R *************************
        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        // Quitar al tener en marcha LOGIN
        if (this->id_flag == 0) {
            cout<<"Error. Al no tener LOGIN se necesita ID para ejecutar el comando"<<endl;
            return;
        }

        carpeta nueva_carpeta;
        nueva_carpeta.mkDir(this->path_valor, this->p_flag, this->id_valor.toLower(), this->montaje, false);
    }
        break;
    case RENAME:
    {   // ********************** C O M A N D O   R E N A M E *************************
        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->name_flag == 0) {
            cout<<"Error. Parametro Name no establecido"<<endl;
            return;
        }

        // Quitar al tener en marcha LOGIN
        if (this->id_flag == 0) {
            cout<<"Error. Al no tener LOGIN se necesita ID para ejecutar el comando"<<endl;
            return;
        }

        carpeta directorio;
        directorio.renameFile(this->id_valor.toLower(), this->path_valor, this->name_valor, this->montaje);

    }
        break;
    case COPY:
    {   // ********************** C O M A N D O   C P *************************
        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->destino_flag == 0) {
            cout<<"Error. Parametro Destino no establecido"<<endl;
            return;
        }

        // Quitar al tener en marcha LOGIN
        if (this->id_flag == 0) {
            cout<<"Error. Al no tener LOGIN se necesita ID para ejecutar el comando"<<endl;
            return;
        }

        carpeta directorio;
        directorio.copyFile(this->path_valor, this->destino_valor, this->montaje, this->id_valor.toLower());
    }
        break;
    case PAUSE:
    {   // ********************** C O M A N D O   P A U S E *************************
        cout << "PAUSE: Presione ENTER para continuar..." << endl;
        //string respuesta;
        //cin >> respuesta;
        cin.get();
    }
        break;
    case EXEC:
    {   // ********************** C O M A N D O   E X E C *************************
        Exec *script = new Exec(this->path_valor);
        script->Ejecutar();
    }
        break;
    case RECOVERY:
    {   // ********************** C O M A N D O   R E C O V E R Y *************************
        Recovery recoveryFS;
        recoveryFS.Ejecutar(this->id_valor.toLower(), this->montaje);
    }
        break;
    case LOSS:
    {   // ********************** C O M A N D O   L O S S *************************
        Loss simulateLoss;
        simulateLoss.Ejecutar(this->id_valor.toLower(), this->montaje);
    }
        break;
    case REP:
    {   // ********************** C O M A N D O   R E P *************************
        if (this->path_flag == 0) {
            cout<<"Error. Parametro Path no establecido"<<endl;
            return;
        }

        if (this->name_flag == 0) {
            cout<<"Error. Parametro NAME no establecido"<<endl;
            return;
        }else {

            if (this->name_valor == "mbr" || this->name_valor == "disk" || this->name_valor == "inode" ||
                    this->name_valor == "journaling" || this->name_valor == "block" || this->name_valor == "bm_inode" ||
                    this->name_valor == "bm_block" || this->name_valor == "tree" || this->name_valor == "sb" ||
                    this->name_valor == "file" || this->name_valor == "ls") {
            } else {
                cout<<"Error. Valor no permitido en NAME de REP: "<<this->fit_valor.toStdString()<<endl;
                return;
            }
        }

        if (this->id_flag == 0) {
            cout<<"Error. Parametro ID no establecido"<<endl;
            return;
        }

        Rep reporte;
        reporte.Ejecutar(this->path_valor, this->name_valor, this->id_valor, this->montaje, this->ruta_valor);
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
    if(comando == "copy") return 9;
    if(comando == "pause") return 10;
    if(comando == "exec") return 11;
    if(comando == "login") return 12;
    if(comando == "rename") return 13;
    if(comando == "recovery") return 14;
    if(comando == "loss") return 15;
    if(comando == "rep") return 16;
}

int Comando::getParametroID(QString parametro)
{
    if(parametro.toLower() == "s") return 1;
    if(parametro.toLower() == "u") return 2;
    if(parametro.toLower() == "f") return 3;
    if(parametro.toLower() == "path") return 4;
    if(parametro.toLower() == "t") return 5;
    if(parametro.toLower() == "delete") return 6;
    if(parametro.toLower() == "name") return 7;
    if(parametro.toLower() == "add") return 8;
    if(parametro.toLower() == "id") return 9;
    if(parametro.toLower() == "fs") return 10;
    if(parametro.toLower() == "p") return 11;
    if(parametro.toLower() == "cont") return 12;
    if(parametro.toLower() == "destino") return 13;
    if(parametro.toLower() == "usuario") return 14;
    if(parametro.toLower() == "password") return 15;
    if(parametro.toLower() == "r") return 16;
    if(parametro.toLower() == "ruta") return 17;
}

bool Comando::existeMontaje(string path, string name_partition)
{
    bool existe = false;

    for (int i = 0; i < 26; i++){

        if (strcmp(montaje.discos[i].path, path.c_str())  == 0){

            for (int j = 0; j < 99; j++){

                if (strcmp(montaje.discos[i].particiones[j].nombre,  name_partition.c_str()) == 0 && montaje.discos[i].particiones[j].estado == 1){

                    existe = true;
                    break;
                }
            }
        }
    }

    return existe;

}
