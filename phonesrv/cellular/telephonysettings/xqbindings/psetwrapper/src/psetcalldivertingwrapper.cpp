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

#include <QtGlobal>
#include <rsssettings.h>
#include <psetcontainer.h>
#include <psetcalldiverting.h>
#include <mmlist.h>
#include <cphcltemergencycall.h>
#include <badesca.h>
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include "psetcalldivertingwrapper.h"
#include "psetcalldivertingwrapper_p.h"
#include "logging.h"

//Constant
_LIT(KCFInvalidChars,"*#pw");
_LIT(KCFPlusSign,"+");

/*!
  PSetCallDivertingWrapper::PSetCallDivertingWrapper
 */
PSetCallDivertingWrapper::PSetCallDivertingWrapper(
        CPsetContainer &psetContainer, 
        QObject *parent) :
    QObject(parent), m_psetCallDiverting(NULL), m_emergencyCall(NULL), 
    m_DefaultNumberListCDes(NULL), m_Priv(NULL)
{
    DPRINT << ": IN";
    
    m_Priv = new PSetCallDivertingWrapperPrivate(*this);
    QScopedPointer<PSetCallDivertingWrapperPrivate> privateImplGuard(m_Priv);
    
    QT_TRAP_THROWING(
        m_psetCallDiverting = psetContainer.CreateCFObjectL( *m_Priv ));
    m_psetCallDiverting->SetRequestObserver(m_Priv);
	QScopedPointer<CPsetCallDiverting> pSetCallDivertingGuard(m_psetCallDiverting);
    
    QT_TRAP_THROWING(m_emergencyCall = CPhCltEmergencyCall::NewL( m_Priv ));
    QScopedPointer<CPhCltEmergencyCall> emergencyCallGuard(m_emergencyCall);
    
    privateImplGuard.take();
    pSetCallDivertingGuard.take();
    emergencyCallGuard.take();
    
    DPRINT << ": OUT";
}

/*!
  PSetCallDivertingWrapper::~PSetCallDivertingWrapper
 */
PSetCallDivertingWrapper::~PSetCallDivertingWrapper()
{
    DPRINT << ": IN";
    
    delete m_psetCallDiverting;
    delete m_emergencyCall;
    
    if (m_DefaultNumberListCDes) {
        m_DefaultNumberListCDes->Reset();
        delete m_DefaultNumberListCDes;
    }
    
    delete m_Priv;
    
    DPRINT << ": OUT";
}

// --------  Wrapper interface  --------- // 
/*!
  PSetCallDivertingWrapper::setCallDiverting
 */
int PSetCallDivertingWrapper::setCallDiverting(
        PSCallDivertingCommand& aSetting, int aBasicServiceGroup)
{
    DPRINT << ": IN";

    TCallDivertSetting param;
    param.iCondition = m_Priv->convert(aSetting.iCondition);
    param.iSetting = m_Priv->convert(aSetting.iSetting);
    param.iStatus = m_Priv->convert(aSetting.iStatus);
    param.iServiceGroup = m_Priv->convert(aSetting.iServiceGroup);
    param.iNumber = TPtrC16(aSetting.iNumber.utf16());
    param.iNoReplyTimer = aSetting.iNoReplyTimer;
    
    int result = validateDivertNumber(param.iNumber);
    if (0 == result) {
        QT_TRAP_THROWING(
            m_psetCallDiverting->SetDivertingL(
                param,
                static_cast<TBasicServiceGroups>(aBasicServiceGroup)));
    } 
    
    DPRINT << ": OUT";
    return result;
}

/*!
  PSetCallDivertingWrapper::getCallDivertingStatus
 */
void PSetCallDivertingWrapper::getCallDivertingStatus(
        const PsServiceGroup aServiceGroup, 
        const PsCallDivertingCondition aCondition,
        int aBsc)
{
    DPRINT << ": IN";
    DPRINT << "aServiceGroup : " << aServiceGroup;
    DPRINT << "aCondition : " << aCondition;
    
    QT_TRAP_THROWING(
        m_psetCallDiverting->GetDivertingStatusL(
            m_Priv->convert(aServiceGroup),
            m_Priv->convert(aCondition),
            (TBasicServiceGroups)aBsc));

    DPRINT << ": OUT";
}

/*!
  PSetCallDivertingWrapper::cancelProcess
 */
void PSetCallDivertingWrapper::cancelProcess()
{
    DPRINT << ": IN";
    
    m_psetCallDiverting->CancelCurrentRequest();
    
    DPRINT << ": OUT";
}

