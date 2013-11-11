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
#include <mpsetbarringobs.h>
#include <psetwrappertypes.h>
#include <nwdefs.h>
#include "ut_psetcallbarringwrapper.h"
#include "testutilities.h"
#define private public
#include <psetcallbarringwrapper.h>
#include "psetcallbarringwrapper_p.h"

const int KInvalidEnumeration = -1;
        
class BarringObserver : public MPsetBarringObserver
{
    void HandleBarringModeChangedL( 
        TBarringProgram aType, 
        TBarringStatus aStatus, 
        TBool aPlural )
    {
        Q_UNUSED(aType);
        Q_UNUSED(aStatus);
        Q_UNUSED(aPlural);
    }

    void HandleBarringModeStatusL( 
        TUint8 aBsc[KPSetNumberOfBsc], 
        TBarringStatus aStatus )
    {
        Q_UNUSED(aBsc);
        Q_UNUSED(aStatus);
    }

    void HandleBarringErrorL( 
        TInt aReason )
    {
        Q_UNUSED(aReason);
    }

    void HandleCBRequestingL( 
        TBool aTrue, 
        TBool aInterrupted )
    {
        Q_UNUSED(aTrue);
        Q_UNUSED(aInterrupted);
    }

    void SetEngineContact( 
        MPsetCallBarring* aBarringEngine )
    {
        Q_UNUSED(aBarringEngine);
    }

    void CbPasswordChangedL( 
        TBool aSuccess )
    {
        Q_UNUSED(aSuccess);
    }
};

void simulateLeaveL()
{
    User::Leave(KErrGeneral);
}

/*!
  UT_PSetCallBarringWrapper::UT_PSetCallBarringWrapper
 */
UT_PSetCallBarringWrapper::UT_PSetCallBarringWrapper()
    :
    m_psetContainerMock(NULL),
    m_wrapper(NULL)
{
}


/*!
  UT_PSetCallBarringWrapper::~UT_PSetCallBarringWrapper
 */
UT_PSetCallBarringWrapper::~UT_PSetCallBarringWrapper()
{
    delete m_psetContainerMock;
    delete m_wrapper;
}


/*!
  UT_PSetCallBarringWrapper::init
 */
void UT_PSetCallBarringWrapper::init()
{
    initialize();
    
    m_psetContainerMock = new CPsetContainer();
    
    BarringObserver dummyObserver;
    RMobilePhone dummyPhone;
    
    CPsetCallBarring *barringMock = NULL;
    TRAPD(result, barringMock = CPsetCallBarring::NewL(dummyObserver, dummyPhone));
    QScopedPointer<CPsetCallBarring> barringMockGuard(barringMock);
    EXPECT(CPsetContainer, CreateCBObjectL)
        .returns(barringMock);
    m_wrapper = new PSetCallBarringWrapper(*m_psetContainerMock, NULL);
    barringMockGuard.take();
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::cleanup
 */
void UT_PSetCallBarringWrapper::cleanup()
{
    reset();
    
    delete m_psetContainerMock;
    m_psetContainerMock = NULL;
    delete m_wrapper;
    m_wrapper = NULL;
}


/*!
  UT_PSetCallBarringWrapper::t_construction
 */
void UT_PSetCallBarringWrapper::t_construction()
{
    EXPECT(CPsetContainer, CreateCBObjectL).
        willOnce(invokeWithoutArguments(this, simulateLeaveAtMockMethodCall));
    PSetCallBarringWrapper *wrapper = NULL;
    EXPECT_EXCEPTION(
        wrapper = new PSetCallBarringWrapper(*m_psetContainerMock, NULL););
}


/*!
  UT_PSetCallBarringWrapper::t_barringStatus
 */
void UT_PSetCallBarringWrapper::t_barringStatus()
{
// test status query with different service groups
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllBarrings);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupData, EBarringTypeAllBarrings);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupFax, EBarringTypeAllBarrings);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupAllTeleservices, EBarringTypeAllBarrings);
    // invalid/unknown enumeration as service group identifier
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllBarrings);
    
    //TODO: remove SeviceGroupUnknown enumeration?
    //m_wrapper->barringStatus(ServiceGroupUnknown);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    m_wrapper->barringStatus(ServiceGroupData,
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    m_wrapper->barringStatus(ServiceGroupFax,
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    m_wrapper->barringStatus(ServiceGroupAllTeleservices,
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    // invalid/unknown enumeration as service group identifier
    m_wrapper->barringStatus(
        static_cast<PsService>(KInvalidEnumeration),
        PSetCallBarringWrapper::BarringTypeAllBarrings);

// test status query with different barring types    
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllOutgoing);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeOutgoingInternational);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, 
            EBarringTypeOutgoingInternationalExceptToHomeCountry);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllIncoming);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeIncomingWhenRoaming);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllServices);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllOutgoingServices);
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllIncomingServices);
    // invalid/unknown enumeration as barring type
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .with(EServiceGroupVoice, EBarringTypeAllBarrings);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternational);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::
            BarringTypeOutgoingInternationalExceptToHomeCountry);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncoming);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllServices);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoingServices);
    m_wrapper->barringStatus(ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncomingServices);
    // invalid/unknown enumeration as barring type
    m_wrapper->barringStatus(ServiceGroupVoice, 
        static_cast<PSetCallBarringWrapper::BarringType>(KInvalidEnumeration));
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::t_barringStatusExceptionOccurs
 */
