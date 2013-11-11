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
 * This file defines the interface into the STS server.
 */
#ifndef STSSERVER_H_
#define STSSERVER_H_

#include <e32base.h>
#include <systemtoneservice.h>

class CSts;
class CStsServerSession;

class CStsServer : private CPolicyServer
    {
public:

    // Runs the server on the current thread, and blocks until the server exits.
    IMPORT_C static void RunServerL();

    void AddSession(CStsServerSession* aSession);

    void DropSession(CStsServerSession* aSession);

private:

    static CStsServer* NewLC();

    CStsServer();
    void ConstructL();
    virtual ~CStsServer();

    CPolicyServer::TCustomResult SecurityCheckAlarm(
            CSystemToneService::TAlarmType aAlarm, const RMessage2& aMsg,
            TSecurityInfo& aMissing);

    CPolicyServer::TCustomResult SecurityCheckTone(
            CSystemToneService::TToneType aTone, const RMessage2& aMsg,
            TSecurityInfo& aMissing);

    // inherited from CPolicyServer    
    virtual CPolicyServer::TCustomResult CustomSecurityCheckL(
            const RMessage2& aMsg, TInt& aAction, TSecurityInfo& aMissing);
    CSession2* NewSessionL(const TVersion& aVersion,
            const RMessage2& aMessage) const;

    RPointerArray<CStsServerSession> iSessions;
    CSts* iSts;

    };

#endif //STSSERVER_H_
