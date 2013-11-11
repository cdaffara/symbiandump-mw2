/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Methods for CAiwInternalDialData class.
*
*/


#include "aiwinternaldialdata.h"
#include "aiwdialdataext.h"

// CONSTANTS    

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// 
// First phase constructor
// 
// --------------------------------------------------------------------------- 
//
EXPORT_C CAiwInternalDialData* CAiwInternalDialData::NewL()
    {
    CAiwInternalDialData* self = CAiwInternalDialData::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// 
// First phase constructor
// 
// --------------------------------------------------------------------------- 
//
EXPORT_C CAiwInternalDialData* CAiwInternalDialData::NewLC()
    {
    CAiwInternalDialData* self = new( ELeave ) CAiwInternalDialData;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// 
// First phase constructor
// Initialises the member data from package.
// 
// --------------------------------------------------------------------------- 
//    
EXPORT_C CAiwInternalDialData* CAiwInternalDialData::NewL( 
    const TDesC8& aPackage )
    {
    CAiwInternalDialData* self = CAiwInternalDialData::NewLC( aPackage );
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// 
// First phase constructor
// Initialises the member data from package.
// 
// --------------------------------------------------------------------------- 
//
EXPORT_C CAiwInternalDialData* CAiwInternalDialData::NewLC(
    const TDesC8& aPackage )
    {
    CAiwInternalDialData* self = new( ELeave ) CAiwInternalDialData;
    CleanupStack::PushL( self );
    self->ConstructL( aPackage );
    return self;
    }

// ---------------------------------------------------------------------------
// 
// Destructor.
// 
// ---------------------------------------------------------------------------
//
CAiwInternalDialData::~CAiwInternalDialData()
    {
    delete iPhoneNumber;
    iPhoneNumber = NULL;
    delete iName;
    iName = NULL;
    delete iContactLink;
    iContactLink = NULL;
    delete iSubAddress;
    iSubAddress = NULL;
    delete iBearer;
    iBearer = NULL;
    delete iUUI;
    iUUI = NULL;
    }
    
// ---------------------------------------------------------------------------
// Fills the parameter list.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::FillInParamListL( 
    CAiwGenericParamList& aInParamList )
    {
    HBufC8* package = PackLC();
    TAiwVariant variant( *package );
    TAiwGenericParam param( EGenericParamCallDialData, variant );
    aInParamList.AppendL( param );
    CleanupStack::PopAndDestroy( package );
    }
    
// ---------------------------------------------------------------------------
// Sets SAT call.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetSATCall( TBool aIsSATCall ) 
    {
    iSATCall = aIsSATCall;
    }
   
// ---------------------------------------------------------------------------
// Sets SubAddress for SAT call.
// ---------------------------------------------------------------------------
EXPORT_C void CAiwInternalDialData::SetSubAddressL( 
    const TDesC& aSubAddress )
    {
    CheckDescriptorMaxLengthL( aSubAddress.Length(), 
                               AIWInternalDialData::KAiwSubAddressLength );
    CopyDescriptorToMemberDescriptorL( aSubAddress, iSubAddress );
    }
    
// ---------------------------------------------------------------------------
// Sets bearer.
// ---------------------------------------------------------------------------
EXPORT_C void CAiwInternalDialData::SetBearerL(
    const TDesC8& aBearer )
    {
    CheckDescriptorMaxLengthL( aBearer.Length(), 
                               AIWInternalDialData::KAiwBearerLength );
    CopyDescriptorToMemberDescriptorL( aBearer, iBearer );
    }

// ---------------------------------------------------------------------------
// Sets end other calls.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetEndOtherCalls( 
    TBool aEndOtherCalls ) 
    {
    iEndOtherCalls = aEndOtherCalls;
    }

// ---------------------------------------------------------------------------
// Sets redial max duation time.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetRedialMaximumDuration( 
    const TTimeIntervalSeconds aMaximumRedialDuration ) 
    {
    iRedialMaximumDuration = aMaximumRedialDuration;
    }
    

