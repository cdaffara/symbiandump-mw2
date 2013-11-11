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
* Description:  Implementation of the CVmbxCenRepHandler class
*
*/


// INCLUDE FILES
#include <centralrepository.h>
#include <cvoicemailboxentry.h>

#include "voicemailboxdomaincrkeys.h"
#include "voicemailboxprivatecrkeys.h"
#include "vmbxlogger.h"
#include "vmbxcenrephandler.h"

// CONSTANTS
const TUint32 KUnknownCenRepKey( 0xffffffff );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::CVmbxCenRepHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxCenRepHandler::CVmbxCenRepHandler()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::CVmbxCenRepHandler <=>" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxCenRepHandler::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::ConstructL =>" );
    iCRSession = CRepository::NewL( KCRUidVoiceMailbox );
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxCenRepHandler* CVmbxCenRepHandler::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::NewL =>" );
    CVmbxCenRepHandler* self = new( ELeave ) CVmbxCenRepHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::~CVmbxCenRepHandler
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxCenRepHandler::~CVmbxCenRepHandler()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::~CVmbxCenRepHandler =>" );
    delete iCRSession;
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::~CVmbxCenRepHandler <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::GetVmbxNumber
// Get saved entry number
// ---------------------------------------------------------------------------
//
TInt CVmbxCenRepHandler::GetVmbxNumber( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::GetVmbxNumber =>" );
    TInt result( KErrNotFound );
    // get right cenrep key
    TUint32 cenRepKey = ResolveKeyByEntry( aEntry );
    if ( KUnknownCenRepKey != cenRepKey )
        {
        HBufC* cenRepEntry = HBufC::New( KVmbxMaxNumberLength );
        TPtr ptr( cenRepEntry->Des() );
        VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::GetVmbxNumber: CenRep Get" );
        result = iCRSession->Get( cenRepKey, ptr );
        // get cenrep value
        if ( KErrNone == result )
            {
            if ( ptr.Length() )
                {
                result = aEntry.SetVmbxNumber( ptr );
                VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::GetVmbxNumber:\
                 Set Entry" ); 
                }
            else
                {
                result = KErrNotFound;
                }
            }
        delete cenRepEntry;
        cenRepEntry = NULL;
        }
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::GetVmbxNumber: result%d <=",
                                 result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::Save
// Saves the mailbox number
// ---------------------------------------------------------------------------
//
TInt CVmbxCenRepHandler::Save( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::Save =>" );
    TInt result( KErrNone );
    // get right cenrep key
    TUint32 cenRepKey = ResolveKeyByEntry( aEntry );

    if ( KUnknownCenRepKey != cenRepKey )
        {
        // set cenrep value
        TPtrC vmbxNumber( KNullDesC );
        aEntry.GetVmbxNumber( vmbxNumber );
        result = iCRSession->Set( cenRepKey, vmbxNumber );
        }
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::Save result=%d <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::ResolveKeyByEntry
// Resolve CenRep key by entry
// ---------------------------------------------------------------------------
//
TUint32 CVmbxCenRepHandler::ResolveKeyByEntry(
                                       const CVoiceMailboxEntry& aEntry  )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::ResolveKeyByEntry =>" );
    TVmbxAlsLineType alsLine = aEntry.VmbxAlsLineType();
    TVmbxType entryType = aEntry.VoiceMailboxType();

    TUint32 cenRepKey( KUnknownCenRepKey );
    switch ( entryType )
        {
        // Voice mailbox
        case EVmbxVoice:
            {
            cenRepKey = VoiceCenRepKey( alsLine );
            break;
            }
        // Video mailbox
        case EVmbxVideo:
            {
            cenRepKey = VideoCenRepKey( alsLine );
            break;
            }
        default:
            break;
        }
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::ResolveKeyByEntry <=" );
    return cenRepKey;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::VoiceCenRepKey
// Voice mailbox cenntral repository key value
// ---------------------------------------------------------------------------
//
TUint32 CVmbxCenRepHandler::VoiceCenRepKey(
                                       const TVmbxAlsLineType& aLine )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VoiceCenRepKey =>" );
    TUint32 cenRepKey( KUnknownCenRepKey );
    if ( EVmbxAlsLine2 == aLine )
        {
        cenRepKey = KVmbxNumberLineAlternate;
        VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VoiceCenRepKey:Line2" );
        }
    else
        {
        cenRepKey = KVmbxNumberLinePrimary;
        VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VoiceCenRepKey:Line1" );
        }
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VoiceCenRepKey <=" );
    return cenRepKey;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::VideoCenRepKey
// Video mailbox cenntral repository key value
// ---------------------------------------------------------------------------
//
TUint32 CVmbxCenRepHandler::VideoCenRepKey(
                                       const TVmbxAlsLineType& aLine )
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoCenRepKey =>" );
    TUint32 cenRepKey( KUnknownCenRepKey );
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::VideoCenRepKey: aLine = %d",
         aLine );
    if ( EVmbxAlsLine2 == aLine )
        {
        cenRepKey = KVmbxVideoNmbrLineAlternate;
        VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoCenRepKey:Line2" );
        }
    else
        {
        cenRepKey = KVmbxVideoNmbrLinePrimary;
        VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoCenRepKey:Line1" );
        }
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoCenRepKey <=" );
    return cenRepKey;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::StoreType
// Fetches active store type
// ---------------------------------------------------------------------------
//
TVmbxMemoryLocation CVmbxCenRepHandler::StoreType()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::StoreType =>" );
    TVmbxMemoryLocation memoryLocation( EVmbxPhoneMemory );
    TInt location;
    TInt result = iCRSession->Get( KVmbxUsesSimMemory, location );
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::StoreType: result = %d",
             result );

    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::StoreType: location = %d<=",
             location );

    if ( KErrNone == result && EVmbxSimMemory == location )
        {
        memoryLocation = EVmbxSimMemory;
        }
