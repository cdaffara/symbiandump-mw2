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

#include "ut_ussd.h"
#include "ut_cphcltussdnotecontroller.h"

// ====================================================================
// MAIN
// ====================================================================

int main(int argc, char *argv[])
{
    qDebug("Ut_ussd: main() >");
    QApplication app(argc, argv);

    int result(0);

    Ut_Ussd test;
//    char *my_argv[3];
//    my_argv[0] = argv[0];
//    my_argv[1] = "-o";
//    my_argv[2] = "c:\\logs\\ussd\\ut_ussd.txt";
    result = QTest::qExec(&test/*, 3, my_argv*/);
    qDebug("Ut_ussd: main() test Ut_Ussd<, result=%d", result);

    Ut_CphCltUssdNoteController testNoteController;
//    char *note_argv[3];
//    note_argv[0] = argv[0];
//    note_argv[1] = "-o";
//    note_argv[2] = "c:\\logs\\ussd\\ut_ussdNoteController.txt";
    result = QTest::qExec(&testNoteController/*, 3, note_argv*/);
    qDebug("Ut_ussd: main() test Ut_CphCltUssdNoteController<, result=%d",
        result);

    return result;
}

//End of file
