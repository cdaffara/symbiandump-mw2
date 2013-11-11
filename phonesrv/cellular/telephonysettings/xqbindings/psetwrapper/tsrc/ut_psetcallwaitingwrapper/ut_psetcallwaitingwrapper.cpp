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

#include <etelmm.h>
#include <PsetContainer.h>
#include <mpsetcallwaitingobs.h>
#include "ut_psetcallwaitingwrapper.h"
#include "testutilities.h"
#define private public
#include "psetcallwaitingwrapper.h"
#include "psetcallwaitingwrapper_p.h"

class WaitingObserver : public MPsetCallWaitingObserver
{
    void HandleCallWaitingGetStatusL( 
        const MPsetCallWaiting::TGetCallWaitingStatus aStatus, 
        TUint8 aBsc[KPSetNumberOfBsc])
    {
        Q_UNUSED(aStatus);
        Q_UNUSED(aBsc);
    }

    void HandleCallWaitingChangedL( 
        const MPsetCallWaiting::TSetCallWaiting aSetting, 
        const TInt aResult)
    {
        Q_UNUSED(aSetting);
        Q_UNUSED(aResult);
    }

    void HandleCWRequestingL(
        TBool aOngoing, 
        TBool aInterrupted)
    {
        Q_UNUSED(aOngoing);
        Q_UNUSED(aInterrupted);
    }

    void HandleCWErrorL(
        TInt aReason)
    {
        Q_UNUSED(aReason);
    }

    void SetEngineContact(
        MPsetCallWaiting& aEngineContact)
    {
        Q_UNUSED(aEngineContact);
    }
};

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}

/*!
  UT_PSetCallWaitingWrapper::UT_PSetCallWaitingWrapper
 */
UT_PSetCallWaitingWrapper::UT_PSetCallWaitingWrapper()
    :
    m_numOfLeaveSimulations(0),
    m_psetContainerMock(NULL),
    m_psetCallWaitingMock(NULL),
    m_wrapper(NULL)
{
}

/*!
  UT_PSetCallWaitingWrapper::~UT_PSetCallWaitingWrapper
 */
UT_PSetCallWaitingWrapper::~UT_PSetCallWaitingWrapper()
{
    delete m_psetContainerMock;
    m_psetCallWaitingMock = NULL;
    delete m_wrapper;
}

/*!
  UT_PSetCallWaitingWrapper::init
 */
void UT_PSetCallWaitingWrapper::init()
{
    initialize();
    m_numOfLeaveSimulations = 0;
    
    m_psetContainerMock = new CPsetContainer();
    
    RMobilePhone dummyHandle;
    WaitingObserver dummyObserver;
    QT_TRAP_THROWING(m_psetCallWaitingMock = 
        CPsetCallWaiting::NewL(dummyHandle, dummyObserver));
    expect("CPsetContainer::CreateCWObjectL").returns(m_psetCallWaitingMock);
    
    // wrapper takes ownership of callDivertingMock during successful 
    // construction
    QScopedPointer<CPsetCallWaiting> callWaitingMockPtr(m_psetCallWaitingMock);
    m_wrapper = new PSetCallWaitingWrapper(*m_psetContainerMock, NULL);
    callWaitingMockPtr.take();
}

/*!
  UT_PSetCallWaitingWrapper::cleanup
 */
void UT_PSetCallWaitingWrapper::cleanup()
{
    reset();
    
    delete m_psetContainerMock;
    m_psetContainerMock = NULL;
    m_psetCallWaitingMock = NULL;
    delete m_wrapper;
    m_wrapper = NULL;
}


/*!
  UT_PSetCallWaitingWrapper::t_construction
 */
void UT_PSetCallWaitingWrapper::t_construction()
{
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") != 0) {
        expect("CPsetContainer::CreateCWObjectL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        
        PSetCallWaitingWrapper *wrapper = NULL;
        EXPECT_EXCEPTION(
            wrapper = new PSetCallWaitingWrapper(*m_psetContainerMock, NULL);
        )
        delete wrapper;
        
        QVERIFY(verify());
    }
}


