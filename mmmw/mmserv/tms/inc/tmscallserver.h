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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSCALLSERVER_H
#define TMSCALLSERVER_H

// INCLUDES
#include <e32base.h>

namespace TMS {

const TUint KTMSCallServerMajorVersionNumber = 8;
const TUint KTMSCallServerMinorVersionNumber = 0;
const TUint KTMSCallServerBuildVersionNumber = 1;

// FORWARD DECLARATIONS
class TMSCallServerStartParam;
class TMSServer;
class TMSCallDelayTimer;

// -----------------------------------------------------------------------------
// TMSCallServer class
// -----------------------------------------------------------------------------
class TMSCallServer : public CServer2
    {
public:
    static TMSCallServer* NewL(TMSServer* aTMSServer);
    ~TMSCallServer();

    void AddSession();
    void DropSession();
    void TerminateServer();

    IMPORT_C static TInt StartThread(TAny* aParams);

private:
    TMSCallServer(TMSServer* aTMSServer);
    void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion,
            const RMessage2& aMessage) const;

    static void StartThreadL(TMSCallServerStartParam& aStart);

private:
    TMSServer* iTMSServer;
    TMSCallDelayTimer* iDelayTimer;
    TUint iSessionCount;
    };

} //namespace TMS

#endif // TMSCALLSERVER_H

// End of file
