#include "exec.h"

Exec::Exec(QString path)
{
    this->path = path;
}

void Exec::Ejecutar()
{
    vector<string> lines;
    string line;

    ifstream input_file(this->path.toStdString().c_str());
    if (!input_file.is_open()) {
        cout<<"Error: No se pudo abrir el Script"<<endl;
        return;
    }

    while (getline(input_file, line)){
        lines.push_back(line);
    }

    for (const auto &i : lines) {

        cout<<endl;
        cout << i << endl;
        this->Interpretar(QString::fromStdString(i));
    }

    input_file.close();
    return;
}

void Exec::Interpretar(QString comando)
{
    YY_BUFFER_STATE buffer = yy_scan_string(comando.toUtf8().constData());

    /*Limpiamos los contadores ya que son variables globales*/
    linea = 0;
    yylineno = 0;

    if (listParam.count() > 0) {
        listParam.clear();
    }

    if(yyparse()==0) // Si nos da un número negativo, signifca error.
    {

    }else {

        printf("\n<OCURRIO UN ERROR!>\n");
    }
    yy_delete_buffer(buffer);
}
