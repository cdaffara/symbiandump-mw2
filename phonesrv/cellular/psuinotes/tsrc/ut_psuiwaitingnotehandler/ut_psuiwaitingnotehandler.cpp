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

#include "ut_psuiwaitingnotehandler.h"
#include "psuiwaitingnotehandler.h"
#include "psetcallwaitingwrapper.h"
#include "qtestmains60.h"
#include <smcmockclassincludes.h>

class CPsetContainer
{
public:
    CPsetContainer(){};
    ~CPsetContainer(){};
};

/*!
  UT_psuiwaitingnotehandler::UT_psuiwaitingnotehandler
 */
UT_psuiwaitingnotehandler::UT_psuiwaitingnotehandler() 
{
    
}

/*!
  UT_psuiwaitingnotehandler::~UT_psuiwaitingnotehandler
 */
UT_psuiwaitingnotehandler::~UT_psuiwaitingnotehandler()
{
    
}

/*!
  UT_psuiwaitingnotehandler::init
 */
void UT_psuiwaitingnotehandler::init()
{
    initialize();
    m_psetContainer = new CPsetContainer();
    m_mockWaitingWrapper = new PSetCallWaitingWrapper(*m_psetContainer,this);
    m_waitingHandler = new PsUiWaitingNoteHandler(*m_mockWaitingWrapper);
}

/*!
  UT_psuiwaitingnotehandler::cleanup
 */
void UT_psuiwaitingnotehandler::cleanup()
{
    delete m_waitingHandler;
    delete m_mockWaitingWrapper;   
    delete m_psetContainer;
    reset();
}

/*!
  UT_psuiwaitingnotehandler::t_memleak
 */
void UT_psuiwaitingnotehandler::t_memleak()
{
    
}

/*!
  UT_psuiwaitingnotehandler::t_construction
 */
void UT_psuiwaitingnotehandler::t_construction()
{
    expect("QObject::connect").times(4);
    CPsetContainer psetContainer;
    PSetCallWaitingWrapper *mockWaitingWrapper = new PSetCallWaitingWrapper(psetContainer,this);
    PsUiWaitingNoteHandler *waitingHandler = new PsUiWaitingNoteHandler(*mockWaitingWrapper);
    delete waitingHandler;
    delete mockWaitingWrapper;
    QVERIFY(verify());
}

void UT_psuiwaitingnotehandler::t_handleCallWaitingGetStatus()
{
    
    PSetCallWaitingWrapper::PsCallWaitingStatus status = 
            PSetCallWaitingWrapper::StatusNotProvisioned;
    QList<unsigned char> serviceGroups;
    expect("PsUiNotes::cancelNote");
    expect("PsUiSettingsWrapper::isFeatureCallWaitingDistiquishNotProvisionedEnabled").returns(true);
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingGetStatus(status,serviceGroups);
    QVERIFY(verify());
    
    status = PSetCallWaitingWrapper::StatusActive;
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingGetStatus(status,serviceGroups);
    QVERIFY(verify());
    
    status = PSetCallWaitingWrapper::StatusDisabled;
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingGetStatus(status,serviceGroups);
    QVERIFY(verify());
}

void UT_psuiwaitingnotehandler::t_handleCallWaitingChanged()
{  
    PSetCallWaitingWrapper::PsCallWaitingCommand setting;
    int result;
    
    setting = PSetCallWaitingWrapper::ActivateCallWaiting;
    result = true;
    expect("PsUiNotes::showGlobalNote");
    m_waitingHandler->handleCallWaitingChanged(setting,result);
    QVERIFY(verify());
    
    setting = PSetCallWaitingWrapper::ActivateCallWaiting;
    result = false;
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingChanged(setting,result);
    QVERIFY(verify());
    
    setting = PSetCallWaitingWrapper::DeactivateCallWaiting;
    result = false;
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingChanged(setting,result);
    QVERIFY(verify());
    
    setting = PSetCallWaitingWrapper::CheckCallWaitingStatus;
    result = false;
    expect("PsUiNotes::showGlobalNotificationDialog");
    m_waitingHandler->handleCallWaitingChanged(setting,result);
    QVERIFY(verify());
}

void UT_psuiwaitingnotehandler::t_handleCallWaitingRequesting()
{
    expect("PsUiNotes::showGlobalProgressNote");
    bool ongoing = true;
    bool interrupted = false;
    m_waitingHandler->handleCallWaitingRequesting(ongoing,interrupted);
    QVERIFY(verify());
    
    expect("PsUiNotes::cancelNote");
    ongoing = false;
    interrupted = true;
    m_waitingHandler->handleCallWaitingRequesting(ongoing,interrupted);
   
    QVERIFY(verify()); 
}

void UT_psuiwaitingnotehandler::t_handleCallWaitingError()
{  
    expect("PsUiNotes::cancelNote");
    expect("PsUiNotes::showGlobalErrorNote");
    int error = -1;
    m_waitingHandler->handleCallWaitingError(error);
    
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_psuiwaitingnotehandler)