void UT_PSetCallBarringWrapper::t_barringStatusExceptionOccurs()
{
    EXPECT(CPsetCallBarring, GetBarringStatusL)
        .willOnce(invokeWithoutArguments(this, simulateLeaveAtMockMethodCall));
    
    EXPECT_EXCEPTION(
        m_wrapper->barringStatus(
            ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
    )
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::t_enableBarring
 */
void UT_PSetCallBarringWrapper::t_enableBarring()
{
    _LIT(KPassword, "1234");
    TCallBarringSetting setting;
    setting.iType = EBarringTypeAllBarrings;
    setting.iSetting = EActivateBarring;
    setting.iServiceGroup = EServiceGroupVoice;
    setting.iPassword.Copy(KPassword());
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        QString("1234"));

    setting.iType = EBarringTypeAllOutgoing;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing,
        QString("1234"));
    
    setting.iType = EBarringTypeOutgoingInternational;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternational,
        QString("1234"));

    setting.iType = EBarringTypeOutgoingInternationalExceptToHomeCountry;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry,
        QString("1234"));
    
    setting.iType = EBarringTypeAllIncoming;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncoming,
        QString("1234"));
    
    setting.iType = EBarringTypeIncomingWhenRoaming;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming,
        QString("1234"));
    
    setting.iType = EBarringTypeAllServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllServices,
        QString("1234"));

    setting.iType = EBarringTypeAllOutgoingServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoingServices,
        QString("1234"));
    
    setting.iType = EBarringTypeAllIncomingServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncomingServices,
        QString("1234"));

// error handling    
    EXPECT(CPsetCallBarring, SetBarringL)
        .willOnce(invokeWithoutArguments(simulateLeaveL));
    EXPECT_EXCEPTION(
        m_wrapper->enableBarring(
            EAllTeleAndBearer,
            ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllIncomingServices,
            QString("1234"));
    )
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::t_disableBarring
 */
void UT_PSetCallBarringWrapper::t_disableBarring()
{
    _LIT(KPassword, "1234");
    TCallBarringSetting setting;
    setting.iType = EBarringTypeAllBarrings;
    setting.iSetting = ECancelBarring;
    setting.iServiceGroup = EServiceGroupVoice;
    setting.iPassword.Copy(KPassword());
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        QString("1234"));

    setting.iType = EBarringTypeAllOutgoing;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoing,
        QString("1234"));
    
    setting.iType = EBarringTypeOutgoingInternational;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternational,
        QString("1234"));

    setting.iType = EBarringTypeOutgoingInternationalExceptToHomeCountry;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeOutgoingInternationalExceptToHomeCountry,
        QString("1234"));
    
    setting.iType = EBarringTypeAllIncoming;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncoming,
        QString("1234"));
    
    setting.iType = EBarringTypeIncomingWhenRoaming;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeIncomingWhenRoaming,
        QString("1234"));
    
    setting.iType = EBarringTypeAllServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllServices,
        QString("1234"));

    setting.iType = EBarringTypeAllOutgoingServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllOutgoingServices,
        QString("1234"));
    
    setting.iType = EBarringTypeAllIncomingServices;
    EXPECT(CPsetCallBarring, SetBarringL)
        .with(setting, EAllTeleAndBearer);
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllIncomingServices,
        QString("1234"));
    