/*!
  UT_PSetCallWaitingWrapper::t_setCallWaiting
 */
void UT_PSetCallWaitingWrapper::t_setCallWaiting()
{
    TBasicServiceGroups basicServiceGroup = EAllTele;
    expect("CPsetCallWaiting::SetCallWaitingL").
        with(MPsetCallWaiting::EActivateCallWaiting, basicServiceGroup);
    expect("CPsetCallWaiting::SetCallWaitingL").
        with(MPsetCallWaiting::EDeactivateCallWaiting, basicServiceGroup);
    
    int invalidEnumeration = -1; 
    expect("CPsetCallWaiting::SetCallWaitingL").
        with(static_cast<MPsetCallWaiting::TSetCallWaiting>(invalidEnumeration), basicServiceGroup).
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    m_wrapper->setCallWaiting(
        PSetCallWaitingWrapper::ActivateCallWaiting,
        basicServiceGroup);
    m_wrapper->setCallWaiting(
        PSetCallWaitingWrapper::DeactivateCallWaiting,
        basicServiceGroup);
    EXPECT_EXCEPTION(
        m_wrapper->setCallWaiting(
            static_cast<PSetCallWaitingWrapper::PsCallWaitingCommand>(invalidEnumeration),
            basicServiceGroup);
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallWaitingWrapper::t_getCallWaitingStatus
 */
void UT_PSetCallWaitingWrapper::t_getCallWaitingStatus()
{
    expect("CPsetCallWaiting::GetCallWaitingStatusL").
        willOnce(invoke(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getCallWaitingStatus();
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallWaitingWrapper::t_cancelProcess
 */
void UT_PSetCallWaitingWrapper::t_cancelProcess()
{
    expect("CPsetCallWaiting::CancelProcess");
    
    m_wrapper->cancelProcess();
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallWaitingWrapper::t_handleCallWaitingGetStatus
 */
Q_DECLARE_METATYPE(PSetCallWaitingWrapper::PsCallWaitingStatus)
Q_DECLARE_METATYPE(QList<unsigned char>)
void UT_PSetCallWaitingWrapper::t_handleCallWaitingGetStatus()
{
    qRegisterMetaType<PSetCallWaitingWrapper::PsCallWaitingStatus>
        ("PSetCallWaitingWrapper::PsCallWaitingStatus");
    qRegisterMetaType<QList<unsigned char> >("QList<unsigned char>");
    QSignalSpy spy(m_wrapper, SIGNAL(handleCallWaitingGetStatus(
        PSetCallWaitingWrapper::PsCallWaitingStatus, const QList<unsigned char> &)));
    
    MPsetCallWaiting::TGetCallWaitingStatus status =
        MPsetCallWaiting::EStatusActive; 
    TUint8 bscList[KPSetNumberOfBsc];
    bscList[0] = EAllTele;
    bscList[1] = KPSetUnusedValue;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingGetStatusL(status, bscList));
    status = MPsetCallWaiting::EStatusDisabled;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingGetStatusL(status, bscList));
    status = MPsetCallWaiting::EStatusNotAvailable;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingGetStatusL(status, bscList));
    status = MPsetCallWaiting::EStatusNotProvisioned;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingGetStatusL(status, bscList));
    status = MPsetCallWaiting::EStatusUnknown;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingGetStatusL(status, bscList));
    
    const int expectedNumOfSignalEmissions = 5;
    QCOMPARE(spy.count(), expectedNumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(PSetCallWaitingWrapper::StatusActive == 
        arguments.at(0).value<PSetCallWaitingWrapper::PsCallWaitingStatus>());
    QVERIFY(EAllTele == arguments.at(1).value<QList<unsigned char> >()[0]);
    QVERIFY(PSetCallWaitingWrapper::StatusDisabled == 
        spy.at(1).at(0).value<PSetCallWaitingWrapper::PsCallWaitingStatus>());
    QVERIFY(EAllTele == spy.at(1).at(1).value<QList<unsigned char> >()[0]);
    QVERIFY(PSetCallWaitingWrapper::StatusNotAvailable == 
        spy.at(2).at(0).value<PSetCallWaitingWrapper::PsCallWaitingStatus>());
    QVERIFY(EAllTele == spy.at(2).at(1).value<QList<unsigned char> >()[0]);
    QVERIFY(PSetCallWaitingWrapper::StatusNotProvisioned == 
        spy.at(3).at(0).value<PSetCallWaitingWrapper::PsCallWaitingStatus>());
    QVERIFY(EAllTele == spy.at(3).at(1).value<QList<unsigned char> >()[0]);
    QVERIFY(PSetCallWaitingWrapper::StatusUnknown == 
        spy.at(4).at(0).value<PSetCallWaitingWrapper::PsCallWaitingStatus>());
    QVERIFY(EAllTele == spy.at(4).at(1).value<QList<unsigned char> >()[0]);
}

/*!
  UT_PSetCallWaitingWrapper::t_handleCallWaitingChanged
 */
Q_DECLARE_METATYPE(PSetCallWaitingWrapper::PsCallWaitingCommand)
void UT_PSetCallWaitingWrapper::t_handleCallWaitingChanged()
{
    qRegisterMetaType<PSetCallWaitingWrapper::PsCallWaitingCommand>
        ("PSetCallWaitingWrapper::PsCallWaitingCommand");
    QSignalSpy spy(m_wrapper, SIGNAL(handleCallWaitingChanged(
        PSetCallWaitingWrapper::PsCallWaitingCommand, int)));
    
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingChangedL(
            MPsetCallWaiting::EActivateCallWaiting, KErrNone));
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCallWaitingChangedL(
            MPsetCallWaiting::EDeactivateCallWaiting, KErrNone));
    
    const int expectedNumOfSignalEmissions = 2;
    QCOMPARE(spy.count(), expectedNumOfSignalEmissions);
    QVERIFY(PSetCallWaitingWrapper::ActivateCallWaiting == 
        spy.at(0).at(0).value<PSetCallWaitingWrapper::PsCallWaitingCommand>());
    QVERIFY(KErrNone == spy.at(0).at(1).toInt());
    QVERIFY(PSetCallWaitingWrapper::DeactivateCallWaiting == 
        spy.at(1).at(0).value<PSetCallWaitingWrapper::PsCallWaitingCommand>());
    QVERIFY(KErrNone == spy.at(1).at(1).toInt());
}

