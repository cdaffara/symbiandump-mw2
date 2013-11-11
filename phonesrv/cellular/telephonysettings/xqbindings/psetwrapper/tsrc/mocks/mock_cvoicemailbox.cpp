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
* Description:
*
*/

#include <QDebug>
#include <smcmockclassincludes.h>
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
#include <mvoicemailboxobserver.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TVoiceMailboxParams::TVoiceMailboxParams
// -----------------------------------------------------------------------------
//
TVoiceMailboxParams::TVoiceMailboxParams(  )
    {
    
    }


// -----------------------------------------------------------------------------
// TVoiceMailboxParams::Version
// -----------------------------------------------------------------------------
//
TInt TVoiceMailboxParams::Version(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// TVoiceMailboxParams::TVoiceMailboxParams
// -----------------------------------------------------------------------------
//
TVoiceMailboxParams::TVoiceMailboxParams( 
        const CVoiceMailboxEntry & aEntry )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::CVoiceMailbox
// -----------------------------------------------------------------------------
//
CVoiceMailbox::CVoiceMailbox(  )
    //:
    //CBase( /**/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::ConstructL
// -----------------------------------------------------------------------------
//
void CVoiceMailbox::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::NewL
// -----------------------------------------------------------------------------
//
CVoiceMailbox * CVoiceMailbox::NewL(  )
    {
    SMC_MOCK_NEWL0( CVoiceMailbox * )

    CVoiceMailbox* self = new( ELeave ) CVoiceMailbox;
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::NewLC
// -----------------------------------------------------------------------------
//
CVoiceMailbox * CVoiceMailbox::NewLC(  )
    {
    SMC_MOCK_NEWLC0( CVoiceMailbox * )

    CVoiceMailbox* self = new( ELeave ) CVoiceMailbox;
    CleanupStack::PushL( self );
    //self->ConstructL()
    return self;
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::~CVoiceMailbox
// -----------------------------------------------------------------------------
//
CVoiceMailbox::~CVoiceMailbox(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::GetStoredEntry
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::GetStoredEntry( 
        const TVoiceMailboxParams & aParams,
        CVoiceMailboxEntry * & aEntry ) const
    {

    CVoiceMailboxEntry* vmbxEntry = NULL;
    vmbxEntry = CVoiceMailboxEntry::NewL();
    vmbxEntry->SetServiceId( KVmbxServiceVoice );
    vmbxEntry->SetVoiceMailboxType( EVmbxVoice );
    vmbxEntry->SetVmbxAlsLineType( EVmbxAlsLine1 );
    aEntry = vmbxEntry;
    SMC_MOCK_METHOD2( TInt, const TVoiceMailboxParams &, aParams, 
        CVoiceMailboxEntry *, aEntry )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::SaveEntry
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::SaveEntry( 
        const CVoiceMailboxEntry & aEntry )
    {
    SMC_MOCK_METHOD1( TInt, const CVoiceMailboxEntry &, aEntry )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::QueryNewEntry
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryNewEntry( 
        const TVoiceMailboxParams & aParams,
        CVoiceMailboxEntry * & aEntry )
    {
    SMC_MOCK_METHOD2( TInt, const TVoiceMailboxParams &, aParams, 
        CVoiceMailboxEntry *, aEntry )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::QueryChangeEntry
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryChangeEntry( 
        const TVoiceMailboxParams & aParams,
        CVoiceMailboxEntry * & aEntry )
    {
    SMC_MOCK_METHOD2( TInt, const TVoiceMailboxParams &, aParams, 
        CVoiceMailboxEntry *, aEntry )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::QueryVmbxType
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryVmbxType( 
        TVoiceMailboxParams & aParams )
    {
    SMC_MOCK_METHOD1( TInt, TVoiceMailboxParams &, aParams )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeL
// -----------------------------------------------------------------------------
//
void CVoiceMailbox::NotifyVmbxNumberChangeL( 
        MVoiceMailboxObserver & aObserver,
        const TBool aNotifyOnActiveLineOnly )
    {
    SMC_MOCK_METHOD2( void, MVoiceMailboxObserver &, aObserver, 
        const TBool, aNotifyOnActiveLineOnly )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeCancel
// -----------------------------------------------------------------------------
//
void CVoiceMailbox::NotifyVmbxNumberChangeCancel(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::CheckConfiguration
// -----------------------------------------------------------------------------
//
TBool CVoiceMailbox::CheckConfiguration( 
        const TVoiceMailboxParams & aParams,
        const TInt aFlags )
    {
    SMC_MOCK_METHOD2( TBool, const TVoiceMailboxParams &, aParams, 
        const TInt, aFlags )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::GetServiceIds
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::GetServiceIds( 
        RIdArray & aProfileIds ) const
    {
    SMC_MOCK_METHOD1( TInt, RIdArray &, aProfileIds )
    }


// -----------------------------------------------------------------------------
// CVoiceMailbox::SaveProvisionedEntry
// -----------------------------------------------------------------------------
//
TInt CVoiceMailbox::SaveProvisionedEntry( 
        const CVoiceMailboxEntry & aEntry )
    {
    SMC_MOCK_METHOD1( TInt, const CVoiceMailboxEntry &, aEntry )
    }


