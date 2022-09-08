#include "rep.h"

Rep::Rep()
{

}

void Rep::Ejecutar(QString path, QString name, QString id, Mount mount, QString rute)
{
    int error = 0;
    // Para reporte MBR, DISK, SB , TREE
    string pathDisco_Particion= "";

    /** EL REPORTE SB, TREE, SE IMPLEMENTO SOLO PARA PARTICIONES PRIMARIAS **/
    // Para reporte SB, TREE
    int startParticion;

    // Para reporte SB
    string nombre_disco;

    this->getDatosParticionMontada(id, mount, &pathDisco_Particion, &nombre_disco, &startParticion, &error);

    // Si 'error' obtiene un '1', el path del ID montado no existe
    if(error == 1){
        return;
    }

    // Extension del reporte a crear
    QString ext = this->getExtension(path);

    // Creacion de carpetas del directorio RUTA
    QString directorio = this->getDirectorioCarpetas(path);

    string comando = "sudo mkdir -p \'"+ directorio.toStdString()+"\'";
    system(comando.c_str());
    string comando2 = "sudo chmod -R 777 \'"+ directorio.toStdString()+"\'";
    system(comando2.c_str());

    // SI EL DISCO EXISTE, SE VERIFICA QUE REPORTE EJECUTAR
    if (name == "mbr")
    {
        this->graficarMBR(pathDisco_Particion, path, ext);
    }
    else if (name == "disk")
    {
        this->graficarDisco(pathDisco_Particion, path, ext);
    }
    else if (name == "sb")
    {
        this->graficarSB(pathDisco_Particion, path, ext, nombre_disco, startParticion);
    }
    else if (name == "tree")
    {
        this->graficarTREE(pathDisco_Particion, path, ext, startParticion);
    }
    /*else if (name == "inode")
    {
        string codigo = "";

        Structs::SuperBloque superBloque;
        FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");
        fseek(disco_particion, startParticion, SEEK_SET);
        // Leo el superbloque al inicio de la particion
        fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_particion);

        char bit = ' ';
        for(int i = 0; i < superBloque.arbol_virtual_count; i++){
            fseek(disco_particion, (superBloque.start_bm_arbol_directorio + i), SEEK_SET);
            fread(&bit, sizeof(char), 1, disco_particion);
            codigo = codigo + bit + " | ";
            if((i+1)%20 == 0){
                codigo = codigo + "\n";
            }
        }
        fclose(disco_particion);

        FILE *validar = fopen(path.c_str(), "r");
        if (validar != NULL){
            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();

            fclose(validar);
        } else{
            string comando1 = "mkdir -p \"" + path + "\"";
            string comando2 = "rmdir \"" + path + "\"";
            system(comando1.c_str());
            system(comando2.c_str());

            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();
        }
    }
    else if (name == "block")
    {
        string codigo = "";
        Structs::SuperBloque superBloque;
        FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");

        fseek(disco_particion, startParticion, SEEK_SET);
        // Leo el superbloque al inicio de la particion
        fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_particion);

        char bit = ' ';
        for(int i = 0; i < superBloque.detalle_directorio_count; i++){
            fseek(disco_particion, (superBloque.start_bm_detalle_directorio + i), SEEK_SET);
            fread(&bit, sizeof(char), 1, disco_particion);
            codigo = codigo + bit + " | ";
            if((i+1)%20 == 0){
                codigo = codigo + "\n";
            }
        }
        fclose(disco_particion);

        FILE *validar = fopen(path.c_str(), "r");
        if (validar != NULL){
            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();

            fclose(validar);
        } else{
            string comando1 = "mkdir -p \"" + path + "\"";
            string comando2 = "rmdir \"" + path + "\"";
            system(comando1.c_str());
            system(comando2.c_str());

            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();
        }

    }
    else if (name == "bm_inode")
    {
        string codigo = "";
        Structs::SuperBloque superBloque;
        FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");

        fseek(disco_particion, startParticion, SEEK_SET);
        // Leo el superbloque al inicio de la particion
        fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_particion);

        char bit = ' ';
        for(int i = 0; i<superBloque.inodos_count; i++){
            fseek(disco_particion, (superBloque.start_bm_inodos + i), SEEK_SET);
            fread(&bit, sizeof(char), 1, disco_particion);
            codigo = codigo + bit + " | ";
            if((i+1)%20 == 0){
                codigo = codigo + "\n";
            }
        }
        fclose(disco_particion);

        FILE *validar = fopen(path.c_str(), "r");
        if (validar != NULL){
            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();

            fclose(validar);
        } else{
            string comando1 = "mkdir -p \"" + path + "\"";
            string comando2 = "rmdir \"" + path + "\"";
            system(comando1.c_str());
            system(comando2.c_str());

            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();
        }
    }
    else if (name == "bm_block")
    {
        string codigo = "";

        Structs::SuperBloque superBloque;
        FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");
        fseek(disco_particion, startParticion, SEEK_SET);
        // Leo el superbloque al inicio de la particion
        fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_particion);

        char bit = ' ';
        for(int i = 0; i<superBloque.bloques_count; i++){
            fseek(disco_particion, (superBloque.start_bm_bloques + i), SEEK_SET);
            fread(&bit, sizeof(char), 1, disco_particion);
            codigo = codigo + bit + " | ";
            if((i+1)%20 == 0){
                codigo = codigo + "\n";
            }
        }
        fclose(disco_particion);

        FILE *validar = fopen(path.c_str(), "r");
        if (validar != NULL){
            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();

            fclose(validar);
        } else{
            string comando1 = "mkdir -p \"" + path + "\"";
            string comando2 = "rmdir \"" + path + "\"";
            system(comando1.c_str());
            system(comando2.c_str());

            std::ofstream outfile(path);
            outfile << codigo.c_str() << endl;
            outfile.close();
        }
    }
    else if (name == "ls")
    {
        cout<<"Reporte para LS no implementado"<<endl;
    }
    else if (name == "journaling")
    {
        Structs::SuperBloque superBloque;
        FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");
        fseek(disco_particion, startParticion, SEEK_SET);
        // Leo el superbloque al inicio de la particion
        fread(&superBloque, sizeof(Structs::SuperBloque), 1, disco_particion);

        Structs::logBitacora raizBitacora;
        fseek(disco_particion, superBloque.start_log, SEEK_SET);
        fread(&raizBitacora, sizeof(Structs::logBitacora), 1, disco_particion);
        fclose(disco_particion);

        string nodos = "";
        string nombre = "";
        string texto = "";
        string enlace = "";
        Structs::logBitacora bitacora;

        for(int i =1; i < raizBitacora.size ; i++){

            FILE *disco_particion = fopen(pathDisco_Particion.c_str(), "rb+");
            fseek(disco_particion, superBloque.start_log + (i * sizeof(Structs::logBitacora)), SEEK_SET);
            fread(&bitacora, sizeof(Structs::logBitacora), 1, disco_particion);

            nombre = "Log "+to_string(i);
            texto = "nodeb"+to_string(i)+" [label=\"{"+nombre+"|{{Tipo_Operacion|"+string(bitacora.tipo_operacion)+"}|"
                                                                                                                   "{Tipo|"+bitacora.tipo+"}|{Path|"+string(bitacora.path)+"}|{Contenido|"+string(bitacora.contenido)+"}|{Fecha Log|"
                                                                                                                                                                                                                      ""+string(bitacora.log_fecha)+"}|{Size|"+to_string(bitacora.size)+"}}}\"];\n";

            nodos = nodos + texto;

            if(i != 1){
                enlace = enlace + "nodeb"+to_string(i-1)+" -> nodeb"+to_string(i)+" [color=\"white\"];\n";
            }
            fclose(disco_particion);
        }

        string codigo =
                "digraph D {\n"
                "node [shape=record fontname=\"Aria\" style=filled, fillcolor=azure1];\n"
                + nodos + enlace +
                "}";

        string path1 = path;
        string pathPdf = path1.substr(0, path1.size() - 4);
        pathPdf = pathPdf + ".pdf";

        FILE *validar = fopen(path1.c_str(), "r");
        if (validar != NULL){
            std::ofstream outfile(path1);
            outfile << codigo.c_str() << endl;
            outfile.close();
            string comando = "dot -Tpdf " + path1 + " -o " + pathPdf;

            std::ofstream outfile1("/home/herberth/Escritorio/directorio.txt");
            outfile1 << codigo.c_str() << endl;
            outfile1.close();

            system(comando.c_str());
            fclose(validar);
        } else{
            string comando1 = "mkdir -p \"" + path + "\"";
            string comando2 = "rmdir \"" + path + "\"";
            system(comando1.c_str());
            system(comando2.c_str());

            std::ofstream outfile(path1);
            outfile << codigo.c_str() << endl;
            outfile.close();

            std::ofstream outfile1("/home/herberth/Escritorio/bitacora.txt");
            outfile1 << codigo.c_str() << endl;
            outfile1.close();
            string comando = "dot -Tpdf " + path1 + " -o " + pathPdf;
            system(comando.c_str());
        }
    }
    */
    else //inode y file.
    {
        cout << "Error: Nombre de reporte incorrecto." << endl;
    }

}

