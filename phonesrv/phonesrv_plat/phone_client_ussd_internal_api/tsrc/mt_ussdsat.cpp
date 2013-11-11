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
* Description:  Implementation of the Mt_UssdSat class
*
*/

#include <cphcltussdsatclient.h>
#include <etelsat.h>
#include "mt_ussdsat.h"

/*------------------------------------------------------------------------------
This module testing project links to phoneclient.dll.
------------------------------------------------------------------------------*/

// -----------------------------------------------------------------------------
// Mt_UssdSat::Mt_UssdSat
// Default constructor, remember to null new members here.
// -----------------------------------------------------------------------------
//
Mt_UssdSat::Mt_UssdSat():mUssdSat(NULL)
{
    qDebug("Mt_UssdSat in-out");
}

// -----------------------------------------------------------------------------
// Mt_UssdSat::~Mt_UssdSat
// -----------------------------------------------------------------------------
Mt_UssdSat::~Mt_UssdSat()
{
    qDebug("~Mt_UssdSat in-out");
}


// -----------------------------------------------------------------------------
// Mt_UssdSat::initTestCase
// QTestLib initialization method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_UssdSat::initTestCase()
{
    qDebug("initTestCase in");
    mUssdSat = CPhCltUssdSatClient::NewL( EFalse );
    qDebug("initTestCase out");
}


// -----------------------------------------------------------------------------
// Mt_UssdSat::cleanupTestCase
// QTestLib cleanup method, called for each test case.
// -----------------------------------------------------------------------------
void Mt_UssdSat::cleanupTestCase()
{
    qDebug("cleanupTestCase in");
    delete mUssdSat;
    qDebug("cleanupTestCase out");
}

// -----------------------------------------------------------------------------
// Mt_UssdSat::testSendSatMessage
// -----------------------------------------------------------------------------
void Mt_UssdSat::testSendSatMessage()
{
    qDebug("testSendSatMessage in");
    
    _LIT( KUssdSat, "*100#"); // valid for BTN
   
    TBuf<RSat::KStringMaxSize> receiveMessage;
    TBool sendCompletedFirst( EFalse );
    TUint8 receiveDcs = 0;
    
    TInt ret = mUssdSat->SendSatMessage(
        KUssdSat,
        CPhCltUssdSatClient::EPhCltDcsUnknown,
        receiveMessage,
        sendCompletedFirst,
        receiveDcs );
        
    qDebug("receive error=%d", ret);
    qDebug("receive receive msg=%s", &receiveMessage);
    qDebug("receive isCompleteFirst=%d", &sendCompletedFirst);
    qDebug("receive receive Dcs=%d", &receiveDcs);
    
    QVERIFY2(KErrNone == ret , "testSendSatMessage failed ");
    qDebug("testSendSatMessage out");
}

// -----------------------------------------------------------------------------
// Mt_UssdSat::testSendSatMessageCancel
// -----------------------------------------------------------------------------
void Mt_UssdSat::testSendSatMessageCancel()
{
    qDebug("testSendSatMessageCancel in");
    
    mUssdSat->SendSatMessageCancel();
    
    qDebug("testSendSatMessageCancel out");
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
    Mt_UssdSat tc; 
    char *pass[3];
    pass[0] = argv[0];
    pass[1] = "-o"; 
    pass[2] = "c:\\logs\\mt_ussdsat.txt";
    const int result = QTest::qExec(&tc, 3, pass);
    qDebug("End testing...");
    
    qDebug("main() out, result=%d", result);
    return result;
    } 

//End file
