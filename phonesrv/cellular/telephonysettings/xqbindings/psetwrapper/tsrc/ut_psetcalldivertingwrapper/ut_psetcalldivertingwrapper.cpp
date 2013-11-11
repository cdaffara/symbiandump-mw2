/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <PsetCallDiverting.h>
#include <mpsetdivertobs.h>
#include "ut_psetcalldivertingwrapper.h"
#include "mock_cphcltemergencycall.h"
#include "testutilities.h"
#define private public
#include "psetcalldivertingwrapper.h"
#include "psetcalldivertingwrapper_p.h"

class DivertObserver : public MPsetDivertObserver
{
    void HandleDivertingChangedL( 
        const TCallDivertSetting& aSetting, TBool aPlural) 
    {
        Q_UNUSED(aSetting);
        Q_UNUSED(aPlural);
    }
    
    void HandleDivertingStatusL(CMobilePhoneCFList& aSetting, TBool aPlural) 
    {
        Q_UNUSED(aSetting);
        Q_UNUSED(aPlural);
    }
    
    void HandleDivertingErrorL(TInt aReason) 
    {
        Q_UNUSED(aReason);
    }
    
    void HandleCFRequestingL(TBool aOngoing, 
        TBool aInterrupted) 
    {
        Q_UNUSED(aOngoing);
        Q_UNUSED(aInterrupted);
    }
    
    void SetEngineContact(MPsetCallDiverting* aDivertEngine) 
    {
        Q_UNUSED(aDivertEngine);
    }
};

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}

/*!
  UT_PSetCallDivertingWrapper::UT_PSetWrapperCallDiverting
 */
UT_PSetCallDivertingWrapper::UT_PSetCallDivertingWrapper() 
    : mWrapper(NULL), mSetContainerMock(NULL) 
{
}

/*!
  UT_PSetCallDivertingWrapper::~UT_PSetWrapperCallDiverting
 */
UT_PSetCallDivertingWrapper::~UT_PSetCallDivertingWrapper()
{
    delete mWrapper;
    delete mSetContainerMock;
    mPsetCallDivertingMock = NULL;
}

/*!
  UT_PSetCallDivertingWrapper::init
 */
void UT_PSetCallDivertingWrapper::init()
{
    initialize();
    CPhCltEmergencyCallMock::iSimulateEmergencyNumberMatch = false;
    
    mSetContainerMock = new CPsetContainer();
    
    RMobilePhone dummyHandle;
    DivertObserver dummyObsever;
    QT_TRAP_THROWING(mPsetCallDivertingMock 
        = CPsetCallDiverting::NewL(dummyObsever, dummyHandle));
    expect("CPsetContainer::CreateCFObjectL").returns(mPsetCallDivertingMock);
    
    // Wrapper takes ownership of callDivertingMock during construction. 
    // Mock must be deleted here if exception happens during construction.
    QScopedPointer<CPsetCallDiverting> divertingMockPtr(mPsetCallDivertingMock);
    mWrapper = new PSetCallDivertingWrapper(*mSetContainerMock, NULL);
    divertingMockPtr.take();
}

/*!
  UT_PSetCallDivertingWrapper::cleanup
 */
void UT_PSetCallDivertingWrapper::cleanup()
{
    reset();
    
    delete mWrapper;
    mWrapper = NULL;
    delete mSetContainerMock;
    mSetContainerMock = NULL;
    mPsetCallDivertingMock = NULL;
}

/*!
  UT_PSetCallDivertingWrapper::t_construction
 */
void UT_PSetCallDivertingWrapper::t_construction()
{
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") != 0) {
        
        expect("CPsetContainer::CreateCFObjectL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        PSetCallDivertingWrapper *wrapper = NULL;
        EXPECT_EXCEPTION(
            wrapper = new PSetCallDivertingWrapper(*mSetContainerMock, NULL);
            delete wrapper;
            wrapper = NULL;
        )
        QVERIFY(verify());
        
        expect("CPhCltEmergencyCallMock::NewL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        EXPECT_EXCEPTION(
            wrapper = new PSetCallDivertingWrapper(*mSetContainerMock, NULL);
            delete wrapper;
            wrapper = NULL;
        )        
        QVERIFY(verify());
    }
}

/*!
  UT_PSetCallDivertingWrapper::t_setCallDivertingWithValidParameters
 */
