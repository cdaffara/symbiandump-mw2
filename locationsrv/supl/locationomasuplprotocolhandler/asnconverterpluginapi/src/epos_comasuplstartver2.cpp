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
* Description:  Header file for supl 2.0 supl start  message
*
*/

// INCLUDE FILES

#include <e32def.h>

#include "lbs/epos_comasuplsetcapabilities.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplstart.h"
#include "epos_comasuplstartver2.h"
#include "epos_suplconstants.h"
#include "epos_comasupllocationid.h"
#include  "lbs/epos_eomasuplposerrors.h"
#include "lbs/epos_comasuplposition.h"
#include "epos_comasupllocationidver2.h"
#include "epos_comasuplsetcapabilitiesver2.h"

// Constructors and destructor

/**
 * @Description Creates a new instance of a COMASuplLocationIdData
 *
 * @return A pointer to COMASuplLocationIdData
 * It leaves with KErrNoMemory if object creation fails        
 */
EXPORT_C COMASuplLocationIdData* COMASuplLocationIdData:: NewL()
    {
    COMASuplLocationIdData* self = new( ELeave ) COMASuplLocationIdData;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/**
 * Destructor.
 */
EXPORT_C COMASuplLocationIdData::~COMASuplLocationIdData()
    {
    if(iLocationId2)
        {
        delete iLocationId2;
        iLocationId2 = NULL;
        }
    }


EXPORT_C void COMASuplLocationIdData::SetTimeStamp(const TInt32 aTime)
    {
    iRelativeTimeStamp = aTime;
    }

EXPORT_C void COMASuplLocationIdData::SetServingFlag(const TBool aFlag)
    {
    iServingFlag = aFlag;
    }

EXPORT_C void COMASuplLocationIdData::SetLocationId(COMASuplLocationIdVer2* aLocationId)
    {
    if(iLocationId2)
        {
        delete iLocationId2;
        iLocationId2 = NULL;
        }
    iLocationId2 = aLocationId ;
    }
//coverity[pass_by_value]
EXPORT_C void COMASuplLocationIdData::SetECellInfo(TOMASuplECellInfo aMmCellInfo)
    {
    iECId = ETrue;
    iMmCellInfo = aMmCellInfo;
    }
EXPORT_C void COMASuplLocationIdData::GetTimeStamp(TInt32& aTime)const
    {
    aTime = iRelativeTimeStamp;
    }

EXPORT_C void COMASuplLocationIdData::GetServingFlag(TBool& aFlag)const
    {
    aFlag = iServingFlag;
    }

EXPORT_C void COMASuplLocationIdData::GetLocationId(COMASuplLocationIdVer2*& aLocationId)const
    {
    if(iLocationId2)
        aLocationId = iLocationId2;
    }

EXPORT_C TInt COMASuplLocationIdData::GetECellInfo(TOMASuplECellInfo& aMmCellInfo)const
    {
    if(iECId)
        {
        aMmCellInfo = iMmCellInfo;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C COMASuplLocationIdData* COMASuplLocationIdData::CloneL()
    {
    COMASuplLocationIdData* retLocIdData = COMASuplLocationIdData::NewL();
    CleanupStack::PushL( retLocIdData );
    if(iECId)
        retLocIdData->SetECellInfo(iMmCellInfo);
    retLocIdData->SetServingFlag(iServingFlag);
    retLocIdData->SetTimeStamp(iRelativeTimeStamp);
    if(iLocationId2)
        retLocIdData->SetLocationId(iLocationId2->CloneL());
    CleanupStack::Pop(retLocIdData);
    return retLocIdData;
    }
/**
 * C++ default constructor.
 */
COMASuplLocationIdData::COMASuplLocationIdData()
    {
    iServingFlag = EFalse;
    iECId = EFalse;
    }

 /**
  * By default Symbian 2nd phase constructor is private.
  */
 void COMASuplLocationIdData::ConstructL()
     {
     
     }

EXPORT_C COMASuplThirdpartyId* COMASuplThirdpartyId::NewL()
    {
    COMASuplThirdpartyId* self = new( ELeave ) COMASuplThirdpartyId;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

 /**
  * Destructor.
  */
 EXPORT_C COMASuplThirdpartyId::~COMASuplThirdpartyId()
     {
     if(iLogicalName)
         {
         delete iLogicalName;
         iLogicalName = NULL;
         }

     if(iEMailAddress)
         {
         delete iEMailAddress;
         iEMailAddress = NULL;
         }
     if(iSipUri)
         {
         delete iSipUri;
         iSipUri = NULL;
         }
     if(iIMSPublicId)
         {
         delete iIMSPublicId;
         iIMSPublicId = NULL;
         }
     if(iURI)
         {
         delete iURI;
         iURI = NULL;
         }
     }


 /**
  * C++ default constructor.
  */
 COMASuplThirdpartyId::COMASuplThirdpartyId()
     {
     iType = COMASuplThirdpartyId::EThirdPartyUnknown;
     }

 /**
  * By default Symbian 2nd phase constructor is private.
  */
 void COMASuplThirdpartyId::ConstructL()
     {
     
     }

 /**
  * Setter method to set logical name
  */
 EXPORT_C void COMASuplThirdpartyId::SetLogicalName(const TDesC& aLogicalName)
     {
     if(iLogicalName)
         {
         delete iLogicalName;
         iLogicalName = NULL;
         }
     iType = COMASuplThirdpartyId::EThirdPartyLogicalName;
     iLogicalName = aLogicalName.Alloc();
     }

 /**
  * Setter method to set MSISDN
  */
 EXPORT_C void COMASuplThirdpartyId::SetMSISDN(const TDesC8& aMSISDN)
     {
     if(aMSISDN.Length() > 0 && aMSISDN.Length() <= KThirdPartyOctetLen)
         {
         iType = COMASuplThirdpartyId::EThirdPartyMSISDN;
         iMSISDN.Copy(aMSISDN);
         }
     }

 /**
  * Setter method to set EMail address
  */
 EXPORT_C void COMASuplThirdpartyId::SetEmailAddress(const TDesC& aEmailAddress)
     {
     if(iEMailAddress)
         {
         delete iEMailAddress;
         iEMailAddress = NULL;
         }
     iType = COMASuplThirdpartyId::EThirdPartyEmail;
     iEMailAddress = aEmailAddress.Alloc();
     }

 /**
  * Setter method to set SIP URI
  */
 EXPORT_C void COMASuplThirdpartyId::SetSipUri(const TDesC8& aSIPUri)
     {
     if(iSipUri)
         {
         delete iSipUri;
         iSipUri = NULL;
         }
     iType = COMASuplThirdpartyId::EThirdPartySIPURI;
     iSipUri = aSIPUri.Alloc();
     }

 /**
  * Setter method to set IMS public id
  */
 EXPORT_C void COMASuplThirdpartyId::SetIMSPublicId(const TDesC8& aPublicId)
     {
     if(iIMSPublicId)
         {
         delete iIMSPublicId;
         iIMSPublicId = NULL;
         }
     iType = COMASuplThirdpartyId::EThirdPartyIMSPublicId;
     iIMSPublicId = aPublicId.Alloc();
     }
 

 /**
  * Setter method to set Min parameter
  */
 EXPORT_C void COMASuplThirdpartyId::SetMinThirdPartyId(const TDesC8& aMin)
     {
     if(aMin.Length() > 0 && aMin.Length() <=  KThirdPartyMinLen)
         {
         iType = COMASuplThirdpartyId::EThirdPartyMin;
         iMin.Copy(aMin);
         }
     }
 /**
  * Setter method to set Mdn parameter
  */
 EXPORT_C void COMASuplThirdpartyId::SetMdnThirdPartyId(const TDesC8& aMdn)
     {
     if(aMdn.Length() > 0 && aMdn.Length() <=  KThirdPartyOctetLen)
         {
         iType = COMASuplThirdpartyId::EThirdPartyMDN;
         iMdn.Copy(aMdn);
         }
     }

 /**
  * Setter method to set URI
  */
 EXPORT_C void COMASuplThirdpartyId::SetURI(const TDesC8& aURI)
     {
     if(iURI)
         {
         delete iURI;
         iURI = NULL;
         }
     iType = COMASuplThirdpartyId::EThirdPartyURI;
     iURI = aURI.Alloc();
     }

 /**
  * Getter method to get logical name
  */
 EXPORT_C void COMASuplThirdpartyId::GetLogicalName(TDes& aLogicalName)
     {
     if(iLogicalName && aLogicalName.MaxLength() >= iLogicalName->Length())
         aLogicalName.Copy(*iLogicalName);
     }

 /**
  * Getter method to Get MSISDN
  */
 EXPORT_C void COMASuplThirdpartyId::GetMSISDN(TDes8& aMSISDN)
     {
     if(aMSISDN.MaxLength() >= iMSISDN.Length())
         aMSISDN.Copy(iMSISDN);
     }

 /**
  * Getter method to Get EMail address
  */
 EXPORT_C void COMASuplThirdpartyId::GetEmailAddress(TDes& aEmailAddress)
     {
     if(iEMailAddress && aEmailAddress.MaxLength() >= iEMailAddress->Length())
         aEmailAddress.Copy(*iEMailAddress);
     }

 /**
  * Getter method to Get SIP URI
  */
 EXPORT_C void COMASuplThirdpartyId::GetSipUri(TDes8& aSIPUri)
     {
     if(iSipUri && aSIPUri.MaxLength() >= iSipUri->Length())
         aSIPUri.Copy(*iSipUri);
     }

 /**
  * Getter method to Get IMS public id
  */
 EXPORT_C void COMASuplThirdpartyId::GetIMSPublicId(TDes8& aPublicId)
     {
     if(iIMSPublicId && aPublicId.MaxLength() >= iIMSPublicId->Length())
         aPublicId.Copy(*iIMSPublicId);
     }

 /**
  * Getter method to Get Min parameter
  */
 EXPORT_C void COMASuplThirdpartyId::GetMinThirdPartyId(TDes8& aMin)
     {
     if(aMin.MaxLength() >= iMin.Length())
         aMin.Copy(iMin);
     }
 /**
  * Getter method to Get Mdn parameter
  */
 EXPORT_C void COMASuplThirdpartyId::GetMdnThirdPartyId(TDes8& aMdn)
     {
     if(aMdn.MaxLength() >= iMdn.Length())
         aMdn.Copy(iMdn);
     }

 /**
  * Getter method to Get URI
  */
 EXPORT_C void COMASuplThirdpartyId::GetURI(TDes8& aURI)
     {
     if(iURI && aURI.MaxLength() >= iURI->Length())
         aURI.Copy(*iURI);
     }

/**
 * Getter method to return the type of third party id present
 */
EXPORT_C COMASuplThirdpartyId::TOMASuplThirdPartyInfoType COMASuplThirdpartyId::GetType()
    {
    return iType;
    }

EXPORT_C COMASuplThirdpartyId* COMASuplThirdpartyId::CloneL()
    {
    COMASuplThirdpartyId* thirdpartyId = COMASuplThirdpartyId::NewL();
    CleanupStack::PushL(thirdpartyId);
    switch(iType)
        {
        case EThirdPartyLogicalName:
            {
            thirdpartyId->SetLogicalName(*iLogicalName);
            }
            break;
        case EThirdPartyMSISDN:
            {
            thirdpartyId->SetMSISDN(iMSISDN);
            }
            break;
        case EThirdPartyEmail:
            {
            thirdpartyId->SetEmailAddress(*iEMailAddress);
            }
            break;
        case EThirdPartySIPURI:
            {
            thirdpartyId->SetSipUri(*iSipUri);
            }
            break;
        case EThirdPartyIMSPublicId:
            {
            thirdpartyId->SetIMSPublicId(*iIMSPublicId);
            }
            break;
        case EThirdPartyMin:
            {
            thirdpartyId->SetMinThirdPartyId(iMin);
            }
            break;
        case EThirdPartyMDN:
            {
            thirdpartyId->SetMdnThirdPartyId(iMdn);
            }
            break;
        case EThirdPartyURI:
            {
            thirdpartyId->SetURI(*iURI);
            }
            break;
        }
    CleanupStack::Pop(thirdpartyId);//thirdpartyId
    return thirdpartyId;
    }

/**
 *        
 */
EXPORT_C COMASuplThirdPartyIds* COMASuplThirdPartyIds::NewL()
    {
    COMASuplThirdPartyIds* self = new( ELeave ) COMASuplThirdPartyIds;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/**
 * Destructor.
 */
EXPORT_C COMASuplThirdPartyIds::~COMASuplThirdPartyIds()
    {
    iThirdpartyArr.ResetAndDestroy();
    }
EXPORT_C COMASuplThirdPartyIds* COMASuplThirdPartyIds::CloneL()
    {
    COMASuplThirdPartyIds* thirdpartyIds = COMASuplThirdPartyIds::NewL();
    CleanupStack::PushL(thirdpartyIds);
    for(TInt iter = 0; iter < iThirdpartyArr.Count(); iter++)
        {
        thirdpartyIds->AddThirdPartyId(iThirdpartyArr[iter]->CloneL());
        }
    CleanupStack::Pop(thirdpartyIds);//thirdpartyIds
    return thirdpartyIds;
    }
/**
 * C++ default constructor.
 */
COMASuplThirdPartyIds::COMASuplThirdPartyIds()
    {
    
    }

/**
 * By default Symbian 2nd phase constructor is private.
 */
void COMASuplThirdPartyIds::ConstructL()
    {
    
    }

/**
 *
 */
EXPORT_C void COMASuplThirdPartyIds::AddThirdPartyId(COMASuplThirdpartyId* aThirdPartyId)
    {
    if(aThirdPartyId)
        iThirdpartyArr.Append(aThirdPartyId);
    }

/**
 * 
 */
EXPORT_C void COMASuplThirdPartyIds::GetThirdPartyIds(RPointerArray<COMASuplThirdpartyId>& aThirdpartyArr)const
    {
    aThirdpartyArr = iThirdpartyArr;
    }

/**
  * @Description Creates a new instance of a COMASuplMultipleLocIds
  *
  * @return A pointer to COMASuplMultipleLocIds
  * It leaves with KErrNoMemory if object creation fails        
  */
 EXPORT_C COMASuplMultipleLocIds* COMASuplMultipleLocIds:: NewL()
     {
     COMASuplMultipleLocIds* self = new( ELeave ) COMASuplMultipleLocIds;
     CleanupStack::PushL( self );
     self->ConstructL();
     CleanupStack::Pop(self);
     return self;
     }

 /**
  * Destructor.
  */
 EXPORT_C COMASuplMultipleLocIds::~COMASuplMultipleLocIds()
     {
     iLocationIdDataEle.ResetAndDestroy();
     }

 /**
  * Setter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
  */
 EXPORT_C void COMASuplMultipleLocIds::AddNextLocIdData(COMASuplLocationIdData* aLocIdData)
     {
     if(aLocIdData)
         iLocationIdDataEle.Append(aLocIdData);
     }

 /**
  * getter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
  */
 EXPORT_C void COMASuplMultipleLocIds::GetLocIdDataArray(RPointerArray<COMASuplLocationIdData>& aLocationIdDataEleArr)
     {
     aLocationIdDataEleArr = iLocationIdDataEle;
     }
 
 EXPORT_C COMASuplMultipleLocIds* COMASuplMultipleLocIds::CloneL()
     {
     COMASuplMultipleLocIds* multipleLocId = COMASuplMultipleLocIds::NewL();
     CleanupStack::PushL( multipleLocId );

      for(TInt locationIdIter = 0; locationIdIter < iLocationIdDataEle.Count();locationIdIter++)
         {
         multipleLocId->AddNextLocIdData((iLocationIdDataEle[locationIdIter]->CloneL()));
         }
     CleanupStack::Pop(multipleLocId);
     return multipleLocId;
     }
 /**
  * C++ default constructor.
  */
 COMASuplMultipleLocIds::COMASuplMultipleLocIds()
     {
     }

 /**
  * By default Symbian 2nd phase constructor is private.
  */
 void COMASuplMultipleLocIds::ConstructL()
     {
     
     }
    
 /**
  * @Description Creates a new instance of a COMASuplVer2SuplStartExtn
  *
  * @return A pointer to COMASuplVer2SuplStartExtn
  * It leaves with KErrNoMemory if object creation fails        
  */
 EXPORT_C COMASuplVer2SuplStartExtn* COMASuplVer2SuplStartExtn:: NewL()
    {
    COMASuplVer2SuplStartExtn* self = new( ELeave ) COMASuplVer2SuplStartExtn;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/**
 * Destructor.
 */
 EXPORT_C COMASuplVer2SuplStartExtn::~COMASuplVer2SuplStartExtn()
     {
     if(iMultipleLocIds)
         {
         delete iMultipleLocIds;
         iMultipleLocIds = NULL;
         }

     if(iThirdparty)
         {
         delete iThirdparty;
         iThirdparty = NULL;
         }

     if(iPosition)
         {
         delete iPosition;
         iPosition = NULL;
         }
     }


 /**
  * C++ default constructor.
  */
 COMASuplVer2SuplStartExtn::COMASuplVer2SuplStartExtn()
     {

     }

/**
 * By default Symbian 2nd phase constructor is private.
 */
void COMASuplVer2SuplStartExtn::ConstructL()
    {
    
    }
       
/**
  * Setter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
 */
EXPORT_C void COMASuplVer2SuplStartExtn::SetMultipleLocIds(COMASuplMultipleLocIds* aMultipleLocIds)
    {
    if(iMultipleLocIds)
        {
        delete iMultipleLocIds;
        iMultipleLocIds = NULL;
        } 
    iMultipleLocIds = aMultipleLocIds;
    }

/**
 * Setter method to set COMASuplThirdpartyId in the Version 2 SUPL Start Extension
 */
EXPORT_C void COMASuplVer2SuplStartExtn::SetThirdParty(COMASuplThirdPartyIds* aThirdParty)
    {
    if(iThirdparty)
        {
        delete iThirdparty;
        iThirdparty = NULL;
        }
    iThirdparty = aThirdParty;
    }

 /**
  * Setter method to set COMASuplPosition in the Version 2 SUPL Start Extension
  */
EXPORT_C void COMASuplVer2SuplStartExtn::SetPosition(COMASuplPosition* aPosition)
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    iPosition = aPosition;
    }

/**
 * Getter method to set COMASuplMultipleLocIds in the Version 2 SUPL Start Extension
 */
EXPORT_C void COMASuplVer2SuplStartExtn::GetMultipleLocIds(COMASuplMultipleLocIds*& aMultipleLocIds)const
    {
    if(iMultipleLocIds)
    aMultipleLocIds = iMultipleLocIds;
    }

 /**
  * Getter method to set COMASuplThirdpartyId in the Version 2 SUPL Start Extension
  */
EXPORT_C void COMASuplVer2SuplStartExtn::GetThirdParty(COMASuplThirdPartyIds*& aThirdParty)const
    {
    if(iThirdparty)
        aThirdParty = iThirdparty;
    }

 /**
  * Getter method to set COMASuplPosition in the Version 2 SUPL Start Extension
  */
EXPORT_C void COMASuplVer2SuplStartExtn::GetPosition(COMASuplPosition*& aPosition)const
    {
    if(iPosition)
        aPosition = iPosition;
    }

EXPORT_C COMASuplVer2SuplStartExtn* COMASuplVer2SuplStartExtn::CloneL()
    {
    COMASuplVer2SuplStartExtn* ver2ExtnToRet = COMASuplVer2SuplStartExtn::NewL();
    CleanupStack::PushL( ver2ExtnToRet );
    if(iMultipleLocIds)
        ver2ExtnToRet->SetMultipleLocIds(iMultipleLocIds->CloneL());
    if(iPosition)
        ver2ExtnToRet->SetPosition(static_cast <COMASuplPosition*>(iPosition->CloneL()));
    if(iThirdparty)
        ver2ExtnToRet->SetThirdParty(iThirdparty->CloneL());
    CleanupStack::Pop(ver2ExtnToRet);
    return ver2ExtnToRet;
    }
// ============================ MEMBER FUNCTIONS of COMASuplStartVer2===============================

// -----------------------------------------------------------------------------
// COMASuplStartVer2::COMASuplStartVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplStartVer2::COMASuplStartVer2()
    {
    ieCID = EFalse; 
    iQopSet = EFalse;
    }
// -----------------------------------------------------------------------------
// COMASuplStartVer2::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplStartVer2* COMASuplStartVer2::NewL()
    {
    COMASuplStartVer2* self = new( ELeave ) COMASuplStartVer2;
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
EXPORT_C COMASuplStartVer2::~COMASuplStartVer2()
    {
    if(iSETCapabilities2)
        {
        delete iSETCapabilities2;
        iSETCapabilities2 = NULL;
        }

    if(iLocationId2)
        {
        delete iLocationId2;
        iLocationId2 = NULL;
        }
            
    if(iVer2Extn)
        {
        delete iVer2Extn;
        iVer2Extn = NULL;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplStartVer2::SuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStartVer2::SuplStart(COMASuplSETCapabilitiesVer2*& aSuplSETCaps,
						  			   COMASuplLocationIdVer2*& aSuplLocationId,
						  			   TBool& aeCID)const
{
	aSuplSETCaps    = iSETCapabilities2;
	aSuplLocationId = iLocationId2;
	aeCID           = ieCID;
}						  			   
// -----------------------------------------------------------------------------
// COMASuplStartVer2::SetVer2SuplStartExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void COMASuplStartVer2::SetVer2SuplStartExtn(COMASuplVer2SuplStartExtn* aVer2Extn)
    {
    
    if(iVer2Extn)
        {
        delete iVer2Extn;
        iVer2Extn = NULL;
        }
    
    iVer2Extn = aVer2Extn;
    if(iVer2Extn)
        iIsV2ExtnPresent = ETrue;
    }
// -----------------------------------------------------------------------------
// COMASuplStartVer2::GetVer2SuplStartExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void COMASuplStartVer2::GetVer2SuplStartExtn(COMASuplVer2SuplStartExtn*& aVer2Extn)
    {
    if(iIsV2ExtnPresent)
        aVer2Extn = iVer2Extn;
    else
        aVer2Extn = NULL;
    }

EXPORT_C TBool COMASuplStartVer2::IsVer2ExtnPresent()
    {
    return iIsV2ExtnPresent;
    }
// -----------------------------------------------------------------------------
// COMASuplStart::SetSuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplStartVer2::SetSuplStart(COMASuplSETCapabilitiesVer2* aSuplSETCaps,
						  			      COMASuplLocationIdVer2* aSuplLocationId,
						  			      TBool	aeCID)
{
	delete iLocationId2;
	iLocationId2 = NULL;
		
	delete iSETCapabilities2;
	iSETCapabilities2 = NULL;
	
	iSETCapabilities2 = aSuplSETCaps;
	iLocationId2     = aSuplLocationId;
	ieCID            = aeCID;
}	
// -----------------------------------------------------------------------------
// COMASuplStart::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplStartVer2::ConstructL()
    {
    iOptionalMask = 0;
    COMASuplStart::ConstructL();
    }		

EXPORT_C void COMASuplStartVer2::Clone(const COMASuplAsnMessageBase* aStart,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aStart,aErrorCode));
    COMASuplStart::Clone(aStart,aErrorCode);

    COMASuplStartVer2* start = (COMASuplStartVer2*) aStart;
    COMASuplSETCapabilitiesVer2* sETCapabilities2 = NULL;   
    COMASuplLocationIdVer2* locationId2 = NULL;
    if(start->iSETCapabilities2)
        {
        TRAP(aErrorCode,sETCapabilities2 = start->iSETCapabilities2->CloneL());
        }
    if(start->iLocationId2)
        {
        TRAP(aErrorCode,locationId2 = start->iLocationId2->CloneL());
        }
    
    SetSuplStart(sETCapabilities2,locationId2,start->ieCID);
    if(start->iVer2Extn)
        TRAP(aErrorCode,SetVer2SuplStartExtn(start->iVer2Extn->CloneL()));    
    }
