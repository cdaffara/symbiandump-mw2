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

#include <PsetContainer.h>
#include <mpsetnetworkinfoobs.h>
#include <gsmerror.h>
#include <mock_cpsetrefreshhandler.h>
#include "ut_psetnetworkwrapper.h"
#include "testutilities.h"
#define private public
#include "psetnetworkwrapper.h"
#include "psetnetworkwrapper_p.h"

class NetworkInfoObserver : public MPsetNetworkInfoObserver
{

    void HandleNetworkInfoReceivedL( 
        const CNetworkInfoArray* aInfoArray, 
        const TInt aResult)
    {
        Q_UNUSED(aInfoArray);
        Q_UNUSED(aResult);
    }

    void HandleCurrentNetworkInfoL( 
        const MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, 
        const TInt aResult)
    {
        Q_UNUSED(aCurrentInfo);
        Q_UNUSED(aResult);
    }

    void HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
        const MPsetNetworkSelect::TCurrentNetworkStatus aStatus, 
        const TInt aResult)
    {
        Q_UNUSED(aCurrentInfo);
        Q_UNUSED(aStatus);
        Q_UNUSED(aResult);
    }

    void HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
        const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus, 
        const TInt aResult)
    {
        Q_UNUSED(aCurrentInfo);
        Q_UNUSED(aStatus);
        Q_UNUSED(aResult);
    }

    void HandleSearchingNetworksL( 
        TServiceRequest aRequest)
    {
        Q_UNUSED(aRequest);
    }

    void HandleRequestingSelectedNetworkL( 
        TBool aOngoing)
    {
        Q_UNUSED(aOngoing);
    }

    void HandleCallActivatedL()
    {
        
    }

    void HandleNetworkErrorL( 
        const MPsetNetworkInfoObserver::TServiceRequest aRequest,
        const TInt aError)
    {
        Q_UNUSED(aRequest);
        Q_UNUSED(aError);
    }
};

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}

/*!
  UT_PSetNetworkWrapper::UT_PSetNetworkWrapper
 */
UT_PSetNetworkWrapper::UT_PSetNetworkWrapper() 
    : 
    m_wrapper(NULL), 
    m_psetContainerMock(NULL), 
    m_psetNetworkMock(NULL),
    m_psetNetworkModeMock(NULL)
{
}

/*!
  UT_PSetNetworkWrapper::~UT_PSetNetworkWrapper
 */
UT_PSetNetworkWrapper::~UT_PSetNetworkWrapper()
{
    delete m_wrapper;
    delete m_psetContainerMock;
    m_psetNetworkMock = NULL;
    m_psetNetworkModeMock = NULL;
}

/*!
  UT_PSetNetworkWrapper::init
 */
void UT_PSetNetworkWrapper::init()
{
    initialize();
    
    m_psetContainerMock = new CPsetContainer();
    m_refreshHandler = CPSetRefreshHandlerMock::NewL();
    SmcDefaultValue<CPSetRefreshHandler *>::SetL(m_refreshHandler);
    
    RMobilePhone dummyHandle;
    NetworkInfoObserver dummyObserver;
    QT_TRAP_THROWING(m_psetNetworkMock = 
        CPsetNetwork::NewL(dummyHandle, dummyObserver));
    QT_TRAP_THROWING(m_psetNetworkModeMock = 
        CPsetNetwork::NewL(dummyHandle, dummyObserver));
    
    expect("CPsetContainer::CreateNetworkObjectL").returns(m_psetNetworkMock);
    expect("CPsetContainer::CreateNetworkModeObjectL").
        returns(m_psetNetworkModeMock);
    
    // wrapper takes ownership of networkmocks during successfull
    // construction
    QScopedPointer<CPsetNetwork> networkMockPtr(m_psetNetworkMock);
    QScopedPointer<CPsetNetwork> networkModeMockPtr(m_psetNetworkModeMock);
    m_wrapper = new PSetNetworkWrapper(*m_psetContainerMock, NULL);
    networkMockPtr.take();
    networkModeMockPtr.take();
}

/*!
  UT_PSetNetworkWrapper::cleanup
 */
void UT_PSetNetworkWrapper::cleanup()
{
    reset();
    
    delete m_wrapper;
    m_wrapper = NULL;
    delete m_psetContainerMock;
    m_psetContainerMock = NULL;
    m_psetNetworkMock = NULL;
    m_psetNetworkModeMock = NULL;
}

/*!
  UT_PSetNetworkWrapper::t_construction
 */
