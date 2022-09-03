#include "mkdisk.h"

Mkdisk::Mkdisk(int size, QString unit, QString fit, QString path)
{
    this->tamano = size;
    this->unidad = unit;
    this->ajuste = fit;
    this->path = path;
}

void Mkdisk::Ejecutar()
{
    int tamanoDisk = 1024;

    // Verificamos si el disco ya existe
    FILE *validar = fopen(this->path.toStdString().c_str(), "r");
    if (validar != NULL)
    {
        cout << "Disco ya existente, intente con un nombre diferente" << endl;
        fclose(validar);
        return;
    }

    // Si el disco y la ruta no existen se intenta crear el directorio pero aun no el archivo del disco
    try {
        this->CrearDirectorio(this->path);
    } catch (...) {
        cout<<"Error. No se pudo crear el disco, revise la ruta"<<endl;
    }

    // Instancia de la estructura MBR
    MBR mbr;
    // Estableciendo la fecha de la creacion del disco. Manera 1
    //time_t t = time(nullptr);
    //tm *now = localtime(&t);
    //string date = to_string(now->tm_mday) + "-" + to_string((now->tm_mon+1)) + "-" + to_string((now->tm_year + 1900)) + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min);
    // Estableciendo la fecha de creacion del disco en el MBR
    //strcpy(mbr.mbr_fecha_creacion, date.c_str());
    // Estableciendo la fecha de la creacion del disco. Manera 2
    time_t t;
    struct tm *tm;
    char fechayhora[20];

    t = time(NULL);
    tm = localtime(&t);
    strftime(fechayhora, 20, "%Y/%m/%d %H:%M:%S", tm);
    // Estableciendo la fecha de la creacion del disco en el MBR
    strcpy(mbr.mbr_fecha_creacion, fechayhora);

    // Estableciendo signature (identificador unico)  del disco en el MBR
    mbr.mbr_disk_signature = rand() % 100;

    // Estableciendo el tamaño del disco en el MBR
    // NOTA: El atributo del mbr.mbr_tamano contiene los bytes completos establecidos
    // NOTA: Mientras que la variable tamanoDisk aun le falta multiplicarse por 1024 para cumplir los bytes establecidos
    // NOTA: Esto porque cuando se ejecuta el comando dd if=/dev/zero en el parametro bs=1024, indica que tamanoDisk se multiplicara por 1024
    if (this->unidad == ""){
        // La unidad del tamaño no esta establecida, por defecto se aplica en M
        mbr.mbr_tamano = this->tamano * 1048576;
        tamanoDisk = this->tamano *1024;
    }else {
        if (this->unidad.toLower() == "m"){
            // La unidad del tamaño esta en Megabytes
            mbr.mbr_tamano = this->tamano * 1048576;
            tamanoDisk = this->tamano * 1024;
        }else {
            // La unida del tamaño esta en Kilobytes
            mbr.mbr_tamano = this->tamano * 1024;
            tamanoDisk = this->tamano;
        }
    }

    // Estableciendo el ajuste del disco en el MBR
    if (this->ajuste != ""){

        if (this->ajuste.toLower() == "bf") {
            mbr.disk_fit = 'B';
        } else if(this->ajuste.toLower() == "ff") {
            mbr.disk_fit = 'F';
        }else {
            mbr.disk_fit = 'W';
        }
    }else mbr.disk_fit = 'F';

    // Iniciando con ceros las particiones que tendra el MBR
    // El atributo mbr_partition del MBR es un arreglo de 4 posiciones
    for (int i = 0; i < 4; i++){
        mbr.mbr_partition[i].part_status = '0';
        mbr.mbr_partition[i].part_type = '0';
        mbr.mbr_partition[i].part_fit = '0';
        mbr.mbr_partition[i].part_size = 0;
        mbr.mbr_partition[i].part_start = -1;
        strcpy(mbr.mbr_partition[i].part_name,"");
    }

    // El comando bs=1024 termina de multiplicar a tamanoDisk para completar los byte establecidos para cada archivo tipo disco
    // Comando para terminar de crear el archivo .dk y establecerle el tamaño que ocupara llenandolo de ceros
    // CREAR DISCO FISICO
    cout<<"Creando disco, espere......."<<endl;
    string comando = "dd if=/dev/zero of=\"" + this->path.toStdString() + "\" bs=1024 count=" + std::to_string(tamanoDisk);
    system(comando.c_str());

    // Se lee el archivo de manera binaria para escribir dentro el MBR creado anteriormente
    FILE *fp = fopen(this->path.toStdString().c_str(), "rb+");
    fseek(fp, 0, SEEK_SET);
    fwrite(&mbr, sizeof(MBR), 1, fp);
    fclose(fp);
    cout << endl;
    cout << "Disco creado con exito" << endl;
}

QString Mkdisk::getDirectorioCarpetas(QString ruta)
{
    // Desfragmenta la ruta para obtener solo los nombres de las carpetas
    // Se forma otro nuevo directorio sin el archivo .disk, solo se tiene carpetas para validar si estas existen
    QStringList carpetas = ruta.split('/', QString::SkipEmptyParts);
    QString new_Directorio = "";
    for (int i = 0; i < carpetas.count() - 1; i++) {
        new_Directorio += "/" + carpetas[i];
        DIR *dir = opendir(new_Directorio.toUtf8());
        if (!dir) {
            auto result = mkdir(new_Directorio.toUtf8().constData(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }

    return new_Directorio;
}

void Mkdisk:: CrearDirectorio(QString ruta){
    QString path_carpetas = this->getDirectorioCarpetas(ruta);

    string c = "sudo mkdir -p \'" + path_carpetas.toStdString() + "\'";
    system(c.c_str());
    string c2 = "sudo chmod -R 777 \'" + path_carpetas.toStdString() + "\'";
    system(c2.c_str());
}
