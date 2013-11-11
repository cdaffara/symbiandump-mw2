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
* Description:  Implementation of the CVmbxCsVoiceEngine class
*
*/


// INCLUDE FILES
#include <cvoicemailboxentry.h>

#include "vmbxenginebase.h"
#include "vmbxutilities.h"

#include "vmbxlogger.h"
#include "vmbxcenrephandler.h"
#include "vmbxsimhandler.h"
#include "vmbxuiutilities.h"

#include "vmbxcsvoiceengine.h"

// CONSTANTS
_LIT (KVmbxIllegalSimCharacter, "w");

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::CVmbxCsVoiceEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CVmbxCsVoiceEngine::CVmbxCsVoiceEngine( MVmbxResourceProvider& aProvider )
                                : CVmbxEngineBase( aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::CVmbxCsVoiceEngine =>" );
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::CVmbxCsVoiceEngine <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::~CVmbxCsVoiceEngine
// Destructor.
// ----------------------------------------------------------------------------
//
CVmbxCsVoiceEngine::~CVmbxCsVoiceEngine()
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::~CVmbxCsVoiceEngine =>" );
    delete iSimHandler;
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::~CVmbxCsVoiceEngine <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CVmbxCsVoiceEngine* CVmbxCsVoiceEngine::NewL( MVmbxResourceProvider& aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::NewL =>" );
    CVmbxCsVoiceEngine* self = 
            new( ELeave ) CVmbxCsVoiceEngine( aProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::NewL <=" );
    return self;
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::ConstructL =>" );
    TVmbxMemoryLocation storeType( EVmbxPhoneMemory );
    storeType = iProvider.VmbxCenRepHandler().StoreType();
    if ( EVmbxSimMemory == storeType )
        {
        // create SimHandler
        TRAPD( err, iSimHandler = CVmbxSimHandler::NewL() );
        // for avoid warning for compiling
        err = err;
        VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::ConstructL:\
                     Create SimHandler %I" , err);
        }
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::ConstructL <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::GetL
// Gets voice mailbox number
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::GetL( CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::GetL =>" );
    TInt result( KErrNotFound );

    CVoiceMailboxEntry* vmbxEntry = CVoiceMailboxEntry::NewLC();
    // get als line info
    vmbxEntry->SetVmbxAlsLineType( VmbxUtilities::AlsLine() );

    vmbxEntry->SetVoiceMailboxType( EVmbxVoice );
    vmbxEntry->SetServiceId( KVmbxServiceVoice );
    // get store type from CenRep 
    TVmbxMemoryLocation storeType = iProvider.VmbxCenRepHandler().StoreType();

    if ( EVmbxSimMemory ==  storeType && iSimHandler )
        {
        // read from sim if it is available and active
        VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::GetL:\
                        SIM available and active");
        TRAP( result, iSimHandler->GetL( *vmbxEntry ) );

        VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::GetL :Sim res=%d", result );
        TPtrC vmbxNumber( KNullDesC );
        vmbxEntry->GetVmbxNumber( vmbxNumber );

        // if no number from sim(both MBDN file and VMBX file)
        if ( KErrNone != result || !vmbxNumber.Length() )
            {
            VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::GetL:\
                       no number from sim, both MBDN file and VMBX file");
            // Get CS number from phone memory
            result = iProvider.VmbxCenRepHandler().GetVmbxNumber( *vmbxEntry );
            vmbxEntry->SetUsingMemoryLocation( EVmbxPhoneMemory );
            }
        else
            {
            vmbxEntry->SetUsingMemoryLocation( EVmbxSimMemory );
            }
        }
    // EVmbxPhoneMemory or sim not available
    else
        {
        VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::GetL:\
                        EVmbxPhoneMemory or sim not available " );
        // Get CS number from phone memory
        result = iProvider.VmbxCenRepHandler().GetVmbxNumber( *vmbxEntry );
        vmbxEntry->SetUsingMemoryLocation( EVmbxPhoneMemory );
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::GetL: res=%d", result );

    User::LeaveIfError( result );
    CleanupStack::Pop( vmbxEntry );
    aEntry = vmbxEntry;
    vmbxEntry = NULL;
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::GetL <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::SaveL
// Saves voice mailbox number
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::SaveL( const CVoiceMailboxEntry& aEntry,
                                TBool aShowNotesAllowed )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveL =>" );

    TPtrC vmbxNumber( KNullDesC );
    aEntry.GetVmbxNumber( vmbxNumber );
    
    if ( vmbxNumber.Length() 
         && ( !VmbxUtilities::IsValidPhoneNumber( vmbxNumber )
         || VmbxUtilities::IsEmergencyNumber( vmbxNumber ) ) )
        {
        VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveL: number invalid <=" );
        User::Leave( KErrArgument );
        }

    TVoiceMailboxParams params;
    if ( IsWritable( params ) )
        {
        TVmbxMemoryLocation storeType = 
                        iProvider.VmbxCenRepHandler().StoreType();
        VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::SaveL: storeType=%I",
                        storeType );  
        if ( EVmbxSimMemory == storeType && iSimHandler )
            {
            // Save to sim
            SaveEntryToSimL( aEntry, aShowNotesAllowed );
            }
        else
            {
            // Save to cenrep
            SaveEntryToPhoneL( aEntry, aShowNotesAllowed );
            }     
        }
    else
        {
        // sim files not exist, its may need by forcing save, eg, OMA.
        if ( !iSimHandler )
            {
            SaveEntryToPhoneL( aEntry, aShowNotesAllowed );
            VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveL: \
                            sim files not exist then save to phone" );
            }
        else
            {
             VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveL: \
                                sim files unwritable<=" );
            User::Leave( KErrNotSupported );
            }
        }

    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveL <=" );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::SaveProvisionedEntryL
// Saves Provisioned voice mailbox number
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::SaveProvisionedEntryL( 
                        const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveProvisionedEntryL =>" );
    SaveL( aEntry, EFalse );
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveProvisionedEntryL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxCsVoiceEngine::CheckConfiguration
// 
// ---------------------------------------------------------------------------
//
TBool CVmbxCsVoiceEngine::CheckConfiguration( const TVoiceMailboxParams& aParams,
                             const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::CheckConfiguration =>" );
    TBool configuration( EFalse );
    // check thr property whether allow user to edit number 
    if ( EVmbxChangeNbrAllowedOnUi & aFlags )
        {
        configuration = iProvider.VmbxCenRepHandler().IsAllowedUserEdit();
        if ( configuration )
            {
            configuration = IsWritable( aParams );
            }
        }
    else
        {
        configuration = CVmbxEngineBase::CheckConfiguration(aParams, aFlags);
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::CheckConfiguration: conf%I <=",
                 configuration );
    return configuration;
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::SaveEntryToPhone
// Saves number to storage that user selects from the offered list
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::SaveEntryToPhoneL( const CVoiceMailboxEntry& aEntry,
                                            TBool aShowNotesAllowed )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveEntryToPhoneL: =>" );
    TInt result( KErrArgument );

    result = iProvider.VmbxCenRepHandler().Save( aEntry );

    if ( KErrNone == result && aShowNotesAllowed )
        {
        iProvider.VmbxUiUtilities().ShowInformationdNoteL( ESavedToPhoneMemory );
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::SaveEntryToPhoneL: result=%I <=",
                             result );
    User::LeaveIfError( result );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::SaveEntryToSim
// For saving to place that user selects
// ----------------------------------------------------------------------------
//
void CVmbxCsVoiceEngine::SaveEntryToSimL( const CVoiceMailboxEntry& aEntry,
                                          TBool aShowNotesAllowed )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::SaveEntryToSimL: =>" );
    TInt result( KErrGeneral );

    // number containing "w"-character is not allowed to save to sim
    TPtrC ptrNumber( KNullDesC );
    TInt numError = aEntry.GetVmbxNumber( ptrNumber );
    if ( KErrNotFound != ptrNumber.Find( KVmbxIllegalSimCharacter ) )
        {
        result = KErrArgument;
        }

    if ( iSimHandler )
        {
        result = iSimHandler->Save( aEntry );
       if ( KErrNone == result && aShowNotesAllowed )
            {
            iProvider.VmbxUiUtilities().ShowInformationdNoteL( ESavedToSimMemory );
            }
        }

    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::\
                    SaveEntryToSimL: result=%I <=", result );

    User::LeaveIfError( result );
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::IsSimWritable
// ETrue if Sim is allowed to be writable
// ----------------------------------------------------------------------------
TBool CVmbxCsVoiceEngine::IsSimWritable()
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::IsSimWritable: =>" );
    TBool result( EFalse );
    if ( !( iProvider.VmbxCenRepHandler().IsSimReadOnly() ) 
        && iSimHandler && iSimHandler->IsWritable() )
        {
        result = ETrue;
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::IsSimWritable:\
             result=%I <=", result );
    return result;
    }

// ----------------------------------------------------------------------------
// CVmbxCsVoiceEngine::IsWritable
// ETrue if Sim is allowed to be writable
// ----------------------------------------------------------------------------
TBool CVmbxCsVoiceEngine::IsWritable( const TVoiceMailboxParams& /*aParams*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::IsWritable =>" );
    TBool result( EFalse );
    TVmbxMemoryLocation storeType = 
                        iProvider.VmbxCenRepHandler().StoreType();

    if ( EVmbxPhoneMemory == storeType )
        {
        result = ETrue;
        }
    else
        {
        if ( iSimHandler )
            {
            #ifdef __WINS__
                result = ETrue;
            #else
                result = IsSimWritable();
            #endif
            }
        else
            {
            VMBLOGSTRING( "VMBX: CVmbxCsVoiceEngine::IsWritable: \
            sim files not exist then use phone memory" );
            result = ETrue;
            }
        }
    VMBLOGSTRING2( "VMBX: CVmbxCsVoiceEngine::IsWritable:\
             result=%I <=", result );
    return result;
    }

// End of file