// -----------------------------------------------------------------------------
// Sets used service id.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetServiceId( TUint32 aServiceId )
    {
    iServiceId = aServiceId;
    }


// -----------------------------------------------------------------------------
// Sets user to user information.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetUUIL( const TDesC& aUUI )
    {
    CheckDescriptorMaxLengthL( aUUI.Length(), 
        AIWInternalDialData::KAiwUUILength );
    CopyDescriptorToMemberDescriptorL( aUUI, iUUI );
    }
    
// ---------------------------------------------------------------------------
// Sets phone number.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetPhoneNumberL(
    const TDesC& aPhoneNumber )
    {
    CheckDescriptorMaxLengthL( aPhoneNumber.Length(), 
                            AIWInternalDialData::KMaximumPhoneNumberLength );
    CopyDescriptorToMemberDescriptorL( aPhoneNumber, iPhoneNumber );
    }
        
// ---------------------------------------------------------------------------
// Sets call type.
// ---------------------------------------------------------------------------
//   
EXPORT_C void CAiwInternalDialData::SetCallType( 
    CAiwDialData::TCallType aCallType ) 
    {
    iCallType = aCallType;
    }

// ---------------------------------------------------------------------------
// Set initiate call.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwInternalDialData::SetInitiateCall( TBool aInitCall )
    {
    iInitCall = aInitCall;
    }
   
// ---------------------------------------------------------------------------
// Set name.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwInternalDialData::SetNameL( const TDesC& aName ) 
    {
    CheckDescriptorMaxLengthL( aName.Length(), 
                               AIWInternalDialData::KMaximumNameLength );
    CopyDescriptorToMemberDescriptorL( aName, iName );
    }
    
// ---------------------------------------------------------------------------
// Sets window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwInternalDialData::SetWindowGroup( TInt aWindowGroup ) 
    {
    iWindowGroup = aWindowGroup;
    }
   
// ---------------------------------------------------------------------------
// Sets redial value.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwInternalDialData::SetRedial( TInt aRedial ) 
    {
    iRedial = aRedial;
    }
    
// ---------------------------------------------------------------------------
// Set show number.
// ---------------------------------------------------------------------------
//    
EXPORT_C void CAiwInternalDialData::SetShowNumber( TBool aShowNumber ) 
    {
    iShowNumber = aShowNumber;
    }
    
// ---------------------------------------------------------------------------
// Sets allow match.
// ---------------------------------------------------------------------------
//   
EXPORT_C void CAiwInternalDialData::SetAllowMatch( TBool aAllowMatch ) 
    {
    iAllowMatch = aAllowMatch;
    }
    
// ---------------------------------------------------------------------------
// Sets used contact link.
// ---------------------------------------------------------------------------
//
EXPORT_C void CAiwInternalDialData::SetContactLinkL( 
    const TDesC8& aContactLink ) 
    {
    CopyDescriptorToMemberDescriptorL( aContactLink, iContactLink );
    }
    
// ---------------------------------------------------------------------------
// Returns SAT call value.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CAiwInternalDialData::SATCall() const
    {
    return iSATCall;    
    }

// ---------------------------------------------------------------------------
// Returns subaddress.
// ---------------------------------------------------------------------------
//   
EXPORT_C const TDesC& CAiwInternalDialData::SubAddress() const
    {
    return *iSubAddress;
    }
    
// ---------------------------------------------------------------------------
// Returns end other calls value.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CAiwInternalDialData::EndOtherCalls() const
    {
    return iEndOtherCalls;
    }
    
// ---------------------------------------------------------------------------
// Returns bearer.
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC8& CAiwInternalDialData::Bearer() const
    {
    return *iBearer;
    }

// ---------------------------------------------------------------------------
// Returns redial max duration.
// ---------------------------------------------------------------------------
//
EXPORT_C const TTimeIntervalSeconds 
    CAiwInternalDialData::RedialMaximumDuration() const
    {
    return iRedialMaximumDuration;
    }
    

