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
* Description:  Implementation of the CVmbxEngineBase class
*
*/


// INCLUDE FILES
#include <cvoicemailboxentry.h>

#include "vmbxlogger.h"
#include "vmbxuiutilities.h"
#include "vmbxenginebase.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxEngineBase::CVmbxEngineBase
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxEngineBase::CVmbxEngineBase( MVmbxResourceProvider& aProvider ) 
                          : iProvider( aProvider ),
                            iVmbxServiceId( KVmbxServiceIdNone )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::CVmbxEngineBase =>" );
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::CVmbxEngineBase <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxEngineBase::~CVmbxEngineBase
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxEngineBase::~CVmbxEngineBase()
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::~CVmbxEngineBase =>" );
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::~CVmbxEngineBase <=" );
    }

// ----------------------------------------------------------------------------
//  CVmbxEngineBase::GetServiceIds()
// ----------------------------------------------------------------------------
//
void CVmbxEngineBase::GetServiceIdsL( RIdArray& /*aProfileIds*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::GetServiceIdsL <=>" );
    User::Leave( KErrNotFound );
    }

// Commented out because branding is not supported yet.
// ----------------------------------------------------------------------------
//  CVmbxEngineBase::GetVmbxImageL()
// ----------------------------------------------------------------------------
//
/* CGulIcon* CVmbxEngineBase::GetVmbxImageL( const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::GetVmbxImageL =>" );
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::GetVmbxImageL <=" );
    return iProvider.VmbxUiUtilities().GetVmbxImageL( aParams );
    } */

// ----------------------------------------------------------------------------
//  CVmbxEngineBase::SetCurrentServiceId()
// ----------------------------------------------------------------------------
//
void CVmbxEngineBase::SetCurrentServiceId( const TServiceId& aVmbxServiceId )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::SetCurrentServiceId =>" );
    iVmbxServiceId = aVmbxServiceId;
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::SetCurrentServiceId <=" );
    }

// ----------------------------------------------------------------------------
//  CVmbxCsVideoEngine::CheckConfiguration()
// ----------------------------------------------------------------------------
//
TBool CVmbxEngineBase::CheckConfiguration( 
            const TVoiceMailboxParams& /*aParams*/, const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::CheckConfiguration =>" );
    TBool conf( EFalse );
    if ( EVmbxChangeNbrAllowedOnUi & aFlags )
        {
        // NOT suppported as default
        }
    else if ( EVmbxVideoMailboxSupported & aFlags )
        {
        // NOT suppported as default
        }
    else
        {
        // default
        }
    VMBLOGSTRING2( "VMBX: CVmbxEngineBase::CheckConfiguration: conf%I <=",
                 conf );
    return conf;
    }

// ---------------------------------------------------------------------------
// CVmbxEngineBase::QueryDefineNumberL
//
// ---------------------------------------------------------------------------
//
void CVmbxEngineBase::QueryDefineNumberL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::QueryDefineNumberL =>" );
    HBufC* defineNumber = HBufC::NewLC( KVmbxMaxNumberLength );
    TPtr ptrNumber( defineNumber->Des() );

    iProvider.VmbxUiUtilities().ShowQueryL( aEntry.VoiceMailboxType(),
                                             EVmbxDefineMode, ptrNumber );
    TInt err = aEntry.SetVmbxNumber( ptrNumber );
    CleanupStack::PopAndDestroy( defineNumber );
    VMBLOGSTRING2( "VMBX: CVmbxEngineBase::QueryDefineNumberL, err%I", err );
    User::LeaveIfError( err );
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::QueryDefineNumberL <= ");
    }

// ----------------------------------------------------------------------------
// CVmbxEngineBase::QueryChangeNumberL
//
// ----------------------------------------------------------------------------
//
void CVmbxEngineBase::QueryChangeNumberL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::QueryChangeNumberL =>" );
    TInt err( KErrNone );
    TPtrC vmbxNumber( KNullDesC );
    err = aEntry.GetVmbxNumber( vmbxNumber );

    if ( KErrNone == err )
        {
        HBufC* changeNumber = HBufC::NewLC( KVmbxMaxNumberLength );
        TPtr tempNumber( changeNumber->Des() );
        tempNumber.Copy( vmbxNumber );
        iProvider.VmbxUiUtilities().ShowQueryL( aEntry.VoiceMailboxType(),
                                   EVmbxChangeMode, tempNumber );
        err = aEntry.SetVmbxNumber( tempNumber );
        CleanupStack::PopAndDestroy( changeNumber );
        }
    VMBLOGSTRING2( "VMBX: CVmbxEngineBase::QueryChangeNumberL, err%I", err );
    User::LeaveIfError( err );
    VMBLOGSTRING( "VMBX: CVmbxEngineBase::QueryChangeNumberL <=" );
    }

// End of file
