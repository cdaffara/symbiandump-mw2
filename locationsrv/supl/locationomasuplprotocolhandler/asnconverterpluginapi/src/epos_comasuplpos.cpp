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
* Description:  Header file for supl pos message
*
*/

// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplpos.h"
#include "epos_suplconstants.h"
#include "lbs/epos_comasuplvelocity.h"
#include "lbs/epos_comasuplpospayload.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplPos===============================
// -----------------------------------------------------------------------------
// COMASuplPos::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPos* COMASuplPos::NewL()
    {
    COMASuplPos* self = new( ELeave ) COMASuplPos;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplPos::~COMASuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplPos::~COMASuplPos()
    {
	delete iPayload;
	delete iVelocity;
    }

// -----------------------------------------------------------------------------
// COMASuplPos::SuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C COMASuplPosPayload* COMASuplPos::SuplPos()const
    {
    return iPayload;
    }						  			   

// -----------------------------------------------------------------------------
// COMASuplPos::SetSuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPos::SetSuplPos(COMASuplPosPayload* aPosPayload)
    {	
    delete iPayload;
    iPayload = NULL;
    iPayload = aPosPayload;
    }	

// -----------------------------------------------------------------------------
// COMASuplPos::Velocity() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplPos::Velocity(COMASuplVelocity*& aVelocity)const
{
    if(iOptionalMask & (1<<(0)))
		{
		aVelocity = iVelocity;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPos::SetVelocity() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPos::SetVelocity(COMASuplVelocity* aVelocity)
{	
	delete iVelocity;
	iVelocity = NULL;
	iVelocity = aVelocity;
	if(iVelocity)
		{
		iOptionalMask |= LEFTSHIFT(0);		
		}	
}				   

// -----------------------------------------------------------------------------
// COMASuplPos::Clone() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPos::Clone(const COMASuplAsnMessageBase* aSuplPos,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aSuplPos,aErrorCode));
	if(aErrorCode)
	 	return;
	COMASuplPos* suplPos = (COMASuplPos*)aSuplPos;
	
	iOptionalMask = suplPos->iOptionalMask;
	
	if(suplPos->iPayload)
	    TRAP(aErrorCode,iPayload =static_cast <COMASuplPosPayload *> (suplPos->iPayload->CloneL()));
	if(aErrorCode)
		{
		return;
		}
	if(iOptionalMask & (1<<(0)))
		{
		delete iVelocity;
		iVelocity = NULL;
		if(suplPos->iVelocity)
		    TRAP(aErrorCode,iVelocity =static_cast <COMASuplVelocity *> (suplPos->iVelocity->CloneL()));
		}
}		
    
// -----------------------------------------------------------------------------
// COMASuplPos::COMASuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPos::COMASuplPos()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplPos::COMASuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPos::ConstructL()
    {
    SetMessageType(COMASuplAsnMessageBase::ESUPL_POS);
    }
    