void UT_PSetNetworkWrapper::t_construction()
{
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") != 0) {
        
        expect("CPsetContainer::CreateNetworkObjectL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        PSetNetworkWrapper *wrapper = NULL;
        EXPECT_EXCEPTION(
            wrapper = new PSetNetworkWrapper(*m_psetContainerMock, NULL);
            delete wrapper;
            wrapper = NULL;
        )
        QVERIFY(verify());
        
        expect("CPsetContainer::CreateNetworkModeObjectL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        EXPECT_EXCEPTION(
            wrapper = new PSetNetworkWrapper(*m_psetContainerMock, NULL);
            delete wrapper;
            wrapper = NULL;
        )
        QVERIFY(verify());
    }
}

/*!
  UT_PSetNetworkWrapper::t_getAvailableNetworks
 */
void UT_PSetNetworkWrapper::t_getAvailableNetworks()
{
    expect("CPsetNetwork::GetAvailableNetworksL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getAvailableNetworks();
    )
    
    QVERIFY(verify());
}


/*!
  UT_PSetNetworkWrapper::t_getNetworkSelectionMode
 */
void UT_PSetNetworkWrapper::t_getNetworkSelectionMode()
{
    expect("CPsetNetwork::GetNetworkSelectMode")/*.
        with(MPsetNetworkSelect::ENetSelectModeManual)*/.
        returns(KErrNone);
    expect("CPsetNetwork::GetNetworkSelectMode")/*.
        with(MPsetNetworkSelect::ENetSelectModeAutomatic)*/.
        returns(KErrGeneral);
    
    PSetNetworkWrapper::NetworkSelectionMode selectionModeManual =
        PSetNetworkWrapper::SelectionModeManual;
    PSetNetworkWrapper::NetworkSelectionMode selectionModeAutomatic =
        PSetNetworkWrapper::SelectionModeAutomatic;
    m_wrapper->getNetworkSelectionMode(selectionModeManual);
    m_wrapper->getNetworkSelectionMode(selectionModeAutomatic);
    
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetNetworkWrapper::t_selectNetwork
 */
void UT_PSetNetworkWrapper::t_selectNetwork()
{
    expect("CPsetNetwork::SelectNetworkL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    
    PSetNetworkWrapper::NetworkInfo info;
    EXPECT_EXCEPTION(
        m_wrapper->selectNetwork(info);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetNetworkWrapper::t_cancelRequest
 */
void UT_PSetNetworkWrapper::t_cancelRequest()
{
    expect("CPsetNetwork::CancelProcess");
    
    m_wrapper->cancelRequest();
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetNetworkWrapper::t_getNetworkAccessMode
 */
void UT_PSetNetworkWrapper::t_getNetworkAccessMode()
{
    expect("CPsetNetwork::GetCurrentNetworkModeSelectionL")
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getNetworkAccessMode();
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetNetworkWrapper::t_setNetworkAccessMode
 */
void UT_PSetNetworkWrapper::t_setNetworkAccessMode()
{
    expect("CPsetNetwork::SetNetworkModeSelectionL").
        with(RMmCustomAPI::KCapsNetworkModeDual);
    expect("CPsetNetwork::SetNetworkModeSelectionL").
        with(RMmCustomAPI::KCapsNetworkModeUmts);
    expect("CPsetNetwork::SetNetworkModeSelectionL")
        .with(RMmCustomAPI::KCapsNetworkModeGsm)
        .willOnce(invokeWithoutArguments(SimulateLeaveL));
    
    m_wrapper->setNetworkAccessMode(PSetNetworkWrapper::AccessModeDual);
    m_wrapper->setNetworkAccessMode(PSetNetworkWrapper::AccessModeUmts);
    EXPECT_EXCEPTION(
        m_wrapper->setNetworkAccessMode(PSetNetworkWrapper::AccessModeGsm);
    )
    
    const int invalidMode = -1;
    m_wrapper->setNetworkAccessMode(
        reinterpret_cast<const PSetNetworkWrapper::NetworkAccessMode&>
            (invalidMode));
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetNetworkWrapper::t_handleNetworkInfoReceived
 */
void UT_PSetNetworkWrapper::t_handleNetworkInfoReceived()
{
    CNetworkInfoArray* infos = NULL;
    
    TRAPD(result, m_wrapper->m_privateImpl->
        HandleNetworkInfoReceivedL(infos, KErrNone));
    QVERIFY(KErrArgument == result);
    
    const TInt KGranularity = 3;
    infos = new CNetworkInfoArray(KGranularity);
    QScopedPointer<CNetworkInfoArray> infoArrayGuard(infos);
    MPsetNetworkSelect::TNetworkInfo info1;
    infos->AppendL(info1);
    TRAP(result, m_wrapper->m_privateImpl->
        HandleNetworkInfoReceivedL(infos, KErrNone));
    QVERIFY(KErrNone == result);
    
    MPsetNetworkSelect::TNetworkInfo info2;
    infos->AppendL(info2);
    TRAP(result, m_wrapper->m_privateImpl->
        HandleNetworkInfoReceivedL(infos, KErrNone));
    QVERIFY(KErrNone == result);
    
    MPsetNetworkSelect::TNetworkInfo info3;
    infos->AppendL(info3);
    TRAP(result, m_wrapper->m_privateImpl->
        HandleNetworkInfoReceivedL(infos, KErrNone));
    QVERIFY(KErrNone == result);
}


/*!
  UT_PSetNetworkWrapper::t_handleCurrentNetworkInfo
 */
void UT_PSetNetworkWrapper::t_handleCurrentNetworkInfo()
{
    MPsetNetworkSelect::TCurrentNetworkInfo networkInfo;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleCurrentNetworkInfoL(networkInfo, KErrNone));
}

/*!
  UT_PSetNetworkWrapper::t_handleNetworkChangedWithNetworkStatus
 */
void UT_PSetNetworkWrapper::t_handleNetworkChangedWithNetworkStatus()
{
    MPsetNetworkSelect::TNetworkInfo networkInfo;
    MPsetNetworkSelect::TCurrentNetworkStatus networkStatus =
        MPsetNetworkSelect::ENotRegisteredNotSearching;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleNetworkChangedL(networkInfo, networkStatus, KErrNone));
}

/*!
  UT_PSetNetworkWrapper::t_handleNetworkChangedWithPhoneRegistrationStatus
 */
void UT_PSetNetworkWrapper::t_handleNetworkChangedWithPhoneRegistrationStatus()
{
    MPsetNetworkSelect::TNetworkInfo networkInfo;
    RMobilePhone::TMobilePhoneRegistrationStatus registrationStatus;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleNetworkChangedL(networkInfo, registrationStatus, KErrNone));
}


/*!
  UT_PSetNetworkWrapper::t_handleSearchingNetworks
 */
void UT_PSetNetworkWrapper::t_handleSearchingNetworks()
{
    MPsetNetworkInfoObserver::TServiceRequest request =
        MPsetNetworkInfoObserver::EServiceRequestNone;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleSearchingNetworksL(request));
}

/*!
  UT_PSetNetworkWrapper::t_handleRequestingSelectedNetwork
 */
void UT_PSetNetworkWrapper::t_handleRequestingSelectedNetwork()
{
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleRequestingSelectedNetworkL(EFalse));
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleRequestingSelectedNetworkL(ETrue));
}

/*!
  UT_PSetNetworkWrapper::t_handleCallActivated
 */
void UT_PSetNetworkWrapper::t_handleCallActivated()
{
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleCallActivatedL());
}

