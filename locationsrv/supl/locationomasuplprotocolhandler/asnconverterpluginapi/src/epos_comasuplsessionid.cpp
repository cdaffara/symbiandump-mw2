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
* Description:  This class provides the the implementation for COMASuplSessionID
*				 					to provide the session IDs for Supl Sessions
*
*/


// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplsessionid.h"
#include  "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================

// -----------------------------------------------------------------------------
// COMASuplSessionID::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSessionID* COMASuplSessionID::NewL()
    {
    COMASuplSessionID* self = new( ELeave ) COMASuplSessionID;
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSessionID::~COMASuplSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplSessionID::~COMASuplSessionID()
    {
    if(iSETSessionID)
	    {
	    delete 	iSETSessionID;
	    iSETSessionID = NULL;
	    }
	if(iSLPSessionID)
		{
		delete iSLPSessionID;
		iSLPSessionID = NULL;
		}
    }

// -----------------------------------------------------------------------------
// COMASuplSessionID::SETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplSessionID::SETSessionID(COMASuplSETSessionID*& aSETSessionID) const
	{
	if(iOptionalMask & (1<<(0)))
		{
		aSETSessionID = iSETSessionID;
		return KErrNone;	
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
	}

// -----------------------------------------------------------------------------
// COMASuplSessionID::SLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplSessionID::SLPSessionID(COMASuplSLPSessionID*& aSLPSessionID) const   
	{
	if(iOptionalMask & (1<<(1)))
		{
		aSLPSessionID = iSLPSessionID;
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
	}
	
// -----------------------------------------------------------------------------
// COMASuplSessionID::SetSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSessionID::SetSETSessionID(COMASuplSETSessionID* aSETSessionID)
	{
	delete iSETSessionID;
	iSETSessionID = NULL;
	
	iOptionalMask &= 0x00FE;
	
	iSETSessionID = aSETSessionID;
	if(iSETSessionID)
		{
		iOptionalMask |= LEFTSHIFT(0);		
		}
	}
	
// -----------------------------------------------------------------------------
// COMASuplSessionID::SetSLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C void COMASuplSessionID::SetSLPSessionID(COMASuplSLPSessionID* aSLPSessionID)
	{
	delete iSLPSessionID;
	iSLPSessionID = NULL;
	
	iOptionalMask &= 0x00FD;
	
	iSLPSessionID = aSLPSessionID;
	if(iSLPSessionID)
		{
		iOptionalMask |= LEFTSHIFT(1);		
		}
	}
EXPORT_C COMASuplSessionID* COMASuplSessionID::CloneL()
    {
    COMASuplSessionID* suplSessionIdToReturn = COMASuplSessionID::NewL();
    CleanupStack::PushL( suplSessionIdToReturn );
    if(iSETSessionID)
        suplSessionIdToReturn->SetSETSessionID(iSETSessionID->CloneL());
    if(iSLPSessionID)
        suplSessionIdToReturn->SetSLPSessionID(iSLPSessionID->CloneL());
    
    CleanupStack::Pop(suplSessionIdToReturn);
    
    return suplSessionIdToReturn;
    }
// -----------------------------------------------------------------------------
// COMASuplSessionID::COMASuplSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSessionID::COMASuplSessionID()
	 : iOptionalMask(0),
	   iSETSessionID(NULL),
	   iSLPSessionID(NULL)
    {
    }

    

// ============================ MEMBER FUNCTIONS of COMASuplSETSessionID===============================
    
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSETSessionID* COMASuplSETSessionID::NewL()
    {
    COMASuplSETSessionID* self = new( ELeave ) COMASuplSETSessionID;
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::~COMASuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplSETSessionID::~COMASuplSETSessionID()
    {
    }
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSETSessionID::SuplSETSessionID(TInt& aSETSessionID,
  							  	   			      TDes8& aIPvAddress)const
    {
    aSETSessionID = iSETSessionID;
    if(aIPvAddress.MaxLength() >= iSETAddress.Length())
        aIPvAddress.Copy(iSETAddress);
    }

// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SuplSETIDType() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C COMASuplSETSessionID::TSuplSETIDType COMASuplSETSessionID::SuplSETIDType()const
	{
	return iChoice;
	}

// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SetSuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSETSessionID::SetSuplSETSessionIDIPv4(TInt aSETSessionID,
  							     					 const TDesC8& aIPv4Address)
{
	iSETSessionID = aSETSessionID;
	if(aIPv4Address.Length() <= KIPv6Address)
	    iSETAddress.Copy(aIPv4Address);
	iChoice = EIPv4Address;
}

// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SetSuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSETSessionID::SetSuplSETSessionIDIPv6(TInt aSETSessionID,
  							     					 const TDesC8& aIPv6Address)
{
	iSETSessionID = aSETSessionID;
	if(aIPv6Address.Length() <= KIPv6Address)
	    iSETAddress.Copy(aIPv6Address);
	iChoice = EIPv6Address;
}

// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SetSuplSETSessionIDIMSI() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSETSessionID::SetSuplSETSessionIDIMSI(TInt aSETSessionID,
  							     	  const TDesC8& aIMSI)
{
	iSETSessionID = aSETSessionID;
	iSETAddress.Copy(aIMSI);
	iChoice = EIMSI;
}
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::SetSuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSETSessionID::SetSuplSETIDType(TSuplSETIDType aSuplSETIDType)
	{
	iChoice = aSuplSETIDType;
	}

// -----------------------------------------------------------------------------
// COMASuplSETSessionID::operator== 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TBool COMASuplSETSessionID::operator==(const COMASuplSETSessionID& aSuplSETSessionID)
	{
		return((iSETSessionID == aSuplSETSessionID.iSETSessionID) && (iSETAddress == aSuplSETSessionID.iSETAddress));
	}


EXPORT_C COMASuplSETSessionID* COMASuplSETSessionID::CloneL()
    {
    COMASuplSETSessionID* setSessionId = COMASuplSETSessionID::NewL();
    setSessionId->SetSuplSETIDType(iChoice);
    TBuf8<KIPv6Address> setAddress;
    setAddress.Append(iSETAddress);
    switch(iChoice)
        {
        case EIPv4Address:
            {
            setSessionId->SetSuplSETSessionIDIPv4(iSETSessionID,setAddress);
            }
            break;
        case EIPv6Address:
            {
            setSessionId->SetSuplSETSessionIDIPv6(iSETSessionID,setAddress);
            }
            break;
        case EIMSI:
            {
            setSessionId->SetSuplSETSessionIDIMSI(iSETSessionID,setAddress);
            }
            break;
        }
    return setSessionId;
    }
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::COMASuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSETSessionID::COMASuplSETSessionID()
	:iSETSessionID(-1),
	iChoice(EIPv4Address)	 
    {
    
    }


// ============================ MEMBER FUNCTIONS of COMASuplSLPSessionID===============================

// -----------------------------------------------------------------------------
// COMASuplSLPSessionID::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSLPSessionID* COMASuplSLPSessionID::NewL()
    {
    COMASuplSLPSessionID* self = new( ELeave ) COMASuplSLPSessionID;
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSLPSessionID::~COMASuplSLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplSLPSessionID::~COMASuplSLPSessionID()
    {
    if(iSLPAddress)
	    {
	    delete iSLPAddress;
	    iSLPAddress = NULL;
	    }
    }
// -----------------------------------------------------------------------------
// COMASuplSLPSessionID::SuplSLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSLPSessionID::SuplSLPSessionID(TDes8& aSLPSessionID,
  							       				  COMASuplSLPAddress*& aSLPAddress)const
    {
    if(aSLPSessionID.MaxLength() >= iSLPSessionID.Length())
        aSLPSessionID.Copy(iSLPSessionID);
 	aSLPAddress   = iSLPAddress;
    }	
    
// -----------------------------------------------------------------------------
// COMASuplSLPSessionID::SetSuplSLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSLPSessionID::SetSuplSLPSessionID(const TDesC8& aSLPSessionID,
  							       				     COMASuplSLPAddress* aSLPAddress)
    {
    if(aSLPSessionID.Length() <= iSLPSessionID.MaxLength())
        iSLPSessionID.Copy(aSLPSessionID);
 	
 	delete iSLPAddress;
 	iSLPAddress = NULL;
 	
 	iSLPAddress   = aSLPAddress;
    }	