void UT_PSetCallDivertingWrapper::t_setCallDivertingWithValidParameters()
{
    PSCallDivertingCommand divertingCmd;
    divertingCmd.iCondition = DivertConditionUnconditional;
    divertingCmd.iSetting = RegisterDiverting;
    divertingCmd.iStatus = DivertingStatusUnknown;
    divertingCmd.iServiceGroup = ServiceGroupVoice;
    divertingCmd.iNumber = "1234567890";
    divertingCmd.iNoReplyTimer = 0;
    
    TCallDivertSetting expectedParam;
    expectedParam.iCondition = EDivertConditionUnconditional;
    expectedParam.iSetting = ERegisterDiverting;
    expectedParam.iStatus = EDivertingStatusUnknown;
    expectedParam.iServiceGroup = EServiceGroupVoice;
    expectedParam.iNumber 
        = TPtrC16(reinterpret_cast<const TUint16*>(divertingCmd.iNumber.utf16()));
    expectedParam.iNoReplyTimer 
        = static_cast<TInt>(divertingCmd.iNoReplyTimer);
    TBool isVmbx = EFalse;
    expect("CPsetCallDiverting::SetDivertingL").
        with(expectedParam, ETelephony, isVmbx);
    
    mWrapper->setCallDiverting(divertingCmd, ETelephony);
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_setCallDivertingWithInvalidNumber
 */
void UT_PSetCallDivertingWrapper::t_setCallDivertingWithInvalidNumber()
{
    PSCallDivertingCommand divertingCmd;
    divertingCmd.iCondition = DivertConditionUnconditional;
    divertingCmd.iSetting = RegisterDiverting;
    divertingCmd.iStatus = DivertingStatusUnknown;
    divertingCmd.iServiceGroup = ServiceGroupVoice;
    divertingCmd.iNoReplyTimer = 0;
    
    expect("CPsetCallDiverting::SetDivertingL").times(0);
    
    // DTMF digits "*#pw" are not allowed characters
    divertingCmd.iNumber = "*";
    int result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSUnknownAlphabet);
    divertingCmd.iNumber = "#";
    result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSUnknownAlphabet);
    divertingCmd.iNumber = "p";
    result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSUnknownAlphabet);
    divertingCmd.iNumber = "w";
    result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSUnknownAlphabet);
    
    // plus allowed only in the beginning of the number
    divertingCmd.iNumber = "++";
    result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSUnknownAlphabet);
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_setCallDivertingWithEmergencyNumber
 */
