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
* Description:  Header file for supl Start message
*
*/


// INCLUDE FILES

#include <e32def.h>

#include "lbs/epos_comasuplsetcapabilities.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplstart.h"
#include "epos_suplconstants.h"
#include "epos_comasupllocationid.h"
#include  "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================
// -----------------------------------------------------------------------------
// COMASuplStart::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplStart* COMASuplStart::NewL()
    {
    COMASuplStart* self = new( ELeave ) COMASuplStart;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplStart::~COMASuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplStart::~COMASuplStart()
    {
    delete iSETCapabilities;
    delete iLocationId;
    }

#if 0
// -----------------------------------------------------------------------------
// COMASuplStart::SuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStart::SuplStart(COMASuplSETCapabilities*& aSuplSETCaps,
						  			   COMASuplLocationId*& aSuplLocationId,
						  			   TBool& aeCID)const
{
	aSuplSETCaps    = iSETCapabilities;
	aSuplLocationId = iLocationId;
	aeCID           = ieCID;
}						  			   
#endif

// -----------------------------------------------------------------------------
// COMASuplStart::Qop() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplStart::Qop(TOMASuplQop& aSuplQop)const
{
	if(iQopSet) 
		{
		aSuplQop    = iSuplQop;	
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}	

// -----------------------------------------------------------------------------
// COMASuplStart::SetSuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStart::SetSuplStart(COMASuplSETCapabilities* aSuplSETCaps,
						  			      COMASuplLocationId* aSuplLocationId,
						  			      TBool	aeCID)
{
	delete iLocationId;
	iLocationId = NULL;
		
	delete iSETCapabilities;
	iSETCapabilities = NULL;
	
	iSETCapabilities = aSuplSETCaps;
	iLocationId      = aSuplLocationId;
	ieCID            = aeCID;
}	

// -----------------------------------------------------------------------------
// COMASuplStart::SetQop() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStart::SetQop(TOMASuplQop& aSuplQop)
{
	iSuplQop = aSuplQop;
	iQopSet  = ETrue;
}					  
	  			   
// -----------------------------------------------------------------------------
// COMASuplStart::COMASuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplStart::COMASuplStart()
	{
	ieCID = EFalse;	
	iQopSet = EFalse;
	iOptionalMask = 0;
	}
// -----------------------------------------------------------------------------
// COMASuplStart::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplStart::ConstructL()
    {
    SetMessageType(COMASuplAsnMessageBase::ESUPL_START);
    }

// -----------------------------------------------------------------------------
// COMASuplStart::IntializeECellId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStart::SetECellId(TOMASuplECellInfo& aECId)
	{
		iMmCellInfo = aECId;
	}

// -----------------------------------------------------------------------------
// COMASuplStart::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStart::Clone(const COMASuplAsnMessageBase* aStart,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aStart,aErrorCode));
    
    
    COMASuplStart* start = (COMASuplStart*) aStart;
    COMASuplSETCapabilities* setcaps = NULL;
    if(start->iSETCapabilities)
        TRAP(aErrorCode,setcaps = static_cast <COMASuplSETCapabilities*> (start->iSETCapabilities->CloneL()));
    COMASuplLocationId* locationId = NULL;
    if(start->iLocationId)
        TRAP(aErrorCode,locationId = start->iLocationId->CloneL());
    SetSuplStart(setcaps,locationId,
            start->ieCID);
    
    if(start->ieCID)
        {
        SetECellId(start->iMmCellInfo);
        }

    SetQop(start->iSuplQop);

    }			

