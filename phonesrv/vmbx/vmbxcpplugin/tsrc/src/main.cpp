#include <QtTest/QtTest>
#include "ut_vmbxcpplugin.h"
#include "ut_vmbxuiengine.h"
#include "ut_actioncustomitem.h"
#include "ut_customedit.h"

// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("main() >");
    
    QApplication app(argc, argv);

    qDebug("ut_vmbxuiengine");
    Ut_VmbxUiEngine tcUiEngine; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxuiengine.txt";
    int result = -1;
    result = QTest::qExec(&tcUiEngine, 3, pass);
    qDebug("ut_vmbxuiengine result %d", result);

    qDebug("ut_vmbxcpplugin");
    Ut_vmbxCpPlugin tcCpPlugin;
    pass[2] = "c:\\logs\\vmbx\\ut_vmbxcpplugin.txt";
    result = QTest::qExec(&tcCpPlugin, 3, pass);
    qDebug("ut_vmbxcpplugin result=%d", result);
    
    qDebug("ut_actioncustomitem");
    Ut_ActionCustomItem tcCustom;
    pass[2] = "c:\\logs\\vmbx\\ut_acttioncustomitem.txt";
    result = QTest::qExec(&tcCustom, 3, pass);
    qDebug("ut_actioncustomitem result=%d", result);    

    qDebug("ut_customedit");
    Ut_CustomEdit tcEdit;
    pass[2] = "c:\\logs\\vmbx\\ut_customedit.txt";
    result = QTest::qExec(&tcEdit, 3, pass);
    qDebug("Ut_CustomEdit result=%d", result);    
    
    
    qDebug("main() <");
    return result;
    }
//End of file
