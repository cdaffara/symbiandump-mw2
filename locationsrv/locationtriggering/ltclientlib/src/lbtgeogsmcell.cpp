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
* Description:  Definition of geo gsm cell class.
*
*/


#include "lbtgeogsmcell.h"


// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoGsmCell* CLbtGeoGsmCell::NewLC()
    {
    CLbtGeoGsmCell* self = CLbtGeoGsmCell::NewL();
    CleanupStack::PushL( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoGsmCell* CLbtGeoGsmCell::NewL()
    {
    CLbtGeoGsmCell* self = new( ELeave ) CLbtGeoGsmCell;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }    
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoGsmCell* CLbtGeoGsmCell::NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                        RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
                                        RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
                                        TUint aLocationAreaCode,
                                        TUint aCellId)
    {
    CLbtGeoGsmCell* self = CLbtGeoGsmCell::NewLC(aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,
                                                 aCellId);
    CleanupStack::Pop( self );
    return self;
    }  
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoGsmCell* CLbtGeoGsmCell::NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                         RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
                                         RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
                                         TUint aLocationAreaCode,
                                         TUint aCellId)
    {
    CLbtGeoGsmCell* self = new( ELeave ) CLbtGeoGsmCell();
    CleanupStack::PushL( self );
    self->ConstructL( aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,aCellId);
    return self;
    }     
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::CLbtGeoGsmCell()
//
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtGeoGsmCell::CLbtGeoGsmCell()
    {
                                 
    }   
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::~CLbtGeoGsmCell()
//
// Destructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoGsmCell::~CLbtGeoGsmCell()
    {
    iBsicArray.Close();
    }  

     
    
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::SetBsic()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtGeoGsmCell::SetBsic( RArray<TInt>& aBsicArray )
    {
    for( TInt i=0;i<aBsicArray.Count();i++ )
        {
        TInt error = iBsicArray.Append( aBsicArray[i] );
        if( error != KErrNone )
            {
            return;
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::Bsic()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C RArray<TInt>&  CLbtGeoGsmCell::Bsic()
    {
    return iBsicArray;
    }    

// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::CGISpecifiedDuringCreation()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtGeoGsmCell::CGISpecifiedDuringCreation() const
    {
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CLbtGeoGsmCell::DoExternalizeL(RWriteStream& aStream) const 
    {
    CLbtGeoCell::DoExternalizeL( aStream );
    TInt count = iBsicArray.Count();
    aStream.WriteInt32L( count ); 
    for( TInt i=0;i<count;i++ )
        {
	    aStream.WriteInt32L( iBsicArray[i] );
	    }
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoGsmCell::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CLbtGeoGsmCell::DoInternalizeL(RReadStream& aStream)
    {
    CLbtGeoCell::DoInternalizeL( aStream );
    TInt count = aStream.ReadInt32L();
    for( TInt i=0;i<count;i++ )
        {
        iBsicArray.AppendL( aStream.ReadInt32L() );
        }
    }
    
    