// error handling    
    EXPECT(CPsetCallBarring, SetBarringL)
        .willOnce(invokeWithoutArguments(simulateLeaveL));
    EXPECT_EXCEPTION(
        m_wrapper->disableBarring(
            EAllTeleAndBearer,
            ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllIncomingServices,
            QString("1234"));
    )
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::t_changeBarringPassword
 */
void UT_PSetCallBarringWrapper::t_changeBarringPassword()
{
    QString oldPassword = "1234";
    QString newPassword = "4321";
    QString verifiedPassword = "4321";
    RMobilePhone::TMobilePhonePasswordChangeV2 passwordChange;
    passwordChange.iOldPassword.Copy(oldPassword.utf16());
    passwordChange.iNewPassword.Copy(newPassword.utf16());
    passwordChange.iVerifiedPassword.Copy(verifiedPassword.utf16());
    EXPECT(CPsetCallBarring, ChangePasswordL)
        .with(passwordChange);
    
    m_wrapper->changeBarringPassword(
        oldPassword, newPassword, verifiedPassword);
    
// error handling    
    EXPECT(CPsetCallBarring, ChangePasswordL)
        .willOnce(invokeWithoutArguments(simulateLeaveL));
    EXPECT_EXCEPTION(
        m_wrapper->changeBarringPassword(
            oldPassword, newPassword, verifiedPassword);
    )
    
    QVERIFY(verify());
}


/*!
  UT_PSetCallBarringWrapper::t_HandleBarringModeChangedL
 */
void UT_PSetCallBarringWrapper::t_HandleBarringModeChangedL()
{
    TBool plural = EFalse;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllBarrings, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllOutgoing, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeOutgoingInternational, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeOutgoingInternationalExceptToHomeCountry, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllIncoming, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeIncomingWhenRoaming, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllServices, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllOutgoingServices, EBarringStatusActive, plural);
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllIncomingServices, EBarringStatusActive, plural);
    )
}


/*!
  UT_PSetCallBarringWrapper::t_HandleBarringErrorL
 */
void UT_PSetCallBarringWrapper::t_HandleBarringErrorL()
{
    // TODO: dummy test as functionality not yet implemented
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleBarringErrorL(KErrGeneral);
    )
}


/*!
  UT_PSetCallBarringWrapper::t_HandleCBRequestingL
 */
void UT_PSetCallBarringWrapper::t_HandleCBRequestingL()
{
    // TODO: dummy test as functionality not yet implemented
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleCBRequestingL(EFalse, EFalse);
    )
}


/*!
  UT_PSetCallBarringWrapper::t_SetEngineContact
 */
void UT_PSetCallBarringWrapper::t_SetEngineContact()
{
    // TODO: dummy test as functionality not yet implemented
    m_wrapper->m_privateImpl->SetEngineContact(NULL);
}


/*!
  UT_PSetCallBarringWrapper::t_CbPasswordChangedL
 */
void UT_PSetCallBarringWrapper::t_CbPasswordChangedL()
{
    TBool success = EFalse;
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->CbPasswordChangedL(ETrue);
        m_wrapper->m_privateImpl->CbPasswordChangedL(EFalse);
    )
}


/*!
  UT_PSetCallBarringWrapper::t_HandleBarringModeStatusL
 */