void UT_PSetCallDivertingWrapper::t_setCallDivertingWithEmergencyNumber()
{
    PSCallDivertingCommand divertingCmd;
    divertingCmd.iCondition = DivertConditionUnconditional;
    divertingCmd.iSetting = RegisterDiverting;
    divertingCmd.iStatus = DivertingStatusUnknown;
    divertingCmd.iServiceGroup = ServiceGroupVoice;
    divertingCmd.iNumber = "112";
    divertingCmd.iNoReplyTimer = 0;
    
    expect("CPsetCallDiverting::SetDivertingL").times(0);
    CPhCltEmergencyCallMock::iSimulateEmergencyNumberMatch = ETrue;
    expect("CPhCltEmergencyCallMock::IsEmergencyPhoneNumber").returns(KErrNone);
    
    int result = mWrapper->setCallDiverting(divertingCmd, ETelephony);
    QVERIFY(result == KErrGsmSSIllegalOperation);
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_setCallDivertingException
 */
void UT_PSetCallDivertingWrapper::t_setCallDivertingException()
{
    PSCallDivertingCommand divertingCmd;
    divertingCmd.iCondition = DivertConditionUnconditional;
    divertingCmd.iSetting = RegisterDiverting;
    divertingCmd.iStatus = DivertingStatusUnknown;
    divertingCmd.iServiceGroup = ServiceGroupVoice;
    divertingCmd.iNumber = "1234567890";
    divertingCmd.iNoReplyTimer = 0;
    
    expect("CPsetCallDiverting::SetDivertingL").
        willOnce(invokeWithoutArguments(this, &simulateLeaveOnMockMethodCallL));
    
    EXPECT_EXCEPTION(
        mWrapper->setCallDiverting(divertingCmd, ETelephony);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_getCallDivertingStatus
 */
void UT_PSetCallDivertingWrapper::t_getCallDivertingStatus()
{
    expect("CPsetCallDiverting::GetDivertingStatusL").
        with(EServiceGroupVoice, EDivertConditionUnconditional, ETelephony);
    
    mWrapper->getCallDivertingStatus(
        ServiceGroupVoice, DivertConditionUnconditional, ETelephony);
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_getCallDivertingStatusException
 */
void UT_PSetCallDivertingWrapper::t_getCallDivertingStatusException()
{
    expect("CPsetCallDiverting::GetDivertingStatusL").
        with(EServiceGroupVoice, EDivertConditionUnconditional, ETelephony).
        willOnce(invokeWithoutArguments(this, &simulateLeaveOnMockMethodCallL));
    
    EXPECT_EXCEPTION(
        mWrapper->getCallDivertingStatus(
            ServiceGroupVoice, DivertConditionUnconditional, ETelephony);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_cancelProcess
 */
void UT_PSetCallDivertingWrapper::t_cancelProcess()
{
    expect("CPsetCallDiverting::CancelCurrentRequest");
    
    mWrapper->cancelProcess();
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_getDefaultNumbers
 */
void UT_PSetCallDivertingWrapper::t_getDefaultNumbers()
{
    expect("CPsetCallDiverting::GetDefaultNumbersL").times(1);
    
    QStringList defNumbers;
    mWrapper->getDefaultNumbers(defNumbers);
    
    // 2 numbers added in mock_psetcalldiverting.cpp
    QVERIFY(2 == defNumbers.count());
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_getDefaultNumbersException
 */
void UT_PSetCallDivertingWrapper::t_getDefaultNumbersException()
{
    expect("CPsetCallDiverting::GetDefaultNumbersL").
        willOnce(invokeWithoutArguments(this, &simulateLeaveOnMockMethodCallL));
    
    QStringList defNumbers;
    EXPECT_EXCEPTION(
        mWrapper->getDefaultNumbers(defNumbers);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_setNewDefaultNumber
 */
void UT_PSetCallDivertingWrapper::t_setNewDefaultNumber()
{
    expect("CPsetCallDiverting::SetNewDefaultNumberL").times(1);
    expect("CPsetCallDiverting::SwapDefaultNumberL").times(0);

    // number not added in mock_psetcalldiverting.cpp
    QString defNumber = "444444";
    mWrapper->setNewDefaultNumber(defNumber);

    expect("CPsetCallDiverting::GetDefaultNumbersL")
        .willOnce(invoke(SimulateLeaveL));
    EXPECT_EXCEPTION(
        mWrapper->setNewDefaultNumber(defNumber);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_getVoiceMailBoxNumber
 */
void UT_PSetCallDivertingWrapper::t_getVoiceMailBoxNumber()
{
    // Fail case #1, illegal argument
    QString defNumber;
    int ret;
    
    expect("CVoiceMailbox::GetStoredEntry").returns(-2).times(1);
    ret = mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, 0);
    
    // Fail case #2, Not supported.
    expect("CVoiceMailbox::GetStoredEntry").returns(-5).times(1);
    ret = mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, -1);

    // Fail case #3, number nok
    expect("CVoiceMailbox::GetStoredEntry").times(1);
    expect("CVoiceMailboxEntry::GetVmbxNumber").returns(-1).times(1);
    ret = mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, 0);

    // ok case#1, number and save ok but no new number. 
    expect("CVoiceMailbox::GetStoredEntry").returns(-1).times(1);
    ret = mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupData);
    QCOMPARE(ret, 0);

    // ok case#2, number ok
    expect("CVoiceMailbox::GetStoredEntry").times(1);
    expect("CVoiceMailboxEntry::GetVmbxNumber").times(1);
    ret = mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, 0);

    expect("CVoiceMailbox::NewL")
        .willOnce(invoke(SimulateLeaveL));
    EXPECT_EXCEPTION(
        mWrapper->getVoiceMailBoxNumber(defNumber, ServiceGroupFax);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_queryVoiceMailBoxNumber
 */
void UT_PSetCallDivertingWrapper::t_queryVoiceMailBoxNumber()
{
    // Fail case #1, illegal argument
    QString defNumber;
    int ret;
    expect("CVoiceMailbox::GetStoredEntry").returns(-2).times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, -2);
    
    // Fail case #2, Not supported.
    expect("CVoiceMailbox::GetStoredEntry").returns(-5).times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, -1);
    
    // Fail case #3, New number not given.
    expect("CVoiceMailbox::GetStoredEntry").times(1);
    expect("CVoiceMailbox::QueryNewEntry").returns(-5).times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, -5);
    
    // Fail case #4, save nok
    expect("CVoiceMailbox::GetStoredEntry").returns(-1).times(1);
    expect("CVoiceMailbox::QueryNewEntry").times(1);
    expect("CVoiceMailbox::SaveEntry").returns(-4).times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupData);
    QCOMPARE(ret, 0);
    
    // ok case#1, number and save ok but no new number. 
    expect("CVoiceMailbox::GetStoredEntry").returns(-1).times(1);
    expect("CVoiceMailbox::QueryNewEntry").times(1);
    expect("CVoiceMailbox::SaveEntry").times(1);
    expect("CVoiceMailboxEntry::GetVmbxNumber").returns(-5).times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, 0);
    
    // ok case#2, number ok
    expect("CVoiceMailbox::GetStoredEntry").times(1);
    expect("CVoiceMailbox::QueryNewEntry").times(1);
    expect("CVoiceMailbox::SaveEntry").times(1);
    expect("CVoiceMailboxEntry::GetVmbxNumber").times(1);
    ret = mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupVoice);
    QCOMPARE(ret, 0);

    expect("CVoiceMailbox::NewL")
        .willOnce(invoke(SimulateLeaveL));
    EXPECT_EXCEPTION(
        mWrapper->queryVoiceMailBoxNumber(defNumber, ServiceGroupFax);
    )
    
    QVERIFY(true == verify());
}

/*!
  UT_PSetCallDivertingWrapper::t_swapDefaultNumber
 */
void UT_PSetCallDivertingWrapper::t_swapDefaultNumber()
{
    expect("CPsetCallDiverting::SetNewDefaultNumberL").times(0);
    expect("CPsetCallDiverting::SwapDefaultNumberL").times(1);

    // same number added in mock_psetcalldiverting.cpp
    QString defNumber = "222222";
    mWrapper->setNewDefaultNumber(defNumber);

    QVERIFY(true == verify());
}


Q_DECLARE_METATYPE(PSCallDivertingCommand)
/*!
  UT_PSetCallDivertingWrapper::t_handleDivertingChanged
 */
void UT_PSetCallDivertingWrapper::t_handleDivertingChanged()
{
    qRegisterMetaType<PSCallDivertingCommand>("PSCallDivertingCommand");
    QSignalSpy spy(mWrapper, 
        SIGNAL(handleDivertingChanged(const PSCallDivertingCommand&, bool)));
    
    TCallDivertSetting setting;
    setting.iCondition = EDivertConditionUnconditional;
    setting.iSetting = EActivateDiverting;
    setting.iStatus = EDivertingStatusUnknown;
    setting.iServiceGroup = EServiceGroupAllTeleservices;
    setting.iNumber = KNullDesC();
    TBool plural = ETrue;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));

    setting.iCondition = EDivertConditionBusy;
    setting.iStatus = EDivertingStatusActive;
    setting.iServiceGroup = EServiceGroupVoice;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));
    setting.iCondition = EDivertConditionNoReply;
    setting.iStatus = EDivertingStatusInactive;
    setting.iServiceGroup = EServiceGroupData;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));
    setting.iCondition = EDivertConditionNotReachable;
    setting.iStatus = EDivertingStatusNotRegistered;
    setting.iServiceGroup = EServiceGroupFax;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));
    setting.iCondition = EDivertConditionAllCalls;
    setting.iStatus = EDivertingStatusNotProvisioned;
    setting.iServiceGroup = EServiceGroupFax;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));
    setting.iCondition = EDivertConditionAllConditionalCases;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingChangedL(setting, plural));
    
    QCOMPARE(spy.count(), 6);
    PSCallDivertingCommand actualCmd = 
        qvariant_cast<PSCallDivertingCommand>(spy.at(0).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionUnconditional);
    QVERIFY(actualCmd.iSetting == ActivateDiverting);
    QVERIFY(actualCmd.iStatus == DivertingStatusUnknown);
    QVERIFY(actualCmd.iServiceGroup == ServiceGroupAllTeleservices);
    QVERIFY(actualCmd.iNumber == QString::fromUtf16(setting.iNumber.Ptr(),
        setting.iNumber.Length()));
    QVERIFY(spy.at(0).at(1).toBool() == true);
    
    actualCmd = qvariant_cast<PSCallDivertingCommand>(spy.at(1).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionBusy);
    QVERIFY(actualCmd.iStatus == DivertingStatusActive);
    QVERIFY(actualCmd.iServiceGroup == ServiceGroupVoice);
    actualCmd = qvariant_cast<PSCallDivertingCommand>(spy.at(2).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionNoReply);
    QVERIFY(actualCmd.iStatus == DivertingStatusInactive);
    QVERIFY(actualCmd.iServiceGroup == ServiceGroupData);
    actualCmd = qvariant_cast<PSCallDivertingCommand>(spy.at(3).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionNotReachable);
    QVERIFY(actualCmd.iStatus == DivertingStatusNotRegistered);
    QVERIFY(actualCmd.iServiceGroup == ServiceGroupFax);
    actualCmd = qvariant_cast<PSCallDivertingCommand>(spy.at(4).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionAllCalls);
    QVERIFY(actualCmd.iStatus == DivertingStatusNotProvisioned);
    QVERIFY(actualCmd.iServiceGroup == ServiceGroupFax);
    actualCmd = qvariant_cast<PSCallDivertingCommand>(spy.at(5).at(0));
    QVERIFY(actualCmd.iCondition == DivertConditionAllConditionalCases);
    
    QVERIFY(true == verify());
}