/*!
  PSetCallDivertingWrapper::getDefaultNumbers
 */
void PSetCallDivertingWrapper::getDefaultNumbers(QStringList &aDefNumbers)
{
    // QStringList?, max 3 numbers to retrieve.

    DPRINT << ": IN";

    while (!m_DefaultnumberListQSList.empty()) {
        m_DefaultnumberListQSList.removeFirst();
    }
    m_DefaultNumberListCDes = new CDesC16ArrayFlat(3);
    QT_TRAP_THROWING(
        m_psetCallDiverting->GetDefaultNumbersL(*m_DefaultNumberListCDes));
    int count = m_DefaultNumberListCDes->Count();
    for (int i = 0; i < count; i++) {
        m_DefaultnumberListQSList.append(QString::fromUtf16(
            m_DefaultNumberListCDes->MdcaPoint(i).Ptr(),
            m_DefaultNumberListCDes->MdcaPoint(i).Length()));
    }
    aDefNumbers = m_DefaultnumberListQSList;
    m_DefaultNumberListCDes->Reset();
    delete m_DefaultNumberListCDes;
    m_DefaultNumberListCDes = NULL;

    DPRINT << ": OUT";
}

/*!
  PSetCallDivertingWrapper::setNewDefaultNumber
 */
void PSetCallDivertingWrapper::setNewDefaultNumber(QString aNumber)
{
    DPRINT << ": IN";
    
    TBuf<KPsetPhoneNumberLength> bufNumber(aNumber.utf16());
    TInt found(KErrNotFound);
    TInt loc(KErrNotFound);
    m_DefaultNumberListCDes = new CDesC16ArrayFlat(3);
    QT_TRAP_THROWING(
        m_psetCallDiverting->GetDefaultNumbersL(*m_DefaultNumberListCDes));
    found = m_DefaultNumberListCDes->Find(bufNumber, loc);
    m_DefaultNumberListCDes->Reset();
    delete m_DefaultNumberListCDes;
    m_DefaultNumberListCDes = NULL;
    if (found != KErrNone) { //number is not part of current def.numbers
        QT_TRAP_THROWING(
            m_psetCallDiverting->SetNewDefaultNumberL(bufNumber));
    } else {
        QT_TRAP_THROWING(
            m_psetCallDiverting->SwapDefaultNumberL(loc+1));
    }
    
    DPRINT << ": OUT";
}

/*!
 * PSetCallDivertingWrapper::getVoiceMailBoxNumber
 * @param aNumber empty if not set
 * @return -1 if not supported
 */
int PSetCallDivertingWrapper::getVoiceMailBoxNumber(
        QString &aNumber, PsService aService)
{
    DPRINT << ": IN";

    CVoiceMailboxEntry* psetVoiceMailboxEntry = NULL;
    CVoiceMailbox* psetVoiceMailBoxScoped = NULL;
    QT_TRAP_THROWING(psetVoiceMailBoxScoped = CVoiceMailbox::NewL());
    QScopedPointer<CVoiceMailbox> psetVoiceMailBox(psetVoiceMailBoxScoped);

    TVoiceMailboxParams psetVoiceMailboxParams;
    // Selected mailbox type is voice
    psetVoiceMailboxParams.iType = m_Priv->convert(aService);
    psetVoiceMailboxEntry = 0;
    int ret = 0;
    TInt error = psetVoiceMailBox->GetStoredEntry(
            psetVoiceMailboxParams, psetVoiceMailboxEntry);
    DPRINT << "error: " << error;
    if (KErrNotSupported == error) {
        ret = -1;
    } else if (KErrNone == error) {
        // Entry ok, check the number
        TPtrC ptrNumber(KNullDesC);
        if (KErrNone == psetVoiceMailboxEntry->GetVmbxNumber(ptrNumber)) {
            // number ok
            aNumber = QString::fromUtf16(ptrNumber.Ptr(),
                    ptrNumber.Length());
        }
        
    } else {
        // illegal argument
    }

    delete psetVoiceMailboxEntry; // Entry ownership was transferred
    psetVoiceMailboxEntry = NULL;

    DPRINT << "aVmbxNumber: " << aNumber;
    DPRINT << ": OUT";
    return ret;
}

/*!
 * PSetCallDivertingWrapper::queryVoiceMailBoxNumber
 * @param aNumber empty if not set
 * @return -1 if not supported or an error code
 */
