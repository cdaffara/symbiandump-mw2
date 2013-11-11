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

#ifndef UT_PSETCALLBARRINGWRAPPER_H
#define UT_PSETCALLBARRINGWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class CPsetContainer;
class PSetCallBarringWrapper;

class UT_PSetCallBarringWrapper : public QObject, public MockService
{
    Q_OBJECT

public:    
    
    UT_PSetCallBarringWrapper();
    ~UT_PSetCallBarringWrapper();
    
private slots:

    void init();
    void cleanup();
    
    void t_construction();
    void t_barringStatus();
    void t_barringStatusExceptionOccurs();
    void t_enableBarring();
    void t_disableBarring();
    void t_changeBarringPassword();
    
    // private implementation tests
    void t_HandleBarringModeStatusL();
    void t_HandleBarringModeStatusNoBasicServiceGroupIdsL();
    
    void t_HandleBarringModeChangedL();
    void t_HandleBarringErrorL();
    void t_HandleCBRequestingL();
    void t_SetEngineContact();
    void t_CbPasswordChangedL();
    
    void t_enableBarringRequestComplete();
    void t_disableBarringRequestComplete();
    void t_changeBarringPasswordRequestComplete();
    void t_RequestStatusChanged();
    
    void t_exceptionSafety();

private:
    
    void simulateLeaveAtMockMethodCall();
    
private:
    
    CPsetContainer *m_psetContainerMock;
    PSetCallBarringWrapper* m_wrapper;
};

#endif  // UT_PSETCALLBARRINGWRAPPER_H
