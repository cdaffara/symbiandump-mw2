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

#ifndef UT_PSETCALLWAITINGWRAPPER_H
#define UT_PSETCALLWAITINGWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class CPsetCallWaiting;
class PSetCallWaitingWrapper;

class UT_PSetCallWaitingWrapper : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_PSetCallWaitingWrapper();
    ~UT_PSetCallWaitingWrapper();
    
private slots:

    void init();
    void cleanup();

    void t_construction();
    void t_setCallWaiting();
    void t_getCallWaitingStatus();
    void t_cancelProcess();
    
    // signal emitting/private implementation tests
    void t_handleCallWaitingGetStatus();
    void t_handleCallWaitingChanged();
    void t_handleCallWaitingRequesting();
    void t_handleCallWaitingError();
    void t_setEngineContact();
    
    void t_exceptionSafety();
    
private:
    
    void SimulateLeaveAtMockMethodCallL();
    
private:
    
    int m_numOfLeaveSimulations;
    CPsetContainer *m_psetContainerMock;
    CPsetCallWaiting *m_psetCallWaitingMock;
    PSetCallWaitingWrapper* m_wrapper;
};

#endif  // UT_PSETCALLWAITINGWRAPPER_H