void Rep::getDatosParticionMontada(QString id, Mount mount, string *path, string *name_disk, int *part_starParticion, int *error)
{
    QString identificador = id; // Ejemplo: 6413Disco1
    identificador.remove(0, 2); // Ejemplo: 13Disco1

    QRegularExpressionMatch match;
    identificador.indexOf(QRegularExpression("[0-9]+"), 0, &match); // Captura 13 en match
    int indice = identificador.indexOf(QRegularExpression("[A-Za-z]"), 0); // Indice en donde empieza el nombre (una letra)
    int numeroInt =  match.captured().toInt(); // 13
    identificador.remove(0, indice); // Disco1
    string nombre_disco = identificador.toStdString(); // Disco1

    bool existePath = false;
    char pathDisco[100] = "";
    int startParticion = -1;
    string nombreParticion = "";

    //Obtener el path del disco.
    for (int i = 0; i < 26; i++){

        if (strcmp(mount.discos[i].nombre, nombre_disco.c_str()) == 0){

            for (int j = 0; j < 99; j++){

                if (mount.discos[i].particiones[j].numero == numeroInt && mount.discos[i].particiones[j].estado == 1){

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

    // Verificar si existe el disco en el Path obtenido
    FILE *disco_actual = fopen(pathDisco, "rb+");
    if(disco_actual != NULL){

        rewind(disco_actual);
        MBR mbr_auxiliar;
        fread(&mbr_auxiliar, sizeof(MBR), 1, disco_actual);

        // Solo se busca en las particiones primarias
        // NOTA: no se implementa la creacion del sistema de archivos en las logicas
        for (Partition particion : mbr_auxiliar.mbr_partition) {

            if (strcmp(particion.part_name, nombreParticion.c_str()) == 0) {

                startParticion = particion.part_start;
            }
        }

        fclose(disco_actual);
    }else{
        cout << "Error. El PATH no fue encontrado." << endl;
        *error = 1;
        return;
    }

    *error = 0;
    *path = string(pathDisco);
    *part_starParticion = startParticion;
    *name_disk = nombre_disco;
}

/**
  * Funcion que retorna la extension de un archivo
  * @param ruta: ruta completa del archivo
  * @return .pdf, .jpg, .png
**/
QString Rep::getExtension(QString ruta)
{
    string aux = ruta.toStdString();
    string delimiter = ".";
    size_t pos = 0;

    while((pos = aux.find(delimiter)) != string::npos){

        aux.erase(0, pos + delimiter.length());
    }

    return QString::fromStdString(aux);
}

/**
  * Funcion que devuelve el directorio en donde se creara el reporte
  * @param ruta: ruta completa del archivo
**/
QString Rep::getDirectorioCarpetas(QString ruta)
{
    string aux = ruta.toStdString();
    string delimiter = "/";
    size_t pos = 0;
    string res = "";

    while((pos = aux.find(delimiter)) != string::npos){
        res += aux.substr(0,pos) + "/";
        aux.erase(0, pos + delimiter.length());
    }
    return QString::fromStdString(res);
}

/**
  * Metodo para generar el reporte del MBR del disco especificado
  * @param string path: Es la direccion donde se encuentra la particion
  * @param QString ruta: Es la ruta donde se creara el reporte
  * @param QString extension: La extension que tendra el reporte .jpg .png ...etc
**/
void Rep::graficarMBR(string path, QString ruta, QString extension)
{
    FILE *disco_actual;
    string destinoDot = ruta.toStdString() + ".dot";
    ofstream file;

    if (disco_actual = fopen(path.c_str(), "r")) {
        file.open(destinoDot);

        // Validar que se pudo crear el archivo txt, para almacenar el codigo graphviz
        if (file.fail()) {
            cout<< "ERROR. No se pudo abrir el archivo txt del dot"<<endl;
            return;
        }

        //graphDot = fopen("grafica.dot", "w");
        file << "digraph G{ \n";
        file << "subgraph cluster{\n label=\"REPORTE DE MBR\"";
        file << "\ntbl[shape=box,label=<\n";
        file << "<table border=\'0\' cellborder=\'1\' cellspacing=\'0\' width=\'300\'  height=\'200\' >\n";
        file << "<tr>  <td width=\'150\'> <b>Nombre</b> </td> <td width=\'150\'> <b>Valor</b> </td>  </tr>\n";

        MBR masterBoot;
        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterBoot, sizeof(MBR), 1, disco_actual);

        int tamano = masterBoot.mbr_size;
        file << "<tr>  <td><b>mbr_size</b></td><td>"+ to_string(tamano) +"</td>  </tr>\n";

        string fecha_creacion(masterBoot.mbr_date_created);
        file << "<tr>  <td bgcolor=\"#8BCEF4\"><b>mbr_date_created</b></td> <td bgcolor=\"#8BCEF4\">"+ fecha_creacion +"</td>  </tr>\n";
        file << "<tr>  <td><b>mbr_disk_signature</b></td> <td>"+ to_string(masterBoot.mbr_disk_signature) +"</td>  </tr>\n";
        string disk_fit(1, masterBoot.mbr_disk_fit);
        file << "<tr>  <td bgcolor=\"#8BCEF4\"><b>mbr_disk_fit</b></td> <td bgcolor=\"#8BCEF4\">"+ disk_fit +"</td>  </tr>\n";


        int index_Extendida = -1;
        for (int i = 0; i < 4; i++){

            if(masterBoot.mbr_partition[i].part_start != -1 && masterBoot.mbr_partition[i].part_status != '1'){

                if(masterBoot.mbr_partition[i].part_type == 'E'){
                    index_Extendida = i;
                }

                string estado = "";
                if(masterBoot.mbr_partition[i].part_status == '0')
                    estado = "0";
                else if(masterBoot.mbr_partition[i].part_status == '2')
                    estado = "2";

                // Fila para graphviz que forma el apartado para el titulo Particion
                file << "<tr>  <td bgcolor=\"#5588F7\"><b>Particion</b></td> <td bgcolor=\"#5588F7\"></td>  </tr>\n";

                file << "<tr>  <td><b>part_status</b></td> <td>"+ estado +"</td>  </tr>\n";
                string part_type(1, masterBoot.mbr_partition[i].part_type);
                file << "<tr>  <td bgcolor=\"#8BCEF4\"><b>part_type</b></td> <td bgcolor=\"#8BCEF4\">"+ part_type +"</td>  </tr>\n";
                string part_fit(1, masterBoot.mbr_partition[i].part_fit);
                file << "<tr>  <td><b>part_fit</b></td> <td>"+ part_fit +"</td>  </tr>\n";
                file << "<tr>  <td bgcolor=\"#8BCEF4\"><b>part_start</b></td> <td bgcolor=\"#8BCEF4\">"+ to_string(masterBoot.mbr_partition[i].part_start) +"</td>  </tr>\n";
                file << "<tr>  <td><b>part_size</b></td> <td>"+ to_string(masterBoot.mbr_partition[i].part_size) +"</td>  </tr>\n";
                string part_name(masterBoot.mbr_partition[i].part_name);
                file << "<tr>  <td bgcolor=\"#8BCEF4\"><b>part_name</b></td> <td bgcolor=\"#8BCEF4\">"+ part_name +"</td>  </tr>\n";
            }
        }

        file << "</table>\n";
        file << ">];\n}\n";

        // Se recorre las logicas dentro de la Extendida
        if(index_Extendida != -1){

            int index_ebr = 1;
            EBR extendedBoot;
            fseek(disco_actual, masterBoot.mbr_partition[index_Extendida].part_start, SEEK_SET);

            while(fread(&extendedBoot, sizeof(EBR), 1, disco_actual) != 0 && (ftell(disco_actual) < masterBoot.mbr_partition[index_Extendida].part_start + masterBoot.mbr_partition[index_Extendida].part_size)) {

                if(extendedBoot.part_status != '1'){

                    file << "subgraph cluster_"+ to_string(index_ebr) +"{\n label=\"EBR_"+ to_string(index_ebr) +"\"\n";
                    file << "\ntbl_"+ to_string(index_ebr) +"[shape=box, label=<\n ";
                    file << "<table border=\'0\' cellborder=\'1\' cellspacing=\'0\'  width=\'300\' height=\'160\' >\n ";
                    file << "<tr>  <td width=\'150\'><b>Nombre</b></td> <td width=\'150\'><b>Valor</b></td>  </tr>\n";

                    string estado = "";
                    if(extendedBoot.part_status == '0')
                        estado = "0";
                    else if(extendedBoot.part_status == '2')
                        estado = "2";

                    // Fila para graphviz que forma el apartado para el titulo de Particion Logica
                    file << "<tr>  <td bgcolor=\"#6AD33F\"><b>Particion Logica</b></td bgcolor=\"#6AD33F\"> <td></td>  </tr>\n";

                    file << "<tr>  <td><b>part_status_1</b></td> <td>"+ estado +"</td>  </tr>\n";
                    string part_fit(1, extendedBoot.part_fit);
                    file << "<tr>  <td bgcolor=\"#B8F79C\"><b>part_fit_1</b></td> <td bgcolor=\"#B8F79C\">"+ part_fit +"</td>  </tr>\n";
                    file << "<tr>  <td><b>part_start_1</b></td> <td>"+ to_string(extendedBoot.part_start) +"</td>  </tr>\n";
                    file << "<tr>  <td bgcolor=\"#B8F79C\"><b>part_size_1</b></td> <td bgcolor=\"#B8F79C\">"+ to_string(extendedBoot.part_size) +"</td>  </tr>\n";
                    file << "<tr>  <td><b>part_next_1</b></td> <td>"+ to_string(extendedBoot.part_next) +"</td>  </tr>\n";
                    string part_name(extendedBoot.part_name);
                    file << "<tr>  <td bgcolor=\"#B8F79C\"><b>part_name_1</b></td> <td bgcolor=\"#B8F79C\">"+ part_name +"</td>  </tr>\n";
                    file << "</table>\n";
                    file << ">];\n}\n";

                    index_ebr++;
                }

                if(extendedBoot.part_next == -1)
                    break;
                else
                    fseek(disco_actual, extendedBoot.part_next, SEEK_SET);
            }
        }

        file << "}\n" << endl;

        fclose(disco_actual);
        file.close();
        string comando = "dot " + destinoDot + " -o "+ ruta.toStdString() + " -T"+ extension.toStdString();
        system(comando.c_str());
        cout << "Reporte MBR generado con exito " << endl;
    }

}

/**
  * Metodo para graficar un disco con la estructura de las particiones
  * @param string path: Es la direccion donde se encuentra la particion
  * @param QString ruta: Es la ruta donde se creara el reporte
  * @param QString extension: La extension que tendra nuestro reporte .jpg .png ...etc
**/
void Rep::graficarDisco(string path, QString ruta, QString extension)
{
    FILE *disco_actual;
    FILE *graphDot;
    string destinoDot = ruta.toStdString() + ".dot";

    if((disco_actual = fopen(path.c_str(), "r"))){
        graphDot = fopen(destinoDot.c_str(), "w");

        fprintf(graphDot,"digraph G{\n\n");
        fprintf(graphDot, "  tbl [\n    shape=box\n    label=<\n");
        fprintf(graphDot, "     <table border=\'0\' cellborder=\'2\' width=\'600\' height=\"200\" color=\'LIGHTSTEELBLUE\'>\n");
        fprintf(graphDot, "     <tr>\n");
        fprintf(graphDot, "     <td height=\'200\' width=\'100\'> MBR </td>\n");

        MBR masterboot;
        fseek(disco_actual, 0, SEEK_SET);
        fread(&masterboot, sizeof(MBR), 1, disco_actual);

        int total = masterboot.mbr_size;

        for(int i = 0; i < 4; i++){

            int parcial = masterboot.mbr_partition[i].part_size;

            if(masterboot.mbr_partition[i].part_start != -1){//Particion vacia

                double porcentaje_real = parcial/(total/100);
                double porcentaje_aux = (porcentaje_real*5);

                if(masterboot.mbr_partition[i].part_status != '1'){
                    if(masterboot.mbr_partition[i].part_type == 'P'){

                        fprintf(graphDot, "     <td height=\'200\' width=\'%.1f\'>PRIMARIA <br/> Ocupado: %.1f%c</td>\n",porcentaje_aux,porcentaje_real,'%');

                        //Verificar que no haya espacio fragmentado
                        if(i != 3){
                            int p1 = masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size;
                            int p2 = masterboot.mbr_partition[i+1].part_start;
                            if(masterboot.mbr_partition[i+1].part_start != -1){

                                if((p2-p1)!=0){//Hay fragmentacion
                                    int fragmentacion = p2-p1;
                                    double porcentaje_real = (fragmentacion*100)/total;
                                    double porcentaje_aux = (porcentaje_real*500)/100;
                                    fprintf(graphDot,"     <td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",porcentaje_aux,porcentaje_real,'%');
                                }
                            }

                        }else{
                            int p1 = masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size;
                            int mbr_size = total + (int)sizeof(MBR);

                            if((mbr_size-p1) != 0){//Libre
                                double libre = (mbr_size - p1) + sizeof(MBR);
                                double porcentaje_real = (libre*100)/total;
                                double porcentaje_aux = (porcentaje_real*500)/100;
                                fprintf(graphDot, "     <td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",porcentaje_aux, porcentaje_real, '%');
                            }
                        }
                    }else{//Extendida
                        EBR extendedBoot;
                        fprintf(graphDot,"     <td  height=\'200\' width=\'%.1f\'>\n     <table border=\'0\'  height=\'200\' WIDTH=\'%.1f\' cellborder=\'1\'>\n",porcentaje_real,porcentaje_real);
                        fprintf(graphDot,"     <tr>  <td height=\'60\' colspan=\'15\'>EXTENDIDA</td>  </tr>\n     <tr>\n");

                        fseek(disco_actual, masterboot.mbr_partition[i].part_start,SEEK_SET);
                        fread(&extendedBoot,sizeof(EBR),1,disco_actual);

                        if(extendedBoot.part_size != 0){//Si hay mas de alguna logica
                            fseek(disco_actual, masterboot.mbr_partition[i].part_start,SEEK_SET);

                            while(fread(&extendedBoot,sizeof (EBR),1,disco_actual) !=0 && (ftell(disco_actual) < (masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size))){
                                parcial = extendedBoot.part_size;
                                porcentaje_real = (parcial*100)/total;

                                if(porcentaje_real!=0){
                                    if(extendedBoot.part_status != '1'){
                                        fprintf(graphDot, "     <td height=\'140\'>EBR</td>\n");
                                        fprintf(graphDot, "     <td height=\'140\'>LOGICA<br/>Ocupado: %.1f%c</td>\n",porcentaje_real,'%');

                                    }else{//Espacio no asignado
                                        fprintf(graphDot, "      <td height=\'150\'>LIBRE 1 <br/> Ocupado: %.1f%c</td>\n",porcentaje_real,'%');
                                    }
                                    if(extendedBoot.part_next==-1){
                                        parcial = (masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size) - (extendedBoot.part_start + extendedBoot.part_size);
                                        porcentaje_real = (parcial*100)/total;

                                        if(porcentaje_real!=0){
                                            fprintf(graphDot, "     <td height=\'150\'>LIBRE 2<br/> Ocupado: %.1f%c </td>\n",porcentaje_real,'%');
                                        }
                                        break;
                                    }else
                                        fseek(disco_actual,extendedBoot.part_next,SEEK_SET);
                                }
                            }
                        }else{
                            fprintf(graphDot,"     <td height=\'140\'> Ocupado %.1f%c</td>",porcentaje_real,'%');
                        }
                        fprintf(graphDot,"     </tr>\n     </table>\n     </td>\n");
                        //Verificar que no haya espacio fragmentado
                        if(i!=3){
                            int p1 = masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size;
                            int p2 = masterboot.mbr_partition[i+1].part_start;

                            if(masterboot.mbr_partition[i+1].part_start != -1){
                                if((p2-p1)!=0){//Hay fragmentacion
                                    int fragmentacion = p2-p1;
                                    double porcentaje_real = (fragmentacion*100)/total;
                                    double porcentaje_aux = (porcentaje_real*500)/100;
                                    fprintf(graphDot,"     <td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",porcentaje_aux,porcentaje_real,'%');
                                }
                            }
                        }else{
                            int p1 = masterboot.mbr_partition[i].part_start + masterboot.mbr_partition[i].part_size;
                            int mbr_size = total + (int)sizeof(MBR);

                            if((mbr_size-p1) != 0){//Libre
                                double libre = (mbr_size - p1) + sizeof(MBR);
                                double porcentaje_real = (libre*100)/total;
                                double porcentaje_aux = (porcentaje_real*500)/100;
                                fprintf(graphDot, "     <td height=\'200\' width=\'%.1f\'>LIBRE<br/> Ocupado: %.1f%c</td>\n",porcentaje_aux, porcentaje_real, '%');
                            }
                        }
                    }
                }else{//Espacio no asignado
                     fprintf(graphDot,"     <td height=\'200\' width=\'%.1f\'>LIBRE <br/> Ocupado: %.1f%c</td>\n",porcentaje_aux,porcentaje_real,'%');
                }
            }
        }

        //fprintf(graphDot,"     <td height='200'> ESPACIO LIBRE <br/> Ocupado: %.1f%c\n     </td>",(100-espacioUsado),'%');

        fprintf(graphDot,"     </tr> \n     </table>        \n>];\n\n}");

        fclose(graphDot);
        fclose(disco_actual);
        string comando = "dot -T"+ extension.toStdString() + " " + destinoDot +" -o "+ ruta.toStdString();
        system(comando.c_str());
        cout << "Reporte Disco generado con exito " << endl;
    }else{
        cout << "ERROR al crear reporte, disco no encontrado" << endl;
    }
}

/** Metodo para generar el reporte del SuperBloque de una particion
 * @param string path: Es la direccion donde se encuentra la particion
 * @param QString ruta: Es la ruta donde se creara el reporte
 * @param QString extension: La extension que tendra el reporte .jpg .png ...etc
 * @param int part_start_SB: Byte donde inicia el super bloque
**/
void Rep::graficarSB(string path, QString ruta, QString extension, string name_disk, int part_start_SB)
{
    FILE* disco_actual = fopen(path.c_str(),"r");
    string destinoDot = ruta.toStdString() + ".dot";
    SuperBloque super;

    // Se recupera la estructura del SB
    fseek(disco_actual, part_start_SB, SEEK_SET);
    fread(&super, sizeof (super), 1, disco_actual);

    FILE *graph = fopen(destinoDot.c_str(), "w");

    fprintf(graph,"digraph G{\n");
    fprintf(graph, "    nodo [shape=none, fontname=\"Century Gothic\" label=<");
    fprintf(graph, "   <table border=\'0\' cellborder='1\' cellspacing=\'0\' bgcolor=\"cornflowerblue\">");
    fprintf(graph, "    <tr> <td COLSPAN=\'2\'> <b>SUPERBLOQUE</b> </td></tr>\n");

    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> sb_nombre_hd </td> <td bgcolor=\"white\"> %s </td> </tr>\n", name_disk.c_str());

    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inodes_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_inodes_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_blocks_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_blocks_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_free_block_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_free_blocks_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_free_inodes_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_free_inodes_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_mtime </td> <td bgcolor=\"white\"> %s </td></tr>\n", super.s_mtime);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_umtime </td> <td bgcolor=\"white\"> %s </td> </tr>\n", super.s_umtime);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_mnt_count </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_mnt_count);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_magic </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_magic);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inode_size </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_inode_size);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_block_size </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_block_size);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_first_ino </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_first_ino);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_first_blo </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_first_blo);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_bm_inode_start </td> <td bgcolor=\"white\"> %d </td></tr>\n", super.s_bm_inode_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_bm_block_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_bm_block_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_inode_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_inode_start);
    fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> s_block_start </td> <td bgcolor=\"white\"> %d </td> </tr>\n", super.s_block_start);
    fprintf(graph, "   </table>>]\n");
    fprintf(graph,"\n}");
    fclose(graph);

    fclose(disco_actual);

    string comando = "dot -T"+extension.toStdString() + " " + destinoDot +" -o " + ruta.toStdString();
    system(comando.c_str());
    cout << "Reporte SuperBloque generado con exito " << endl;
}

