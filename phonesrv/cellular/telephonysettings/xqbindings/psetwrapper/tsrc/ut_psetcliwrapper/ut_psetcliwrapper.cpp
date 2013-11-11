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
#include <mpsetcliobserver.h>
#include <psetwrappertypes.h>
#include <PsetCli.h>
#include "ut_psetcliwrapper.h"
#include "testutilities.h"
#define private public
#include "psetcliwrapper.h"
#include "psetcliwrapper_p.h"

class CliObserver : public MPsetCliObserver
{
    void SetEngineContact( MPsetCli* aEngine )
    {
        Q_UNUSED(aEngine);
    }

    void HandleCliRequestingL( TBool aOngoing, 
        TBool aInterrupted )
    {
        Q_UNUSED(aOngoing);
        Q_UNUSED(aInterrupted);
    }

    void CliInformationL( TPsuiCli aType )
    {
        Q_UNUSED(aType);
    }

    void HandleCliStatusL( TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode )
    {
        Q_UNUSED(aBsc);
        Q_UNUSED(aMode);
    }
      
    void HandleCnapStatusL( TInt aStatus )
    {
        Q_UNUSED(aStatus);
    }

    void HandleCliErrorL( TInt aError )
    {
        Q_UNUSED(aError);
    }
};

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}


/*!
  UT_PSetCliWrapper::UT_PSetCliWrapper
 */
UT_PSetCliWrapper::UT_PSetCliWrapper()
    :
    m_numOfLeaveSimulations(0),
    m_psetContainerMock(NULL),
    m_wrapper(NULL),
    m_cliMock(NULL)
{
}

/*!
  UT_PSetCliWrapper::~UT_PSetCliWrapper
 */
UT_PSetCliWrapper::~UT_PSetCliWrapper()
{
    delete m_psetContainerMock;
    delete m_wrapper;
    m_cliMock = NULL;
}

/*!
  UT_PSetCliWrapper::init
 */
void UT_PSetCliWrapper::init()
{
    initialize();
    
    m_numOfLeaveSimulations = 0;
    m_psetContainerMock = new CPsetContainer();
    
    RMobilePhone dummyHandle;
    CliObserver dummyObserver;
    QT_TRAP_THROWING(m_cliMock = 
        CPsetCli::NewL(dummyHandle, dummyObserver));
    expect("CPsetContainer::CreateCliObjectL").returns(m_cliMock);
    // wrapper takes ownership of callDivertingMock during successfull
    // construction.
    QScopedPointer<CPsetCli> cliMockPtr(m_cliMock);
    m_wrapper = new PSetCliWrapper(*m_psetContainerMock, NULL);
    cliMockPtr.take();
}

/*!
  UT_PSetCliWrapper::cleanup
 */
void UT_PSetCliWrapper::cleanup()
{
    reset();
    
    delete m_psetContainerMock;
    m_psetContainerMock = NULL;
    delete m_wrapper;
    m_wrapper = NULL;
    m_cliMock = NULL;
}

/*!
  UT_PSetCliWrapper::t_construction
 */
void UT_PSetCliWrapper::t_construction()
{
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") != 0) {
        expect("CPsetContainer::CreateCliObjectL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        
        PSetCliWrapper *wrapper = NULL;
        EXPECT_EXCEPTION(
            wrapper = new PSetCliWrapper(*m_psetContainerMock, NULL);
        )
        delete wrapper;
        
        QVERIFY(verify());
    }
}

/*!
  UT_PSetCliWrapper::t_getColpMode
 */