// -----------------------------------------------------------------------------
// COMASuplSLPSessionID::operator==() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C TBool COMASuplSLPSessionID::operator==(const COMASuplSLPSessionID& aSuplSLPSessionID)  							       				     
    {
       	if(iSLPSessionID == aSuplSLPSessionID.iSLPSessionID) 
    	{
    		if(iSLPAddress && aSuplSLPSessionID.iSLPAddress)
    			{
    			return(*iSLPAddress == *aSuplSLPSessionID.iSLPAddress);  
    			}
    			  				
    	}
    	return EFalse;
 	}	

EXPORT_C COMASuplSLPSessionID* COMASuplSLPSessionID::CloneL()
    {
    COMASuplSLPSessionID* suplSLPSessionIDToRet = COMASuplSLPSessionID::NewL();
    TBuf8<KSLPSessionID>  slpSessionID;
    slpSessionID.Copy(iSLPSessionID);
    suplSLPSessionIDToRet->SetSuplSLPSessionID(slpSessionID,iSLPAddress->CloneL());
    return suplSLPSessionIDToRet;
    }
// -----------------------------------------------------------------------------
// COMASuplSETSessionID::COMASuplSETSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSLPSessionID::COMASuplSLPSessionID()
	:iSLPAddress(NULL)
    {
    }

    

