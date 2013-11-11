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
* Description:  Header file for supl end message
*
*/


// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplend.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_comasuplvelocity.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplEnd===============================
    
// -----------------------------------------------------------------------------
// COMASuplEnd::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplEnd* COMASuplEnd::NewL()
    {
    COMASuplEnd* self = new( ELeave ) COMASuplEnd;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplEnd::~COMASuplEnd() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplEnd::~COMASuplEnd()
    {
	if(iPosition)
		{
		delete iPosition;
		iPosition = NULL;
		}
	
    }
// -----------------------------------------------------------------------------
// COMASuplEnd::Position()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplEnd::Position(COMASuplPosition*& aPosition)const
{
	if(iOptionalMask & (1<<(0)))
		{
		aPosition = iPosition;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplEnd::SetPosition() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplEnd::SetPosition(COMASuplPosition* aPosition)
{	
	delete iPosition;
	iPosition = NULL;
	iPosition = aPosition;
	if(iPosition)
		{
		iOptionalMask |= LEFTSHIFT(0);		
		}	
}	
// -----------------------------------------------------------------------------
// COMASuplEnd::StatusCode()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplEnd::StatusCode(TOMASuplStatusCode& aStatusCode)const
{
	if(iOptionalMask & (1<<(1)))
		{
		aStatusCode = iStatusCode;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplEnd::SetStatusCode() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplEnd::SetStatusCode(TOMASuplStatusCode aStatusCode)
{
	iStatusCode = aStatusCode;
	iOptionalMask |= LEFTSHIFT(1);
}	

// -----------------------------------------------------------------------------
// COMASuplEnd::StatusCode()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplEnd::Ver(TDes8& aVer)const
{
	if(iOptionalMask & (1<<(2)))
		{
		if(aVer.MaxLength() >= iVer.Length())
		    {
		    aVer.Copy(iVer);
		    return KErrNone;
		    }
		return KErrArgument;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   
// -----------------------------------------------------------------------------
// COMASuplEnd::SetStatusCode() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplEnd::SetVer(TDesC8& aVer)
    {
    if(aVer.Length() <= iVer.MaxLength())
        {
        iVer.Copy(aVer);
        iOptionalMask |= LEFTSHIFT(2);
        }
    }  
// -----------------------------------------------------------------------------
// COMASuplEnd::Clone() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplEnd::Clone(const COMASuplAsnMessageBase* aEnd,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aEnd,aErrorCode));
	if(aErrorCode)
	 	return;
	COMASuplEnd* suplEnd = (COMASuplEnd*)aEnd;
	iOptionalMask = suplEnd->iOptionalMask;
	if(suplEnd->iOptionalMask & (1<<(1)))
		{
		iStatusCode = suplEnd->iStatusCode;	
		}
	if(suplEnd->iOptionalMask & (1<<(2)))
		{
		iVer.Copy(suplEnd->iVer);
		}
	if(suplEnd->iOptionalMask & (1<<(0)))
		{
		delete iPosition;
		iPosition = NULL;
		TRAP(aErrorCode,iPosition =static_cast <COMASuplPosition *> (suplEnd->iPosition->CloneL()));
		}
}						  			   
// -----------------------------------------------------------------------------
// COMASuplEnd::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplEnd::ConstructL()
    {
    iOptionalMask = 0;
    iStatusCode = EUnspecified;
    SetMessageType(COMASuplAsnMessageBase::ESUPL_END);
    }
// -----------------------------------------------------------------------------
// COMASuplEnd::COMASuplEnd() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplEnd::COMASuplEnd()
	{
		
	}


