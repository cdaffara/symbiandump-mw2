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
* Description:
*
*/
#include <QtGui>
#include <QTranslator>

#include <hbapplication.h>
#include <hbinstance.h>

#include "cbsuimainwindow.h"
#include "cbslog.h"

int main(int argc, char *argv[])
{
    LOG(NORMAL,"CBSUI: main IN");
    
    // Initialization
    HbApplication app(argc, argv);
    
    QTranslator translator;
    bool ok = translator.load(":/translations/cbsui_en");
    LOG2(NORMAL, "CBSUI: main, translation %d (1=OK, 0=fail)", ok )
    app.installTranslator(&translator);

    // Main window widget. 
    // Includes decorators such as signal strength and battery life indicator.
    CbsUiMainWindow mainWindow;
    #ifndef __WINSCW
    LOG(NORMAL,"CBSUI: main set LANDSCAPE mode");
    hbInstance->setOrientation(Qt::Horizontal);
    #endif
    mainWindow.show();
    LOG(NORMAL,"CBSUI: main mainwindow show over");
    // Enter event loop
    int result = app.exec();
    LOG2(NORMAL,"CBSUI: main exit %d", result);
    return result;
}
