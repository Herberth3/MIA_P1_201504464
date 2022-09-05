#include "login.h"

Login::Login()
{

}

void Login::Ejecutar(QString id, QString user, QString password, Mount mount, bool &is_login, Sesion &currentSession)
{
    this->identificador = id;
    this->usuario = user;
    this->contrasena = password;
    this->montaje = mount;

    int error = 0;
    int index = 0;
    int respuesta = 0;
    string pathDisco_Particion= "";
    string nameParticion;

    // Se verifica si ya hay alguna sesion activa
    if (is_login) {
        cout<<"Error: Ya hay una sesion activa, cierre sesion y vuelva a intentarlo"<<endl;
        return;
    }

    // Se obtienen datos sobre la particion montada en donde se va a loguear el usuario
    this->getDatosParticionMontada(this->identificador, this->montaje, &pathDisco_Particion, &nameParticion, &index, &error);

    if (error == 1) {
        return;
    }

    respuesta = this->log_in(pathDisco_Particion, nameParticion, this->usuario, this->contrasena, index, currentSession);

    /*if (respuesta == 1) {
        is_login = true;
        cout<<"Sesion iniciada exitosamente!!"<<endl;
    } else if(respuesta == 2) {
        cout<<"Error: Contraseña incorrecta"<<endl;
    } else if(respuesta == 0) {
        cout<<"Error: Usuario incorrecto"<<endl;
    }*/

}