#ifdef __WINS__
    memoryLocation = EVmbxPhoneMemory;
#endif
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::StoreType: memory = %d<=",
             memoryLocation );
    return memoryLocation;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::VideoSupported
// Returns ETrue if supported
// ---------------------------------------------------------------------------
//
TBool CVmbxCenRepHandler::VideoSupported()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoSupported =>" );
    TBool supported( EFalse);

    TInt res = iCRSession->Get( KVmbxVideoMbxSupport, supported );

    if ( KErrNone != res )
        {
        supported = EFalse;
        }
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::VideoSupported: result %d",
         supported );
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::VideoSupported <=" );
    return supported;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::IsSimReadOnly
// ETrue if CenRep setting Sim read only
// ---------------------------------------------------------------------------
//
TBool CVmbxCenRepHandler::IsSimReadOnly()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::IsSimReadOnly =>" );
    TBool result( EFalse );
    TInt  res = iCRSession->Get( KVmbxNoSimWriting, result );

    if ( KErrNone != res )
        {
        result = EFalse;
        }
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::IsSimReadOnly: result %I <=",
         result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxCenRepHandler::IsAllowedUserEdit
// ETrue if number Allowed to edit
// ---------------------------------------------------------------------------
//
TBool CVmbxCenRepHandler::IsAllowedUserEdit()
    {
    VMBLOGSTRING( "VMBX: CVmbxCenRepHandler::IsAllowedUserEdit =>" );
    TBool result( EFalse );
    TInt temp( 0 );
    TInt  res = iCRSession->Get( KVmbxUiCaps, temp );
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::IsAllowedUserEdit: res %d",
         res );

    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::IsAllowedUserEdit: temp %d",
         temp );

    if ( ( KErrNone == res ) && !( KVmbxUiUserEditNotAllowed & temp ) )
        {
        result = ETrue;
        }
    VMBLOGSTRING2( "VMBX: CVmbxCenRepHandler::IsAllowedUserEdit: result %d <=",
         result );
    return result;
    }

// End of file