/*!
  UT_PSetCallDivertingWrapper::t_handleDivertingStatus
 */
void UT_PSetCallDivertingWrapper::t_handleDivertingStatus()
{
    qRegisterMetaType<QList<PSCallDivertingStatus*> >("QList<PSCallDivertingStatus*>&");
    QSignalSpy spy(mWrapper, 
        SIGNAL(handleDivertingStatus(QList<PSCallDivertingStatus*>&, bool)));
    
    // With empty list
    CMobilePhoneCFList* cfList = NULL;
    QT_TRAP_THROWING(cfList = CMobilePhoneCFList::NewL());
    QScopedPointer<CMobilePhoneCFList> cfListDeleter(cfList);
    TBool plural = ETrue;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingStatusL(*cfList, plural));
    
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(1).toBool() == true);
    spy.clear();
    
    // With list entry
    RMobilePhone::TMobilePhoneCFInfoEntryV1 entry;
    entry.iStatus = RMobilePhone::ECallForwardingStatusNotProvisioned;
    entry.iCondition = RMobilePhone::ECallForwardingAllConditionalCases;
    cfList->AddEntryL(entry);
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingStatusL(*cfList, plural));
    
    QCOMPARE(spy.count(), 1);
    QList<PSCallDivertingStatus*> result = 
            *reinterpret_cast<const QList<PSCallDivertingStatus*> *>(spy.at(0).at(0).constData());

    QVERIFY(result.at(0)->iStatus == DivertingStatusNotProvisioned);
    QVERIFY(result.at(0)->iCondition == DivertConditionAllConditionalCases);
    QVERIFY(spy.at(0).at(1).toBool() == true);
    spy.clear();
    
    // With list, two entries
    QT_TRAP_THROWING(cfList = CMobilePhoneCFList::NewL());
    //delete old cfList and put new in the QScopedPointer;
    cfListDeleter.reset( cfList );

    entry.iStatus = RMobilePhone::ECallForwardingStatusNotActive;
    entry.iCondition = RMobilePhone::ECallForwardingBusy;
    cfList->AddEntryL(entry); // Takes copy
    
    RMobilePhone::TMobilePhoneCFInfoEntryV1 entry2;
    entry2.iStatus = RMobilePhone::ECallForwardingStatusActive;
    entry2.iCondition = RMobilePhone::ECallForwardingUnconditional;
    cfList->AddEntryL(entry2);
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleDivertingStatusL(*cfList, plural));
    
    QCOMPARE(spy.count(), 1);
    result = *reinterpret_cast<const QList<PSCallDivertingStatus*> *>(spy.at(0).at(0).constData());
    QVERIFY(result.at(0)->iStatus == DivertingStatusInactive);
    QVERIFY(result.at(0)->iCondition == DivertConditionBusy);
    
    QVERIFY(result.at(1)->iStatus == DivertingStatusActive);
    QVERIFY(result.at(1)->iCondition == DivertConditionUnconditional);
    QVERIFY(spy.at(0).at(1).toBool() == true);
    spy.clear();
}

