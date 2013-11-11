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
* Description: Implementation of CVmbxCenRepObserver class.
 *
*/

// INCLUDE FILES
#include <centralrepository.h>

#include "vmbxlogger.h"
#include "voicemailboxdomaincrkeys.h"
#include "voicemailboxprivatecrkeys.h"
#include "voicemailboxdefsinternal.h"
#include "vmbxcenrepobserver.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::CVmbxCenRepObserver
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CVmbxCenRepObserver::CVmbxCenRepObserver( MVmbxCenRepObserver& aObserver ):
                                                iObserver( aObserver )
    {
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::CVmbxCenRepObserver =>" );
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::CVmbxCenRepObserver <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxCenRepObserver::ConstructL()
    {
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::ConstructL =>" );
    iCRSession = CRepository::NewL( KCRUidVoiceMailbox );
    iNotifyHandler = CCenRepNotifyHandler::NewL( *this, *iCRSession );
    iNotifyHandler->StartListeningL();
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxCenRepObserver* CVmbxCenRepObserver::NewL( MVmbxCenRepObserver& aObserver )
    {
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::NewL =>" );
    CVmbxCenRepObserver* self = CVmbxCenRepObserver::NewLC( aObserver );
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxCenRepObserver* CVmbxCenRepObserver::NewLC( MVmbxCenRepObserver& aObserver )
    {
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::NewLC =>" );
    CVmbxCenRepObserver* self = new (ELeave) CVmbxCenRepObserver( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::NewLC <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::~CVmbxCenRepObserver
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxCenRepObserver::~CVmbxCenRepObserver()
    {
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::~CVmbxCenRepObserver =>" );
    if ( iNotifyHandler )
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        iNotifyHandler = NULL;
        }
    delete iCRSession;
    VMBLOGSTRING( "VMBX:CVmbxCenRepObserver::~CVmbxCenRepObserver <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepObserver::HandleNotifyGeneric
// To notify the client about changes for string value keys
// ---------------------------------------------------------------------------
//
void CVmbxCenRepObserver::HandleNotifyGeneric( TUint32 aId )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepObserver::HandleNotifyGeneric =>" );
    if ( KVmbxNumberLinePrimary == aId 
        || KVmbxVideoNmbrLinePrimary == aId 
        || KVmbxNumberLineAlternate == aId 
        || KVmbxVideoNmbrLineAlternate == aId 
        )
        {
        VMBLOGSTRING2( "VMBX: CVmbxCenRepObserver::HandleNotifyGeneric: aId %X",
            aId );

        TVmbxCenRepKey cenRepKey( EVmbxCenRepMax );
        if ( KVmbxNumberLinePrimary == aId  )
            {
            cenRepKey = EVmbxVoiceLinePrimaryKey;
            }
        else if ( KVmbxVideoNmbrLinePrimary == aId  )
            {
            cenRepKey = EVmbxVideoLinePrimaryKey;
            }
        else if ( KVmbxNumberLineAlternate == aId  )
            {
            cenRepKey = EVmbxVoiceLineAlternateKey;
            }
        else if ( KVmbxVideoNmbrLineAlternate == aId  )
            {
            cenRepKey = EVmbxVideoLineAlternateKey;
            }

        // Notify the primary/alternate support keys are changed
        iObserver.HandleCenRepChange( cenRepKey );
        }
    VMBLOGSTRING( "VMBX: CVmbxCenRepObserver::HandleNotifyGeneric<="); 
    }

//  End of File
