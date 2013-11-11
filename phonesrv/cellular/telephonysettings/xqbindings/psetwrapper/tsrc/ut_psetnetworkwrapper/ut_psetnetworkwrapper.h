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

class PSetNetworkWrapper;
class CPsetContainer;
class CPsetNetwork;
class CPSetRefreshHandlerMock;

class UT_PSetNetworkWrapper : public QObject, public MockService
{
    Q_OBJECT

public:    
    UT_PSetNetworkWrapper();
    ~UT_PSetNetworkWrapper();
    
private slots:

    void init();
    void cleanup();
    
    void t_construction();
    void t_getAvailableNetworks();
    void t_getNetworkSelectionMode();
    void t_selectNetwork();
    void t_cancelRequest();
    
    void t_getNetworkAccessMode();
    void t_setNetworkAccessMode();

    // private implementation tests
    // tests for MPsetNetworkInfoObserver functionality
    void t_handleNetworkInfoReceived();
    void t_handleCurrentNetworkInfo();
    void t_handleNetworkChangedWithNetworkStatus();
    void t_handleNetworkChangedWithPhoneRegistrationStatus();
    void t_handleSearchingNetworks();
    void t_handleRequestingSelectedNetwork();
    void t_handleCallActivated();
    void t_handleNetworkErrorFromInfoObserver();
    
    // tests for MPsetNetworkModeObserver functionality
    void t_handleNetworkSystemModeEvents();
    void t_handleNetworkErrorFromModeObserver();
    
    void t_exceptionSafety();
    
    void t_isManualNetworkSelectionSupported();
    void t_AllowRefresh();
    void t_Refresh();
    
private:
    PSetNetworkWrapper *m_wrapper;
    CPsetContainer *m_psetContainerMock;
    CPsetNetwork *m_psetNetworkMock;
    CPsetNetwork *m_psetNetworkModeMock;
    CPSetRefreshHandlerMock *m_refreshHandler;
};

#endif
