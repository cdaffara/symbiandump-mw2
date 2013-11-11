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
* Description:  Implementation of the CVmbxVoIpEngine class
*
*/

// INCLUDE FILES
#include <fbs.h>
#include <cvoicemailboxentry.h>

#include "vmbxenginebase.h"
#include "vmbxlogger.h"
#include "vmbxuiutilities.h"
#include "vmbxutilities.h"
#include "vmspshandler.h"
#include "voicemailboxdefsinternal.h"
#include "vmbxvoipengine.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::CVmbxVoIpEngine
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxVoIpEngine::CVmbxVoIpEngine( MVmbxResourceProvider& aProvider )
                                : CVmbxEngineBase( aProvider )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::CVmbxVoIpEngine =>" );
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::CVmbxVoIpEngine <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::~CVmbxVoIpEngine
// Destructor.
// ---------------------------------------------------------------------------
//
CVmbxVoIpEngine::~CVmbxVoIpEngine()
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::~CVmbxVoIpEngine =>" );
    delete iVmSpsHandler;
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::~CVmbxVoIpEngine <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVmbxVoIpEngine* CVmbxVoIpEngine::NewL( MVmbxResourceProvider& aProvider)
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::NewL =>" );
    if ( !VmbxUtilities::VoIPSupported() )
        {
        User::Leave( KErrNotSupported );
        }

    CVmbxVoIpEngine* self = new( ELeave ) CVmbxVoIpEngine( aProvider );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::ConstructL =>" );
    if ( VmbxUtilities::VoIPSupported() )
        {
        iVmSpsHandler = CVmSPSHandler::NewL();
        }

    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::GetL