// -----------------------------------------------------------------------------
// Returns used service id.
// -----------------------------------------------------------------------------
//
EXPORT_C TUint32 CAiwInternalDialData::ServiceId() const
    {
    return iServiceId;
    }

    
// ---------------------------------------------------------------------------
// Returns name.
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CAiwInternalDialData::Name() const 
    {
    return *iName;
    }      

// ---------------------------------------------------------------------------
// Returns initiate call.
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CAiwInternalDialData::InitiateCall() const 
    {
    return iInitCall;
    } 
        
// ---------------------------------------------------------------------------
// Returns phone number.
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC& CAiwInternalDialData::PhoneNumber() const 
    {
    return *iPhoneNumber;    
    }
    
// ---------------------------------------------------------------------------
// Returns call type.
// ---------------------------------------------------------------------------
//    
EXPORT_C CAiwDialData::TCallType CAiwInternalDialData::CallType() const 
    {
    return iCallType;
    }
        
// ---------------------------------------------------------------------------
// Return window group.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CAiwInternalDialData::WindowGroup() const
    {
    return iWindowGroup;
    }
// ---------------------------------------------------------------------------
// Return redial value.
// ---------------------------------------------------------------------------
//    
EXPORT_C TInt CAiwInternalDialData::Redial() const 
    {
    return iRedial;
    }
// ---------------------------------------------------------------------------
// Returns show number.
// ---------------------------------------------------------------------------
//   
EXPORT_C TBool CAiwInternalDialData::ShowNumber() const 
    {
    return iShowNumber;
    }
    
// ---------------------------------------------------------------------------
// Return allow match.
// ---------------------------------------------------------------------------
//   
EXPORT_C TBool CAiwInternalDialData::AllowMatch() const 
    {
    return iAllowMatch;
    }
    
// ---------------------------------------------------------------------------
// Return contact link.
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC8& CAiwInternalDialData::ContactLink() const 
    {
    return *iContactLink;
    }  
    
// ---------------------------------------------------------------------------
// Return user to user information
// ---------------------------------------------------------------------------
//   
EXPORT_C const TDesC& CAiwInternalDialData::UUI() const
    {
    return *iUUI;
    }
    