void Rep::graficarTREE(string path, QString ruta, QString extension, int part_start_Partition)
{
    FILE *disco_actual = fopen(path.c_str(),"r");
    string destinoDot = ruta.toStdString() + ".dot";

    SuperBloque super;
    InodoTable inodo;
    BloqueCarpeta carpeta;
    BloqueArchivo archivo;
    BloqueApuntadores apuntador;

    fseek(disco_actual, part_start_Partition, SEEK_SET);
    fread(&super, sizeof(SuperBloque), 1, disco_actual);

    int aux = super.s_bm_inode_start;
    int i = 0;

    char buffer;

    FILE *graph = fopen(destinoDot.c_str(), "w");

    fprintf(graph, "digraph G{\n\n");
    fprintf(graph, "    rankdir=\"LR\" \n");

    //Creamos los inodos
    while(aux < super.s_bm_block_start){

        fseek(disco_actual, super.s_bm_inode_start + i, SEEK_SET);
        buffer = static_cast<char>(fgetc(disco_actual));
        aux++;
        int port = 0;

        if(buffer == '1'){
            fseek(disco_actual, super.s_inode_start + static_cast<int>(sizeof(InodoTable)) * i, SEEK_SET);
            fread(&inodo, sizeof(InodoTable), 1, disco_actual);

            fprintf(graph, "    inodo_%d [ shape=plaintext fontname=\"Century Gothic\" label=<\n",i);
            fprintf(graph, "   <table bgcolor=\"royalblue\" border=\'0\' >");
            fprintf(graph, "    <tr> <td colspan=\'2\'><b>Inode %d</b></td></tr>\n",i);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_uid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n", inodo.i_uid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_gid </td> <td bgcolor=\"white\"> %d </td>  </tr>\n", inodo.i_gid);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_size </td><td bgcolor=\"white\"> %d </td> </tr>\n", inodo.i_size);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_atime </td> <td bgcolor=\"white\"> %s </td> </tr>\n", inodo.i_atime);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_ctime </td> <td bgcolor=\"white\"> %s </td> </tr>\n", inodo.i_ctime);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_mtime </td> <td bgcolor=\"white\"> %s </td> </tr>\n", inodo.i_mtime);

            for(int b = 0; b < 15; b++){
                fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_block_%d </td> <td bgcolor=\"white\" port=\"f%d\"> %d </td></tr>\n", port, b, inodo.i_block[b]);
                port++;
            }

            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_type </td> <td bgcolor=\"white\"> %c </td>  </tr>\n", inodo.i_type);
            fprintf(graph, "    <tr> <td bgcolor=\"lightsteelblue\"> i_perm </td> <td bgcolor=\"white\"> %d </td>  </tr>\n", inodo.i_perm);
            fprintf(graph, "   </table>>]\n\n");

            //Creamos los bloques relacionados al inodo
            for (int j = 0; j < 15; j++) {
                port = 0;

                if(inodo.i_block[j] != -1){

                    fseek(disco_actual, super.s_bm_block_start + inodo.i_block[j], SEEK_SET);
                    buffer = static_cast<char>(fgetc(disco_actual));

                    if(buffer == '1'){//Bloque carpeta
                        fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)) * inodo.i_block[j], SEEK_SET);
                        fread(&carpeta, sizeof(BloqueCarpeta), 1, disco_actual);

                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n", inodo.i_block[j]);
                        fprintf(graph, "   <table bgcolor=\"seagreen\" border=\'0\'>\n");
                        fprintf(graph, "    <tr> <td colspan=\'2\'><b>Folder block %d</b></td></tr>\n", inodo.i_block[j]);
                        fprintf(graph, "    <tr> <td bgcolor=\"mediumseagreen\"> b_name </td> <td bgcolor=\"mediumseagreen\"> b_inode </td></tr>\n");

                        for(int c = 0;c < 4; c++){
                            fprintf(graph, "    <tr> <td bgcolor=\"white\" > %s </td> <td bgcolor=\"white\"  port=\"f%d\"> %d </td></tr>\n", carpeta.b_content[c].b_name, port, carpeta.b_content[c].b_inodo);
                            port++;
                        }

                        fprintf(graph, "   </table>>]\n\n");

                        //Relacion de bloques a inodos
                        for(int c = 0; c < 4; c++){
                            if(carpeta.b_content[c].b_inodo !=-1){
                                if(strcmp(carpeta.b_content[c].b_name,".")!=0 && strcmp(carpeta.b_content[c].b_name,"..")!=0)
                                    fprintf(graph, "    bloque_%d:f%d -> inodo_%d;\n",inodo.i_block[j],c,carpeta.b_content[c].b_inodo);
                            }
                        }

                    }else if(buffer == '2'){//Bloque archivo

                        fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueArchivo)) * inodo.i_block[j], SEEK_SET);
                        fread(&archivo, sizeof(BloqueArchivo), 1, disco_actual);

                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n", inodo.i_block[j]);
                        fprintf(graph, "   <table border=\'0\' bgcolor=\"sandybrown\">\n");
                        fprintf(graph, "    <tr> <td> <b>File block %d</b></td></tr>\n", inodo.i_block[j]);
                        fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td></tr>\n", archivo.b_content);
                        fprintf(graph, "   </table>>]\n\n");

                    }else if(buffer == '3'){//Bloque apuntador

                        fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueApuntadores)) * inodo.i_block[j], SEEK_SET);
                        fread(&apuntador, sizeof(BloqueApuntadores), 1, disco_actual);

                        fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n", inodo.i_block[j]);
                        fprintf(graph, "   <table border=\'0\' bgcolor=\"khaki\">\n");
                        fprintf(graph, "    <tr> <td> <b>Pointer block %d</b></td></tr>\n", inodo.i_block[j]);

                        for(int a = 0; a < 16; a++){
                            fprintf(graph, "    <tr> <td bgcolor=\"white\" port=\"f%d\">%d</td> </tr>\n", port, apuntador.b_pointer[a]);
                            port++;
                        }

                        fprintf(graph, "   </table>>]\n\n");

                        //Bloques carpeta/archivos del bloque de apuntadores
                        for (int x = 0; x < 16; x++) {
                            port = 0;

                            if(apuntador.b_pointer[x] != -1){

                                fseek(disco_actual, super.s_bm_block_start + apuntador.b_pointer[x], SEEK_SET);
                                buffer = static_cast<char>(fgetc(disco_actual));

                                if(buffer == '1'){
                                    fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueCarpeta)) * apuntador.b_pointer[x], SEEK_SET);
                                    fread(&carpeta, sizeof(BloqueCarpeta), 1, disco_actual);

                                    fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n", apuntador.b_pointer[x]);
                                    fprintf(graph, "   <table border=\'0\' bgcolor=\"seagreen\" >\n");
                                    fprintf(graph, "    <tr> <td colspan=\'2\'> <b>Folder block %d</b> </td></tr>\n", apuntador.b_pointer[x]);
                                    fprintf(graph, "    <tr> <td bgcolor=\"mediumseagreen\"> b_name </td> <td bgcolor=\"mediumseagreen\"> b_inode </td></tr>\n");

                                    for(int c = 0; c < 4; c++){
                                        fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td> <td bgcolor=\"white\" port=\"f%d\"> %d </td></tr>\n", carpeta.b_content[c].b_name, port, carpeta.b_content[c].b_inodo);
                                        port++;
                                    }

                                    fprintf(graph, "   </table>>]\n\n");

                                    //Relacion de bloques a inodos
                                    for(int c = 0; c < 4; c++){
                                        if(carpeta.b_content[c].b_inodo != -1){

                                            if(strcmp(carpeta.b_content[c].b_name, ".") != 0 && strcmp(carpeta.b_content[c].b_name, "..") != 0)
                                                fprintf(graph, "    bloque_%d:f%d -> inodo_%d;\n",apuntador.b_pointer[x],c,carpeta.b_content[c].b_inodo);
                                        }
                                    }

                                }else if(buffer == '2'){

                                    fseek(disco_actual, super.s_block_start + static_cast<int>(sizeof(BloqueArchivo)) * apuntador.b_pointer[x], SEEK_SET);
                                    fread(&archivo, sizeof(BloqueArchivo), 1, disco_actual);

                                    fprintf(graph, "    bloque_%d [shape=plaintext fontname=\"Century Gothic\" label=< \n", apuntador.b_pointer[x]);
                                    fprintf(graph, "   <table border=\'0\' bgcolor=\"sandybrown\">\n");
                                    fprintf(graph, "    <tr> <td> <b>File block %d</b></td></tr>\n", apuntador.b_pointer[x]);
                                    fprintf(graph, "    <tr> <td bgcolor=\"white\"> %s </td></tr>\n", archivo.b_content);
                                    fprintf(graph, "   </table>>]\n\n");

                                }else if(buffer == '3'){
                                    // NO SE IMPLEMENTO
                                }
                            }
                        }

                        //Relacion de bloques apuntador a bloques archivos/carpetas
                        for(int b = 0; b < 16; b++){
                            if(apuntador.b_pointer[b] != -1)
                                fprintf(graph, "    bloque_%d:f%d -> bloque_%d;\n", inodo.i_block[j], b, apuntador.b_pointer[b]);
                        }
                    }
                    //Relacion de inodos a bloques
                    fprintf(graph, "    inodo_%d:f%d -> bloque_%d; \n", i, j, inodo.i_block[j]);
                }
            }
        }
        i++;
    }

    fprintf(graph,"\n\n}");

    fclose(graph);
    fclose(disco_actual);

    string comando = "dot -T" + extension.toStdString() + " " + destinoDot + " -o "+ ruta.toStdString();
    system(comando.c_str());
    cout << "Reporte Tree generado con exito " << endl;
}

