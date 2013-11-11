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
#include "cvoicemailboxentry.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::CVoiceMailboxEntry
// -----------------------------------------------------------------------------
//
CVoiceMailboxEntry::CVoiceMailboxEntry(  )
    //:
    //CBase( /**/ )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::ConstructL
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::ConstructL(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::NewL
// -----------------------------------------------------------------------------
//
CVoiceMailboxEntry * CVoiceMailboxEntry::NewL(  )
    {
    SMC_MOCK_NEWL0( CVoiceMailboxEntry * )

    CVoiceMailboxEntry* self = new( ELeave ) CVoiceMailboxEntry;
    CleanupStack::PushL( self );
    //self->ConstructL()
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::NewLC
// -----------------------------------------------------------------------------
//
CVoiceMailboxEntry * CVoiceMailboxEntry::NewLC(  )
    {
    SMC_MOCK_NEWLC0( CVoiceMailboxEntry * )

    CVoiceMailboxEntry* self = new( ELeave ) CVoiceMailboxEntry;
    CleanupStack::PushL( self );
    //self->ConstructL()
    return self;
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::~CVoiceMailboxEntry
// -----------------------------------------------------------------------------
//
CVoiceMailboxEntry::~CVoiceMailboxEntry(  )
    {
    
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::ServiceId
// -----------------------------------------------------------------------------
//
TServiceId CVoiceMailboxEntry::ServiceId(  ) const
    {
    SMC_MOCK_METHOD0( TServiceId )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetServiceId
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetServiceId( 
        const TServiceId & aVmbxServiceId )
    {
    SMC_MOCK_METHOD1( void, const TServiceId &, aVmbxServiceId )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::VoiceMailboxType
// -----------------------------------------------------------------------------
//
TVmbxType CVoiceMailboxEntry::VoiceMailboxType(  ) const
    {
    SMC_MOCK_METHOD0( TVmbxType )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVoiceMailboxType
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetVoiceMailboxType( 
        const TVmbxType & aVmbxType )
    {
    SMC_MOCK_METHOD1( void, const TVmbxType &, aVmbxType )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::VmbxAlsLineType
// -----------------------------------------------------------------------------
//
TVmbxAlsLineType CVoiceMailboxEntry::VmbxAlsLineType(  ) const
    {
    SMC_MOCK_METHOD0( TVmbxAlsLineType )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxAlsLineType
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetVmbxAlsLineType( 
        const TVmbxAlsLineType & aLine )
    {
    SMC_MOCK_METHOD1( void, const TVmbxAlsLineType &, aLine )
    }

// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxNumber
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::GetVmbxNumber( 
        TPtrC & aVmbxNumber ) const
    {
    SMC_MOCK_METHOD1( TInt, TPtrC &, aVmbxNumber )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxNumber
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetVmbxNumber( 
        const TDesC & aVmbxNumber )
    {
    SMC_MOCK_METHOD1( TInt, const TDesC &, aVmbxNumber )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::GetBrandId
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::GetBrandId( 
        TPtrC8 & aBrandId ) const
    {
    SMC_MOCK_METHOD1( TInt, TPtrC8 &, aBrandId )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetBrandId
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetBrandId( 
        const TDesC8 & aBrandId )
    {
    SMC_MOCK_METHOD1( TInt, const TDesC8 &, aBrandId )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxName
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::GetVmbxName( 
        TPtrC & aVmbxName ) const
    {
    SMC_MOCK_METHOD1( TInt, TPtrC &, aVmbxName )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxName
// -----------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetVmbxName( 
        const TDesC & aVmbxName )
    {
    SMC_MOCK_METHOD1( TInt, const TDesC &, aVmbxName )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::UsingMemoryLocation
// -----------------------------------------------------------------------------
//
TVmbxMemoryLocation CVoiceMailboxEntry::UsingMemoryLocation(  ) const
    {
    SMC_MOCK_METHOD0( TVmbxMemoryLocation )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::Reset
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::Reset(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// CVoiceMailboxEntry::SetUsingMemoryLocation
// -----------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetUsingMemoryLocation( 
        const TVmbxMemoryLocation & aType )
    {
    SMC_MOCK_METHOD1( void, const TVmbxMemoryLocation &, aType )
    }


