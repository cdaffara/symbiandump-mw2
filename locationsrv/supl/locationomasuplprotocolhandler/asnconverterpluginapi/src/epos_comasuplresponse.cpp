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
* Description:  Header file for supl response message
*
*/


// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplresponse.h"
#include "epos_comasuplsessionid.h"
#include  "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================
    
// -----------------------------------------------------------------------------
// COMASuplResponse::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponse* COMASuplResponse::NewL()
    {
    COMASuplResponse* self = new( ELeave ) COMASuplResponse;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplResponse::~COMASuplResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplResponse::~COMASuplResponse()
    {
    
	if(iSLPAddress)
		{
		delete iSLPAddress;
		iSLPAddress = NULL;
		}    
	}
 
// -----------------------------------------------------------------------------
// COMASuplResponse::SuplResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TOMASuplPosMethod COMASuplResponse::SuplResponse()const
{
	return iPosMethod;
}						  			   

// -----------------------------------------------------------------------------
// COMASuplResponse::SetSuplResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplResponse::SetSuplResponse(TOMASuplPosMethod aPosMethod)
{
	iPosMethod = aPosMethod;
	iPosMethodSet = ETrue;
}						  			   

// -----------------------------------------------------------------------------
// COMASuplResponse::SLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplResponse::SLPAddress(COMASuplSLPAddress*& aAddress)const
{
	if(iOptionalMask & (1<<(1)))
		{
		aAddress = iSLPAddress;
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplResponse::SetSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplResponse::SetSLPAddress(COMASuplSLPAddress* aAddress)
{	
	delete iSLPAddress;
	iSLPAddress = NULL;
	iSLPAddress = aAddress;
	if(iSLPAddress)
		{
		iOptionalMask |= LEFTSHIFT(1);		
		}	
}	

// -----------------------------------------------------------------------------
// COMASuplResponse::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplResponse::Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aResponse,aErrorCode));
	if(aErrorCode)
	 	return;
	COMASuplResponse* response = (COMASuplResponse*)aResponse;
	iPosMethod = response->iPosMethod;
	iOptionalMask = response->iOptionalMask;
	
	if(iOptionalMask & (1<<(1)))
		{
		TRAP(aErrorCode,CloneSLPAddressL(response));
		}
}						  			   
   
// -----------------------------------------------------------------------------
// COMASuplResponse::COMASuplResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponse::COMASuplResponse()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplResponse::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponse::ConstructL()
    {
    SetMessageType(COMASuplAsnMessageBase::ESUPL_RESPONSE);
    iPosMethodSet = EFalse;
    iOptionalMask = 0;
    }
// -----------------------------------------------------------------------------
// COMASuplResponse::CloneSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 	
void COMASuplResponse::CloneSLPAddressL(const COMASuplResponse* aSuplResponse)
	{
	TBuf8<KIPv6Address>  lIPvAddress;
	
	COMASuplSLPAddress* cloneSLPAddress =  NULL;
	
	delete iSLPAddress;
	iSLPAddress = NULL;
	iSLPAddress = COMASuplSLPAddress::NewL();
	
	aSuplResponse->SLPAddress(cloneSLPAddress);
	COMASuplSLPAddress::TSuplSLPAddressType lChoice = cloneSLPAddress->SLPAddressType();

	switch(lChoice)
	    {
	    case COMASuplSLPAddress::EIPv4Address: 
	        {
	        cloneSLPAddress->IPvAddress(lIPvAddress);
	        iSLPAddress->SetIPv4Address(lIPvAddress);
	        break;				
	        }
	    case COMASuplSLPAddress::EIPv6Address: 
	        {
	        cloneSLPAddress->IPvAddress(lIPvAddress);

	        iSLPAddress->SetIPv6Address(lIPvAddress);
	        break;	
	        }
	    case COMASuplSLPAddress::EFqdn: 	
	        {
	        HBufC8* cloneFqdn = NULL;
	        cloneSLPAddress->Fqdn(cloneFqdn);

	        HBufC8* fqdn = HBufC8::NewL(cloneFqdn->Length());

	        *fqdn = *cloneFqdn;
	        iSLPAddress->SetFqdn(fqdn);

	        break;	
	        }
	    }
	}

	