void Rep::getTreeFile(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, int pointer, vector<string> path)
{
    FILE *disco_particion = fopen(pathD.c_str(), "rb+");

    string primero = to_string(avd.array_subdirectorios[0]);
    string segundo = to_string(avd.array_subdirectorios[1]);
    string tercero = to_string(avd.array_subdirectorios[2]);
    string cuarto = to_string(avd.array_subdirectorios[3]);
    string quinto = to_string(avd.array_subdirectorios[4]);
    string sexto = to_string(avd.array_subdirectorios[5]);
    string detalle = to_string(avd.array_subdirectorios[6]);
    string indirecto = to_string(avd.array_subdirectorios[7]);

    string nombre = avd.nombre_directorio;
    if(nombre == "raiz"){
        nombre = " ";
    }

    string texto = "node"+to_string(pointer)+" [label=\"{\\\\"+nombre+"|{{"+primero+"|<c"+primero+">}|{"+segundo+"|<c"+segundo+">}|{"+tercero+"|"
                                                                                                                                              "<c"+tercero+">}|{"+cuarto+"|<c"+cuarto+">}|{"+quinto+"|<c"+quinto+">}|{"+sexto+"|<c"+sexto+">}|{"+detalle+"|<d"+detalle+">}|{"+indirecto+"|<c"+indirecto+">}}}\"];\n";

    *codigo = *codigo + texto;

    string enlace = "";
    int apuntador = 0;

    for(int i = 0; i < 6; i++){
        apuntador = avd.array_subdirectorios[i];
        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_particion, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_particion);

        if(carpetaHijo.nombre_directorio == path[0]){
            path.erase(path.begin());
            // Me direcciono a un Detalle de Directorio.
            if(path.size() == 1){
                apuntador = carpetaHijo.detalle_directorio;
                Structs::detalleDirectorio Archivos;
                // Nos posicionamos en el detalle de directorio.
                fseek(disco_particion, (superBloque.start_detalle_directorio+(apuntador*sizeof(Structs::detalleDirectorio))), SEEK_SET);
                fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_particion);
                fclose(disco_particion);

                //Creo el enlace
                enlace = "node"+to_string(apuntador)+":d"+to_string(apuntador)+" -> noded"+to_string(apuntador)+";\n";
                *codigoEnlaces = *codigoEnlaces + enlace;

                enlace = "node"+to_string(pointer)+":c"+to_string(apuntador)+" -> node"+to_string(apuntador)+";\n";
                *codigoEnlaces = *codigoEnlaces + enlace;

                string textod = "node"+to_string(apuntador)+" [label=\"{\\\\"+string(carpetaHijo.nombre_directorio)+"|{|}|{|}|{|}|{|}|{|}|{|}|"
                                                                                                                    "{"+to_string(apuntador)+"|<d"+to_string(apuntador)+">}|{|}}\"];\n";

                *codigo = *codigo + textod;

                this->recorrerDetalle(Archivos, apuntador, path, pathD, superBloque, codigo, codigoEnlaces, carpetaHijo.nombre_directorio);
                return;
            }else{

                fclose(disco_particion);
                enlace = "node"+to_string(pointer)+":c"+to_string(apuntador)+" -> node"+to_string(apuntador)+";\n";
                *codigoEnlaces = *codigoEnlaces + enlace;
                *codigo = *codigo + texto;
                this->getTreeFile(carpetaHijo, pathD, superBloque, codigo, codigoEnlaces, apuntador, path);
                return;
            }
        }
    }

    //NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;
    Structs::arbolVirtual carpetaIndirecta;
    // Nos posicionamos en la carpeta hija
    fseek(disco_particion, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_particion);
    fclose(disco_particion);

    enlace = "node"+to_string(pointer)+":c"+to_string(apuntador)+" -> node"+to_string(apuntador)+";\n";
    *codigoEnlaces = *codigoEnlaces + enlace;

    this->getTreeFile(carpetaIndirecta, pathD, superBloque, codigo, codigoEnlaces, apuntador, path);
    return;
}

