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

#include "epos_comasuplauthresponse.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================
    
// -----------------------------------------------------------------------------
// COMASuplAuthResponse::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAuthResponse* COMASuplAuthResponse::NewL()
    {
    COMASuplAuthResponse* self = new( ELeave ) COMASuplAuthResponse;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplAuthResponse::~COMASuplAuthResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplAuthResponse::~COMASuplAuthResponse()
    {
	iSPCAuthKey.Close();
	iKeyIdentity3.Close();
	}
 

// -----------------------------------------------------------------------------
// COMASuplAuthResponse::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplAuthResponse::Clone(const COMASuplAsnMessageBase* aAuthResponse,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aAuthResponse,aErrorCode));
	COMASuplAuthResponse* suplAuthResponse = (COMASuplAuthResponse*)aAuthResponse;
	iSPCAuthKey.Copy(suplAuthResponse->iSPCAuthKey); 
	iKeyIdentity3.Copy(suplAuthResponse->iKeyIdentity3); 
}						  			   
   
// -----------------------------------------------------------------------------
// COMASuplAuthResponse::COMASuplAuthResponse() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAuthResponse::COMASuplAuthResponse()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplAuthResponse::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAuthResponse::ConstructL()
    {
		iKeyIdentity3.CreateL(KMaxKeyIdentityLength);
		iSPCAuthKey.CreateL(KMaxLongSPCKey);
		
		SetMessageType(COMASuplAsnMessageBase::ESUPL_AUTH_RESP);
    }

// -----------------------------------------------------------------------------
// COMASuplAuthRequest::SetKeyIdentity3()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::SetKeyIdentity3(TDesC8& aKeyIdentity3)
{
	if(aKeyIdentity3.Length() <= KMaxKeyIdentityLength)
		{
			iOptionalMask |= LEFTSHIFT(2);		 
			iKeyIdentity3.Copy(aKeyIdentity3);
			return KErrNone;
		}
	else
		{
			return KErrArgument;
		}	
} 

// -----------------------------------------------------------------------------
// COMASuplAuthResponse::SetShortSPCKey()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::SetShortSPCKey(TDesC8& aSPCShortKey)
{
	if(aSPCShortKey.Length() <= KMaxKeyIdentityLength)
		{
			iOptionalMask |= LEFTSHIFT(0);		 
			iSPCAuthKey.Copy(aSPCShortKey);
			return KErrNone;
		}
	else
		{
			return KErrArgument;
		}	
}  

// -----------------------------------------------------------------------------
// COMASuplAuthResponse::SetLongSPCKey()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::SetLongSPCKey(TDesC8& aSPCLongKey)
{
	if(aSPCLongKey.Length() <= KMaxLongSPCKey)
		{
			iOptionalMask |= LEFTSHIFT(1);		 
			iSPCAuthKey.Copy(aSPCLongKey);
			return KErrNone;
		}
	else
		{
			return KErrArgument;
		}	
}  

// -----------------------------------------------------------------------------
// COMASuplAuthRequest::KeyIdentity3()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::KeyIdentity3(TDes8& aKeyIdentity3) const
{
	if(aKeyIdentity3.MaxLength() <= KMaxKeyIdentityLength)
		{
			if(iOptionalMask & (1<<(2)))
			{
			   aKeyIdentity3.Copy(iKeyIdentity3);
			  return KErrNone;
			}
			else
			{
				return KErrOMASuplParamNotSet;			
			}
		}
		return KErrArgument;
} 

// -----------------------------------------------------------------------------
// COMASuplAuthResponse::ShortSPCKey()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::ShortSPCKey(TDes8& aSPCShortKey) const
{
	if(aSPCShortKey.MaxLength() <= KMaxKeyIdentityLength)
 		{
			if(iOptionalMask & (1<<(0)))
			{
			   aSPCShortKey.Copy(iSPCAuthKey);
			   return KErrNone;
			}
			else
			{
				return KErrOMASuplParamNotSet;			
			}
		}
		
	return KErrArgument;

}  

// -----------------------------------------------------------------------------
// COMASuplAuthResponse::LongSPCKey()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplAuthResponse::LongSPCKey(TDes8& aSPCLongKey ) const
{
	if(aSPCLongKey.MaxLength() <= KMaxKeyIdentityLength)
 		{
			if(iOptionalMask & (1<<(1)))
			{
				  aSPCLongKey.Copy(iSPCAuthKey);
				  return KErrNone;
			}
			else
			{
	    			return KErrOMASuplParamNotSet;			
			}
		}
	return KErrArgument;

}  
