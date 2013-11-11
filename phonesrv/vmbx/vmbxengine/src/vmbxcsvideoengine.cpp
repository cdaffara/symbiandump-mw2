/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CVmbxCsVideoEngine class
*
*/


// INCLUDE FILES
#include <cvoicemailboxentry.h>

#include "vmbxuiutilities.h"
#include "vmbxutilities.h"
#include "vmbxenginebase.h"
#include "vmbxlogger.h"
#include "vmbxcenrephandler.h"
#include "vmbxcsvideoengine.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::CVmbxCsVideoEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxCsVideoEngine::CVmbxCsVideoEngine( MVmbxResourceProvider& aProvider )
                                : CVmbxEngineBase( aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::CVmbxCsVideoEngine <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::~CVmbxCsVideoEngine
// Destructor.
// ---------------------------------------------------------------------------
//
CVmbxCsVideoEngine::~CVmbxCsVideoEngine()
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::~CVmbxCsVideoEngine <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxCsVideoEngine* CVmbxCsVideoEngine::NewL( MVmbxResourceProvider& aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::NewL =>" );
    if ( !VmbxUtilities::VideoSupported() )
        {
        User::Leave( KErrNotSupported );
        }

    CVmbxCsVideoEngine* self = new( ELeave ) CVmbxCsVideoEngine( aProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxCsVideoEngine::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::ConstructL =>" );
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::GetL
// Gets voice mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxCsVideoEngine::GetL( CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::GetL =>" );
    CVoiceMailboxEntry* vmbxEntry = CVoiceMailboxEntry::NewLC();
    // get number from store
    vmbxEntry->SetVoiceMailboxType( EVmbxVideo );
    vmbxEntry->SetUsingMemoryLocation( EVmbxPhoneMemory );
    vmbxEntry->SetVmbxAlsLineType( VmbxUtilities::AlsLine() );
    vmbxEntry->SetServiceId( KVmbxServiceVideo );
    TInt result = iProvider.VmbxCenRepHandler().GetVmbxNumber( *vmbxEntry );

    VMBLOGSTRING2( "VMBX: CVmbxCsVideoEngine::GetL: result=%I ", result );
    User::LeaveIfError( result );
    CleanupStack::Pop( vmbxEntry );
    aEntry = vmbxEntry;
    vmbxEntry = NULL;
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::GetL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::SaveL
// Saves voice mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxCsVideoEngine::SaveL( const CVoiceMailboxEntry& aEntry,
                                TBool aShowNotesAllowed )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::SaveL =>" );

    TPtrC vmbxNumber( KNullDesC );
    aEntry.GetVmbxNumber( vmbxNumber );

    if ( vmbxNumber.Length() 
         && ( !VmbxUtilities::IsValidPhoneNumber( vmbxNumber )
         || VmbxUtilities::IsEmergencyNumber( vmbxNumber ) ) )
        {
        VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::SaveL: number invalid <=" );
        User::Leave( KErrArgument );
        }

    // Save to cenrep
    TInt result = iProvider.VmbxCenRepHandler().Save( aEntry );
    VMBLOGSTRING2( "VMBX: CVmbxCsVideoEngine::SaveL: result%I ", result );
    User::LeaveIfError( result );

    // show note
    if ( KErrNone == result && aShowNotesAllowed )
        {
        iProvider.VmbxUiUtilities().ShowInformationdNoteL( EVideoNumberSaved );
        }

    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::SaveL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVideoEngine::SaveL
// Saves Provisioned video mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxCsVideoEngine::SaveProvisionedEntryL( 
                const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::SaveProvisionedEntryL =>" );
    SaveL( aEntry, EFalse );
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::SaveProvisionedEntryL <=" );
    }

// ----------------------------------------------------------------------------
//  CVmbxCsVideoEngine::CheckConfiguration()
// ----------------------------------------------------------------------------
//
TBool CVmbxCsVideoEngine::CheckConfiguration( 
            const TVoiceMailboxParams& aParams, const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVideoEngine::CheckConfiguration =>" );
    TBool conf( EFalse );
    if ( EVmbxChangeNbrAllowedOnUi & aFlags )
        {
        conf = iProvider.VmbxCenRepHandler().IsAllowedUserEdit();
        }
    else if ( EVmbxVideoMailboxSupported & aFlags )
        {
        conf = ETrue;
        }
    else
        {
        conf = CVmbxEngineBase::CheckConfiguration(aParams, aFlags);
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVideoEngine::CheckConfiguration: conf%I <=",
                 conf );
    return conf;
    }
// End of file
