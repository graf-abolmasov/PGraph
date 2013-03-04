#include "astylewrapper.h"
#include "../../src/common/globalvariables.h"

// allow for different calling conventions in Linux and Windows

#if defined(_WIN32) && !defined(ASTYLE_STATIC)
#define STDCALL __stdcall
#else
#define STDCALL
#endif

// functions to call AStyleMain
extern "C" char* STDCALL AStyleMain(const char* pSourceIn,
                         const char* pOptions,
                         void(STDCALL* fpError)(int, char*),
                         char*(STDCALL* fpAlloc)(unsigned long));
void  STDCALL ASErrorHandler(int errorNumber, char* errorMessage);
char* STDCALL ASMemoryAlloc(unsigned long memoryNeeded);

// other functions
extern "C" char* STDCALL AStyleGetVersion();

// Error handler for the Artistic Style formatter
void STDCALL ASErrorHandler(int errorNumber, char* errorMessage)
{
    globalLogger->log(QString("AStyleBeautifier: Error %1 - %2").arg(errorNumber).arg(errorMessage), Logger::Error);
}

// Allocate memory for the Artistic Style formatter
char* STDCALL ASMemoryAlloc(unsigned long memoryNeeded)
{   // error condition is checked after return from AStyleMain
    char* buffer = new(std::nothrow) char [memoryNeeded];
    return buffer;
}

void AStyleWrapper::beautify(const QFileInfoList &files)
{
    const char* options = "style=stroustrup align-pointer=name";

    QString version(AStyleGetVersion());
    globalLogger->log(QString("Beautify code with AStyle beautifier %1").arg(version), Logger::Compile);

    foreach(QFileInfo fi, files) {
        QString fileName = fi.canonicalFilePath();
        Q_ASSERT(QFile::exists(fileName));
        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        QTextStream ts(&f);
        ts.setCodec("UTF-8");
        QString textIn = ts.readAll();
        f.close();
        QString textOut(AStyleMain(textIn.toStdString().c_str(),
                                   options,
                                   ASErrorHandler,
                                   ASMemoryAlloc));
        Q_ASSERT(!textOut.isEmpty());
        f.open(QIODevice::WriteOnly);
        f.write(textOut.toUtf8());
        f.close();
    }
}
