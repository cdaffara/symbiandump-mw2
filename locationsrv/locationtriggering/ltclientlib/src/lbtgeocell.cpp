/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Geographical cellular area class definition
*
*/

#include "lbtgeocell.h"
#include "lbterrors.h"


// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtGeoCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoCell* CLbtGeoCell::NewL()
    {
    CLbtGeoCell* self = CLbtGeoCell::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoCell* CLbtGeoCell::NewLC()
    {
    CLbtGeoCell* self = new( ELeave ) CLbtGeoCell;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoCell* CLbtGeoCell::NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                        TNetworkCountryCode aCountryCode,
                                        TNetworkIdentity aNetworkId,
                                        TUint aLocationAreaCode,
                                        TUint aCellId)
    {
    CLbtGeoCell* self = CLbtGeoCell::NewLC(aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,
                                                 aCellId);
    CleanupStack::Pop( self );
    return self;
    }  
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoCell* CLbtGeoCell::NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                         TNetworkCountryCode aCountryCode,
                                         TNetworkIdentity aNetworkId,
                                         TUint aLocationAreaCode,
                                         TUint aCellId)
    {
    CLbtGeoCell* self = new( ELeave ) CLbtGeoCell();
    CleanupStack::PushL( self );
    self->ConstructL( aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,aCellId);
    return self;
    }      