void UT_PSetCliWrapper::t_getColpMode()
{
    expect("CPsetCli::GetColpModeL").
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getColpMode();
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCliWrapper::t_getClipMode
 */
void UT_PSetCliWrapper::t_getClipMode()
{
    expect("CPsetCli::GetClipModeL").
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getClipMode();
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCliWrapper::t_getClirMode
 */
void UT_PSetCliWrapper::t_getClirMode()
{
    expect("CPsetCli::GetClirModeL").
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getClirMode();
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCliWrapper::t_getColrMode
 */
void UT_PSetCliWrapper::t_getColrMode()
{
    expect("CPsetCli::GetColrModeL").
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getColrMode();
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCliWrapper::t_getCnap
 */
void UT_PSetCliWrapper::t_getCnap()
{
    expect("CPsetCli::GetCnapL").
        willOnce(invokeWithoutArguments(this, &SimulateLeaveAtMockMethodCallL));
    
    EXPECT_EXCEPTION(
        m_wrapper->getCnap();
    )
    
    QVERIFY(1 == m_numOfLeaveSimulations);
    QVERIFY(true == verify());
}

/*!
  UT_PSetCliWrapper::t_cancelAll
 */
void UT_PSetCliWrapper::t_cancelAll()
{
    expect("CPsetCli::CancelAll");
    
    m_wrapper->cancelAll();
    
    QVERIFY(true == verify());
}


/*!
  UT_PSetCliWrapper::t_handleCliRequesting
 */
void UT_PSetCliWrapper::t_handleCliRequesting()
{
    QSignalSpy spy(m_wrapper, SIGNAL(handleCliRequesting(bool, bool)));
    
    TBool isOngoing = ETrue;
    TBool isInterrupted = ETrue;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCliRequestingL(isOngoing, isInterrupted));
    
    const int expectednumOfSignalEmissions = 1;
    const int expectedNumOfSignalArguments = 2;
    QCOMPARE(spy.count(), expectednumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(expectedNumOfSignalArguments == arguments.count());
    QVERIFY(ETrue == arguments.at(0).toBool());
    QVERIFY(ETrue == arguments.at(1).toBool());
}

/*!
  UT_PSetCliWrapper::t_cliInformation
 */
Q_DECLARE_METATYPE(PsCallLineIdentity)
void UT_PSetCliWrapper::t_cliInformation()
{
    qRegisterMetaType<PsCallLineIdentity>("PsCallLineIdentity");
    QSignalSpy spy(m_wrapper, SIGNAL(cliInformation(PsCallLineIdentity)));
    
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->CliInformationL(EPsuiClirOn));
    
    const int expectednumOfSignalEmissions = 1;
    const int expectedNumOfSignalArguments = 1;
    QCOMPARE(spy.count(), expectednumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(expectedNumOfSignalArguments == arguments.count());
    QVERIFY(ClirOn == arguments.at(0).value<PsCallLineIdentity>());
}

/*!
  UT_PSetCliWrapper::t_handleCliStatus
 */
Q_DECLARE_METATYPE(unsigned char *)
void UT_PSetCliWrapper::t_handleCliStatus()
{
    qRegisterMetaType<unsigned char *>("unsigned char *");
    QSignalSpy spy(m_wrapper, 
        SIGNAL(handleCliStatus(unsigned char *, int, PsCallLineIdentity)));
    
    TUint8 bscList[KPSetNumberOfBsc] = { EAllTele };
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCliStatusL(bscList, EPsuiClirOn));
    
    const int expectednumOfSignalEmissions = 1;
    const int expectedNumOfSignalArguments = 3;
    QCOMPARE(spy.count(), expectednumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(expectedNumOfSignalArguments == arguments.count());
    QVERIFY(EAllTele == arguments.at(0).value<unsigned char *>()[0]);
    QVERIFY(KPSetNumberOfBsc == arguments.at(1).toInt());
    QVERIFY(ClirOn == arguments.at(2).value<PsCallLineIdentity>());
}

/*!
  UT_PSetCliWrapper::t_handleCnapStatus
 */
void UT_PSetCliWrapper::t_handleCnapStatus()
{
    QSignalSpy spy(m_wrapper, SIGNAL(handleCnapStatus(int)));
    
    const int status = 1;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleCnapStatusL(status));
    
    const int expectednumOfSignalEmissions = 1;
    const int expectedNumOfSignalArguments = 1;
    QCOMPARE(spy.count(), expectednumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(expectedNumOfSignalArguments == arguments.count());
    QVERIFY(status == arguments.at(0).toInt());
}

/*!
  UT_PSetCliWrapper::t_handleCliError
 */
void UT_PSetCliWrapper::t_handleCliError()
{
    QSignalSpy spy(m_wrapper, SIGNAL(handleCliError(int)));
    
    const int expectedErrorCode = KErrGeneral;
    QT_TRAP_THROWING(m_wrapper->m_privateImpl->HandleCliErrorL(expectedErrorCode));
    
    const int expectednumOfSignalEmissions = 1;
    const int expectedNumOfSignalArguments = 1;
    QCOMPARE(spy.count(), expectednumOfSignalEmissions);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(expectedNumOfSignalArguments == arguments.count());
    QVERIFY(expectedErrorCode == arguments.at(0).toInt());
}

/*!
  UT_PSetCliWrapper::t_setEngineContact
 */
void UT_PSetCliWrapper::t_setEngineContact()
{
    // dummy test as no real implementation for the method currently
    m_wrapper->m_privateImpl->SetEngineContact(NULL);
}

/*!
  UT_PSetCliWrapper::t_exceptionSafety
 */
void UT_PSetCliWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}

void UT_PSetCliWrapper::SimulateLeaveAtMockMethodCallL()
{
    m_numOfLeaveSimulations++;
    User::Leave( KErrGeneral );
}

QTEST_MAIN_S60(UT_PSetCliWrapper)