/*!
  UT_PSetCallDivertingWrapper::t_handleDivertingError
 */
void UT_PSetCallDivertingWrapper::t_handleDivertingError()
{
    QSignalSpy spy(mWrapper, SIGNAL(handleDivertingError(int)));
    
    QT_TRAP_THROWING(mWrapper->m_Priv->HandleDivertingErrorL(KErrGeneral));
    
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0).toInt() == KErrGeneral);
}

/*!
  UT_PSetCallDivertingWrapper::t_handleCFRequesting
 */
void UT_PSetCallDivertingWrapper::t_handleCFRequesting()
{
    QSignalSpy spy(mWrapper, SIGNAL(handleCFRequesting(bool, bool)));
    
    TBool ongoing = ETrue;
    TBool interrupted = ETrue;
    QT_TRAP_THROWING(
        mWrapper->m_Priv->HandleCFRequestingL(ongoing, interrupted));
    
    QCOMPARE(spy.count(), 1);
    QVERIFY(spy.at(0).at(0).toBool() == true);
    QVERIFY(spy.at(0).at(1).toBool() == true);
}

/*!
  UT_PSetCallDivertingWrapper::t_setEngineContact
  TODO: dummy test because no implementation for the method currently.
 */
void UT_PSetCallDivertingWrapper::t_setEngineContact()
{
    mWrapper->m_Priv->SetEngineContact(mPsetCallDivertingMock);
}

