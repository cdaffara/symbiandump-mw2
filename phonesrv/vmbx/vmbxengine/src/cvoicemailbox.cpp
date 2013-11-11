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
* Description:  Implementation of the CVoiceMailbox class
*
*/

// INCLUDE FILES
#include <exception>

#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include "voicemailboximpl.h"
#include "vmbxlogger.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TVoiceMailboxParams::Version()
    {
    return iVersion;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TVoiceMailboxParams::TVoiceMailboxParams():
                                        iType( EVmbxNone ),
                                        iServiceId( KVmbxServiceIdNone ),
                                        iLineType( EVmbxAlsLineDefault )
    {
    VMBLOGSTRING( "VMBX: TVoiceMailboxParams: default cnstructor =>" );
    VMBLOGSTRING( "VMBX: TVoiceMailboxParams: default cnstructor<=" );
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
EXPORT_C TVoiceMailboxParams::TVoiceMailboxParams( 
                        const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: TVoiceMailboxParams: Entry cnstructor =>" );
    iType = aEntry.VoiceMailboxType();
    iServiceId = aEntry.ServiceId();
    iLineType = aEntry.VmbxAlsLineType();
    VMBLOGSTRING( "VMBX: TVoiceMailboxParams: Entry cnstructor <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::CVoiceMailboxImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVoiceMailbox::CVoiceMailbox()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::CVoiceMailbox <=>" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVoiceMailbox::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::ConstructL =>" );
    iBody =CVoiceMailboxImpl::NewL();
    VMBLOGSTRING( "VMBX: CVoiceMailbox::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CVoiceMailbox* CVoiceMailbox::NewL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NewL =>" );
    CVoiceMailbox* self = CVoiceMailbox::NewLC();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CVoiceMailbox* CVoiceMailbox::NewLC()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NewLC =>" );
    CVoiceMailbox* self = new( ELeave ) CVoiceMailbox;
    CleanupStack::PushL( self );
    self->ConstructL();
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NewLC <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::~CVoiceMailbox
// Destructor
// ---------------------------------------------------------------------------
//
CVoiceMailbox::~CVoiceMailbox()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::~CVoiceMailbox =>" );
    delete iBody;
    VMBLOGSTRING( "VMBX: CVoiceMailbox::~CVoiceMailbox <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::GetStoredEntry
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::GetStoredEntry( 
    const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::GetStoredEntry <=>" );
    return iBody->GetStoredEntry( aParams, aEntry );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::SaveEntry
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::SaveEntry( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::SaveEntry <=>" );
    return iBody->SaveEntry( aEntry );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryNewEntry
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::QueryNewEntry(
        const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::QueryNewEntry <=>" );
    return iBody->QueryNewEntry( aParams, aEntry );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryChangeEntry
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::QueryChangeEntry( 
        const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::QueryChangeEntry <=>" );
    return iBody->QueryChangeEntry( aParams, aEntry );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryVmbxType
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::QueryVmbxType(
                            TVoiceMailboxParams& aParams )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::QueryVmbxType <=>" );
    return iBody->QueryVmbxType( aParams );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeL
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailbox::NotifyVmbxNumberChangeL(
    MVoiceMailboxObserver& aObserver, const TBool aNotifyOnActiveLineOnly )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NotifyVmbxNumberChangeL =>" );
    iBody->NotifyVmbxNumberChangeL( aObserver,
                                             aNotifyOnActiveLineOnly);
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NotifyVmbxNumberChangeL <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeCancel
//
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailbox::NotifyVmbxNumberChangeCancel()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NotifyVmbxNumberChangeCancel =>" );
    iBody->NotifyVmbxNumberChangeCancel();
    VMBLOGSTRING( "VMBX: CVoiceMailbox::NotifyVmbxNumberChangeCancel <=" );
    }

// ---------------------------------------------------------------------------
//  CVoiceMailbox::CheckConfiguration
//
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CVoiceMailbox::CheckConfiguration(
    const TVoiceMailboxParams& aParams, const TInt aFlags )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::CheckConfiguration <=>" );
    return iBody->CheckConfiguration( aParams, aFlags );
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::GetServiceIds
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::GetServiceIds( RIdArray& aProfileIds ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::GetServiceIds <=>" );
    return iBody->GetServiceIds( aProfileIds );
    }

// Commented out because branding not yet supported.
// ---------------------------------------------------------------------------
// CVoiceMailbox::GetVmbxImage
//
// ---------------------------------------------------------------------------
/* EXPORT_C CGulIcon* CVoiceMailbox::GetVmbxImage(
                        const TVoiceMailboxParams& aParams ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::GetVmbxImageL <=>" );
    return iBody->GetVmbxImage( aParams );
    }
 */
// ---------------------------------------------------------------------------
// CVoiceMailbox::GetVmbxImages
//
// ---------------------------------------------------------------------------
//
/* EXPORT_C CArrayPtr<CGulIcon>* CVoiceMailbox::GetVmbxImages(
                            RArray<TVoiceMailboxParams>& aParams ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::GetVmbxImages <=>" );
    return iBody->GetVmbxImages( aParams );
    }
 */
// ---------------------------------------------------------------------------
// CVoiceMailbox::SaveProvisionedEntry
//
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailbox::SaveProvisionedEntry( 
                                const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailbox::SaveProvisionedEntry <=>" );
    return iBody->SaveProvisionedEntry( aEntry );
    }

// End of file
