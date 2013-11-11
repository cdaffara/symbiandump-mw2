/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: satapp main
*
*
*/

#include <QtGlobal>
#include <QApplication>
#include <QTranslator>
#include <hbapplication.h>
#include <hbmainwindow.h>
#include "satappmainhandler.h"


#ifdef SAT_DEBUG_TO_FILE
    // ------------------------------------------------------------
    // this segment provides functionality that copies all QT debug
    // output to RFileLogger, and also the standard QT debug stream
    #include <flogger.h>
    #include <e32svr.h>
    _LIT(KTfLogFolder, "sat");
    _LIT(KTfLogFile, "satui.txt");
    // original message handler
    QtMsgHandler originalMsgHandler;
    // debug redirection function
    void qDebugToRFileLogger(QtMsgType type, const char *msg)
    {
        TPtrC8 symbian_msg((const TUint8*)msg);
        RFileLogger::Write(KTfLogFolder,KTfLogFile,
            EFileLoggingModeAppend,symbian_msg);
        if (originalMsgHandler) originalMsgHandler(type,msg);
    }
    // ------------------------------------------------------------
#endif


    
int main(int argc, char *argv[])
{
#ifdef SAT_DEBUG_TO_FILE
    originalMsgHandler = qInstallMsgHandler(qDebugToRFileLogger);
#endif

    qDebug("SATAPP: main");

    HbApplication app(argc, argv);
    qDebug("SATAPP: main app created");
    
    QTranslator translator;
    bool ok = translator.load(":/translations/satapp");
    qDebug("SATUI: main, translation %d (1=OK, 0=fail)", ok );
    app.installTranslator(&translator);

    HbMainWindow window;
    qDebug("SATAPP: main window created");

    SatAppMainHandler *mainHandler = new SatAppMainHandler(window);

    window.show();
    qDebug("SATAPP: main view showed");

    int result = app.exec();
    qDebug("SATAPP: main exit %d", result);

    delete mainHandler;
    return result;
}

//End of file
