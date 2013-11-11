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
* Description:  Processing for supl auth response message
*
*/


// INCLUDE FILES

#include <e32def.h>
#include "epos_comasuplauthrequest.h"
#include "epos_comasuplsessionid.h"
#include "epos_suplconstants.h"


// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================
    
// -----------------------------------------------------------------------------
// COMASuplAuthRequest::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAuthRequest* COMASuplAuthRequest::NewL()
    {
    COMASuplAuthRequest* self = new( ELeave ) COMASuplAuthRequest;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplAuthRequest::~COMASuplAuthRequest() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplAuthRequest::~COMASuplAuthRequest()
    {
    	iKeyIdentity.Close();
    	iSETNounce.Close();
	}
// -----------------------------------------------------------------------------
// COMASuplAuthRequest::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplAuthRequest::Clone(const COMASuplAsnMessageBase* aSuplAuthRequest,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aSuplAuthRequest,aErrorCode));
	if(aErrorCode)
	 	return;
	
	COMASuplAuthRequest* suplAuthRequest = (COMASuplAuthRequest*)aSuplAuthRequest;
	iSETNounce.Copy(suplAuthRequest->iSETNounce); 
	iKeyIdentity.Copy(suplAuthRequest->iKeyIdentity); 
}						  			   
   
// -----------------------------------------------------------------------------
// COMASuplAuthRequest::COMASuplAuthRequest() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAuthRequest::COMASuplAuthRequest()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplAuthRequest::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAuthRequest::ConstructL()
    {
		iSETNounce.CreateL(KMaxKeyIdentityLength);
		iKeyIdentity.CreateL(KMaxKeyIdentityLength);
		SetMessageType(COMASuplAsnMessageBase::ESUPL_AUTH_REQ);
    }

// -----------------------------------------------------------------------------
// COMASuplAuthRequest::SetSETNounce()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthRequest::SetSETNounce(TDesC8& aSETNounce)
{
	if(aSETNounce.Length() <= KMaxKeyIdentityLength)
		{
			iSETNounce.Copy(aSETNounce);
			return KErrNone;
		}
	else
		{
			return KErrArgument;
		}	
}  

// -----------------------------------------------------------------------------
// COMASuplAuthRequest::SetKeyIdentity()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthRequest::SetKeyIdentity(TDesC8& aKeyIdentity)
{
	if(aKeyIdentity.Length() <= KMaxKeyIdentityLength)
		{
			iKeyIdentity.Copy(aKeyIdentity);
			return KErrNone;
		}
	else
		{
			return KErrArgument;
		}	
} 
 
