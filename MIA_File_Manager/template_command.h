#ifndef TEMPLATE_COMMAND_H
#define TEMPLATE_COMMAND_H

class Template_Command
{
public:
    Template_Command();
    virtual void ejecutar() = 0;
    virtual string getParametros() = 0;
};

#endif // TEMPLATE_COMMAND_H