int PSetCallDivertingWrapper::queryVoiceMailBoxNumber(
        QString &aNumber, PsService aService)
{
    DPRINT << ": IN";

    CVoiceMailboxEntry* psetVoiceMailboxEntry = NULL;
    CVoiceMailbox* psetVoiceMailBoxScoped = NULL;
    QT_TRAP_THROWING(psetVoiceMailBoxScoped = CVoiceMailbox::NewL());
    QScopedPointer<CVoiceMailbox> psetVoiceMailBox(psetVoiceMailBoxScoped);

    TVoiceMailboxParams psetVoiceMailboxParams;
    // Selected mailbox type is voice
    psetVoiceMailboxParams.iType = m_Priv->convert(aService);
    psetVoiceMailboxEntry = 0;
    int ret = 0;
    TInt error = psetVoiceMailBox->GetStoredEntry(
            psetVoiceMailboxParams, psetVoiceMailboxEntry);
    
    if (KErrNotSupported == error) {
        ret = -1;
    } else if (KErrNone == error || KErrNotFound == error) {
        // No number defined, query new entry from user
        error = psetVoiceMailBox->QueryNewEntry(
                psetVoiceMailboxParams, psetVoiceMailboxEntry);

        if (KErrNone == error) {
            // Save new entry and get the number.
            error = psetVoiceMailBox->SaveEntry( *psetVoiceMailboxEntry );
            TPtrC ptrNumber( KNullDesC );
            if ( KErrNone == psetVoiceMailboxEntry->GetVmbxNumber( ptrNumber )) {
                // New number ok.
                aNumber = QString::fromUtf16(ptrNumber.Ptr(),
                        ptrNumber.Length());
            }
        
        } else {
            // New number not given.
            DPRINT << "New number error: " << error;
            ret = error; 
        }
    } else {
        // illegal argument 
        DPRINT << "error: " << error;
        ret = error; 
    }

    delete psetVoiceMailboxEntry; // Entry ownership was transferred
    psetVoiceMailboxEntry = NULL;

    DPRINT << "aVmbxNumber: " << aNumber;
    DPRINT << ": OUT";
    return ret;
}

/*!
  PSetCallDivertingWrapper::validateDivertNumber
 */
int PSetCallDivertingWrapper::validateDivertNumber(
        const TDesC16& aDivertTo) const
{
    DPRINT << ": IN ";
    
    int result = 0;
    
    QString diverto = QString::fromUtf16(aDivertTo.Ptr(),aDivertTo.Length());
    DPRINT << "aDivertTo: " << diverto;
    TInt noOfInvalidChars = KCFInvalidChars().Length();
    TBool isEmergency(EFalse);
    TTelNumber number;
    TInt error = m_emergencyCall->IsEmergencyPhoneNumber(aDivertTo, number,
            isEmergency);
    if ((error == KErrNone) && isEmergency) {
        result = KErrGsmSSIllegalOperation;
        DPRINT << ": emergency number.";
    } else {
        // Plus is a special case, as it is accepted as first character.
        if (findPlus(aDivertTo)) {
            result = KErrGsmSSUnknownAlphabet;
        }
        for (int index = 0; index < noOfInvalidChars; index++) {
            if (aDivertTo.Find(KCFInvalidChars().Mid(index, 1)) == KErrNone) {
                result = KErrGsmSSUnknownAlphabet;
                DPRINT << ": Invalid divert number. ";
            }
        }
    }

    DPRINT << ": OUT : result " << result;
    return result;
}

/*!
  PSetCallDivertingWrapper::findPlus
  Seeks '+' from given number. If '+' is first character, it is ignored.
 */
bool PSetCallDivertingWrapper::findPlus(const TDesC16& aDivertTo) const
{
    DPRINT << ": IN ";

    bool plusFound(false);
    if (!(aDivertTo.Find(KCFPlusSign) == KErrNotFound)) {
        //first char '+', but there can additional '+'s
        //ignore first '+'
        TPtrC concat = aDivertTo.Right(aDivertTo.Length() - 1);
        if (concat.Find(KCFPlusSign) == KErrNone) {
            plusFound = true;
            DPRINT << ": Invalid divert number. ";
        }
    }

    DPRINT << ": OUT : plusFound " << plusFound;
    return plusFound;
}

/**
 * Get CPsetCallDiverting reference.
 */
CPsetCallDiverting &  PSetCallDivertingWrapper::getCPsetCallDiverting() const
    {
    DPRINT;
    
    return *m_psetCallDiverting;
    }

// End of File. 