void Rep::recorrerDetalle(Structs::detalleDirectorio Archivos, int apuntador, vector<string> path, string pathDisco, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, string nombreCarpeta)
{
    FILE *disco_particion = fopen(pathDisco.c_str(), "rb+");

    string primero = to_string(Archivos.archivos[0].noInodo);
    string segundo = to_string(Archivos.archivos[1].noInodo);
    string tercero = to_string(Archivos.archivos[2].noInodo);
    string cuarto = to_string(Archivos.archivos[3].noInodo);
    string quinto = to_string(Archivos.archivos[4].noInodo);
    string primeroN = string(Archivos.archivos[0].nombre_directorio);
    string segundoN = string(Archivos.archivos[1].nombre_directorio);
    string terceroN = string(Archivos.archivos[2].nombre_directorio);
    string cuartoN = string(Archivos.archivos[3].nombre_directorio);
    string quintoN = string(Archivos.archivos[4].nombre_directorio);
    string indirecto = to_string(Archivos.dd_siguiente);

    string texto = "noded"+to_string(apuntador)+" [label=\"{DD "+nombreCarpeta+"|{"+primeroN+"|"+primero+"|<i"+primero+">}"
                                                                                                                       "|{"+segundoN+"|"+segundo+"|<i"+segundo+">}|{"+terceroN+"|"+tercero+"|<i"+tercero+">}|{"+cuartoN+"|"+cuarto+""
                                                                                                                                                                                                                                   "|<i"+cuarto+">}|{"+quintoN+"|"+quinto+"|<i"+quinto+">}|{*|"+indirecto+"|<d"+indirecto+">}}\"];\n";

    *codigo = *codigo + texto;

    for(int i = 0; i < 5; i++){

        int pointer = Archivos.archivos[i].noInodo;
        if(path[0] == Archivos.archivos[i].nombre_directorio){

            string enlace = "noded"+to_string(apuntador)+":i"+to_string(pointer)+" -> nodei"+to_string(pointer)+";\n";
            *codigoEnlaces = *codigoEnlaces + enlace;
            //Mando a crear el inodo.
            Structs::InodoArchivo inodo;

            fseek(disco_particion, (superBloque.start_inodos + (pointer*(sizeof(Structs::InodoArchivo)))), SEEK_SET);
            fread(&inodo, sizeof(Structs::InodoArchivo), 1, disco_particion);
            fclose(disco_particion);

            this->recorrerInodo(inodo, superBloque, pathDisco, codigo, codigoEnlaces, path[0]);
            return;
        }
    }

    //Si no lo encuentra se va al apuntador indirecto.
    int apuntadorA = Archivos.dd_siguiente;
    Structs::detalleDirectorio detalleIndirecto;
    // Nos posicionamos en la carpeta hija
    fseek(disco_particion, (superBloque.start_detalle_directorio+(apuntadorA*sizeof(Structs::detalleDirectorio))), SEEK_SET);
    fread(&detalleIndirecto, sizeof(Structs::detalleDirectorio), 1, disco_particion);
    fclose(disco_particion);

    string enlace = "noded"+to_string(apuntador)+":i"+to_string(apuntadorA)+" -> noded"+to_string(apuntadorA)+";\n";
    *codigoEnlaces = *codigoEnlaces + enlace;

    this->recorrerDetalle(detalleIndirecto, apuntadorA, path,  pathDisco, superBloque, codigo, codigoEnlaces, nombreCarpeta);
    return;
}

