/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Server application for launching SendUi with low 
*                capability requirements.
*
*/



#include "senduilauncherdoc.h"
#include "senduilauncherapp.h"
#include "senduilauncherappui.h"
#include "senduiservice.h"
#include "senduiserviceclient.h"
#include "senduiserviceslog.h"
#include <SendUiConsts.h>
#include "senduilauncher.h"

// ---------------------------------------------------------------------------
// Creates SendUiService or pass service request to the framework
// ---------------------------------------------------------------------------
//
class CSendUiServer : public CAknAppServer
    {
    public:
    
    CApaAppServiceBase* CreateServiceL( TUid aServiceType ) const
        {
        if( aServiceType.iUid == KSendUiLauncherAppServiceUid.iUid )
            {
            return CSendUiService::NewL();
            }
        else
            {
            return CAknAppServer::CreateServiceL( aServiceType );
            }
        }
    };

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructs and returns an application object.
// ---------------------------------------------------------------------------
//
CApaDocument* CSendUiLauncherApplication::CreateDocumentL()
    {  
    // Create an SendUiServer document, and return a pointer to it
    CApaDocument* document = CSendUiLauncherDocument::NewL(*this);
    return document;
    }

// ---------------------------------------------------------------------------
// AppDllUid
// Return application Uid
// ---------------------------------------------------------------------------
//
TUid CSendUiLauncherApplication::AppDllUid() const
    {
    // Return the UID for the SendUiServer application
    return KSendUiLauncherAppUid;
    }

// ---------------------------------------------------------------------------
// NewAppServerL
// Returns new CSendUIServer object for the framework
// ---------------------------------------------------------------------------
//
void CSendUiLauncherApplication::NewAppServerL( CApaAppServer* & aAppServer )
    {
    aAppServer = new(ELeave)CSendUiServer;
    }

