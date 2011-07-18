#ifndef COMPILER_H
#define COMPILER_H

class ICompiler
{
public:
    virtual void compile() = 0;
private:
    virtual void init(){}
    virtual void finalize(){}
};



#endif // COMPILER_H