Q_DECLARE_METATYPE(PSetCallBarringWrapper::BarringStatus)
Q_DECLARE_METATYPE(QList<unsigned char>)
void UT_PSetCallBarringWrapper::t_HandleBarringModeStatusL()
{
    qRegisterMetaType<PSetCallBarringWrapper::BarringStatus>
        ("PSetCallBarringWrapper::BarringStatus");
    qRegisterMetaType<QList<unsigned char> >("QList<unsigned char>");
    QSignalSpy spy(m_wrapper, 
        SIGNAL(barringStatusRequestCompleted(
            int,
            const QList<unsigned char> &,
            PSetCallBarringWrapper::BarringStatus)));
    
    TUint8 basicServiceGroupIds[KPSetNumberOfBsc];
    basicServiceGroupIds[0] = EServiceGroupVoice;
    basicServiceGroupIds[1] = KPSetUnusedValue;
// test with different barring statuses    
    QT_TRAP_THROWING(
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusActive);
        m_wrapper->m_privateImpl->RequestComplete();
        
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusInactive);
        m_wrapper->m_privateImpl->RequestComplete();
        
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusNotProvisioned);
        m_wrapper->m_privateImpl->RequestComplete();
        
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusUnavailable);
        m_wrapper->m_privateImpl->RequestComplete();
        
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusUnknown);
        m_wrapper->m_privateImpl->RequestComplete();
        
        // invalid/unknown enumeration as barring status
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds,
            static_cast<TBarringStatus>(KInvalidEnumeration));
        m_wrapper->m_privateImpl->RequestComplete();
    )
    
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") == 0) {
        // Signal emissions from RequestComplete will fail with exception safety 
        // tests so it's not possible to verify signal data.
        return;
    }
    
    QCOMPARE(spy.count(), 6);
    QList<unsigned char> resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(0).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusActive == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(0).at(2)));
    
    resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(1).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusInactive == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(1).at(2)));

    resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(2).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusNotProvisioned == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(2).at(2)));
    
    resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(3).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusUnavailable == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(3).at(2)));

    resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(4).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusUnknown == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(4).at(2)));
    
    // invalid/unknown enumeration as barring status
    resultBasicServiceGroupIds = 
        qvariant_cast<QList<unsigned char> >(spy.at(5).at(1));
    QVERIFY(ServiceGroupVoice == resultBasicServiceGroupIds[0]);
    QVERIFY(PSetCallBarringWrapper::BarringStatusUnknown == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(5).at(2)));
}


/*!
  UT_PSetCallBarringWrapper::t_HandleBarringModeStatusNoBasicServiceGroupIdsL
 */
void UT_PSetCallBarringWrapper::t_HandleBarringModeStatusNoBasicServiceGroupIdsL()
{
    TUint8 basicServiceGroupIds[KPSetNumberOfBsc];
    basicServiceGroupIds[0] = KPSetUnusedValue;
    QSignalSpy spy(m_wrapper, 
        SIGNAL(barringStatusRequestCompleted(
            int,
            const QList<unsigned char> &,
            PSetCallBarringWrapper::BarringStatus)));
    
    QT_TRAP_THROWING(
        m_wrapper->barringStatus(ServiceGroupVoice, 
            PSetCallBarringWrapper::BarringTypeAllBarrings);
        m_wrapper->m_privateImpl->HandleBarringModeStatusL(
            basicServiceGroupIds, EBarringStatusActive);
        m_wrapper->m_privateImpl->RequestComplete();
    )
    
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") == 0) {
        // Signal emissions from RequestComplete will fail with exception safety 
        // tests so it's not possible to verify signal data.
        return;
    }
    
    QCOMPARE(spy.count(), 1);
    QVERIFY(PSetCallBarringWrapper::BarringStatusActive == 
        qvariant_cast<PSetCallBarringWrapper::BarringStatus>(spy.at(0).at(2)));
}


/*!
  UT_PSetCallBarringWrapper::t_enableBarringRequestComplete
 */