/*!
  UT_PSetNetworkWrapper::t_handleNetworkErrorFromInfoObserver
 */
Q_DECLARE_METATYPE(PSetNetworkWrapper::RequestType)
Q_DECLARE_METATYPE(PSetNetworkWrapper::ErrorCode)
void UT_PSetNetworkWrapper::t_handleNetworkErrorFromInfoObserver()
{
    qRegisterMetaType<PSetNetworkWrapper::RequestType>
        ("PSetNetworkWrapper::RequestType");
    qRegisterMetaType<PSetNetworkWrapper::ErrorCode>
        ("PSetNetworkWrapper::ErrorCode");
    QSignalSpy spy(m_wrapper, SIGNAL(networkReqestFailed(
        PSetNetworkWrapper::ErrorCode, PSetNetworkWrapper::RequestType)));
    
    MPsetNetworkInfoObserver::TServiceRequest request =
        MPsetNetworkInfoObserver::EServiceRequestNone;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleNetworkErrorL(
        request, KErrGsmNetCauseCallActive));
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleNetworkErrorL(
        request, KErrGsm0707NoNetworkService));
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleNetworkErrorL(
        request, KErrGsmOfflineOpNotAllowed));
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleNetworkErrorL(
        request, KErrGeneral));
    
    QCOMPARE(spy.count(), 4);
    QVERIFY(PSetNetworkWrapper::ErrCauseCallActive == 
        qvariant_cast<PSetNetworkWrapper::ErrorCode>(spy.at(0).at(0)));
    QVERIFY(PSetNetworkWrapper::ErrNoNetworkService == 
        qvariant_cast<PSetNetworkWrapper::ErrorCode>(spy.at(1).at(0)));
    QVERIFY(PSetNetworkWrapper::ErrOfflineOpNotAllowed == 
        qvariant_cast<PSetNetworkWrapper::ErrorCode>(spy.at(2).at(0)));
    QVERIFY(PSetNetworkWrapper::ErrNoNetworkAccess == 
        qvariant_cast<PSetNetworkWrapper::ErrorCode>(spy.at(3).at(0)));
}


