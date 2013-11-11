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
* Description:  Header file for location id information
*
*/

// INCLUDE FILES
#include "epos_comasupllocationid.h"


// ============================ MEMBER FUNCTIONS OF COMASuplGSMCellInfo ===============================
// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplGSMCellInfo* COMASuplGSMCellInfo::NewL()
    {
    COMASuplGSMCellInfo* self = new( ELeave ) COMASuplGSMCellInfo;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::~COMASuplGSMCellInfo() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplGSMCellInfo::~COMASuplGSMCellInfo()
    {
    }

// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::SuplGSMCellInfo()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplGSMCellInfo::SuplGSMCellInfo(TInt& aRefMNC,TInt& aRefMcc,TInt& aRefCI,TInt& aRefLAC) const 
{
	SuplCellInfo(aRefMNC,aRefMcc,aRefCI);
	aRefLAC = iRefLAC; 		
}
// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::SeCSuplGSMCellInfo()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplGSMCellInfo::SetSuplGSMCellInfo(TInt aRefMNC,TInt aRefMcc,TInt aRefCI,TInt aRefLAC) 
{
	SetSuplCellInfo(aRefMNC,aRefMcc,aRefCI);
	iRefLAC = aRefLAC; 		
}
// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::deafult constructor() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C  COMASuplGSMCellInfo::COMASuplGSMCellInfo()
	{		
	}
// -----------------------------------------------------------------------------
// COMASuplGSMCellInfo::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplGSMCellInfo::ConstructL()
    {
  	 iRefLAC = -1;
    }


// ============================ MEMBER FUNCTIONS OF COMASuplCellInfo ===============================

// -----------------------------------------------------------------------------
// COMASuplCellInfo::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplCellInfo* COMASuplCellInfo::NewL()
    {
    COMASuplCellInfo* self = new( ELeave ) COMASuplCellInfo;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplCellInfo::~COMASuplCellInfo() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplCellInfo::~COMASuplCellInfo()
    {
    }
// -----------------------------------------------------------------------------
// COMASuplCellInfo::SuplWCDMACellInfo()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplCellInfo::SuplCellInfo(TInt& aRefMNC,
										  TInt& aRefMCC,
										  TInt& aRefCI)const
{
	aRefMNC = iRefMNC;	
	aRefMCC = iRefMCC; 		
	aRefCI  = iRefCI; 		
}
// -----------------------------------------------------------------------------
// COMASuplCellInfo::SeCSuplCellInfo()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplCellInfo::SetSuplCellInfo(TInt aRefMNC,
											 TInt aRefMCC,
											 TInt aRefCI)												    
{
	iRefMNC = aRefMNC;	
	iRefMCC = aRefMCC; 		
	iRefCI  = aRefCI; 		
}
// -----------------------------------------------------------------------------
// COMASuplCellInfo::GetCellId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplCellInfo::GetCellId()
    {
    return iRefCI;
    }
// -----------------------------------------------------------------------------
// COMASuplCellInfo::deafult constructor() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C  COMASuplCellInfo::COMASuplCellInfo()
	{		
	}
// -----------------------------------------------------------------------------
// COMASuplCellInfo::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplCellInfo::ConstructL()
    {
  	 iRefMNC = -1;	
	 iRefMCC = -1;
	 iRefCI  = -1; 	   
    }
    

// ============================ MEMBER FUNCTIONS OF COMASuplLocationId ===============================
   
// -----------------------------------------------------------------------------
// COMASuplCellInfo::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplLocationId* COMASuplLocationId::NewL()
    {
    COMASuplLocationId* self = new( ELeave ) COMASuplLocationId;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplLocationId::~COMASuplLocationId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplLocationId::~COMASuplLocationId()
    {
    if(iCellInfo)
	    {
	    delete 	iCellInfo;
	    iCellInfo = NULL;
	    }
    }

// -----------------------------------------------------------------------------
// COMASuplLocationId::SuplLocationId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt  COMASuplLocationId::SuplLocationId(COMASuplCellInfo*& aCellInfo,
											   TOMASuplStatus& aStatus) const
{
 	if(EWCDMA == iChoice)
	 	{
	 	aCellInfo = iCellInfo;
	 	aStatus = iStatus;	
	 	return KErrNone;
	 	}
	else
		{
		return KErrArgument;
		}			
}
// -----------------------------------------------------------------------------
// COMASuplLocationId::SuplLocationId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplLocationId::SuplLocationId(COMASuplGSMCellInfo*& aCellInfo,
											  TOMASuplStatus& aStatus) const	
{
 	if(EGSM == iChoice)
	 	{
	 	aCellInfo = (COMASuplGSMCellInfo*)iCellInfo;
	 	aStatus = iStatus;	
	 	return KErrNone;
	 	}
	else
		{
		return KErrArgument;
		}		
}	

// -----------------------------------------------------------------------------
// COMASuplCellInfo::SuplCellInfoType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C COMASuplLocationId::TOMASuplCellInfoType COMASuplLocationId::SuplCellInfoType()const
{
 		return iChoice;
}
// -----------------------------------------------------------------------------
// COMASuplLocationId::SetSuplLocationId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void  COMASuplLocationId::SetSuplLocationId(COMASuplCellInfo* aCellInfo,
											      TOMASuplStatus& aStatus)
    {
    iChoice = EWCDMA;
    iCellInfo = aCellInfo;
    iStatus = aStatus;	

    }
// -----------------------------------------------------------------------------
// COMASuplLocationId::SetSuplLocationId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void  COMASuplLocationId::SetSuplLocationId(COMASuplGSMCellInfo* aCellInfo,
											      TOMASuplStatus& aStatus)
    {
    delete iCellInfo;
    iCellInfo = NULL;

    iChoice = EGSM;
    iCellInfo = aCellInfo;
    iStatus = aStatus;	
    }
// -----------------------------------------------------------------------------
// COMASuplLocationId::GetCellId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplLocationId::GetCellId()
    {
    if(iCellInfo)
        return iCellInfo->GetCellId();
    else
    	return KErrNotFound;
    }
// -----------------------------------------------------------------------------
// COMASuplLocationId::CloneL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplLocationId* COMASuplLocationId::CloneL()
    {
    COMASuplLocationId* retLocationId = COMASuplLocationId::NewL();
    CleanupStack::PushL(retLocationId);
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
// COMASuplLocationId::deafult constructor() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C  COMASuplLocationId::COMASuplLocationId()
	{		
	}
// -----------------------------------------------------------------------------
// COMASuplCellInfo::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplLocationId::ConstructL()
    {
    iStatus = EUnknown;
    iChoice = EWCDMA;
    }

//  End of File  
