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
* Description:  Definition of geo WCDMA cell class.
*
*/


#include "lbtgeowcdmacell.h"

EXPORT_C TLbtWcdmaCellInfo::~TLbtWcdmaCellInfo()
	{
	iMeasurementResult.Close();
	}


// ================= LOCAL FUNCTIONS ========================
 
// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoWcdmaCell* CLbtGeoWcdmaCell::NewLC()
    {
    CLbtGeoWcdmaCell* self = CLbtGeoWcdmaCell::NewL();
    CleanupStack::PushL( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoWcdmaCell* CLbtGeoWcdmaCell::NewL()
    {
    CLbtGeoWcdmaCell* self = new( ELeave ) CLbtGeoWcdmaCell;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }    
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::NewL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoWcdmaCell* CLbtGeoWcdmaCell::NewL(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                        RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
                                        RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
                                        TUint aLocationAreaCode,
                                        TUint aCellId)
    {
    CLbtGeoWcdmaCell* self = CLbtGeoWcdmaCell::NewLC(aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,
                                                 aCellId);
    CleanupStack::Pop( self );
    return self;
    }  
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::NewLC()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoWcdmaCell* CLbtGeoWcdmaCell::NewLC(RMobilePhone::TMobilePhoneNetworkMode aNetworkType,
                                         RMobilePhone::TMobilePhoneNetworkCountryCode aCountryCode,
                                         RMobilePhone::TMobilePhoneNetworkIdentity aNetworkId,
                                         TUint aLocationAreaCode,
                                         TUint aCellId)
    {
    CLbtGeoWcdmaCell* self = new( ELeave ) CLbtGeoWcdmaCell();
    CleanupStack::PushL( self );
    self->ConstructL( aNetworkType,aCountryCode,aNetworkId,aLocationAreaCode,aCellId);
    return self;
    }     
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::CLbtGeoWcdmaCell()
//
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtGeoWcdmaCell::CLbtGeoWcdmaCell()
    {
                                 
    }   
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::~CLbtGeoWcdmaCell()
//
// Destructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CLbtGeoWcdmaCell::~CLbtGeoWcdmaCell()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::SetWcdmaCellInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CLbtGeoWcdmaCell::SetWcdmaCellInfo( const TLbtWcdmaCellInfo& aWcdmaCellInfo )
    {
    iCellInfo.iCell = aWcdmaCellInfo.iCell;
    for(TInt i=0;i<aWcdmaCellInfo.iMeasurementResult.Count();++i)
    	{
    	TLbtWcdmaCellInfo::TLbtCellMeasuredResult nmr = aWcdmaCellInfo.iMeasurementResult[i];
    	TInt error = iCellInfo.iMeasurementResult.Append(nmr);
    	if( error != KErrNone )
    	    {
            return;
    	    }
    	}    
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::WcdmaCellInfo()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C const TLbtWcdmaCellInfo& CLbtGeoWcdmaCell::WcdmaCellInfo()
    {
    return iCellInfo;
    }

// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::CGISpecifiedDuringCreation()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CLbtGeoWcdmaCell::CGISpecifiedDuringCreation() const
    {
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::DoExternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CLbtGeoWcdmaCell::DoExternalizeL(RWriteStream& aStream) const 
    {
    CLbtGeoCell::DoExternalizeL( aStream );
    aStream.WriteInt8L(iCellInfo.iCell.iCarrierRssi);
    aStream.WriteInt8L(iCellInfo.iCell.iEcNo);
    aStream.WriteInt8L(iCellInfo.iCell.iRscp);
    aStream.WriteInt32L(iCellInfo.iCell.iPathLoss);
    TInt count = iCellInfo.iMeasurementResult.Count();
    aStream.WriteInt32L( count ); 
    for( TInt i=0;i<count;i++ )
        {
        TLbtWcdmaCellInfo::TLbtCellMeasuredResult cellMeas = iCellInfo.iMeasurementResult[i];
	    aStream.WriteInt16L(cellMeas.iEcNo);
	    aStream.WriteInt16L(cellMeas.iRscp);
	    }
    }
    
// ---------------------------------------------------------------------------
// CLbtGeoWcdmaCell::DoInternalizeL()
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CLbtGeoWcdmaCell::DoInternalizeL(RReadStream& aStream)
    {
    CLbtGeoCell::DoInternalizeL( aStream );
    iCellInfo.iCell.iCarrierRssi = aStream.ReadInt8L();
    iCellInfo.iCell.iEcNo = aStream.ReadInt8L();
    iCellInfo.iCell.iRscp = aStream.ReadInt8L();
    iCellInfo.iCell.iPathLoss = aStream.ReadInt32L();
    TInt count = aStream.ReadInt32L();
    for( TInt i=0;i<count;i++ )
        {
        TLbtWcdmaCellInfo::TLbtCellMeasuredResult cellMeas;
        cellMeas.iEcNo = aStream.ReadInt16L();
        cellMeas.iRscp = aStream.ReadInt16L();
        iCellInfo.iMeasurementResult.AppendL(cellMeas);
        }
    }
