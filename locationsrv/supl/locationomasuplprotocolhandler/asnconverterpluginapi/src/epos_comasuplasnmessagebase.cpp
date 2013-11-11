/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  base class for all the supl messages
*
*/


// INCLUDE FILES

#include "epos_comasuplasnmessagebase.h"
#include "epos_comasuplsessionid.h"
#include "epos_suplconstants.h"

// ============================ MEMBER FUNCTIONS OF TOMASuplVersion ===============================

// -----------------------------------------------------------------------------
// TOMASuplVersion::TOMASuplVersion,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C TOMASuplVersion::TOMASuplVersion()
{  
	iMajor   = -1;
	iMinor   = -1;
	iServInd = -1;   
}
// -----------------------------------------------------------------------------
// TOMASuplVersion::SuplVersion()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVersion::SuplVersion(TInt& aMajor,
								  	    TInt& aMinor,
								  		TInt& aServInd) const
{
	aMajor   =  iMajor;  
	aMinor	 =  iMinor;
	aServInd =  iServInd;
}
// -----------------------------------------------------------------------------
// TOMASuplVersion::SuplVersion()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVersion::SetSuplVersion(TInt aMajor,
								  	       TInt aMinor,
								  	       TInt aServInd)
{
	iMajor   =  aMajor;  
	iMinor	 =  aMinor;
	iServInd =  aServInd;
}

// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::Destructor()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAsnMessageBase::~COMASuplAsnMessageBase()
    {
    if(iSessionId)
	    {
//    	delete iSessionId;
//    	iSessionId = NULL;
	    }
    }
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::MessageBase()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAsnMessageBase::MessageBase(TOMASuplVersion& aVersion,
								  				  COMASuplSessionID*& aSessionId) const
{
	aVersion 	   = iVersion ;
	aSessionId   = iSessionId;
}
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::SessionId()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAsnMessageBase::SessionId(COMASuplSessionID*& aSessionId) const
{
	aSessionId     = iSessionId;
}
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::MessageType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplAsnMessageBase::TSuplMessageType COMASuplAsnMessageBase::MessageType()const
	{
		return iMessageType;	
	}
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::MessageLength()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplAsnMessageBase::MessageLength() const
	{
		return iMessageLength;
	}
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::SetMessageBase()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAsnMessageBase::SetMessageBase(const TOMASuplVersion& aVersion,
								  				  	 COMASuplSessionID* aSessionId)
{
	iVersion 	   = aVersion ;
	iSessionId =  aSessionId;
}

// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::SetMessageLength()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAsnMessageBase::SetMessageLength(TInt aMessageLength)
{
	iMessageLength = aMessageLength;
}
// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::EncodeL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C HBufC8* COMASuplAsnMessageBase::EncodeL(TInt& /*aErrorCode*/)
	{
	return NULL;	
	}


// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::InitializeBase()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplAsnMessageBase::CloneBaseL(const COMASuplAsnMessageBase* aBase,TInt& aErrorCode)
{
	aErrorCode = KErrNone;
	iVersion = aBase->iVersion;
	
	delete iSessionId;
	iSessionId = NULL;
	iSessionId = COMASuplSessionID::NewL();

	iSessionId->iOptionalMask = aBase->iSessionId->iOptionalMask;

  	if(aBase->iSessionId->iOptionalMask & (1<<(0)))
		{
		COMASuplSETSessionID* lSuplSETSessionID = NULL;
		
		COMASuplSETSessionID* lSETId = COMASuplSETSessionID::NewL();
								
		aBase->iSessionId->SETSessionID(lSuplSETSessionID);
		COMASuplSETSessionID::TSuplSETIDType lChoice = lSuplSETSessionID->SuplSETIDType();
		
		TInt lSETSessionID;
		TBuf8<KIPv6Address> lIPvAddress;
		lSuplSETSessionID->SuplSETSessionID(lSETSessionID,lIPvAddress);
		
		if(COMASuplSETSessionID::EIPv4Address == lChoice)
			{
			lSETId->SetSuplSETSessionIDIPv4(lSETSessionID,lIPvAddress);	
			}
		else if(COMASuplSETSessionID::EIPv6Address == lChoice)
			{
			lSETId->SetSuplSETSessionIDIPv6(lSETSessionID,lIPvAddress);		
			}
        else
            {                
			lSETId->SetSuplSETSessionIDIMSI(lSETSessionID,lIPvAddress);
			}                    
	
		iSessionId->SetSETSessionID(lSETId);
		}
	
		if(aBase->iSessionId->iOptionalMask & (1<<(1)))
		{
		TBuf8<KIPv6Address>  lIPvAddress;
		TBuf8<KSLPSessionID> lSLPSessionID;
				
		COMASuplSLPSessionID* lSuplSLPSessionID = NULL;
		COMASuplSLPSessionID* lSLPSession = COMASuplSLPSessionID::NewL();
		CleanupStack::PushL(lSLPSession);
	
		COMASuplSLPAddress* lSLPAddress =  NULL;
		COMASuplSLPAddress* lSLPAddr = COMASuplSLPAddress::NewL();
		CleanupStack::PushL(lSLPAddr);
		
		aBase->iSessionId->SLPSessionID(lSuplSLPSessionID);
		lSuplSLPSessionID->SuplSLPSessionID(lSLPSessionID,lSLPAddress);
							
		COMASuplSLPAddress::TSuplSLPAddressType lChoice = lSLPAddress->SLPAddressType();

		switch(lChoice)
		{
			case COMASuplSLPAddress::EIPv4Address: 
					{
					lSLPAddress->IPvAddress(lIPvAddress);
					lSLPAddr->SetIPv4Address(lIPvAddress);
					break;				
					}
			case COMASuplSLPAddress::EIPv6Address: 
					{
					lSLPAddress->IPvAddress(lIPvAddress);
										
					lSLPAddr->SetIPv6Address(lIPvAddress);
					break;	
					}
			case COMASuplSLPAddress::EFqdn: 	
					{
					HBufC8* lFqdn = NULL;
					lSLPAddress->Fqdn(lFqdn);
					/* Copy data from tempsessionId to iSessionId*/
					HBufC8* lSLPFqdn = HBufC8::NewL(lFqdn->Length());
										
					*lSLPFqdn = *lFqdn;
					lSLPAddr->SetFqdn(lSLPFqdn);
					break;	
					}

			}
		lSLPSession->SetSuplSLPSessionID(lSLPSessionID,lSLPAddr);
		iSessionId->SetSLPSessionID(lSLPSession);
		CleanupStack::Pop(lSLPAddr);
		CleanupStack::Pop(lSLPSession);
		}
	}

// -----------------------------------------------------------------------------
// COMASuplAsnMessageBase::SetMessageType()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C void COMASuplAsnMessageBase::SetMessageType(TSuplMessageType aMessageType)
	{
		iMessageType = aMessageType;
	}

// end of file
