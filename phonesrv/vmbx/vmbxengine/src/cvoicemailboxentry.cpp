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
* Description:  Implementation of the CVoiceMailboxEntry class
*
*/

// INCLUDE FILES
#include <cvoicemailboxentry.h>
#include "vmbxlogger.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::NewL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CVoiceMailboxEntry* CVoiceMailboxEntry::NewL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::NewL =>" );
    CVoiceMailboxEntry* self = CVoiceMailboxEntry::NewLC();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::NewLC
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CVoiceMailboxEntry* CVoiceMailboxEntry::NewLC()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::NewLC =>" );
    CVoiceMailboxEntry* self = new( ELeave ) CVoiceMailboxEntry;
    CleanupStack::PushL( self );
    self->ConstructL();
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::NewLC <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::~CVoiceMailboxEntry
// Destructor
// ---------------------------------------------------------------------------
//
CVoiceMailboxEntry::~CVoiceMailboxEntry()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::~CVoiceMailboxEntry =>" );
    delete ivmbxNumber;
    delete iVmbxBrandId;
    delete ivmbxName;
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::~CVoiceMailboxEntry <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::ServiceId
// Get ServiceId
// ---------------------------------------------------------------------------
//
EXPORT_C TServiceId CVoiceMailboxEntry::ServiceId() const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::ServiceId =>" );
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::ServiceId:\
    ServiceId=%d <=", iVmbxServiceId );
    return iVmbxServiceId;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetServiceId
// Set entry's service id
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetServiceId( 
                        const TServiceId& aVmbxServiceId )
    {
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetServiceId: ServiceId=%d =>",
    aVmbxServiceId );
    iVmbxServiceId = aVmbxServiceId;
    VMBLOGSTRING( "VMBX: CVoiceMailboxImpl::SetServiceId <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::VoiceMailboxType
// Returns type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxType CVoiceMailboxEntry::VoiceMailboxType( ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::VoiceMailboxType <=>" );
    return iVmbxType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVoiceMailboxType
// Set Type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailboxEntry::SetVoiceMailboxType( const TVmbxType& aType )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetVoiceMailboxType =>" );
    iVmbxType = aType;
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVoiceMailboxType:\
    iVmbxType=%d <=", iVmbxType );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::VmbxAlsLineType
// Get ALS line Type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxAlsLineType CVoiceMailboxEntry::VmbxAlsLineType() const
    {
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::VmbxAlsLineType:\
    iVmbxLineType=%d <=>", iVmbxLineType );
    return iVmbxLineType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxAlsLineType
// Set ALS line Type of the entry instance
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetVmbxAlsLineType( 
                                const TVmbxAlsLineType& aLine )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetVmbxAlsLineType =>" );
    if ( EVmbxAlsLineDefault == aLine  )
        {
        iVmbxLineType = EVmbxAlsLine1;
        }
    else
        {
        iVmbxLineType = aLine;
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVmbxAlsLineType:\
    iVmbxLineType=%d <=", iVmbxLineType );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxNumber
// Get number or address of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::GetVmbxNumber( TPtrC& aVmbxNumber ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxNumber =>" );
    TInt result( KErrNotFound );
    if ( ivmbxNumber )
        {
        aVmbxNumber.Set( ivmbxNumber->Des() );
        VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::GetVmbxNumber:\
        ivmbxNumber = %S", &aVmbxNumber );
        result = KErrNone;
        }
    else
        {
        VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxNumber:KNullDesC" );
        aVmbxNumber.Set( KNullDesC );
        }
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxNumber <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxNumber
// Set number or address of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::SetVmbxNumber( const TDesC& aVmbxNumber )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetVmbxNumber =>" );
    TInt result( KErrNoMemory );
    if ( aVmbxNumber.Length() > KVmbxMaxNumberLength )
        {
        result = KErrOverflow;
        }
    else
        {
        delete ivmbxNumber;
        ivmbxNumber = aVmbxNumber.Alloc(); // Returns NULL if fails.
        if ( ivmbxNumber )
            {
            result = KErrNone;
            VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVmbxNumber:\
            ivmbxNumber = %S", ivmbxNumber );
            }
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVmbxNumber:\
    result = %I<=", result ); 
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxAlsLineType
// Get Brand Id of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::GetBrandId( TPtrC8& aBrandId ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetBrandId =>" );
    TInt result( KErrNotFound );
    if ( iVmbxBrandId )
        {
        aBrandId.Set( iVmbxBrandId->Des() );
        result = KErrNone;
        }
    else
        {
        aBrandId.Set( KNullDesC8 );
        }
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetBrandId <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetBrandId
// Set Brand Id of the entry instance
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetBrandId( const TDesC8& aBrandId )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetBrandId =>" );
    TInt result( KErrNoMemory );
    if ( KVmbxMaxNumberLength < aBrandId.Length() )
        {
        result = KErrArgument;
        }
    else
        {
        delete iVmbxBrandId;
        iVmbxBrandId = aBrandId.Alloc();
        if ( iVmbxBrandId )
            {
            result = KErrNone;
            VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetBrandId:\
            iVmbxBrandId = %S", iVmbxBrandId );
            }
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetBrandId:result = %d <=",
    result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxName
// Get Name of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::GetVmbxName( TPtrC& aVmbxName ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxName =>" );
    TInt result( KErrNotFound );
    if ( ivmbxName )
        {
        aVmbxName.Set( ivmbxName->Des() );
        VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::GetVmbxName:\
        ivmbxName = %S", &aVmbxName );
        result = KErrNone;
        }
    else
        {
        VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxName:KNullDesC" );
        aVmbxName.Set( KNullDesC );
        }      
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::GetVmbxName <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxName
// Set Name of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::SetVmbxName( const TDesC& aVmbxName )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetVmbxName =>" );
    TInt result( KErrNoMemory );
    if ( KVmbxMaxNumberLength < aVmbxName.Length() )
        {
        result = KErrOverflow;
        }
    else
        {
        delete ivmbxName;
        ivmbxName = aVmbxName.Alloc(); // Returns NULL if fails.
        if ( ivmbxName )
            {
            result = KErrNone;
            VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVmbxName:\
            ivmbxName = %S", ivmbxName );
            }       
        }
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetVmbxName: result %d <=", 
                    result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::UsingMemoryLocation
// Returns memory location of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxMemoryLocation CVoiceMailboxEntry::UsingMemoryLocation( ) const
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::UsingMemoryLocation <=>" );
    return iUsingMemory;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::Reset
// Reset the data members of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailboxEntry::Reset()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::Reset =>" );
    iVmbxServiceId = KVmbxServiceIdNone;
    iVmbxType = EVmbxNone;
    iVmbxLineType = EVmbxAlsLineDefault;
    if ( ivmbxNumber )
    	{
    	ivmbxNumber->Des().Zero();
    	VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::Reset vmbx number" );
    	}
    if ( ivmbxName )
    	{
    	ivmbxName->Des().Zero();
    	VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::Reset vmbx name" );
    	}
    if ( iVmbxBrandId )
    	{
    	iVmbxBrandId->Des().Zero();
    	VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::Reset vmbx brandId" );
    	}
    iUsingMemory = EVmbxSimMemory;
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::Reset <=" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetUsingMemoryLocation
// Set using memory of the entry instance
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetUsingMemoryLocation( 
                                        const TVmbxMemoryLocation& aType  )
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::SetUsingMemoryLocation =>" );
    iUsingMemory = aType;
    VMBLOGSTRING2( "VMBX: CVoiceMailboxEntry::SetUsingMemoryLocation:\
    iUsingMemory=%I <=", iUsingMemory );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::CVoiceMailboxEntry
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVoiceMailboxEntry::CVoiceMailboxEntry()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::CVoiceMailboxEntry <=>" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVoiceMailboxEntry::ConstructL <=>" );
    }

// End of file