void Rep::getTreeDirectorio(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, int pointer, vector<string> path)
{
    FILE *disco_particion = fopen(pathD.c_str(), "rb+");

    if(path.size() == 0){
        string primero = to_string(avd.array_subdirectorios[0]);
        string segundo = to_string(avd.array_subdirectorios[1]);
        string tercero = to_string(avd.array_subdirectorios[2]);
        string cuarto = to_string(avd.array_subdirectorios[3]);
        string quinto = to_string(avd.array_subdirectorios[4]);
        string sexto = to_string(avd.array_subdirectorios[5]);
        string detalle = to_string(avd.array_subdirectorios[6]);
        string indirecto = to_string(avd.array_subdirectorios[7]);
        string nombre = avd.nombre_directorio;
        int detail = avd.array_subdirectorios[6];

        if(nombre == "raiz"){
            nombre = " ";
        }

        string texto = "node"+to_string(pointer)+" [label=\"{\\\\"+nombre+"|{{"+primero+"|<c"+primero+">}|{"+segundo+"|<c"+segundo+">}|{"+tercero+"|"
                                                                                                                                                  "<c"+tercero+">}|{"+cuarto+"|<c"+cuarto+">}|{"+quinto+"|<c"+quinto+">}|{"+sexto+"|<c"+sexto+">}|{"+detalle+"|<d"+detalle+">}|{"+indirecto+"|<c"+indirecto+">}}}\"];\n";

        *codigo = *codigo + texto;

        string enlace = "";

        // Creo el enlace
        enlace = "node"+to_string(pointer)+":d"+to_string(detail)+" -> noded"+to_string(detail)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace;

        Structs::detalleDirectorio Archivos;
        // Nos posicionamos en el detalle de directorio.
        fseek(disco_particion, (superBloque.start_detalle_directorio+(detail*sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_particion);
        fclose(disco_particion);

        this->recorrerDetalleDirectorio(Archivos, detail, pathD, superBloque, codigo, codigoEnlaces, avd.nombre_directorio);

        return;
    }

    int apuntador = 0;

    for(int i = 0; i < 6; i++){

        apuntador = avd.array_subdirectorios[i];
        Structs::arbolVirtual carpetaHijo;
        // Nos posicionamos en la carpeta hija
        fseek(disco_particion, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, disco_particion);

        if(carpetaHijo.nombre_directorio == path[0]){
            path.erase(path.begin());
            fclose(disco_particion);

            this->getTreeDirectorio(carpetaHijo, pathD, superBloque, codigo, codigoEnlaces, apuntador, path);
            return;
        }
    }

    //NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;
    Structs::arbolVirtual carpetaIndirecta;
    // Nos posicionamos en la carpeta hija
    fseek(disco_particion, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, disco_particion);
    fclose(disco_particion);

    this->getTreeDirectorio(carpetaIndirecta, pathD, superBloque, codigo, codigoEnlaces, apuntador, path);
}

void Rep::recorrerDetalleDirectorio(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, string nombreCarpeta)
{
    string primero = to_string(Archivos.archivos[0].noInodo);
    string segundo = to_string(Archivos.archivos[1].noInodo);
    string tercero = to_string(Archivos.archivos[2].noInodo);
    string cuarto = to_string(Archivos.archivos[3].noInodo);
    string quinto = to_string(Archivos.archivos[4].noInodo);
    string primeroN = string(Archivos.archivos[0].nombre_directorio);
    string segundoN = string(Archivos.archivos[1].nombre_directorio);
    string terceroN = string(Archivos.archivos[2].nombre_directorio);
    string cuartoN = string(Archivos.archivos[3].nombre_directorio);
    string quintoN = string(Archivos.archivos[4].nombre_directorio);
    string indirecto = to_string(Archivos.dd_siguiente);

    string texto = "noded"+to_string(apuntador)+" [label=\"{DD "+nombreCarpeta+"|{"+primeroN+"|"+primero+"|<i"+primero+">}"
                                                                                                                       "|{"+segundoN+"|"+segundo+"|<i"+segundo+">}|{"+terceroN+"|"+tercero+"|<i"+tercero+">}|{"+cuartoN+"|"+cuarto+""
                                                                                                                                                                                                                                   "|<i"+cuarto+">}|{"+quintoN+"|"+quinto+"|<i"+quinto+">}|{*|"+indirecto+"|<d"+indirecto+">}}\"];\n";

    *codigo = *codigo + texto;

    // Se va al apuntador indirecto.
    int apuntadorA = Archivos.dd_siguiente;

    if(apuntadorA != -1){

        FILE *disco_particion = fopen(pathDisco.c_str(), "rb+");
        Structs::detalleDirectorio detalleIndirecto;
        // Nos posicionamos en la carpeta hija
        fseek(disco_particion, (superBloque.start_detalle_directorio+(apuntadorA*sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fread(&detalleIndirecto, sizeof(Structs::detalleDirectorio), 1, disco_particion);
        fclose(disco_particion);

        string enlace = "noded"+to_string(apuntador)+":d"+to_string(apuntadorA)+" -> noded"+to_string(apuntadorA)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace;

        this->recorrerDetalleDirectorio(detalleIndirecto, apuntadorA, pathDisco, superBloque, codigo, codigoEnlaces, nombreCarpeta);
    }
}

void Rep::getTreeComplete(Structs::arbolVirtual avd, string pathD, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, int pointer)
{
    int apuntador = 0;
    string primero = to_string(avd.array_subdirectorios[0]);
    string segundo = to_string(avd.array_subdirectorios[1]);
    string tercero = to_string(avd.array_subdirectorios[2]);
    string cuarto = to_string(avd.array_subdirectorios[3]);
    string quinto = to_string(avd.array_subdirectorios[4]);
    string sexto = to_string(avd.array_subdirectorios[5]);
    string detalle = to_string(avd.array_subdirectorios[6]);
    string indirecto = to_string(avd.array_subdirectorios[7]);
    int detail = avd.array_subdirectorios[6];
    string nombre = avd.nombre_directorio;

    if(nombre == "raiz"){
        nombre = " ";
    }

    string texto = "node"+to_string(pointer)+" [label=\"{\\\\"+nombre+"|{{"+primero+"|<c"+primero+">}|{"+segundo+"|<c"+segundo+">}|{"+tercero+"|"
                                                                                                                                              "<c"+tercero+">}|{"+cuarto+"|<c"+cuarto+">}|{"+quinto+"|<c"+quinto+">}|{"+sexto+"|<c"+sexto+">}|{"+detalle+"|<d"+detalle+">}|{"+indirecto+"|<c"+indirecto+">}}}\"];\n";

    string enlace = "";
    // Recorro Carpetas para generar enlaces
    for(int i = 0; i < 6; i++){
        apuntador = avd.array_subdirectorios[i];

        if(apuntador == -1){
            break;
        }
        enlace = enlace + "node"+to_string(pointer)+":c"+to_string(apuntador)+" -> node"+to_string(apuntador)+";\n";
    }

    *codigo = *codigo + texto;
    *codigoEnlaces = *codigoEnlaces + enlace;

    // Valido si existen archivos en su detalle de directorio
    FILE *disco_particion = fopen(pathD.c_str(), "rb+");
    Structs::detalleDirectorio Archivos;
    // Nos posicionamos en el detalle de directorio
    fseek(disco_particion, (superBloque.start_detalle_directorio+(detail*sizeof(Structs::detalleDirectorio))), SEEK_SET);
    fread(&Archivos, sizeof(Structs::detalleDirectorio), 1, disco_particion);
    fclose(disco_particion);

    if(Archivos.archivos[0].noInodo != -1){ //Tiene al menos un archivo, debo graficarlo.
        //Creo el enlace
        enlace = "node"+to_string(pointer)+":d"+to_string(detail)+" -> noded"+to_string(detail)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace;
        recorrerDetalleComplete(Archivos, detail, pathD, superBloque, codigo, codigoEnlaces, nombre);
    }

    //Creo carpetas
    for(int i = 0; i<6; i++){
        apuntador = avd.array_subdirectorios[i];

        if(apuntador == -1){
            return;
        }
        FILE *bfileC = fopen(pathD.c_str(), "rb+");
        Structs::arbolVirtual carpetaHijo;
        fseek(bfileC, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);//nos posicionamos en la carpeta hija
        fread(&carpetaHijo, sizeof(Structs::arbolVirtual), 1, bfileC);
        fclose(bfileC);
        getTreeComplete(carpetaHijo, pathD, superBloque, codigo, codigoEnlaces, apuntador);
    }



    //NINGUNO CUMPLE, SE MUEVE AL APUNTADOR INDIRECTO
    apuntador = avd.avd_siguiente;

    if(apuntador == -1){
        return;
    }

    FILE *bfile1 = fopen(pathD.c_str(), "rb+");
    Structs::arbolVirtual carpetaIndirecta;
    fseek(bfile1, (superBloque.start_arbol_directorio+(apuntador*sizeof(Structs::arbolVirtual))), SEEK_SET);//nos posicionamos en la carpeta hija
    fread(&carpetaIndirecta, sizeof(Structs::arbolVirtual), 1, bfile1);

    enlace = "node"+to_string(pointer)+":c"+to_string(apuntador)+" -> node"+to_string(apuntador)+";\n";
    *codigoEnlaces = *codigoEnlaces + enlace;
    fclose(bfile1);
    getTreeComplete(carpetaIndirecta, pathD, superBloque, codigo, codigoEnlaces, apuntador);
}

void Rep::recorrerDetalleComplete(Structs::detalleDirectorio Archivos, int apuntador, string pathDisco, Structs::SuperBloque superBloque, string *codigo, string *codigoEnlaces, string nombreCarpeta)
{
    string primero = to_string(Archivos.archivos[0].noInodo);
    string segundo = to_string(Archivos.archivos[1].noInodo);
    string tercero = to_string(Archivos.archivos[2].noInodo);
    string cuarto = to_string(Archivos.archivos[3].noInodo);
    string quinto = to_string(Archivos.archivos[4].noInodo);
    string primeroN = string(Archivos.archivos[0].nombre_directorio);
    string segundoN = string(Archivos.archivos[1].nombre_directorio);
    string terceroN = string(Archivos.archivos[2].nombre_directorio);
    string cuartoN = string(Archivos.archivos[3].nombre_directorio);
    string quintoN = string(Archivos.archivos[4].nombre_directorio);
    string indirecto = to_string(Archivos.dd_siguiente);

    string texto = "noded"+to_string(apuntador)+" [label=\"{DD "+nombreCarpeta+"|{"+primeroN+"|"+primero+"|<i"+primero+">}"
                                                                                                                       "|{"+segundoN+"|"+segundo+"|<i"+segundo+">}|{"+terceroN+"|"+tercero+"|<i"+tercero+">}|{"+cuartoN+"|"+cuarto+""
                                                                                                                                                                                                                                   "|<i"+cuarto+">}|{"+quintoN+"|"+quinto+"|<i"+quinto+">}|{*|"+indirecto+"|<d"+indirecto+">}}\"];\n";

    *codigo = *codigo + texto;

    for(int i = 0; i < 5; i++){
        int pointer = Archivos.archivos[i].noInodo;
        string nombreArchivo = Archivos.archivos[i].nombre_directorio;

        if(pointer == -1){
            return;
        }

        FILE *disco_particion = fopen(pathDisco.c_str(), "rb+");
        string enlace = "noded"+to_string(apuntador)+":i"+to_string(pointer)+" -> nodei"+to_string(pointer)+";\n";

        *codigoEnlaces = *codigoEnlaces + enlace;

        //Mando a crear el inodo.
        Structs::InodoArchivo inodo;
        fseek(disco_particion, (superBloque.start_inodos + (pointer*(sizeof(Structs::InodoArchivo)))), SEEK_SET);
        fread(&inodo, sizeof(Structs::InodoArchivo), 1, disco_particion);
        fclose(disco_particion);
        this->recorrerInodo(inodo, superBloque, pathDisco, codigo, codigoEnlaces, nombreArchivo);
    }

    //se va al apuntador indirecto.
    int apuntadorA = Archivos.dd_siguiente;
    if(apuntadorA != -1){

        FILE *disco_particion = fopen(pathDisco.c_str(), "rb+");
        Structs::detalleDirectorio detalleIndirecto;
        // Nos posicionamos en la carpeta hija
        fseek(disco_particion, (superBloque.start_detalle_directorio+(apuntadorA*sizeof(Structs::detalleDirectorio))), SEEK_SET);
        fread(&detalleIndirecto, sizeof(Structs::detalleDirectorio), 1, disco_particion);
        fclose(disco_particion);

        string enlace = "noded"+to_string(apuntador)+":d"+to_string(apuntadorA)+" -> noded"+to_string(apuntadorA)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace;

        this->recorrerDetalleComplete(detalleIndirecto, apuntadorA, pathDisco, superBloque, codigo, codigoEnlaces, nombreCarpeta);
    }
}

void Rep::recorrerInodo(Structs::InodoArchivo inodo, Structs::SuperBloque superBloque, string pathDisco, string *codigo, string *codigoEnlaces, string nombre)
{
    string primero = to_string(inodo.array_bloques[0]);
    string segundo = to_string(inodo.array_bloques[1]);
    string tercero = to_string(inodo.array_bloques[2]);
    string cuarto = to_string(inodo.array_bloques[3]);
    int indirecto = inodo.ap_indirecto;

    string texto = "nodei"+to_string(inodo.count_inodo)+" [label=\"{Inodo "+nombre+"|{"+primero+"|<b"+primero+">}|{"+segundo+"|"
                                                                                                                             "<b"+segundo+">}|{"+tercero+"|<b"+tercero+">}|{"+cuarto+"|<b"+cuarto+">}|{*"+to_string(indirecto)+"|<i"+to_string(indirecto)+">}}\"];\n";

    *codigo = *codigo + texto;

    for(int i = 0; i < inodo.count_bloques_asignados;i++){
        int pointer = inodo.array_bloques[i];

        FILE *disco_particion = fopen(pathDisco.c_str(), "rb+");
        Structs::bloqueDatos bloque;
        fseek(disco_particion, (superBloque.start_bloques + (pointer * sizeof(Structs::bloqueDatos))), SEEK_SET);
        fread(&bloque, sizeof(Structs::bloqueDatos), 1, disco_particion);
        fclose(disco_particion);

        string textoB = "nodeb"+to_string(pointer)+" [label=\""+bloque.db_data+"\"];\n";

        *codigo = *codigo + textoB;

        string enlace = "nodei"+to_string(inodo.count_inodo)+":b"+to_string(pointer)+" -> nodeb"+to_string(pointer)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace;
    }

    //INDIRECTO
    if(inodo.ap_indirecto != -1){

        FILE *disco_particion2 = fopen(pathDisco.c_str(), "rb+");
        Structs::InodoArchivo inodoIndirecto;
        fseek(disco_particion2, (superBloque.start_inodos + (indirecto*(sizeof(Structs::InodoArchivo)))), SEEK_SET);
        fread(&inodoIndirecto, sizeof(Structs::InodoArchivo), 1, disco_particion2);
        fclose(disco_particion2);

        string enlace2 = "nodei"+to_string(inodo.count_inodo)+":i"+to_string(indirecto)+" -> nodei"+to_string(indirecto)+";\n";
        *codigoEnlaces = *codigoEnlaces + enlace2;

        this->recorrerInodo(inodoIndirecto, superBloque, pathDisco, codigo, codigoEnlaces, nombre);
    }
}
