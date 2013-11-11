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
* Description:  Implementation of the VmbxUtilities class
*
*/


// INCLUDE FILES
#include <cvoicemailboxentry.h>

#include "voicemailboxdefsinternal.h"
#include "vmbxutilities.h"
#include "vmbxlogger.h"
#include "vmbxuiutilities.h"
#include "vmbxuihandler.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// VmbxUtilities::CVmbxUiUtilities
// C++ default constructor can NOT contain any code
// ---------------------------------------------------------------------------
CVmbxUiUtilities::CVmbxUiUtilities()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::CVmbxUiUtilities <=>" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::~CVmbxUiUtilities
// Destructor
// ---------------------------------------------------------------------------
CVmbxUiUtilities::~CVmbxUiUtilities()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::~CVmbxUiUtilities =>" );
    delete iUiHandler;
    //If there are still dialog is running, The dialogs should be dismissed.
    //The leave will be ignored as the CVmbxUiUtilities is distructing
    TRAP_IGNORE( DismissDialogL() );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::~CVmbxUiUtilities <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxUiUtilities* CVmbxUiUtilities::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::NewL =>" );
    CVmbxUiUtilities* vmbx = new( ELeave )CVmbxUiUtilities();
    CleanupStack::PushL( vmbx );
    vmbx->ConstructL();
    CleanupStack::Pop( vmbx );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::NewL <=" );
    return vmbx;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxImpl::ConstructL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ConstructL =>" );
    // create resource handler
    TRAP_IGNORE(iUiHandler = CVmbxUiHandler::NewL());
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowQuery
// Show query
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowQueryL( const TVmbxType& aType,
        const TVmbxQueryMode & aMode, TDes& aNumber )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryL =>" );
    VMBLOGSTRING3( "VMBX: CVmbxUiUtilities::ShowQueryL: aMode=%d aNumber=%S ",
                    aMode, &aNumber );
    if ( !iUiHandler ) 
        {
        VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryL no qt env Exit" )
        User::Leave(KErrNotSupported);
        }
    
    // Show "define mailbox number" notification at define mode
    if (EVmbxDefineMode == aMode) {
        switch( aType ) {
        case EVmbxVoice:
            ShowInformationdNoteL(EDefineVoiceNumber);
            break;
        case EVmbxVideo:
            ShowInformationdNoteL(EDefineVideoNumber);
            break;
        default:
            break;
        }
    }
    
    TInt result( KErrNone );
    FOREVER
        {
        ShowQueryDialogL( aType, aMode, aNumber, result );
        if ( KErrCancel == result )
            {
            break;
            }

        if ( KErrNone == result )
            {
            if ( aNumber.Length() )
                {
                // Convert back to western digits
                // check the given number here (emergency numbers not allowed)
                if ( VmbxUtilities::IsValidPhoneNumber( aNumber )
                     && !VmbxUtilities::IsEmergencyNumber( aNumber ) )
                    {
                    break;
                    }// valid number
                else
                    {
                    //invalid dialog
                    ShowInvalidWarningDialogL();
                    continue;
                    }// invalid number
                }
            else
                {
                result = KErrNone;
                break;
                }// number NULL
            }
        }// End FOREVER
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryL: result=%I ", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowInformationdNote
//
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowInformationdNoteL(const TVmbxNoteType aType )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInformationdNoteL =>" );
    if ( iUiHandler ) 
        {
        iUiHandler->ShowInformationdNoteL( aType );
        }
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInformationdNoteL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowDefineSelectionDialogL
// Show define number in selection list
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowDefineSelectionDialogL( TVmbxType& aType,
                                                         TInt& aResult )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL =>" );
    if (!iUiHandler ) 
        {
        VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL\
            no qt env Exit" )
        User::Leave(KErrNotSupported);
        }
    iUiHandler->ShowDefineSelectionDialogL( aType, aResult );
    VMBLOGSTRING3( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL: \
                     aType=%I, aResult=%I <=", aType, aResult );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowDefineSelectionDialogL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowCallSelectionDialogL
// Show call number in selection list
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowCallSelectionDialogL(
                    const RPointerArray<CVoiceMailboxEntry>& aArray,
                    TVoiceMailboxParams& aParams,
                    TInt& aResult )
    {
    VMBLOGSTRING("VMBX: CVmbxUiUtilities::ShowCallSelectionDialogL  =>");
    if (!iUiHandler) 
        {
        VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowCallSelectionDialogL\
            no qt env Exit" )
        User::Leave(KErrNotSupported);
        }
    if ( aArray.Count() < 1 )
        {
        User::Leave( KErrArgument );
        }
    iUiHandler->ShowCallSelectionDialogL( aArray, aParams, aResult );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowCallSelectionDialogL <=" );
    }

// --------------------------------------------------------------------------
// CVmbxUiUtilities::ShowNotAllowedEditingDialogL
// --------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowNotAllowedEditingDialogL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowNotAllowedEditingDialogL =>" );
    // Information user can't edit.
    if ( iUiHandler) 
        {
        iUiHandler->ShowInformationdNoteL( ENotAllowUserEditing );
        }
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowNotAllowedEditingDialogL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowInvalidWarningDialogL
//
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowInvalidWarningDialogL()
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInvalidWarningDialogL =>" );
    if ( iUiHandler) 
        {
        iUiHandler->ShowInformationdNoteL( EInvalidNumber );
        }
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowInvalidWarningDialogL <=" );
    }

// ---------------------------------------------------------------------------
// VmbxUtilities::DismissDialogL
// Tries to close all open dialogs
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::DismissDialogL()
    {
    VMBLOGSTRING( 
    "VMBX: CVmbxUiUtilities::DismissDialogL EMPTY IMPLEMENTATION!=>" );
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::DismissDialogL <=" );
    }

// Commented out because branding is not supported yet.
// ---------------------------------------------------------------------------
// CVmbxUiUtilities::GetVmbxImageL
//
// ---------------------------------------------------------------------------
/*
CGulIcon* CVmbxUiUtilities::GetVmbxImageL(
                        const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::GetVmbxImageL" );
*/

// ---------------------------------------------------------------------------
// VmbxUtilities::ShowQueryDialogL
// Show query dialog
// ---------------------------------------------------------------------------
//
void CVmbxUiUtilities::ShowQueryDialogL( const TVmbxType& aType,
                                         const TVmbxQueryMode& /*aMode*/,
                                         TDes& aNumber, TInt& aResult )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL =>" );

    if( EVmbxVideo != aType && EVmbxVoice != aType  )
        {
        VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: \
                            User::Leave( KErrNotSupported )" );
        User::Leave( KErrArgument );
        }
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: in\
    aNumber = %S", &aNumber );
    // to show dialog via qt part
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::ShowQueryDialogL to show qt" );
    iUiHandler->ShowVmbxQueryDialogL( aType, aNumber, aResult );
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: out\
    aNumber = %S", &aNumber );
    VMBLOGSTRING2( "VMBX: CVmbxUiUtilities::ShowQueryDialogL: aResult=%I <=",
     aResult );
    }

// -----------------------------------------------------------------------------
// CVmbxUiUtilities::FetchNumberFromPhonebook2L
// Create and show Phonebook's Single Fetch Dialog
// -----------------------------------------------------------------------------
//
void CVmbxUiUtilities::FetchNumberFromPhonebook2L( 
                                          TDes& /*aPhoneNumber*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::FetchNumberFromPhonebook2L =>" );
    /* TODO: Later remove this or replace with some Qt Phonebook API
        */
    VMBLOGSTRING( "VMBX: CVmbxUiUtilities::FetchNumberFromPhonebook2L <=" );
    }

// End of file
