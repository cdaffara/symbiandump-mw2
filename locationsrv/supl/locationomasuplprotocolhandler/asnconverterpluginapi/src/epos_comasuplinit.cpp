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
* Description:  Header file for supl init message
*
*/


// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplinit.h"
#include "epos_comasuplnotification.h"
#include "epos_comasuplsessionid.h"
#include "epos_suplconstants.h"
#include  "lbs/epos_eomasuplposerrors.h"

// ============================ MEMBER FUNCTIONS of COMASuplInit===============================

// -----------------------------------------------------------------------------
// COMASuplInit::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplInit* COMASuplInit::NewL()
    {
    COMASuplInit* self = new( ELeave ) COMASuplInit;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplInit::~COMASuplInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplInit::~COMASuplInit()
    {
    if(iSessionId)
	    {
	    delete iSessionId;
	    iSessionId = NULL;	
	    }
	if(iNotification)
		{
		delete iNotification;
		iNotification = NULL;
		}
	if(iSLPAddress)
		{
		delete iSLPAddress;
		iSLPAddress = NULL;
		}
    }
    
// -----------------------------------------------------------------------------
// COMASuplInit::SuplInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::SuplInit(TOMASuplPosMethod& aPosMethod,TOMASuplSLPMode& aSLPMode)const
{
	aPosMethod = iPosMethod;
	aSLPMode = iSLPMode;
}						  			   

// -----------------------------------------------------------------------------
// COMASuplInit::SetSuplInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::SetSuplInit(TOMASuplPosMethod aPosMethod,TOMASuplSLPMode aSLPMode)
{
	iPosMethod = aPosMethod;
	iSLPMode = aSLPMode;
}						  			   

// -----------------------------------------------------------------------------
// COMASuplInit::SLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplInit::SLPAddress(COMASuplSLPAddress*& aAddress)const
{
	if(iOptionalMask & (1<<(2)))
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
// COMASuplInit::SetSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::SetSLPAddress(COMASuplSLPAddress* aAddress)
{	
	delete iSLPAddress;
	iSLPAddress = NULL;
	iSLPAddress = aAddress;
	if(iSLPAddress)
		{
		iOptionalMask |= LEFTSHIFT(2);		
		}	
}						  			   
// -----------------------------------------------------------------------------
// COMASuplInit::Qop() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplInit::Qop(TOMASuplQop& aSuplQop)const
{
	if(iOptionalMask & (1<<(0)))
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
// COMASuplInit::SetQop() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::SetQop(TOMASuplQop& aSuplQop)
{
	iSuplQop = aSuplQop;
	iOptionalMask |= LEFTSHIFT(0);
}	
				  			   
// -----------------------------------------------------------------------------
// COMASuplInit::Notification() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplInit::Notification(COMASuplNotification*& aNotification)const
{
	if(iOptionalMask & (1<<(1)))
		{
		aNotification = iNotification;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplInit::SetNotification() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::SetNotification(COMASuplNotification* aNotification)
{
	delete iNotification;
	iNotification = NULL;
	iNotification = aNotification;
	if(iNotification)
	   iOptionalMask |= LEFTSHIFT(1);
}						  			   



// -----------------------------------------------------------------------------
// COMASuplResponse::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInit::Clone(const COMASuplAsnMessageBase* aInit,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aInit,aErrorCode));
	if(aErrorCode)
	 	return;
	COMASuplInit* suplInit = (COMASuplInit*)aInit;
	
	iPosMethod = suplInit->iPosMethod;
	iSLPMode = suplInit->iSLPMode;
	iOptionalMask = suplInit->iOptionalMask;
	
	if(iOptionalMask & (1<<(0)))
		{
		iSuplQop = suplInit->iSuplQop;
		}
	if(iOptionalMask & (1<<(1)))
		{
		TRAP(aErrorCode,CloneNotificationL(suplInit));
		if(aErrorCode)
			{
			return;
			}
		}
	if(iOptionalMask & (1<<(2)))
		{
		TRAP(aErrorCode,CloneSLPAddressL(suplInit));
		}
}	
// -----------------------------------------------------------------------------
// COMASuplInit::COMASuplInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplInit::COMASuplInit()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplInit::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplInit::ConstructL()
    {
    iOptionalMask = 0;
    SetMessageType(COMASuplAsnMessageBase::ESUPL_INIT);
    }
    
// -----------------------------------------------------------------------------
// COMASuplInit::CloneNotification() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 	
void COMASuplInit::CloneNotificationL(const COMASuplInit* aSuplInit)
	{
	TInt error = KErrNone;
	
	delete iNotification;
	iNotification = NULL;
	iNotification = COMASuplNotification::NewL();
	
	COMASuplNotification::TOMASuplEncodingType encodeType;
	
	COMASuplNotification* cloneNotification = NULL;
	aSuplInit->Notification(cloneNotification);
	
	
	COMASuplNotification::TOMASuplNotificationType notifyType = cloneNotification->SuplNotification();
	iNotification->SetSuplNotification(notifyType);
	
	error = cloneNotification->EncodingType(encodeType);
	if(!error)
		{
		iNotification->SetEncodingType(encodeType);		
		}
	
	COMASuplNotification::TOMASuplFormatIndicator formatIndicator;
	HBufC8* cloneName = NULL;
	error = cloneNotification->ClientName(cloneName,formatIndicator);
	if(!error)
		{
		HBufC8* name = HBufC8::NewL(cloneName->Length());
		*name = *cloneName;
		iNotification->SetClientName(name,formatIndicator);
		}
		
	HBufC8* cloneRequestor = NULL;
	error = cloneNotification->RequestorId(cloneRequestor,formatIndicator);
	if(!error)
		{
		HBufC8* requestor = HBufC8::NewL(cloneRequestor->Length());
		*requestor = *cloneRequestor;
		iNotification->SetRequestorId(requestor,formatIndicator);
		}
	}

// -----------------------------------------------------------------------------
// COMASuplInit::CloneSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 	
void COMASuplInit::CloneSLPAddressL(const COMASuplInit* aSuplInit)
	{
	TBuf8<KIPv6Address>  lIPvAddress;
	
	COMASuplSLPAddress* cloneSLPAddress =  NULL;
	
	delete iSLPAddress;
	iSLPAddress = NULL;
	iSLPAddress = COMASuplSLPAddress::NewL();
	
	User::LeaveIfError(aSuplInit->SLPAddress(cloneSLPAddress));
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

	
