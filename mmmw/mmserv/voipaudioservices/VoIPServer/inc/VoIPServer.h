/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Definition of CVoIPAudioServer server class.
 *
 */

#ifndef VOIPAUDIOSERVER_H
#define VOIPAUDIOSERVER_H

#include <e32base.h>

// FORWARD DECLARATIONS
class CVoIPAudioServerSession;
class CDelayVoIPServerShutDown;
class CVoIPECallEventHandler;

// -----------------------------------------------------------------------------
// CVoIPAudioServer class
// -----------------------------------------------------------------------------
class CVoIPAudioServer : public CServer2
    {
public:
    static CVoIPAudioServer* NewL();
    ~CVoIPAudioServer();

    void AddSession();
    void DropSession();
    TInt SessionCount();
    void SetDnLinkSession(CVoIPAudioServerSession* aSession, TBool aStatus);
    void SetUpLinkSession(CVoIPAudioServerSession* aSession, TBool aStatus);
    TBool HasDnLinkSession();
    TBool HasUpLinkSession();

    // ECall event handling
    void StartECallEventNotifierL();
    void CancelECallEventNotifier();
    void NotifyECallEvent(TInt aECall);

private:
    CVoIPAudioServer();
    void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion,
            const RMessage2& aMessage) const;

private:
    TInt iSession;
    TBool iECallInProgress;
    CDelayVoIPServerShutDown *iShutdownTimer;
    CVoIPECallEventHandler* iECallHandler;
    CVoIPAudioServerSession* iDnlinkSession;
    CVoIPAudioServerSession* iUplinkSession;
    };

#endif //VOIPAUDIOSERVER_H

// End of file
