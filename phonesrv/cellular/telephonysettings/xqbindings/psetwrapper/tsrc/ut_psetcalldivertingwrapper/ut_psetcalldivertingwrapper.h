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

#ifndef UT_PSETCALLDIVERTINGWRAPPER_H
#define UT_PSETCALLDIVERTINGWRAPPER_H

#include <QtTest/QtTest>
#include <mockservice.h>

class PSetCallDivertingWrapper;
class CPsetContainer;
class CPsetCallDiverting;

class UT_PSetCallDivertingWrapper : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_PSetCallDivertingWrapper();
    ~UT_PSetCallDivertingWrapper();
    
private slots:

    void init();
    void cleanup();
    void t_construction();
    
    void t_setCallDivertingWithValidParameters();
    void t_setCallDivertingWithInvalidNumber();
    void t_setCallDivertingWithEmergencyNumber();
    void t_setCallDivertingException();
    
    void t_getCallDivertingStatus();
    void t_getCallDivertingStatusException();
    void t_cancelProcess();

    void t_getDefaultNumbers();
    void t_getDefaultNumbersException();
    void t_setNewDefaultNumber();
    void t_getVoiceMailBoxNumber();
    void t_queryVoiceMailBoxNumber();
    void t_swapDefaultNumber();

    // tests for private implementation
    void t_handleDivertingChanged();
    void t_handleDivertingStatus();
    void t_handleDivertingError();
    void t_handleCFRequesting();
    void t_setEngineContact();
    void t_handleEmergencyDial();
    
    void t_convertPsCallDivertingCondition();
    void t_convertPsCallDivertingStatus();
    void t_convertPsCallDivertingSetting();
    void t_exceptionSafety();
    
private:
    
    void simulateLeaveOnMockMethodCallL();
    
private:
    PSetCallDivertingWrapper *mWrapper;
    CPsetContainer *mSetContainerMock;
    CPsetCallDiverting *mPsetCallDivertingMock;
};

#endif