// Gets voice mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::GetL( CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetL =>" );
    TInt result( KErrNone );
    CVoiceMailboxEntry* vmbxEntry = CVoiceMailboxEntry::NewLC();

    vmbxEntry->SetVoiceMailboxType( EVmbxVoip );
    vmbxEntry->SetServiceId( iVmbxServiceId );

    HBufC8* brandId = HBufC8::NewLC( KSPMaxDesLength );
    TPtr8 ptr( brandId->Des() );
    BrandIdL( iVmbxServiceId, ptr );
    // set brand id
    result = vmbxEntry->SetBrandId( ptr );
    CleanupStack::PopAndDestroy( brandId );
    if ( KErrNone == result )
        {
        TVmbxServiceInfo serviceInfo;
        serviceInfo.iServiceId = iVmbxServiceId;
        result = GetServiceInfo( serviceInfo );
        if ( KErrNone == result )
            {
            // set voip name
            result = vmbxEntry->SetVmbxName( serviceInfo.iName );
            if ( KErrNone == result )
                {
                // set voip address
                result = vmbxEntry->SetVmbxNumber( serviceInfo.iAddress );
                }
            }
        }
    VMBLOGSTRING2( "VMBX: CVmbxVoIpEngine::GetL: result=%I", result );
    User::LeaveIfError( result );
    CleanupStack::Pop( vmbxEntry );
    aEntry = vmbxEntry;
    vmbxEntry = NULL;
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::SaveL
// Saves voice mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::SaveL( const CVoiceMailboxEntry& /*aEntry*/,
                             TBool /*aShowNotesAllowed*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::SaveL <=>" );
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::SaveProvisionedEntryL
// Saves Provisioned voice mailbox number
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::SaveProvisionedEntryL( 
                                const CVoiceMailboxEntry& /*aEntry*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::SaveProvisionedEntryL <=>" );
    User::Leave( KErrNotSupported );
    }

// ----------------------------------------------------------------------------
//  CVmbxVoIpEngine::CheckConfiguration()
// ----------------------------------------------------------------------------
//
TBool CVmbxVoIpEngine::CheckConfiguration( 
        const TVoiceMailboxParams& aParams, const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::CheckConfiguration: =>" );
    TBool result( EFalse );
    result = CVmbxEngineBase::CheckConfiguration(aParams, aFlags);
    VMBLOGSTRING2( "VMBX: CVmbxVoIpEngine::CheckConfiguration: result%I <=",
     result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::QueryDefineNumberL
//
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::QueryDefineNumberL( CVoiceMailboxEntry& /*aEntry*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::QueryDefineNumberL <=>" );
    User::Leave( KErrNotSupported );
    }

// ----------------------------------------------------------------------------
// CVmbxEngineBase::QueryChangeNumberL
//
// ----------------------------------------------------------------------------
//
void CVmbxVoIpEngine::QueryChangeNumberL( CVoiceMailboxEntry& /*aEntry*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::QueryChangeNumberL <=>" );
    User::Leave( KErrNotSupported );        
    }

// Commented out because branding is not supported yet.
// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::GetVmbxImageL
// 
// ---------------------------------------------------------------------------
//
/* CGulIcon* CVmbxVoIpEngine::GetVmbxImageL( const TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetVmbxImageL =>" );
    CGulIcon* guiIcon( NULL );
    if ( IsVoIPProfilesFound() )
        {
        TBuf8<KSPMaxDesLength> brandId( KNullDesC8 );
        BrandIdL(aParams.iServiceId, brandId );

        CFbsBitmap* brandedBitmap( NULL );
        CFbsBitmap* brandedBitmapMask( NULL );  
        TRAPD( err, GetBrandedIconL( 
                        brandId, brandedBitmap, brandedBitmapMask ) );

        if ( KErrNotFound == err )
            {
            VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetVmbxImageL: \
                                                 no found profiles<= " );
            guiIcon = iProvider.VmbxUiUtilities().GetVmbxImageL( aParams );
            }
        else if ( KErrNone == err )
            {
            guiIcon = 
                       CGulIcon::NewL( brandedBitmap, brandedBitmapMask );
            VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetVmbxImageL: \
                            found profiles" );
            }
        }
    else
        {
        VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetVmbxImageL: use default " );
        guiIcon = iProvider.VmbxUiUtilities().GetVmbxImageL( aParams );
        }
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetVmbxImageL <=" );
    return guiIcon; 
    } */

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::IsVoIPProfilesFound
// 
// ---------------------------------------------------------------------------
//
TBool CVmbxVoIpEngine::IsVoIPProfilesFound()
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::IsVoIPProfilesFound <=>" );
    return iVmSpsHandler->IsVoIPProfilesFound();
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::GetServiceIds
// 
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::GetServiceIdsL( RIdArray& aProfileIds )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetServiceIds: =>" );
    if ( VmbxUtilities::VoIPSupported() && IsVoIPProfilesFound() )
        {
        iVmSpsHandler->GetServiceIdsL( aProfileIds );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetServiceIds: <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::GetBrandedIconL
// 
// ---------------------------------------------------------------------------
//
/* void CVmbxVoIpEngine::GetBrandedIconL( const TDesC8& aBrandingId,
    CFbsBitmap*& aBrandedBitmap, CFbsBitmap*& aBrandedBitmapMask )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetBrandedIconL: =>" );
    
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetBrandedIconL: <=" );
    }
 */
// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::BrandIdL
// 
// ---------------------------------------------------------------------------
//
void CVmbxVoIpEngine::BrandIdL( TInt aServiceId, TDes8& aBrandId )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::BrandIdL: =>" );

    if ( VmbxUtilities::VoIPSupported() )
        {
        iVmSpsHandler->BrandIdL( aServiceId, aBrandId );
        }

    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::BrandIdL: <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxVoIpEngine::GetServiceInfo
// 
// ---------------------------------------------------------------------------
//
TInt CVmbxVoIpEngine::GetServiceInfo( TVmbxServiceInfo& aServiceInfo )
    {
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetServiceInfo: =>" );
    TInt ret( KErrNotFound );
    if ( VmbxUtilities::VoIPSupported() )
        {
        ret = iVmSpsHandler->GetServiceInfo( aServiceInfo );
        }
    VMBLOGSTRING( "VMBX: CVmbxVoIpEngine::GetServiceInfo: <=" );
    return ret;
    }

// End of file
