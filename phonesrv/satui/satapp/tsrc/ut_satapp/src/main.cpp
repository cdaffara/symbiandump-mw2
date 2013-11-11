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
* Description:Unit testing of SatApp
*
*
*/


#include <QtTest/QtTest>
#include "hbapplication.h"
#include "hbinstance.h"

#include "ut_satappmainhandler.h"
#include "ut_satappmenuprovider.h"
#include "ut_satappinputprovider.h"
#include "ut_satapptoneprovider.h"
#include "ut_satapppopupprovider.h"
#include "ut_satappserverdispatcher.h"
#include "ut_satappconfirmprovider.h"
#include "ut_satappaction.h"

int main(int argc, char *argv[])
{
    qDebug("Ut_SatApp: main() >");
    HbApplication app(argc,argv);
    HbInstance::instance();
    HbMainWindow mainWindow;

    // test cmdline parameters
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 

    int result(0);

    Ut_SatAppPopupProvider utPopupProvider;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppPopupProvider.txt";
    result += QTest::qExec(&utPopupProvider, /*3*/ 1, pass);

    Ut_SatAppConfirmProvider utConfirmProvide;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppConfirmProvider.txt";
    result += QTest::qExec(&utConfirmProvide, /*3*/ 1, pass);

    Ut_SatAppAction utAction;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppAction.txt";
    result += QTest::qExec(&utAction, /*3*/ 1, pass);

    Ut_SatAppMenuProvider utMenuProvider(&mainWindow);
    pass[2] = "c:\\logs\\sat\\Ut_SatAppMenuProvider.txt";
    result += QTest::qExec(&utMenuProvider, /*3*/ 1, pass);

    Ut_SatAppInputProvider utInputProvider;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppInputProvider.txt";
    result += QTest::qExec(&utInputProvider, /*3*/ 1, pass);

    Ut_SatAppToneProvider utToneProvider;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppToneProvider.txt";
    result += QTest::qExec(&utToneProvider, /*3*/ 1, pass);

    Ut_SatAppServerDispatcher utServerDispatcher;
    pass[2] = "c:\\logs\\sat\\Ut_SatAppServerDispatcher.txt";
    result += QTest::qExec(&utServerDispatcher, /*3*/ 1, pass);

    Ut_SatAppMainHandler utMainHandler(&mainWindow);
    pass[2] = "c:\\logs\\sat\\ut_SatAppMainHandler.txt";
    result = QTest::qExec(&utMainHandler, /*3*/ 1, pass);

    // cause a crash to free binary, so that we can recompile and run
    // unit tests without restarting the emulator
    //int crash = ((QString*)0)->length();
    return result;
}

//End of file
