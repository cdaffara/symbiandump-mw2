/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_psuidivertnotehandler.h"
#include "psuidivertnotehandler.h"
#include "psetcalldivertingwrapper.h"
#include "qtestmains60.h"
#include <smcmockclassincludes.h>

class CPsetContainer
{
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
};

/*QString hbTrId(const char *id, int n)
{
    SMC_MOCK_METHOD2( QString, const char *, id, int, n )    
}*/


/*!
  UT_psuidivertnotehandler::UT_psuidivertnotehandler
 */
UT_psuidivertnotehandler::UT_psuidivertnotehandler() 
{
    
}

/*!
  UT_psuidivertnotehandler::~UT_psuidivertnotehandler
 */
UT_psuidivertnotehandler::~UT_psuidivertnotehandler()
{
    
}

/*!
  UT_psuidivertnotehandler::init
 */
void UT_psuidivertnotehandler::init()
{
    initialize();
}

/*!
  UT_psuidivertnotehandler::cleanup
 */
void UT_psuidivertnotehandler::cleanup()
{
    reset();
}

/*!
  UT_psuidivertnotehandler::t_memleak
 */
void UT_psuidivertnotehandler::t_memleak()
{
    
}

/*!
  UT_psuidivertnotehandler::t_construction
 */
void UT_psuidivertnotehandler::t_construction()
{
    expect("QObject::connect").times(4);
    CPsetContainer psetContainer;
    PSetCallDivertingWrapper *mockDivertWrapper = new PSetCallDivertingWrapper(psetContainer,this);
    PsUiDivertNoteHandler *divertHandler = new PsUiDivertNoteHandler(*mockDivertWrapper);
    delete divertHandler;
    delete mockDivertWrapper;
    QVERIFY(verify());
}

void UT_psuidivertnotehandler::t_handleDivertingChanged()
{
    CPsetContainer psetContainer;
    PSetCallDivertingWrapper *mockDivertWrapper = new PSetCallDivertingWrapper(psetContainer,this);
    PsUiDivertNoteHandler *divertHandler = new PsUiDivertNoteHandler(*mockDivertWrapper);
    
    // Divert activated, plural false
    PSCallDivertingCommand testCommand;
    testCommand.iStatus = DivertingStatusActive;
    expect("PsUiNotes::showGlobalNotificationDialog");
    divertHandler->handleDivertingChanged(testCommand, false);
    QVERIFY(verify());  
    
    // Divert activated, plural true
    testCommand.iStatus = DivertingStatusActive;
    expect("PsUiNotes::showGlobalNotificationDialog");
    divertHandler->handleDivertingChanged(testCommand, true);
    QVERIFY(verify());  
    
    // Divert deactivated, plural false
    testCommand.iStatus = DivertingStatusInactive;
    expect("PsUiNotes::showGlobalNotificationDialog");
    divertHandler->handleDivertingChanged(testCommand, false);
    QVERIFY(verify());  
    
    // Divert deactivated, plural true
    testCommand.iStatus = DivertingStatusInactive;
    expect("PsUiNotes::showGlobalNotificationDialog");
    divertHandler->handleDivertingChanged(testCommand, true);
    QVERIFY(verify());  
    
    // Divert status unknown
    testCommand.iStatus = DivertingStatusUnknown;
    expect("PsUiNotes::showGlobalNotificationDialog");
    divertHandler->handleDivertingChanged(testCommand, true);
    QVERIFY(verify());  

    delete divertHandler;
    delete mockDivertWrapper;
}

void UT_psuidivertnotehandler::t_handleDivertingStatus()
{
    CPsetContainer psetContainer;
    PSetCallDivertingWrapper *mockDivertWrapper = new PSetCallDivertingWrapper(psetContainer,this);
    PsUiDivertNoteHandler *divertHandler = new PsUiDivertNoteHandler(*mockDivertWrapper);
    QList<PSCallDivertingStatus*> divertList; 
    
    expect("PsUiNotes::showCallDivertDetails").with(divertList);
    divertHandler->handleDivertingStatus(divertList,false);
       
    QVERIFY(verify());  
    delete divertHandler;
    delete mockDivertWrapper;    
}

void UT_psuidivertnotehandler::t_handleDivertingError()
{
    CPsetContainer psetContainer;
    PSetCallDivertingWrapper *mockDivertWrapper = new PSetCallDivertingWrapper(psetContainer,this);
    PsUiDivertNoteHandler *divertHandler = new PsUiDivertNoteHandler(*mockDivertWrapper);
    int errorCode = -1;
    expect("PsUiNotes::cancelNote");
    expect("PsUiNotes::showGlobalErrorNote");
    divertHandler->handleDivertingError(errorCode);
     
    QVERIFY(verify());  
    delete divertHandler;
    delete mockDivertWrapper;        
}

void UT_psuidivertnotehandler::t_handleCFRequesting()
{
    CPsetContainer psetContainer;
    PSetCallDivertingWrapper *mockDivertWrapper = new PSetCallDivertingWrapper(psetContainer,this);
    PsUiDivertNoteHandler *divertHandler = new PsUiDivertNoteHandler(*mockDivertWrapper);
  
    bool ongoing = true;
    bool interrupted = false;
    expect("PsUiNotes::showGlobalProgressNote");
    divertHandler->handleCFRequesting(ongoing,interrupted); 
    QVERIFY(verify()); 
    
    ongoing = false;
    interrupted = true;
    expect("PsUiNotes::cancelNote").times(2);
    divertHandler->handleCFRequesting(ongoing,interrupted);
    QVERIFY(verify());
    
    ongoing = false;
    interrupted = false;
    expect("PsUiNotes::cancelNote");
    divertHandler->handleCFRequesting(ongoing,interrupted);
    QVERIFY(verify());

    delete divertHandler;
    delete mockDivertWrapper;           
}

QTEST_MAIN_S60(UT_psuidivertnotehandler)
