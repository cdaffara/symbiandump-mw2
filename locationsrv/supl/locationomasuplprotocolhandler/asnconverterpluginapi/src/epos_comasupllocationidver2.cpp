/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Header file for supl 2.0 location id  message
*
*/

// INCLUDE FILES
#include "epos_comasupllocationid.h"
#include "epos_comasupllocationidver2.h"
#include "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS OF COMASuplLocationIdVer2 ===============================
   
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplLocationIdVer2* COMASuplLocationIdVer2::NewL()
    {
    COMASuplLocationIdVer2* self = new( ELeave ) COMASuplLocationIdVer2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::~COMASuplLocationIdVer2()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplLocationIdVer2::~COMASuplLocationIdVer2()
    {
    }

// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::deafult constructor() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplLocationIdVer2::COMASuplLocationIdVer2()
	{		
	iIsV2ExtnSet = EFalse;
	}
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplLocationIdVer2::ConstructL()
    {
    COMASuplLocationId::ConstructL();            
    }
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::Ver2CellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplLocationIdVer2::Ver2CellInfo(TOMASuplVer2CellInfoExtn& aTOMASuplVer2CellInfo) const
    {
    if(iIsV2ExtnSet)
        {
        aTOMASuplVer2CellInfo = iOMASuplVer2CellInfoExtn;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::CloneL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplLocationIdVer2* COMASuplLocationIdVer2::CloneL()
    {
    COMASuplLocationIdVer2* retLocationId = COMASuplLocationIdVer2::NewL();
    CleanupStack::PushL( retLocationId );
    if(iIsV2ExtnSet)
        {
        retLocationId->COMASuplLocationIdVer2::SetVer2CellInfo(iOMASuplVer2CellInfoExtn);
        }
    TInt refMNC,refMCC,refCI,refLac;
    
    switch(iChoice)
        {
        case EGSM:
            {
            ((COMASuplGSMCellInfo*)iCellInfo)->SuplGSMCellInfo(refMNC,refMCC,refCI,refLac);
            COMASuplGSMCellInfo* cellInfo = COMASuplGSMCellInfo::NewL();
            cellInfo->SetSuplGSMCellInfo(refMNC,refMCC,refCI,refLac);
            retLocationId->SetSuplLocationId(cellInfo,iStatus);
            }
            break;
        case EWCDMA:
            {
            iCellInfo->SuplCellInfo(refMNC,refMCC,refCI);
            COMASuplCellInfo* cellInfo = COMASuplCellInfo::NewL();
            cellInfo->SetSuplCellInfo(refMNC,refMCC,refCI);
            retLocationId->SetSuplLocationId(cellInfo,iStatus);
            }
            break;
                    
        }
    CleanupStack::Pop(retLocationId);
    return retLocationId;
    }
// -----------------------------------------------------------------------------
// COMASuplLocationIdVer2::SetVer2CellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplLocationIdVer2::SetVer2CellInfo(TOMASuplVer2CellInfoExtn aTOMASuplVer2CellInfo)
    {
    iChoice = EWLAN;
    iOMASuplVer2CellInfoExtn = aTOMASuplVer2CellInfo;
    }

// -----------------------------------------------------------------------------
// TOMASuplReportedLocation::GetLocationData(TOMASuplLocationData &aLocationData)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplReportedLocation::GetLocationData(TOMASuplLocationData &aLocationData) const
    {
    aLocationData = iLocationData;
    }
// ------------------------------------------------
// TOMASuplReportedLocation::GetLocationEncDescriptor
// (other items were commented in a header).
// ------------------------------------------------
//
EXPORT_C void TOMASuplReportedLocation::GetLocationEncDescriptor(TOMASuplLocEncDescriptor&   aLocationEncodingDescriptor) const
    {
    aLocationEncodingDescriptor = iLocationEncodingDescriptor;
    }
// ---------------------------------------------------------------------------
// TOMASuplReportedLocation::SetLocationData(TOMASuplLocationData aLocationData)
// (other items were commented in a header).
// ---------------------------------------------------------------------------

//coverity[pass_by_value]
EXPORT_C void TOMASuplReportedLocation::SetLocationData(TOMASuplLocationData aLocationData) 
    {
    iLocationData = aLocationData;
    }
// ---------------------------------------------------------------------------
// TOMASuplReportedLocation::SetLocationEncDescriptor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplReportedLocation::SetLocationEncDescriptor(TOMASuplLocEncDescriptor aLocationEncodingDescriptor) 
    {
    iLocationEncodingDescriptor = aLocationEncodingDescriptor;
    }
// ---------------------------------------------------------------------------
// TOMASuplLocationData::TOMASuplLocationData
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TOMASuplLocationData::TOMASuplLocationData()
    {
    iIsLocationAccSet = EFalse;
    }

// ---------------------------------------------------------------------------
// TOMASuplLocationData::GetLocationAccuracy
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplLocationData::GetLocationAccuracy(TInt &aLocationAccuracy) const
    {
    if(iIsLocationAccSet)
        {
        aLocationAccuracy = iLocationAccuracy;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// ---------------------------------------------------------------------------
// TOMASuplLocationData::GetLocationValue
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplLocationData::GetLocationValue(TDes8& aLocationValue) const
    {
    if(aLocationValue.MaxLength() >= iLocationValue.Length())
        aLocationValue.Copy(iLocationValue);
    }
// ---------------------------------------------------------------------------
// TOMASuplLocationData::SetLocationAccuracy
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplLocationData::SetLocationAccuracy(const TInt aLocationAccuracy) 
    {
    iIsLocationAccSet = EFalse;
    iLocationAccuracy = aLocationAccuracy;
    }
// ---------------------------------------------------------------------------
// TOMASuplLocationData::SetLocationValue
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplLocationData::SetLocationValue(TDes8& aLocationValue) 
    {
    if(iLocationValue.MaxLength() >= aLocationValue.Length())
        iLocationValue.Copy( aLocationValue);
    }

// ---------------------------------------------------------------------------
// TOMASuplVer2CellInfoExtn::TOMASuplVer2CellInfoExtn
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TOMASuplVer2CellInfoExtn::TOMASuplVer2CellInfoExtn()
    {
    iTypePresent = EOMASuplCellInfoUnknown;
    }
// ---------------------------------------------------------------------------
// TOMASuplVer2CellInfoExtn::GetCellInfoExtnType
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVer2CellInfoExtn::GetCellInfoExtnType(TOMASuplVer2CellInfoExtn::TOMASuplCellInfoExtnType &aTypePresent) const
    {
    aTypePresent = iTypePresent;
    }
// ---------------------------------------------------------------------------
// TOMASuplVer2CellInfoExtn::SetCellInfoExtnType
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVer2CellInfoExtn::SetCellInfoExtnType(const TOMASuplVer2CellInfoExtn::TOMASuplCellInfoExtnType aTypePresent) 
    {
    iTypePresent = aTypePresent;
    }


//  End of File  