Q_DECLARE_METATYPE(PSetCallBarringWrapper::BarringType)
void UT_PSetCallBarringWrapper::t_enableBarringRequestComplete()
{
    qRegisterMetaType<PSetCallBarringWrapper::BarringType>(
        "PSetCallBarringWrapper::BarringType");
    QSignalSpy spy(
        m_wrapper, 
        SIGNAL(enableBarringRequestCompleted(int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus, 
            bool)));
    
    TCallBarringSetting setting;
    m_wrapper->enableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        QString("1234"));
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllBarrings, EBarringStatusActive, EFalse);
    );
    m_wrapper->m_privateImpl->RequestComplete();
    
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") == 0) {
        // Signal emissions from RequestComplete will fail with exception safety 
        // tests so it's not possible to verify signal data.
        return;
    }
    
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 0);
    QCOMPARE(spy.at(0).at(1).value<PSetCallBarringWrapper::BarringType>(), 
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    QCOMPARE(spy.at(0).at(2).value<PSetCallBarringWrapper::BarringStatus>(), 
        PSetCallBarringWrapper::BarringStatusActive);
    QCOMPARE(spy.at(0).at(3).toBool(), false);
}


/*!
  UT_PSetCallBarringWrapper::t_disableBarringRequestComplete
 */
void UT_PSetCallBarringWrapper::t_disableBarringRequestComplete()
{
    QSignalSpy spy(
        m_wrapper, 
        SIGNAL(disableBarringRequestCompleted(int,
            PSetCallBarringWrapper::BarringType,
            PSetCallBarringWrapper::BarringStatus, 
            bool)));
    
    TCallBarringSetting setting;
    m_wrapper->disableBarring(
        EAllTeleAndBearer,
        ServiceGroupVoice, 
        PSetCallBarringWrapper::BarringTypeAllBarrings,
        QString("1234"));
    QT_TRAP_THROWING(
        m_wrapper->m_privateImpl->HandleBarringModeChangedL(
            EBarringTypeAllBarrings, EBarringStatusNotProvisioned, EFalse);
    );
    m_wrapper->m_privateImpl->RequestComplete();
    
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") == 0) {
        // Signal emissions from RequestComplete will fail with exception safety 
        // tests so it's not possible to verify signal data.
        return;
    }
    
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 0);
    QCOMPARE(spy.at(0).at(1).value<PSetCallBarringWrapper::BarringType>(), 
        PSetCallBarringWrapper::BarringTypeAllBarrings);
    QCOMPARE(spy.at(0).at(2).value<PSetCallBarringWrapper::BarringStatus>(), 
        PSetCallBarringWrapper::BarringStatusNotProvisioned);
    QCOMPARE(spy.at(0).at(3).toBool(), false);
}


/*!
  UT_PSetCallBarringWrapper::t_changeBarringPasswordRequestComplete
 */
void UT_PSetCallBarringWrapper::t_changeBarringPasswordRequestComplete()
{
    QSignalSpy spy(
        m_wrapper, 
        SIGNAL(barringPasswordChangeRequestCompleted(int)));
    
    QString oldPassword = "1234";
    QString newPassword = "4321";
    QString verifiedPassword = "4321";
    m_wrapper->changeBarringPassword(
        oldPassword, newPassword, verifiedPassword);
    
    m_wrapper->m_privateImpl->RequestComplete();
    
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") == 0) {
        // Signal emissions from RequestComplete will fail with exception safety 
        // tests so it's not possible to verify signal data.
        return;
    }
    
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 0);
}


/*!
  UT_PSetCallBarringWrapper::t_RequestStatusChanged
 */
void UT_PSetCallBarringWrapper::t_RequestStatusChanged()
{
    m_wrapper->m_privateImpl->RequestStatusChanged(0);
}


/*!
  UT_PSetCallBarringWrapper::t_exceptionSafety
 */
void UT_PSetCallBarringWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}


/*!
  UT_PSetCallBarringWrapper::simulateLeaveAtMockMethodCall
 */
void UT_PSetCallBarringWrapper::simulateLeaveAtMockMethodCall()
{
    User::Leave(KErrGeneral);
}

QTEST_MAIN_S60(UT_PSetCallBarringWrapper)