void Login::getDatosParticionMontada(QString id, Mount mount, string *path, string *part_nameParticion, int *index, int *error)
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
    int indice = -1;

    //Obtener el nombre de la particion y el path del disco.
    for (int i = 0; i < 26; i++){

        if (mount.discos[i].numero == numeroInt){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].letra == letra && mount.discos[i].particiones[j].estado == 1){

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
        for (int i = 0; i < 4; i++) {
            if (mbr_auxiliar.mbr_partition[i].part_status != '0') {
                if(strcmp(mbr_auxiliar.mbr_partition[i].part_name, nombreParticion.c_str()) == 0){
                    indice = i;
                    break;
                }
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
    *index = indice;
}

int Login::log_in(string path_disco, string nombre_particion, QString user, QString password, int index, Sesion &currentSession)
{/*
    int pos_particion = index;
    string fechaActual = this->getFecha();

    // Particiones Primarias
    if(pos_particion != -1){

        MBR masterboot;
        SuperBloque superBloque;
        InodoTable inodo;

        FILE *disco_actual = fopen(path_disco.c_str(), "rb+");

        fread(&masterboot, sizeof(MBR), 1, disco_actual);
        fseek(disco_actual, masterboot.mbr_partition[pos_particion].part_start, SEEK_SET);
        fread(&superBloque, sizeof(SuperBloque), 1, disco_actual);
        fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        fread(&inodo, sizeof(InodoTable), 1, disco_actual);
        fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
        strcpy(inodo.i_atime, fechaActual.c_str());
        fwrite(&inodo, sizeof(InodoTable), 1, disco_actual);
        fclose(disco_actual);

        currentSession.inicioSuper = masterboot.mbr_partition[pos_particion].part_start;
        currentSession.fit = masterboot.mbr_partition[pos_particion].part_fit;
        currentSession.inicioJournal = masterboot.mbr_partition[pos_particion].part_start + static_cast<int>(sizeof(SuperBloque));
        currentSession.tipo_sistema = superBloque.s_filesystem_type;

        return verificarDatos(user, password, path_disco, currentSession);
    }else{
        // Particiones logicas
        pos_particion = this->buscarParticion_L(path_disco, nombre_particion);

        if(pos_particion != -1){

            SuperBloque superBloque;
            InodoTable inodo;

            FILE *disco_actual = fopen(path_disco.c_str(), "rb+");

            fseek(disco_actual, pos_particion + static_cast<int>(sizeof(EBR)),SEEK_SET);
            fread(&superBloque, sizeof(SuperBloque), 1, disco_actual);
            fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
            fread(&inodo, sizeof(InodoTable), 1, disco_actual);
            fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)),SEEK_SET);
            strcpy(inodo.i_atime, fechaActual.c_str());
            fwrite(&inodo, sizeof(InodoTable), 1, disco_actual);
            fclose(disco_actual);

            currentSession.inicioSuper = pos_particion + static_cast<int>(sizeof(EBR));
            //this->sesion_actual->fit = getLogicFit(path_disco,nombre_particion);

            return verificarDatos(user, password, path_disco, currentSession);
        }
    }
    return 0;*/
}

string Login::getFecha()
{
    //fecha
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    string fechaActual(buffer);
    return fechaActual;
}

int Login::buscarParticion_L(string path, string partition_name)
{
    FILE *disco_actual;
    if((disco_actual = fopen(path.c_str(),"rb+"))){
        int pos_extendida = -1;
        MBR masterboot;

        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);

        for(int i = 0; i < 4; i++){
            if(masterboot.mbr_partition[i].part_type == 'E'){
                pos_extendida = i;
                break;
            }
        }

        if(pos_extendida != -1){

            EBR extendedBoot;
            fseek(disco_actual, masterboot.mbr_partition[pos_extendida].part_start, SEEK_SET);

            while(fread(&extendedBoot, sizeof(EBR), 1, disco_actual) != 0 && (ftell(disco_actual) < masterboot.mbr_partition[pos_extendida].part_start + masterboot.mbr_partition[pos_extendida].part_size)){

                if(strcmp(extendedBoot.part_name, partition_name.c_str()) == 0){
                    return (ftell(disco_actual) - sizeof(EBR));
                }
            }
        }
        fclose(disco_actual);
    }
    return -1;
}

int Login::verificarDatos(QString user, QString password, string path, Sesion &currentSession)
{/*
    FILE *disco_actual = fopen(path.c_str(),"rb+");

    char cadena[400] = "\0";
    SuperBloque superBloque;
    InodoTable inodo;

    fseek(disco_actual, currentSession.inicioSuper, SEEK_SET);
    fread(&superBloque, sizeof(SuperBloque), 1, disco_actual);

    //Leemos el inodo del archivo users.txt
    fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo, sizeof(InodoTable), 1, disco_actual);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){

            BloqueArchivo archivo;
            fseek(disco_actual, superBloque.s_block_start, SEEK_SET);

            for(int j = 0; j <= inodo.i_block[i]; j++){
                fread(&archivo, sizeof(BloqueArchivo), 1, disco_actual);
            }
            strcat(cadena, archivo.b_content);
        }
    }

    fclose(disco_actual);

    char *end_str;
    char *token = strtok_r(cadena,"\n",&end_str);

    while(token != nullptr){
        char id[2];
        char tipo[2];
        QString group;
        char user_[12];
        char password_[12];
        char *end_token;
        char *token2 = strtok_r(token, ",", &end_token);
        strcpy(id,token2);

        if(strcmp(id,"0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr, ",", &end_token);
            strcpy(tipo, token2);
            if(strcmp(tipo, "U") == 0){
                token2 = strtok_r(nullptr, ",", &end_token);
                group = token2;
                token2 = strtok_r(nullptr, ",", &end_token);
                strcpy(user_, token2);
                token2 = strtok_r(nullptr, ",", &end_token);
                strcpy(password_, token2);

                if(strcmp(user_, user.toStdString().c_str()) == 0){
                    if(strcmp(password_, password.toStdString().c_str()) == 0){
                        currentSession.direccion = QString::fromStdString(path);
                        currentSession.id_user = atoi(id);
                        currentSession.id_grp = buscarGrupo(group, currentSession);
                        return 1;
                    }else
                        return 2;
                }
            }
        }
        token = strtok_r(nullptr, "\n", &end_str);
    }

    return 0;*/
}

int Login::buscarGrupo(QString group_name, Sesion &currentSession)
{/*
    FILE *disco_actual = fopen(currentSession.direccion.toStdString().c_str(),"rb+");

    char cadena[400] = "\0";
    SuperBloque superBloque;
    InodoTable inodo;

    fseek(disco_actual, currentSession.inicioSuper, SEEK_SET);
    fread(&superBloque, sizeof(SuperBloque), 1, disco_actual);
    //Leemos el inodo del archivo users.txt
    fseek(disco_actual, superBloque.s_inode_start + static_cast<int>(sizeof(InodoTable)), SEEK_SET);
    fread(&inodo, sizeof(InodoTable), 1, disco_actual);

    for(int i = 0; i < 15; i++){
        if(inodo.i_block[i] != -1){
            BloqueArchivo archivo;
            fseek(disco_actual, superBloque.s_block_start, SEEK_SET);
            for(int j = 0; j <= inodo.i_block[i]; j++){
                fread(&archivo, sizeof(BloqueArchivo), 1, disco_actual);
            }
            strcat(cadena, archivo.b_content);
        }
    }

    fclose(disco_actual);

    char *end_str;
    char *token = strtok_r(cadena, "\n", &end_str);

    while(token != nullptr){
        char id[2];
        char tipo[2];
        char group[12];
        char *end_token;
        char *token2 = strtok_r(token, ",", &end_token);
        strcpy(id, token2);

        if(strcmp(id, "0") != 0){//Verificar que no sea un U/G eliminado
            token2 = strtok_r(nullptr, ",", &end_token);
            strcpy(tipo, token2);
            if(strcmp(tipo, "G") == 0){
                strcpy(group, end_token);
                if(strcmp(group,group_name.toStdString().c_str()) == 0)
                    return atoi(id);
            }
        }
        token = strtok_r(nullptr,"\n",&end_str);
    }

    return -1;*/
}
