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

#ifndef PSETCALLDIVERTINGWRAPPER_H_
#define PSETCALLDIVERTINGWRAPPER_H_

#include <QObject>
#include <QList>
#include <QStringList>
#include <psetwrappertypes.h>

// Forward declarations 
class CPsetCallDiverting;
class CPsetContainer;
class PSetCallDivertingWrapperPrivate;
class CPhCltEmergencyCall;
class CDesC16ArrayFlat;
class TDesC16;

// Call diverting command
class PSCallDivertingCommand
{
public:
    //member data
    //Divert condition
    PsCallDivertingCondition iCondition;
    //Divert setting
    PsCallDivertingSetting iSetting;
    //Divert status
    PsCallDivertingStatus iStatus;
    //Divert service group
    PsServiceGroup iServiceGroup;
    //Diverted-to number
    QString iNumber;
    //Delay time before starting diverting
    int iNoReplyTimer;
};

// Call diverting command
class PSCallDivertingStatus
{
public:
    //member data
    //Divert condition
    PsCallDivertingCondition iCondition;
    //Divert service group
    PsServiceGroup iServiceGroup;
    //Divert status
    PsCallDivertingStatus iStatus;
    //Diverted-to number
    QString iNumber;
    /** The "No Reply" time-out (in seconds) registered for the call forwarding no reply 
    condition.

    Equals to -1 if this value is not applicable. In GSM mode, will be between 
    5 and 30 and in steps of 5 if this value is applicable. */
    int iTimeout;
};

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetCallDivertingWrapper : public QObject
{
Q_OBJECT

public:

    explicit PSetCallDivertingWrapper(CPsetContainer &psetContainer,
            QObject *parent = NULL);

    virtual ~PSetCallDivertingWrapper();

public:
    // Functions (adaptees):  

    /**
     * Sets call diverting to the network.
     *
     * @param aSetting New settings for the call diverting.
     * @param aBsc Basic service group concerned.
     * @return  Error code.
     */
    int setCallDiverting(PSCallDivertingCommand& aSetting,
            int aBasicServiceGroup);

    /**
     * Checks the call diverting status from network.
     */
    void getCallDivertingStatus(const PsServiceGroup aServiceGroup,
            const PsCallDivertingCondition aCondition, int aBsc);

    /**
     * Cancels the call diverting-request process.
     */
    void cancelProcess();

    /**
     * Retrieve the default (last forwarded-to) numbers. 
     */
    void getDefaultNumbers(QStringList &aDefNumbers); // QStringList

    /**
     * Sets new number to the default numbers (last forwarded-to) list.
     */
    void setNewDefaultNumber(QString aNumber);

    /**
     * Get voicemailbox number.
     * @param aNumber empty if not set
     * @return -1 if not supported
     */
    int getVoiceMailBoxNumber(QString &aNumber, PsService aService);
    
    /**
     * Query voicemailbox number.
     * @param aNumber empty if not set
     * @return -1 if not supported
     */
    int queryVoiceMailBoxNumber(QString &aNumber, PsService aService);

    /**
     * Get CPsetCallDiverting reference.
     */
    CPsetCallDiverting & getCPsetCallDiverting() const;

signals: // Notify via signals     

    void handleDivertingChanged(const PSCallDivertingCommand& aSetting,
            bool aPlural);

    void handleDivertingStatus(QList<PSCallDivertingStatus*>& diverList,
            bool aPlural);

    void handleDivertingError(int aReason);

    void handleCFRequesting(bool aOngoing, bool aInterrupted);
    
    void requestDone();

private:
    
    int validateDivertNumber(const TDesC16& aDivertTo) const;
    bool findPlus(const TDesC16& aDivertTo) const;

private:
    // Data: 
    // Phone setting handlers, own
    CPsetCallDiverting* m_psetCallDiverting;

    // Owned: Phone client emergency number session.
    CPhCltEmergencyCall* m_emergencyCall;

    // Own
    CDesC16ArrayFlat* m_DefaultNumberListCDes;
    QStringList m_DefaultnumberListQSList;

    // Own
    PSetCallDivertingWrapperPrivate* m_Priv;
    friend class PSetCallDivertingWrapperPrivate;
};

#endif /* PSETCALLDIVERTINGWRAPPER_H_ */