/*!
  UT_PSetCallWaitingWrapper::t_handleCallWaitingRequesting
 */
void UT_PSetCallWaitingWrapper::t_handleCallWaitingRequesting()
{
    QSignalSpy spy(m_wrapper, 
        SIGNAL(handleCallWaitingRequesting(bool, bool)));
    
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCWRequestingL(ETrue, ETrue));
    
    QCOMPARE(spy.count(), 1);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(true == arguments.at(0).toBool());
    QVERIFY(true == arguments.at(1).toBool());
}

/*!
  UT_PSetCallWaitingWrapper::t_handleCallWaitingError
 */
void UT_PSetCallWaitingWrapper::t_handleCallWaitingError()
{
    QSignalSpy spy(m_wrapper, SIGNAL(handleCallWaitingError(int)));
    
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleCWErrorL(KErrGeneral));
    
    QCOMPARE(spy.count(), 1);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(KErrGeneral == arguments.at(0).toInt());
}

/*!
  UT_PSetCallWaitingWrapper::t_setEngineContact
 */
void UT_PSetCallWaitingWrapper::t_setEngineContact()
{
    // dummy test as no real implementation for the method currently
    m_wrapper->m_privateImpl->SetEngineContact(
        reinterpret_cast<MPsetCallWaiting&>(*this));
}

/*!
  UT_PSetCallWaitingWrapper::t_exceptionSafety
 */
void UT_PSetCallWaitingWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}

void UT_PSetCallWaitingWrapper::SimulateLeaveAtMockMethodCallL()
{
    m_numOfLeaveSimulations++;
    User::Leave( KErrGeneral );
}

QTEST_MAIN_S60(UT_PSetCallWaitingWrapper)
