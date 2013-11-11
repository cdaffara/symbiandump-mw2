/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Inline implementation
*
*/

#include <s32mem.h>
#include <mcallremotepartyinfo.h>

namespace RemotePartyMediatorApi 
    {
    enum TPanic
        {
        ENullPointer
        }; 
    _LIT( KRemotePartyMediatorApi, "RPMA");
    }

// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfo* CCallRemotePartyInfo::NewL()
    {
    CCallRemotePartyInfo* self = CCallRemotePartyInfo::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfo* CCallRemotePartyInfo::NewLC()
    {
    CCallRemotePartyInfo* self = new( ELeave )CCallRemotePartyInfo();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfo::~CCallRemotePartyInfo()
    {
    delete iCompanyName;
    delete iMatchedName;
    delete iNameFromNetwork;
    delete iAddress;
    }

// ---------------------------------------------------------------------------
// Extarnalizes remote party info to stream
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteDescriptorToStreamL( aStream, *iCompanyName );
    WriteDescriptorToStreamL( aStream, *iMatchedName );
    WriteDescriptorToStreamL( aStream, *iNameFromNetwork );
    WriteDescriptorToStreamL( aStream, *iAddress );
    aStream.WriteInt32L( iCallIndex );
    aStream.WriteInt32L( iRemoteIdentity );
    aStream.WriteInt32L( iSATCall );
    }

// ---------------------------------------------------------------------------
// Internalizes remote party info from stream
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::InternalizeL( RReadStream& aStream )
    {
    HBufC* companyName = ReadDescriptorFromStreamLC( aStream );
    HBufC* matchedName = ReadDescriptorFromStreamLC( aStream );
    HBufC* nameFromNetwork = ReadDescriptorFromStreamLC( aStream );    
    HBufC* address = ReadDescriptorFromStreamLC( aStream );    
    const TInt callIndex = aStream.ReadInt32L(); 
    const TInt remoteIdentity = aStream.ReadInt32L();
    const TInt SATCall = aStream.ReadInt32L();
    
    CleanupStack::Pop( 4, companyName ); 
    
    
    delete iCompanyName;
    delete iMatchedName;
    delete iNameFromNetwork;
    delete iAddress;
    
    
    iCompanyName = companyName;
    iMatchedName = matchedName;
    iNameFromNetwork = nameFromNetwork;
    iAddress = address;
    iCallIndex = callIndex;     
    iRemoteIdentity = static_cast < TRemoteIdentityStatus > ( remoteIdentity );
    iSATCall = SATCall;
    }

// ---------------------------------------------------------------------------
// Size of the descriptor required for the information to be externalized
// ---------------------------------------------------------------------------
//
TInt CCallRemotePartyInfo::Size() const
    {
    // each member descriptor also needs the length of the descriptor
    TInt size = sizeof(TInt) + iCompanyName->Size();    
    size += sizeof( TInt ) + iMatchedName->Size();
    size += sizeof( TInt ) + iNameFromNetwork->Size();
    size += sizeof( TInt ) + iAddress->Size();
    size += sizeof( TInt ); // Call index
    size += sizeof( TInt ); // remote identity    
    size += sizeof( TInt ); // SAT call
    return size;
    }
    
// ---------------------------------------------------------------------------
// Sets remote matched name
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetMatchedNameL( const TDesC& aName )
    {
    HBufC* name = aName.AllocL();
    delete iMatchedName;
    iMatchedName = name;
    }

// ---------------------------------------------------------------------------
// Sets remote name from network
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetNameFromNetworkL( const TDesC& aName )
    {
    HBufC* name = aName.AllocL();
    delete iNameFromNetwork;
    iNameFromNetwork = name;
    }

// ---------------------------------------------------------------------------
// Sets remote company name
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetCompanyNameL( const TDesC& aName )
    {
    HBufC* name = aName.AllocL();
    delete iCompanyName;
    iCompanyName = name;
    } 
    
    
// ---------------------------------------------------------------------------
// Sets remote address
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetAddressL( const TDesC& aAddress )
    {
    HBufC* address = aAddress.AllocL();
    delete iAddress;
    iAddress = address;
    }

// ---------------------------------------------------------------------------
// Sets call index
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetCallIndex( TInt aCallIndex ) 
    {
    iCallIndex = aCallIndex;
    }


// ---------------------------------------------------------------------------
// Sets remote identity
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetRemoteIdentity( TRemoteIdentityStatus aRemoteIdentity )
    {
    iRemoteIdentity = aRemoteIdentity;
    }

// ---------------------------------------------------------------------------
// Sets SAT call flag
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::SetSATCall( TBool aSatCall )
    {
    iSATCall = aSatCall;
    }
    
// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
const TDesC& CCallRemotePartyInfo::Address() const
    {
    return *iAddress;
    }

// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
const TDesC& CCallRemotePartyInfo::CompanyName() const
    {
    return *iCompanyName;
    }

// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
const TDesC& CCallRemotePartyInfo::MatchedName() const
    {
    return *iMatchedName;
    }

// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
const TDesC& CCallRemotePartyInfo::NameFromNetwork() const
    {
    return *iNameFromNetwork;
    }
    

// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
TInt CCallRemotePartyInfo::CallIndex() const
    {
    return iCallIndex;
    }


// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
MCallRemotePartyInfo::TRemoteIdentityStatus CCallRemotePartyInfo::RemoteIdentity() const
    {
    return iRemoteIdentity;
    }

// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
TBool CCallRemotePartyInfo::SATCall() const
    {
    return iSATCall;
    }




// ---------------------------------------------------------------------------
// From base class MCallRemotePartyInfo
// ---------------------------------------------------------------------------
//
TBool CCallRemotePartyInfo::operator==(const MCallRemotePartyInfo& aRemoteInfo) const
    {
        
    if ( iMatchedName->Match( aRemoteInfo.MatchedName() ) == KErrNone  &&  
         iNameFromNetwork->Match( aRemoteInfo.NameFromNetwork() ) == KErrNone  &&
         iCompanyName->Match( aRemoteInfo.CompanyName() ) == KErrNone  &&
         iAddress->Match( aRemoteInfo.Address() ) == KErrNone  && 
         iCallIndex == aRemoteInfo.CallIndex() &&
         iRemoteIdentity == aRemoteInfo.RemoteIdentity() &&
         iSATCall == aRemoteInfo.SATCall() )
        {
        return ETrue;
        }   
        
    return EFalse;
    }

// ---------------------------------------------------------------------------
// Reads descriptor from stream.
// ---------------------------------------------------------------------------
//
HBufC* CCallRemotePartyInfo::ReadDescriptorFromStreamLC( RReadStream& aStream )
    {
    const TInt descLength = aStream.ReadInt32L();
    HBufC* desc = HBufC::NewLC( descLength );
    TPtr descPtr = desc->Des();
    aStream.ReadL( descPtr, descLength );
    return desc;
    }

// ---------------------------------------------------------------------------
// writes descriptor to stream.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::WriteDescriptorToStreamL( 
    RWriteStream& aStream,
    const TDesC& aDesc )
    {
    aStream.WriteInt32L( aDesc.Length() );
    aStream.WriteL( aDesc );
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfo::CCallRemotePartyInfo() : 
    iCallIndex( -1 ),
    iRemoteIdentity( MCallRemotePartyInfo::ERemoteIdentityUnknown )
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfo::ConstructL()
    {
    iCompanyName = KNullDesC().AllocL();
    iMatchedName = KNullDesC().AllocL();
    iNameFromNetwork = KNullDesC().AllocL();        
    iAddress = KNullDesC().AllocL();    
    }
// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfos* CCallRemotePartyInfos::NewL()
    {
    CCallRemotePartyInfos* self = CCallRemotePartyInfos::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// static constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfos* CCallRemotePartyInfos::NewLC()
    {
    CCallRemotePartyInfos* self = new( ELeave )CCallRemotePartyInfos();
    CleanupStack::PushL( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfos::~CCallRemotePartyInfos()
    {
    iInfos.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// Adds remote party information
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfos::AddL( CCallRemotePartyInfo* aInfo )
    {    
    __ASSERT_ALWAYS( aInfo, 
        User::Panic(RemotePartyMediatorApi::KRemotePartyMediatorApi, 
                    RemotePartyMediatorApi::ENullPointer ) );
    iInfos.AppendL( aInfo );
    }

// ---------------------------------------------------------------------------
// Remote party information access
// ---------------------------------------------------------------------------
//
const RPointerArray<CCallRemotePartyInfo>& 
    CCallRemotePartyInfos::RemotePartyInfos() const
    {
    return iInfos;
    }

// ---------------------------------------------------------------------------
// Packages remote party information to descriptor
// ---------------------------------------------------------------------------
//
HBufC8* CCallRemotePartyInfos::ExternalizeL() const
    {
    // Calculate the size of the descriptor required
    TInt sizeRequired = sizeof(TInt); // Count of the infos
    TInt count = iInfos.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        sizeRequired += iInfos[i]->Size();
        }
    HBufC8* package = HBufC8::NewLC( sizeRequired );
    TPtr8 packagePtr = package->Des();
    RDesWriteStream stream( packagePtr );
    stream.PushL();
    stream.WriteInt32L( count );
    for( TInt i = 0; i < count; i++ )
        {
        iInfos[i]->ExternalizeL( stream );
        }
    stream.CommitL();
    stream.Pop();
    stream.Close();
    CleanupStack::Pop( package );
    return package;
    }

// ---------------------------------------------------------------------------
// Unpacks remote party informations from descriptor.
// ---------------------------------------------------------------------------
//
void CCallRemotePartyInfos::InternalizeL( const TDesC8& aData ) 
    {   
    iInfos.ResetAndDestroy();
    
    RDesReadStream stream( aData );
    stream.PushL();
    const TInt numberOfInfos = stream.ReadInt32L();
    for ( TInt i = 0; i < numberOfInfos; i++ )
        {
        CCallRemotePartyInfo* info = CCallRemotePartyInfo::NewLC();
        info->InternalizeL( stream );
        iInfos.AppendL( info );
        CleanupStack::Pop( info );
        }
    
    stream.Pop();
    stream.Close();      
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CCallRemotePartyInfos::CCallRemotePartyInfos()
    {
    
    }