/*!
  UT_PSetNetworkWrapper::t_handleNetworkSystemModeEvents
 */
void UT_PSetNetworkWrapper::t_handleNetworkSystemModeEvents()
{
    QSignalSpy spy(m_wrapper, SIGNAL(networkAccessModeGot(int)));
    
    MPsetNetworkModeObserver::TServiceRequest serviceRequest =
        MPsetNetworkModeObserver::EServiceRequestGetCurrentNetworkMode;
    RMmCustomAPI::TNetworkModeCaps modeCaps = 
        RMmCustomAPI::KCapsNetworkModeDual;
    QT_TRAP_THROWING( 
        m_wrapper->m_privateImpl->HandleNetworkSystemModeEventsL(
        serviceRequest, modeCaps));
    modeCaps =RMmCustomAPI::KCapsNetworkModeUmts;
    QT_TRAP_THROWING( 
        m_wrapper->m_privateImpl->HandleNetworkSystemModeEventsL(
        serviceRequest, modeCaps));
    modeCaps = RMmCustomAPI::KCapsNetworkModeGsm;
    QT_TRAP_THROWING( 
        m_wrapper->m_privateImpl->HandleNetworkSystemModeEventsL(
        serviceRequest, modeCaps));
    
    QCOMPARE(spy.count(), 3);
    const QList<QVariant> &arguments0 = spy.at(0);
    QVERIFY(PSetNetworkWrapper::AccessModeDual == arguments0.at(0).toInt());
    const QList<QVariant> &arguments1 = spy.at(1);
    QVERIFY(PSetNetworkWrapper::AccessModeUmts == arguments1.at(0).toInt());
    const QList<QVariant> &arguments2 = spy.at(2);
    QVERIFY(PSetNetworkWrapper::AccessModeGsm == arguments2.at(0).toInt());
    
    // TODO: test skeleton for switch cases not yet implemented 
    serviceRequest = MPsetNetworkModeObserver::EServiceRequestNone;
    QT_TRAP_THROWING( 
        m_wrapper->m_privateImpl->HandleNetworkSystemModeEventsL(
        serviceRequest, modeCaps));
    serviceRequest = 
        MPsetNetworkModeObserver::EServiceRequestSetSelectedNetworkMode;
    QT_TRAP_THROWING( 
        m_wrapper->m_privateImpl->HandleNetworkSystemModeEventsL(
        serviceRequest, modeCaps));
}

/*!
  UT_PSetNetworkWrapper::t_handleNetworkErrorFromModeObserver
 */
void UT_PSetNetworkWrapper::t_handleNetworkErrorFromModeObserver()
{
    MPsetNetworkModeObserver::TServiceRequest request =
        MPsetNetworkModeObserver::EServiceRequestNone;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->
        HandleNetworkErrorL(request, KErrNone));
}

/*!
  UT_PSetNetworkWrapper::t_exceptionSafety
 */
void UT_PSetNetworkWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}

/*!
  UT_PSetNetworkWrapper::t_isManualNetworkSelectionSupported
 */
void UT_PSetNetworkWrapper::t_isManualNetworkSelectionSupported()
{
    expect("CPsetCustomerServiceProfile::IsNetworkSelectionSupported").returns(KErrNone);
    m_wrapper->isManualNetworkSelectionSupported();
    QVERIFY(verify());
    
    expect("CPsetCustomerServiceProfile::IsNetworkSelectionSupported").returns(KErrGeneral);
    m_wrapper->isManualNetworkSelectionSupported();
    QVERIFY(verify());
}

/*!
  UT_PSetNetworkWrapper::t_AllowRefresh
 */
void UT_PSetNetworkWrapper::t_AllowRefresh()
{
    TSatRefreshType type;
    TSatElementaryFiles file;
    m_refreshHandler->trigerAllowRefresh(type, file);
}

/*!
  UT_PSetNetworkWrapper::t_Refresh
 */
void UT_PSetNetworkWrapper::t_Refresh()
{
    TSatElementaryFiles file;
    expect("CPsetCustomerServiceProfile::IsNetworkSelectionSupported").returns(KErrNone);
    m_refreshHandler->trigerRefresh(EFileChangeNotification, KCsp1Ef);
    QVERIFY(verify());
    
    expect("CPsetCustomerServiceProfile::IsNetworkSelectionSupported").returns(KErrGeneral);
    m_refreshHandler->trigerRefresh(EFileChangeNotification, KCsp1Ef);
    QVERIFY(verify());
 
    expect("CPsetCustomerServiceProfile::IsNetworkSelectionSupported").times(0);
    m_refreshHandler->trigerRefresh(EFileChangeNotification, KCsp2Ef);
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_PSetNetworkWrapper)
