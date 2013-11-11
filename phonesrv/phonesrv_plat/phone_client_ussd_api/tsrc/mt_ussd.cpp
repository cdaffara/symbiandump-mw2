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
* Description:  Implementation of the Mt_Ussd class
*
*/

#include <cphcltussd.h>
#include "mt_ussd.h"

/*------------------------------------------------------------------------------
This module testing project links to phoneclient.dll.
------------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Mt_Ussd::Mt_Ussd
// Default constructor, remember to null new members here.
// -----------------------------------------------------------------------------
//
Mt_Ussd::Mt_Ussd():mUssd(NULL)
{
    qDebug("Mt_Ussd in-out");
}

// -----------------------------------------------------------------------------
// Mt_Ussd::~Mt_Ussd
// -----------------------------------------------------------------------------
Mt_Ussd::~Mt_Ussd()
{
    qDebug("~Mt_Ussd in-out");
}


// -----------------------------------------------------------------------------
// Mt_Ussd::initTestCase
// QTestLib initialization method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_Ussd::initTestCase()
{
    qDebug("initTestCase in");
    mUssd = CPhCltUssd::NewL( EFalse );
    qDebug("initTestCase out");
}


// -----------------------------------------------------------------------------
// Mt_Ussd::cleanupTestCase
// QTestLib cleanup method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_Ussd::cleanupTestCase()
{
    qDebug("cleanupTestCase in");
    delete mUssd;
    qDebug("cleanupTestCase out");
}

// -----------------------------------------------------------------------------
// Mt_Ussd::testSendUssd
// -----------------------------------------------------------------------------
void Mt_Ussd::testSendUssdUnicode()
{
    qDebug("testSendUssdUnicode in");
    
    mUssd->SetDCS( KPhCltDcsUnknown );
    
    _LIT( KUnicodeUSSD, "Ussd msg");
    TInt ret = mUssd->SendUssd( KUnicodeUSSD );
    qDebug("ussd str=%S, ret=%d", KUnicodeUSSD, ret);
    
    QVERIFY2(KErrNone == ret , "testSendUssdUnicode failed ");
    qDebug("testSendUssdUnicode out");
}

// -----------------------------------------------------------------------------
// Mt_Ussd::testSendUssd
// -----------------------------------------------------------------------------
void Mt_Ussd::testSendUssd()
{
    qDebug("testSendUssd in");
    
    mUssd->SetDCS( KPhCltDcsUnknown );
    _LIT8( KMsg, "*#123#" );
    TInt ret = mUssd->SendUssd( KMsg );
    qDebug("ussd str=%S, ret=%d", KMsg, ret);
    
    QVERIFY2(KErrNone == ret , "testSendUssd failed ");
    qDebug("testSendUssd out");
}

// -----------------------------------------------------------------------------
// Mt_Ussd::testSendUssd
// -----------------------------------------------------------------------------
void Mt_Ussd::testSendUssdWithDCS()
{
    qDebug("testSendUssdWithDCS in");
    
    _LIT8( KMsg, "*#123#" );
    TInt ret = mUssd->SendUssd( KMsg, KPhCltDcsUnknown );
    qDebug("ussd str=%S, ret=%d", KMsg, ret);
    
    QVERIFY2(KErrNone == ret , "testSendUssdWithDCS failed ");
    qDebug("testSendUssdWithDCS out");
}

void Mt_Ussd::testSendUssdBtn()
{
    qDebug("testSendUssdBtn in");
    
    _LIT8( KMsg, "*100#" );
    TInt ret = mUssd->SendUssd( KMsg, KPhCltDcsUnknown );
    qDebug("ussd str=%S, ret=%d", KMsg, ret);
    
    QVERIFY2(KErrNone == ret , "testSendUssdBtn failed ");
    qDebug("testSendUssdBtn out");
}

void Mt_Ussd::testSendUssdCmcc()
{
    qDebug("testSendUssdCmcc in");
    
    _LIT8( KMsg, "*188#" );
    TInt ret = mUssd->SendUssd( KMsg, KPhCltDcsUnknown );
    qDebug("ussd str=%S, ret=%d", KMsg, ret);
    
    QVERIFY2(KErrNone == ret , "testSendUssdCmcc failed ");
    qDebug("testSendUssdCmcc out");
}

// -----------------------------------------------------------------------------
// main()
// Main method implemented for directing test output to a file.
// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
    {
    qDebug("main() in");
    QApplication app(argc, argv);

    qDebug("Start testing...");
    Mt_Ussd tc; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\ussd\\mt_ussdclient.txt";
    const int result = QTest::qExec(&tc, 3, pass);
    qDebug("End testing...");
    
    qDebug("main() out, result=%d", result);
    return result;
    } 

//End file