// ---------------------------------------------------------------------------
// 
// C++ constructor
// 
// ---------------------------------------------------------------------------
//
CAiwInternalDialData::CAiwInternalDialData() :
    iShowNumber( ETrue ),
    iAllowMatch( ETrue ),
    iServiceId( 0 )
    {
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS constructor
// 
// ---------------------------------------------------------------------------
//
void CAiwInternalDialData::ConstructL()
    {
    iPhoneNumber = KNullDesC().AllocL();
    iName = KNullDesC().AllocL();
    iContactLink = KNullDesC8().AllocL();
    iSubAddress = KNullDesC().AllocL();
    iBearer = KNullDesC8().AllocL();
    iUUI = KNullDesC().AllocL();
    iCallType = CAiwDialData::EAIWVoice;
    iWindowGroup = AIWDialData::KAiwGoToIdle;
    iInitCall = EFalse;
    iRedial = AIWDialDataExt::KAIWRedialDefault;
    iShowNumber = ETrue;
    iAllowMatch = ETrue;
    iRedialMaximumDuration = NULL;
    iSATCall = EFalse;
    iEndOtherCalls = EFalse;
    }

// ---------------------------------------------------------------------------
// 
// Symbian OS constructor  
// Initialises the member data from package.
//
// ---------------------------------------------------------------------------
//
void CAiwInternalDialData::ConstructL(
    const TDesC8& aPackage ) 
    {
    RDesReadStream stream( aPackage );
    CleanupClosePushL( stream );
    
    ReadDescriptorFromStreamL( stream, iPhoneNumber );
    iCallType = static_cast<CAiwDialData::TCallType>( stream.ReadUint32L() );
    ReadDescriptorFromStreamL( stream, iName );
    iWindowGroup = stream.ReadInt32L();
    iRedial = stream.ReadInt32L();
    iShowNumber = stream.ReadInt32L();
    iAllowMatch = stream.ReadInt32L();
    ReadDescriptorFromStreamL( stream, iContactLink );
    iSATCall = stream.ReadInt32L();
    ReadDescriptorFromStreamL( stream, iSubAddress );
    iEndOtherCalls = stream.ReadInt32L();
    ReadDescriptorFromStreamL( stream, iBearer );
    iRedialMaximumDuration = stream.ReadInt32L();
    iInitCall = stream.ReadInt32L();
    iServiceId = stream.ReadInt32L();    
    ReadDescriptorFromStreamL( stream, iUUI );
    
    CleanupStack::PopAndDestroy( 1 ); // stream;
    }

// ---------------------------------------------------------------------------
// Calculates the size of all member data.
// ---------------------------------------------------------------------------
//
TInt CAiwInternalDialData::CalculatePackageSize() const
    {
    TInt packageSize = 0;
    packageSize += CalculateDescriptorSize( *iPhoneNumber );
    packageSize += sizeof( iCallType );
    packageSize += CalculateDescriptorSize( *iName );
    packageSize += sizeof( iWindowGroup );
    packageSize += sizeof( iRedial );
    packageSize += sizeof( iShowNumber );
    packageSize += sizeof( iAllowMatch );
    packageSize += CalculateDescriptorSize( *iContactLink );
    packageSize += sizeof( iSATCall );
    packageSize += CalculateDescriptorSize( *iSubAddress );
    packageSize += sizeof( iEndOtherCalls );
    packageSize += CalculateDescriptorSize( *iBearer );
    packageSize += sizeof( iRedialMaximumDuration );
    packageSize += sizeof( iInitCall );
    packageSize += sizeof( iServiceId );
    packageSize += CalculateDescriptorSize( *iUUI );
    
    return packageSize;
    }

// ---------------------------------------------------------------------------
// Calculates the size of descriptor in package. Note that to retreave 
// a descriptor from stream in ConstructL we need to know the size of a
// descriptor in order to allocate right size descriptor. This is the reason
// for adding sizeof( int ) to descriptor length.
// ---------------------------------------------------------------------------
// 
TInt CAiwInternalDialData::CalculateDescriptorSize( 
    const TDesC& aDescriptor ) const 
    {
    // sizeof( TInt ) is for the length of a descriptor.
    return sizeof( TInt ) + aDescriptor.Length() * 2; // unicode takes 2 bytes.
    }

// ---------------------------------------------------------------------------
// Calculates the size of descriptor in package. Note that to retreave 
// a descriptor from stream in ConstructL we need to know the size of a
// descriptor in order to allocate right size descriptor. This is the reason
// for adding sizeof( int ) to descriptor length.
// ---------------------------------------------------------------------------
// 
TInt CAiwInternalDialData::CalculateDescriptorSize( 
    const TDesC8& aDescriptor ) const 
    {
    // sizeof( TInt ) is for the length of a descriptor in stream
    return sizeof( TInt ) + aDescriptor.Length();
    }    

// ---------------------------------------------------------------------------
// Reads descriptor from stream.
// ---------------------------------------------------------------------------
//
void CAiwInternalDialData::ReadDescriptorFromStreamL( 
    RDesReadStream& aStream,
    HBufC*& aDescriptor 
    )
    {
    const TInt descLength( aStream.ReadInt32L() );
    
    // Not to cleanupstack, aDescriptor will be member variable
    aDescriptor = HBufC::NewL( descLength ); 
    TPtr16 ptr = aDescriptor->Des();
    aStream.ReadL( ptr, descLength );
    }
  
// ---------------------------------------------------------------------------
// Reads descriptor from stream.
// ---------------------------------------------------------------------------
//  
void CAiwInternalDialData::ReadDescriptorFromStreamL( 
    RDesReadStream& aStream,
    HBufC8*& aDescriptor 
    )
    {
    const TInt descLength( aStream.ReadInt32L() );
    
    // Not to cleanupstack, aDescriptor is member variable
    aDescriptor = HBufC8::NewL( descLength ); 
    TPtr8 ptr = aDescriptor->Des();
    aStream.ReadL( ptr, descLength );
    }    
   
// ---------------------------------------------------------------------------
// Writes descriptor to stream.
// ---------------------------------------------------------------------------
//  
void CAiwInternalDialData::WriteDescriptorToStreamL( 
    RDesWriteStream& aStream, 
    const TDesC& aDescriptor ) 
    {
    aStream.WriteInt32L( aDescriptor.Length() );
    aStream.WriteL( aDescriptor );
    }

// ---------------------------------------------------------------------------
// Writes descriptor to stream.
// ---------------------------------------------------------------------------
//  
void CAiwInternalDialData::WriteDescriptorToStreamL( 
    RDesWriteStream& aStream, 
    const TDesC8& aDescriptor ) 
    {
    aStream.WriteInt32L( aDescriptor.Length() );
    aStream.WriteL( aDescriptor );
    }
   
// ---------------------------------------------------------------------------
// Copies aSource descriptor to aMemberDescriptor. This quarantees that member
// descriptor will be valid even if the allocation fails.
// ---------------------------------------------------------------------------
//   
void CAiwInternalDialData::CopyDescriptorToMemberDescriptorL( 
    const TDesC& aSource, 
    HBufC*& aMemberDescriptor )
    {
    HBufC* temp = aSource.AllocL();
    delete aMemberDescriptor;
    aMemberDescriptor = temp;
    }

// ---------------------------------------------------------------------------
// Copies aSource descriptor to aMemberDescriptor. This quarantees that member
// descriptor will be valid even if the allocation fails.
// ---------------------------------------------------------------------------
//       
void CAiwInternalDialData::CopyDescriptorToMemberDescriptorL( 
    const TDesC8& aSource, 
    HBufC8*& aMemberDescriptor )
    {
    HBufC8* temp = aSource.AllocL();
    delete aMemberDescriptor;
    aMemberDescriptor = temp;
    }

// ---------------------------------------------------------------------------
// Checks that descriptorLength is not longer than maximum allowed descriptor
// length.
// ---------------------------------------------------------------------------
//   
void CAiwInternalDialData::CheckDescriptorMaxLengthL( 
    TInt aDescriptorLength, TInt aMaxAllowedDescriptorLength ) const
    {
    if( aDescriptorLength > aMaxAllowedDescriptorLength )
        {
        User::Leave( KErrArgument );
        }
    }

// ---------------------------------------------------------------------------
// Packs the member data to descriptor. Ownershipd of returned descriptor
// is given for caller.
// ---------------------------------------------------------------------------
//  
HBufC8* CAiwInternalDialData::PackLC() 
    {
    HBufC8* package = HBufC8::NewLC( CalculatePackageSize() );
    
    TPtr8 des = package->Des();
    RDesWriteStream stream( des );
    CleanupClosePushL( stream );
    
    WriteDescriptorToStreamL( stream, *iPhoneNumber );
    stream.WriteInt32L( iCallType );
    WriteDescriptorToStreamL( stream, *iName );
    stream.WriteInt32L( iWindowGroup );
    stream.WriteInt32L( iRedial );
    stream.WriteInt32L( iShowNumber );
    stream.WriteInt32L( iAllowMatch );
    WriteDescriptorToStreamL( stream, *iContactLink );
    stream.WriteInt32L( iSATCall );
    WriteDescriptorToStreamL( stream, *iSubAddress );
    stream.WriteInt32L( iEndOtherCalls );
    WriteDescriptorToStreamL( stream, *iBearer );
    stream.WriteInt32L( iRedialMaximumDuration.Int() );
    stream.WriteInt32L( iInitCall );
    stream.WriteUint32L( iServiceId );
    WriteDescriptorToStreamL( stream, *iUUI );
        
    stream.CommitL();
    
    CleanupStack::PopAndDestroy( 1 ); // stream
    
    return package;
    }

//  End of File
