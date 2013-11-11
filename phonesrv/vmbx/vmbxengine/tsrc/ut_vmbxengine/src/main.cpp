#include <e32base.h>
#include <QObject>
#include <QtTest/QtTest>
#include "ut_vmbxengine.h"
#include "ut_pbkstore.h"
#include "ut_simhandler.h"

int main(int argc, char *argv[])
{
    qDebug("main() IN");
//    CTrapCleanup* cleanup = CTrapCleanup::New();
    QCoreApplication app(argc, argv);
    
    qDebug("testing start...");
    
    // API tester
    int result = -1;
    char *pass[3];  
    pass[0] = argv[0];
    pass[1] = "-o"; 
    
    Ut_VmbxEngine tc1;
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxengine.txt";
    result = QTest::qExec(&tc1, 3, pass);
    qDebug("ut_vmbxengine result=%d", result);

    Ut_PbkStore tc2;
    pass[2] = "c:\\logs\\vmbx\\ut_pbkstore.txt";
    result = QTest::qExec(&tc2, 3, pass);
    qDebug("Ut_PbkStore result=%d", result);
    
    // SIM can not be handled in Emulator.
    // Root cause: CVmbxSimHandler::ConstructL()
//    Ut_SimHandler tc3;
//    pass[2] = "c:\\logs\\vmbx\\ut_simhandler.txt";
//    result = QTest::qExec(&tc3, 3, pass);
//    qDebug("Ut_SimHandler result=%d", result);
    
    qDebug("testing end...");
    qDebug("main() OUT");
//    delete cleanup;
//    cleanup = NULL;
    return result;
}