/*!
  UT_PSetCallDivertingWrapper::t_handleEmergencyDial
  TODO: dummy test because no implementation for the method currently.
 */
void UT_PSetCallDivertingWrapper::t_handleEmergencyDial()
{
    QT_TRAP_THROWING(mWrapper->m_Priv->HandleEmergencyDialL(KErrNone));
}

/*!
  UT_PSetCallDivertingWrapper::t_convertPsCallDivertingCondition
 */
void UT_PSetCallDivertingWrapper::t_convertPsCallDivertingCondition()
{
    QVERIFY(EDivertConditionUnconditional == 
        mWrapper->m_Priv->convert(DivertConditionUnknown));
    QVERIFY(EDivertConditionUnconditional == 
        mWrapper->m_Priv->convert(DivertConditionUnconditional));
    QVERIFY(EDivertConditionBusy == 
        mWrapper->m_Priv->convert(DivertConditionBusy));
    QVERIFY(EDivertConditionNoReply == 
        mWrapper->m_Priv->convert(DivertConditionNoReply));
    QVERIFY(EDivertConditionNotReachable == 
        mWrapper->m_Priv->convert(DivertConditionNotReachable));
    QVERIFY(EDivertConditionAllCalls == 
        mWrapper->m_Priv->convert(DivertConditionAllCalls));
    QVERIFY(EDivertConditionAllConditionalCases == 
        mWrapper->m_Priv->convert(DivertConditionAllConditionalCases));
}

/*!
  UT_PSetCallDivertingWrapper::t_convertPsCallDivertingStatus
 */
void UT_PSetCallDivertingWrapper::t_convertPsCallDivertingStatus()
{
    QVERIFY(EDivertingStatusActive == 
        mWrapper->m_Priv->convert(DivertingStatusActive));
    QVERIFY(EDivertingStatusInactive == 
        mWrapper->m_Priv->convert(DivertingStatusInactive));
    QVERIFY(EDivertingStatusNotRegistered == 
        mWrapper->m_Priv->convert(DivertingStatusNotRegistered));
    QVERIFY(EDivertingStatusNotProvisioned == 
        mWrapper->m_Priv->convert(DivertingStatusNotProvisioned));
}

/*!
  UT_PSetCallDivertingWrapper::t_convertPsCallDivertingSetting
 */
void UT_PSetCallDivertingWrapper::t_convertPsCallDivertingSetting()
{
    QVERIFY(EActivateDiverting == 
        mWrapper->m_Priv->convert(ActivateDiverting));
    QVERIFY(ECancelDiverting == 
        mWrapper->m_Priv->convert(CancelDiverting));
    QVERIFY(ERegisterDiverting == 
        mWrapper->m_Priv->convert(RegisterDiverting));
    QVERIFY(EEraseDiverting == 
        mWrapper->m_Priv->convert(EraseDiverting));
    QVERIFY(ECheckStatus == 
        mWrapper->m_Priv->convert(CheckStatus));
    const int invalidEnumeration = -1;
    QVERIFY(ECheckStatus == mWrapper->m_Priv->convert(
        static_cast<PsCallDivertingSetting>(invalidEnumeration)));
}

/*!
  UT_PSetCallDivertingWrapper::t_exceptionSafety
 */
void UT_PSetCallDivertingWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}

/*!
  UT_PSetCallDivertingWrapper::simulateLeaveOnMockMethodCallL
 */
void UT_PSetCallDivertingWrapper::simulateLeaveOnMockMethodCallL()
{
    User::Leave(KErrGeneral);
}

QTEST_MAIN_S60(UT_PSetCallDivertingWrapper)