// ---------------------------------------------------------------------------
// CLbtGeoCell::CLbtGeoCell()
//
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtGeoCell::CLbtGeoCell()
    {
                                 
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::ConstructL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                             TNetworkCountryCode aCountryCode,
                             TNetworkIdentity aNetworkId,
                             TUint aLocationAreaCode,
                             TUint aCellId)
    {
    SetNetworkType( aNetworkType );
    SetNetworkCountryCode( aCountryCode );
    SetNetworkIdentityCode( aNetworkId );
    SetLocationAreaCode( aLocationAreaCode );
    SetCellId( aCellId );
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::ConstructL()
    {
    iNetworkType = RMobilePhone::ENetworkModeUnknown;
    iCountryCode = 0;
    iNetworkId = 0;
    iLocationAreaCode = 0;
    iCellId = 0;
    }      
     


// ---------------------------------------------------------------------------
// CLbtGeoCell::~CLbtGeoCell()
//
// Destructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoCell::~CLbtGeoCell()
    {
 
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::Type()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CLbtGeoAreaBase::TGeoAreaType CLbtGeoCell::Type() const   
    {
    return CLbtGeoAreaBase::ECellular;
    } 
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::SetNetworkType()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoCell::SetNetworkType( RMobilePhone::TMobilePhoneNetworkMode aNetworkType )
    {
    if( aNetworkType != RMobilePhone::ENetworkModeGsm && aNetworkType != RMobilePhone::ENetworkModeWcdma &&
        aNetworkType != RMobilePhone::ENetworkModeTdcdma )
        
        User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
        
    iNetworkType = aNetworkType;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::NetworkType()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C RMobilePhone::TMobilePhoneNetworkMode CLbtGeoCell::NetworkType() const
    {
    return iNetworkType;
    }    
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::SetNetworkCountryCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoCell::SetNetworkCountryCode( TNetworkCountryCode aCountryCode )
    {
    TInt countryCode ;
    TLex valLex( aCountryCode );
    TInt error = valLex.Val( countryCode );
    
    if( countryCode <= 0 || error!=KErrNone )
   
   	    User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
    			
    iCountryCode = countryCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::NetworkCountryCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TNetworkCountryCode CLbtGeoCell::NetworkCountryCode() const
    {
    TNetworkCountryCode countryCode;
    countryCode.AppendNum( iCountryCode );
    return countryCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::SetNetworkIdentityCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoCell::SetNetworkIdentityCode( TNetworkIdentity aNetworkId )
    {
    TInt networkCode ;
    TLex valLex( aNetworkId );
    TInt error = valLex.Val( networkCode );
     
    if( networkCode<= 0 || error!=KErrNone  )
   
   	    User::Panic(KLbtClientPanicCategory,ELbtErrArgument);	
   	    
    iNetworkId = networkCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::NetworkIdentityCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TNetworkIdentity CLbtGeoCell::NetworkIdentityCode() const
    {
    TNetworkIdentity networkCode;
    networkCode.AppendNum( iNetworkId );
    
    return networkCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::SetLocationAreaCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoCell::SetLocationAreaCode(TUint aLocationAreaCode)
    {
    if( aLocationAreaCode == 0 )

		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
		
    iLocationAreaCode = aLocationAreaCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::LocationAreaCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CLbtGeoCell::LocationAreaCode() const
    {
    return iLocationAreaCode;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::SetCellId()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CLbtGeoCell::SetCellId( TUint aCellId )    
    {
    if( aCellId == 0 )

		User::Panic(KLbtClientPanicCategory,ELbtErrArgument);
		
    iCellId = aCellId;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCell::CellId()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TUint CLbtGeoCell::CellId() const
    {
    return iCellId;
    }

// ---------------------------------------------------------------------------
// CLbtGeoCell::CGISpecifiedDuringCreation()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CLbtGeoCell::CGISpecifiedDuringCreation() const
    {
    return ETrue;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::DoExternalizeL( RWriteStream& aStream ) const
    {
    RMobilePhone::TMobilePhoneNetworkMode networkType = NetworkType();
    if( networkType == RMobilePhone::ENetworkModeGsm )
        aStream.WriteUint8L( 1 );
    else if( networkType == RMobilePhone::ENetworkModeWcdma )
        aStream.WriteUint8L( 2 );
    else if( networkType == RMobilePhone::ENetworkModeTdcdma)
        aStream.WriteUint8L( 3 );
    else 
        aStream.WriteUint8L( 0 );
    
    aStream.WriteUint32L( iCountryCode );
	aStream.WriteUint32L(iNetworkId );
	aStream.WriteUint32L( iLocationAreaCode );
	aStream.WriteUint32L( iCellId );
	}    
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::DoInternalizeL( RReadStream& aStream ) 
    {
    TInt networkType = aStream.ReadUint8L();
    if( networkType == 1 )
        iNetworkType = RMobilePhone::ENetworkModeGsm;
    else if ( networkType == 2 )
        iNetworkType = RMobilePhone::ENetworkModeWcdma;
    else if ( networkType == 3 )
        iNetworkType = RMobilePhone::ENetworkModeTdcdma;
    else 
        iNetworkType = RMobilePhone::ENetworkModeUnknown;
       
    
    iCountryCode = aStream.ReadUint32L();
    iNetworkId = aStream.ReadUint32L();
    iLocationAreaCode = aStream.ReadUint32L();
    iCellId = aStream.ReadUint32L();
    
    }     


// ---------------------------------------------------------------------------
// CLbtGeoCell::ValidateCreateCellInformationL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::ValidateCreateCellInformationL()
    {
    if( ( iNetworkType == RMobilePhone::ENetworkModeUnknown ) || !iCountryCode 
        || !iNetworkId || !iLocationAreaCode || !iCellId )
        User::Leave( KErrArgument );
    }
    
    
// ---------------------------------------------------------------------------
// CLbtGeoCell::ValidateCellInformationL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CLbtGeoCell::ValidateCellInformationL()
    {
    if( iNetworkType != RMobilePhone::ENetworkModeGsm && iNetworkType != RMobilePhone::ENetworkModeWcdma
        && iNetworkType != RMobilePhone::ENetworkModeTdcdma )
        User::Leave( KErrArgument );
    
    if( iCellId && (!iLocationAreaCode || !iNetworkId || !iCountryCode ) )
        User::Leave( KErrArgument );
    
    if( iLocationAreaCode && ( !iNetworkId || !iCountryCode  ) )
        User::Leave( KErrArgument );
    
    if( iNetworkId && !iCountryCode )
        User::Leave( KErrArgument );
    }
    
// end of file