// ============================ MEMBER FUNCTIONS of COMASuplSLPAddress===============================
   
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSLPAddress* COMASuplSLPAddress::NewL()
    {
    COMASuplSLPAddress* self = new( ELeave ) COMASuplSLPAddress;
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::~COMASuplSLPSessionID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplSLPAddress::~COMASuplSLPAddress()
    {
    if(iFqdn)
    	{
    	delete iFqdn;
    	iFqdn = NULL;
    	}
    }
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::IPvAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt COMASuplSLPAddress::IPvAddress(TDes8& aIPvAddress)const
    {
    if((EIPv6Address == iChoice) || (EIPv4Address == iChoice))
	 	{
	 	if(aIPvAddress.MaxLength() >= iIPvAddress.Length())
	 	    {
	 	    aIPvAddress.Copy(iIPvAddress);
	 	    return KErrNone;
	 	    }
	 	else 
	 	    return KErrArgument;
	 	}
	 else
	 	{
	 	return KErrOMASuplParamNotSet;
	 	}
    }	
    
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::Fqdn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
EXPORT_C TInt COMASuplSLPAddress::Fqdn(HBufC8*& aFqdn)const
    {
 	 if(EFqdn == iChoice)
	 	{
         aFqdn = iFqdn;
		 return KErrNone;
	 	}
	else
		{
		return KErrOMASuplParamNotSet;
		}
    }
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::SLPAddressType() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C COMASuplSLPAddress::TSuplSLPAddressType COMASuplSLPAddress::SLPAddressType()const
	{
	return iChoice;	
	}

// -----------------------------------------------------------------------------
// COMASuplSLPAddress::SetIPv4Address() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSLPAddress::SetIPv4Address(const TDesC8& aIPv4Address)
    {
	if(iFqdn)
		{
		delete iFqdn;
		iFqdn = NULL;
		}
   	iIPvAddress.Copy(aIPv4Address);
   	iChoice = EIPv4Address;
    }	

// -----------------------------------------------------------------------------
// COMASuplSLPAddress::SetIPv6Address() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSLPAddress::SetIPv6Address(const TDesC8& aIPv6Address)
    {
	if(iFqdn)
		{
		delete iFqdn;
		iFqdn = NULL;
		}
    iIPvAddress.Copy(aIPv6Address);
   	iChoice = EIPv6Address;
    }	

// -----------------------------------------------------------------------------
// COMASuplSLPAddress::SetIPv6Address() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C void COMASuplSLPAddress::SetFqdn(HBufC8* aFqdn)
    {
	if(iFqdn)
		{
		delete iFqdn;
		iFqdn = NULL;
		}
   	iFqdn = aFqdn;
   	if(iFqdn)
   		{
   		iChoice = EFqdn;		
   		}   	
    }
 
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::SLPAddressType() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void COMASuplSLPAddress::SetSLPAddressType(TSuplSLPAddressType aSLPAddressType)
	{
	iChoice = aSLPAddressType;	
	}
  


// -----------------------------------------------------------------------------
// COMASuplSLPAddress::Compare() 
// Compares two SLPAddress...
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
EXPORT_C TBool COMASuplSLPAddress::operator==(const COMASuplSLPAddress& aSuplSLPAddress)
	{
	
		if( iChoice == aSuplSLPAddress.iChoice)
			{
				if(iChoice == EFqdn)
					{
					return(*iFqdn == *(aSuplSLPAddress.iFqdn));
					}
					
				if(iChoice == EIPv4Address || iChoice == EIPv6Address )	
					{
					return iIPvAddress == aSuplSLPAddress.iIPvAddress;
					}
			}
			
		return EFalse;
	}

EXPORT_C COMASuplSLPAddress* COMASuplSLPAddress::CloneL()
    {
    COMASuplSLPAddress* suplSLPAddressToRet = COMASuplSLPAddress::NewL();
    suplSLPAddressToRet->SetSLPAddressType(iChoice);
    switch(iChoice)
        {
        case EIPv4Address:
            {
            TBuf8<KIPv6Address>  iPvAddress;
            iPvAddress.Append(iIPvAddress);
            suplSLPAddressToRet->SetIPv4Address(iPvAddress);
            }
            break;
        case EIPv6Address:
            {
            TBuf8<KIPv6Address>  iPvAddress;
            iPvAddress.Append(iIPvAddress);
            suplSLPAddressToRet->SetIPv6Address(iPvAddress);
            }
            break;
        case EFqdn:
            {
            HBufC8* fqdn = HBufC8::NewL(iFqdn->Length());
            *fqdn = *iFqdn;
            suplSLPAddressToRet->SetFqdn(fqdn);
            }
            break;
        }
    return suplSLPAddressToRet;
    }
// -----------------------------------------------------------------------------
// COMASuplSLPAddress::COMASuplSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSLPAddress::COMASuplSLPAddress()
	:iChoice(EIPv4Address)
    {
    }



