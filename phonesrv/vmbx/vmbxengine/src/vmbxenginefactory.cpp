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
* Description:  Implementation of the CVmbxEngineFactory class
*
*/

// INCLUDE FILES
#include "vmbxenginebase.h"
#include "vmbxcsvoiceengine.h"
#include "vmbxcsvideoengine.h"
#include "vmbxvoipengine.h"
#include "vmbxutilities.h"

#include "vmbxlogger.h"

#include "vmbxenginefactory.h"


// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxEntryFactory::CVmbxEntryFactory
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxEngineFactory::CVmbxEngineFactory( MVmbxResourceProvider& aProvider )
                                    : iProvider( aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::CVmbxEntryFactory <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxEntryFactory::~CVmbxEntryFactory
// Destructor.
// ---------------------------------------------------------------------------
//
CVmbxEngineFactory::~CVmbxEngineFactory()
    {
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::~CVmbxEntryFactory <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxEntryFactory::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxEngineFactory* CVmbxEngineFactory::NewL( 
                                    MVmbxResourceProvider& aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::NewL =>" );
    CVmbxEngineFactory* self = 
        new( ELeave ) CVmbxEngineFactory( aProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxEntryFactory::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxEngineFactory::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::ConstructL =>" );
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxEntryFactory::CreateEngineL
// Loads instance of the mailbox entry
// ---------------------------------------------------------------------------
//
void CVmbxEngineFactory::CreateEngineL( CVmbxEngineBase*& aEngine,
                                                     TVmbxType aType )
    {
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::CreateEngineL =>" );
    aEngine = NULL;

    TInt result( KErrNone );
    switch( aType )
        {
        case EVmbxVoice:
            {
             // Create voice mailbox
            CVmbxEngineBase* voice = CVmbxCsVoiceEngine::NewL( iProvider );
            aEngine = voice;        
            }
            break;
        case EVmbxVideo:
            {
            if ( VmbxUtilities::VideoSupported() )
                {
                 // Create video mailbox
                CVmbxEngineBase* video = CVmbxCsVideoEngine::NewL( iProvider );
                aEngine = video;            
                }
            else
                {
                result = KErrNotSupported;
                }
            }
            break;
        case EVmbxVoip:
            {
            if ( VmbxUtilities::VoIPSupported() )
                {
                 // Create Voip mailbox
                CVmbxEngineBase* voip = CVmbxVoIpEngine::NewL( iProvider);
                aEngine =  voip;
                }
            else
                {
                result = KErrNotSupported;
                }
            }
            break;
        default:
            result = KErrArgument;
            break;
        }

    VMBLOGSTRING2( "VMBX: CVmbxEntryFactory::CreateEngineL: result%I",
    result );
    if ( KErrNone != result )
        {
        User::Leave( result );
        }
    VMBLOGSTRING( "VMBX: CVmbxEntryFactory::CreateEngineL <=" );
    }

// End of file
