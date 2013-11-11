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
* Description:  Implementation file for supl 2.0 init message
*
*/

// INCLUDE FILES

#include <e32def.h>
#include "epos_comasuplinitver2.h"
#include "epos_suplconstants.h"
#include "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplnotificationver2.h"
#include "epos_comasuplsessionid.h"

// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplVer2SuplInitExtn* COMASuplVer2SuplInitExtn::NewL()
    {
    COMASuplVer2SuplInitExtn* self = new(ELeave) COMASuplVer2SuplInitExtn;
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::~COMASuplVer2SuplInitExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C  COMASuplVer2SuplInitExtn::~COMASuplVer2SuplInitExtn()
    {
    if(iSLPAddress)
        {
        delete iSLPAddress;
        iSLPAddress = NULL;
        }
    }
    
// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::COMASuplVer2SuplInitExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplVer2SuplInitExtn::COMASuplVer2SuplInitExtn()
    {
    iOptionalMask = 0;
    }
// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::CloneL(COMASuplVer2SuplInitExtn* aCloneV2Extn)
    {    
    if(aCloneV2Extn)
        {
        iOptionalMask = aCloneV2Extn->iOptionalMask  ;	
        aCloneV2Extn->GetHistoricRep(iHistoricReporting);
        aCloneV2Extn->GetMinMajorVer(iMinMajorVersion);
        aCloneV2Extn->GetNotificationMode(iNotificationMode);
        aCloneV2Extn->GetProtLevel(iProtectionLevel);
        aCloneV2Extn->GetSupNwInfo(iSupportedNetworkInformation);
        aCloneV2Extn->GetHSLPAddress(iSLPAddress);
        aCloneV2Extn->GetTrigType(iTriggerType);
        CloneSLPAddressL(aCloneV2Extn);
        }
    }

// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::CloneSLPAddress() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void COMASuplVer2SuplInitExtn::CloneSLPAddressL(COMASuplVer2SuplInitExtn* aCloneV2Extn)
    {
    TBuf8<KIPv6Address>  lIPvAddress;

    COMASuplSLPAddress* cloneSLPAddress =  NULL;


    if(aCloneV2Extn->GetHSLPAddress(cloneSLPAddress) == KErrNone)
        {
        delete iSLPAddress;
        iSLPAddress = NULL;
        iSLPAddress = COMASuplSLPAddress::NewL();

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
    }
// -----------------------------------------------------------------------------
// TOMASuplTimeWindow::TOMASuplTimeWindow() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplTimeWindow::TOMASuplTimeWindow()
    {
    iIsTimeWindowSet = EFalse;
    iStartTime = 0;
    iStopTime = 0;
    }
// -----------------------------------------------------------------------------
// TOMASuplTimeWindow::GetTimeWindow() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplTimeWindow::GetTimeWindow(TInt32& aStartTime, TInt32& aStopTime)
    {
    if(iIsTimeWindowSet)
        {
        aStartTime = iStartTime;
        aStopTime = iStopTime;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
 
    }
// -----------------------------------------------------------------------------
// TOMASuplTimeWindow::SetTimeWindow() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplTimeWindow::SetTimeWindow(TInt32 aStartTime,TInt32 aStopTime)
    {
    iIsTimeWindowSet = ETrue;
    iStartTime = aStartTime;
    iStopTime = aStopTime;
    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::TOMASuplReportingCriteria() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplReportingCriteria::TOMASuplReportingCriteria()
    {
    iOptionalMask = 0;
    iMaxNumberofReports = -1;
    iMinTimeInterval = -1;
    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::GetTimeWindow(TOMASuplTimeWindow& aTimeWindow) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplReportingCriteria::GetTimeWindow(TOMASuplTimeWindow& aTimeWindow)
    {
    if(iOptionalMask & (1<<2))
        {
        aTimeWindow  = iTimeWindow;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::GetNumOfreports() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt32 TOMASuplReportingCriteria::GetNumOfreports()
    {
    if(iOptionalMask & (1<<1))
          {
          return iMaxNumberofReports;
          }
      return KErrOMASuplParamNotSet;

    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::GrtMinTimeInt() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt32 TOMASuplReportingCriteria::GrtMinTimeInt()
    {
    if(iOptionalMask & (1))
            {
            return iMinTimeInterval;
            }
        return KErrOMASuplParamNotSet;


    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::SetTimeWindow(TOMASuplTimeWindow aTimeWindow) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplReportingCriteria::SetTimeWindow(TOMASuplTimeWindow aTimeWindow)
    {
    iOptionalMask |= (1<<2);
    iTimeWindow = aTimeWindow;
    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::SetNumOfreports(TInt32 aMaxRepNum) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplReportingCriteria::SetNumOfreports(TInt32 aMaxRepNum)
    {
    iOptionalMask |= (1<<1);
    iMaxNumberofReports = aMaxRepNum;
    }
// -----------------------------------------------------------------------------
// TOMASuplReportingCriteria::SetMinTimeInt(TInt32 aMinIntTime) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplReportingCriteria::SetMinTimeInt(TInt32 aMinIntTime)
    {
    iOptionalMask |= (1);
    iMinTimeInterval = aMinIntTime;
    }
// -----------------------------------------------------------------------------
// TOMASuplHistoricReporting::TOMASuplHistoricReporting() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplHistoricReporting::TOMASuplHistoricReporting()
    {
    iOptionalMask = 0;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplHistoricReporting::SetAllowedRepType(TOMASuplAllowedReportingType aAllowedRepType) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplHistoricReporting::SetAllowedRepType(TOMASuplAllowedReportingType aAllowedRepType)
    {
    iOptionalMask |= (1<<1);
    iAllowedReportingType = aAllowedRepType;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplHistoricReporting::SetRepCriteria(TOMASuplReportingCriteria aRepCrit) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplHistoricReporting::SetRepCriteria(TOMASuplReportingCriteria aRepCrit)
    {
    iOptionalMask |= (1);
    iReportingCriteria = aRepCrit;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplHistoricReporting::GetAllowedRepType(TOMASuplAllowedReportingType& aAllowedRepType) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplHistoricReporting::GetAllowedRepType(TOMASuplAllowedReportingType& aAllowedRepType)
    {
    if(iOptionalMask & (1<<1))
        {
        aAllowedRepType = iAllowedReportingType;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    
    }
// -----------------------------------------------------------------------------------------
// TOMASuplHistoricReporting::GetRepCriteria(TOMASuplReportingCriteria& aRepCrit) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplHistoricReporting::GetRepCriteria(TOMASuplReportingCriteria& aRepCrit)
    {
    if(iOptionalMask & (1))
           {
           aRepCrit = iReportingCriteria;
           return KErrNone;
           }
       return KErrOMASuplParamNotSet;

    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::GetKeyIdentifier(TDes8& aKeyId) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplBasicProtectionParams::GetKeyIdentifier(TDes8& aKeyId)
    {
    if(aKeyId.MaxLength() >= iKeyIdentifier.Length())
        aKeyId = iKeyIdentifier;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::GetReplayCounter() 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C TInt32 TOMASuplBasicProtectionParams::GetReplayCounter()
    {
    return iBasicReplayCounter;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::GetBasicMac(TDes8& aBasicMac) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplBasicProtectionParams::GetBasicMac(TDes8& aBasicMac)
    {
    if(aBasicMac.MaxLength() >= iBasicMAC.Length())
        aBasicMac = iBasicMAC;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::SetKeyIdentifier(TDes8& aKeyId) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplBasicProtectionParams::SetKeyIdentifier(TDes8& aKeyId)
    {
    if(iKeyIdentifier.MaxLength() >= aKeyId.Length())
        iKeyIdentifier = aKeyId;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::SetReplayCounter(TInt32 aCounter) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplBasicProtectionParams::SetReplayCounter(TInt32 aCounter)
    {
    iBasicReplayCounter = aCounter;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplBasicProtectionParams::SetBasicMac(TDes8& aBasicMac) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C void TOMASuplBasicProtectionParams::SetBasicMac(TDes8& aBasicMac)
    {
    if(iBasicMAC.MaxLength() >= aBasicMac.Length())
        iBasicMAC = aBasicMac;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplProtectionLevel::GetProtLevel(TOMASuplProtLevelType& aProtLevel) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplProtectionLevel::GetProtLevel(TOMASuplProtLevelType& aProtLevel)
    {
    if(iOptionalMask & (1<<1))
        {
        aProtLevel = iProtlevel;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// -----------------------------------------------------------------------------------------
// TOMASuplProtectionLevel::GetProtParams(TOMASuplBasicProtectionParams& aBasicProtParams) 
// (other items were commented in a header).
// ------------------------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplProtectionLevel::GetProtParams(TOMASuplBasicProtectionParams& aBasicProtParams)
    {
    if(iOptionalMask & (1<<1))
           {
           aBasicProtParams = iBasicProtectionParams;
           return KErrNone;
           }
       return KErrOMASuplParamNotSet;

    }
// -----------------------------------------------------------------------------
// TOMASuplProtectionLevel::TOMASuplProtectionLevel() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C TOMASuplProtectionLevel::TOMASuplProtectionLevel()
    {
    iProtlevel = ETOMASuplProtLevelTypeUnknown;
    iOptionalMask = 0;
    }
// -----------------------------------------------------------------------------
// TOMASuplProtectionLevel::SetProtLevel(TOMASuplProtLevelType aProtLevel) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void TOMASuplProtectionLevel::SetProtLevel(TOMASuplProtLevelType aProtLevel)
    {
    iOptionalMask |= (1<<1);
    iProtlevel = aProtLevel;
    }
// -----------------------------------------------------------------------------
// TOMASuplProtectionLevel::SetProtParams(TOMASuplBasicProtectionParams aBasicProtParams) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void TOMASuplProtectionLevel::SetProtParams(TOMASuplBasicProtectionParams aBasicProtParams)
    {
    iOptionalMask |= 1;
    iBasicProtectionParams = aBasicProtParams;
    }

// -----------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetNotificationMode(TOMASuplNotificationMode& aMode) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetNotificationMode(TOMASuplNotificationMode& aMode)
    {
    if(iOptionalMask & (1<<7))
        {
        aMode = iNotificationMode;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetSupNwInfo(TOMASuplSuppNetworkInformation& aSupportedNetworkInformation) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
// 
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetSupNwInfo(TOMASuplSuppNetworkInformation& aSupportedNetworkInformation)
    {
    if(iOptionalMask & (1<<6))
        {
        aSupportedNetworkInformation = iSupportedNetworkInformation;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetTrigType(COMASuplTriggeredStart::TOMASuplTrigType& aTrigType) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
// 
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetTrigType(COMASuplTriggeredStart::TOMASuplTrigType& aTrigType)
    {
    if(iOptionalMask & (1<<5))
        {
        aTrigType = iTriggerType;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetHSLPAddress(COMASuplSLPAddress*& aSLPAddress) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
// 
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetHSLPAddress(COMASuplSLPAddress*& aSLPAddress)
    {
    if(iOptionalMask & (1<<4))
        {
        aSLPAddress = iSLPAddress;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;

    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetHistoricRep(TOMASuplHistoricReporting& aHistoricReporting) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetHistoricRep(TOMASuplHistoricReporting& aHistoricReporting)
    {
    if(iOptionalMask & (1<<3))
        {
        aHistoricReporting = iHistoricReporting;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetProtLevel(TOMASuplProtectionLevel& aProtectionLevel) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetProtLevel(TOMASuplProtectionLevel& aProtectionLevel)
    {
    if(iOptionalMask & (1<<2))
         {
         aProtectionLevel = iProtectionLevel;
         return KErrNone;
         }
     return KErrOMASuplParamNotSet;

    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::GetMinMajorVer(TInt& aVer) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplVer2SuplInitExtn::GetMinMajorVer(TInt& aVer)
    {
    if(iOptionalMask & (1))
         {
         iMinMajorVersion = aVer;
         return KErrNone;
         }
     return KErrOMASuplParamNotSet;


    }

// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetNotificationMode(TOMASuplNotificationMode aMode) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetNotificationMode(TOMASuplNotificationMode aMode)
    {
    iOptionalMask |= (1<<7);
    iNotificationMode = aMode;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetSupNwInfo(TOMASuplSuppNetworkInformation aSupportedNetworkInformation) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetSupNwInfo(TOMASuplSuppNetworkInformation aSupportedNetworkInformation)
    {
    iOptionalMask |= (1<<6);
    iSupportedNetworkInformation = aSupportedNetworkInformation;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetTrigType(COMASuplTriggeredStart::TOMASuplTrigType aTrigType) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetTrigType(COMASuplTriggeredStart::TOMASuplTrigType aTrigType)
    {
    iOptionalMask |= (1<<5);
    iTriggerType = aTrigType;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetHSLPAddress(COMASuplSLPAddress* aSLPAddress) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetHSLPAddress(COMASuplSLPAddress* aSLPAddress)
    {
    if(iSLPAddress)
        {
        delete iSLPAddress;
        iSLPAddress = NULL;
        }
    if(aSLPAddress)
        {
        iOptionalMask |= (1<<4);
        iSLPAddress = aSLPAddress;
        }
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetHistoricRep(TOMASuplHistoricReporting aHistoricReporting) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetHistoricRep(TOMASuplHistoricReporting aHistoricReporting)
    {
    iOptionalMask |= (1<<3);
    iHistoricReporting = aHistoricReporting;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetProtLevel(TOMASuplProtectionLevel aProtectionLevel) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetProtLevel(TOMASuplProtectionLevel aProtectionLevel)
    {
    iOptionalMask |= (1<<2);
    iProtectionLevel = aProtectionLevel;
    }
// ----------------------------------------------------------------------------------------------------
// COMASuplVer2SuplInitExtn::SetMinMajorVer(TInt aVer) 
// (other items were commented in a header).
// ----------------------------------------------------------------------------------------------------
//
EXPORT_C void COMASuplVer2SuplInitExtn::SetMinMajorVer(TInt aVer)
    {
    iOptionalMask |= (1);
    iMinMajorVersion = aVer;
    }


// ============================ MEMBER FUNCTIONS of COMASuplInitVer2===============================

// -----------------------------------------------------------------------------
// COMASuplInit::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplInitVer2* COMASuplInitVer2::NewL()
    {
    COMASuplInitVer2* self = new( ELeave ) COMASuplInitVer2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplInitVer2::~COMASuplInitVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplInitVer2::~COMASuplInitVer2()
    {
    if(iNotificationV2)
        {
        delete iNotificationV2;
        iNotificationV2 = NULL;
        }
    if(iVer2InitExtn)
        {
        delete iVer2InitExtn;
        iVer2InitExtn = NULL;
        }
    }
    


// -----------------------------------------------------------------------------
// COMASuplResponse::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplInitVer2::Clone(const COMASuplAsnMessageBase* aInit,TInt& aErrorCode)
{
	TRAP(aErrorCode, CloneBaseL(aInit,aErrorCode))
	if(aErrorCode)
	 	return;
	COMASuplInit::Clone(aInit,aErrorCode);
	COMASuplInitVer2* suplInit = (COMASuplInitVer2*)aInit;
	iOptionalMask = suplInit->iOptionalMask;

	if(suplInit->iOptionalMask & (1<<(1)))
		{
		TRAP(aErrorCode,CloneNotificationL(suplInit))
		if(aErrorCode)
			{
			return;
			}
		}
	if(suplInit->iOptionalMask & (1<<(3)))
	    {
	    TRAP(aErrorCode,CloneInitV2ExtnL(suplInit))
	    if(aErrorCode)
	        {
	        return;
	        }
	    }
}	
// -----------------------------------------------------------------------------
// COMASuplInitVer2::COMASuplInitVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplInitVer2::COMASuplInitVer2()
	{
		
	}
// -----------------------------------------------------------------------------
// COMASuplInitVer2::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplInitVer2::ConstructL()
    {
    COMASuplInit::ConstructL();
    }
// -----------------------------------------------------------------------------
// COMASuplInit::CloneNotification() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
void COMASuplInitVer2::CloneNotificationV2L(const COMASuplInitVer2* aSuplInit)
    {
    TInt error = KErrNone;
    
    delete iNotificationV2;
    iNotificationV2 = NULL;
    iNotificationV2 = COMASuplNotificationVer2::NewL();
    
    COMASuplNotificationVer2::TOMASuplEncodingType encodeType;
    
    COMASuplNotificationVer2* cloneNotification = NULL;
    aSuplInit->GetVer2Notification(cloneNotification);
    
    
    COMASuplNotification::TOMASuplNotificationType notifyType = cloneNotification->SuplNotification();
    iNotificationV2->SetSuplNotification(notifyType);
    
    error = cloneNotification->EncodingType(encodeType);
    if(!error)
        {
        iNotificationV2->SetEncodingType(encodeType);  
        }
    
    COMASuplNotification::TOMASuplFormatIndicator formatIndicator;
    HBufC8* cloneName = NULL;
    error = cloneNotification->ClientName(cloneName,formatIndicator);
    if(!error)
        {
        HBufC8* name = HBufC8::NewL(cloneName->Length());
        *name = *cloneName;
        iNotificationV2->SetClientName(name,formatIndicator);
        }
        
    HBufC8* cloneRequestor = NULL;
    error = cloneNotification->RequestorId(cloneRequestor,formatIndicator);
    if(!error)
        {
        HBufC8* requestor = HBufC8::NewL(cloneRequestor->Length());
        *requestor = *cloneRequestor;
        iNotificationV2->SetRequestorId(requestor,formatIndicator);
        }

    }
// -----------------------------------------------------------------------------
// COMASuplInitVer2::CloneNotification() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void COMASuplInitVer2::CloneInitV2ExtnL(COMASuplInitVer2* aSuplInit)
    {
    if(iVer2InitExtn)
        {
        delete iVer2InitExtn;
        iVer2InitExtn = NULL;
        }
        
    iVer2InitExtn = COMASuplVer2SuplInitExtn::NewL();
    COMASuplVer2SuplInitExtn* cloneV2SuplInitExtn = NULL;
    aSuplInit->GetVer2SuplInitExtn(cloneV2SuplInitExtn);
    iVer2InitExtn->CloneL(cloneV2SuplInitExtn);
    
    }
// -----------------------------------------------------------------------------
// COMASuplInitVer2::SetVer2SuplInitExtn(COMASuplVer2SuplInitExtn* aVer2Extn) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplInitVer2::SetVer2SuplInitExtn(COMASuplVer2SuplInitExtn* aVer2Extn)
    {
    if(iVer2InitExtn)
        {
        delete iVer2InitExtn;
        iVer2InitExtn = NULL;
        }
    iVer2InitExtn = aVer2Extn;
    if(iVer2InitExtn)
           iOptionalMask |= LEFTSHIFT(3);
    }

// -----------------------------------------------------------------------------
// COMASuplInitVer2::GetVer2SuplInitExtn(COMASuplVer2SuplInitExtn*& aVer2Extn) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplInitVer2::GetVer2SuplInitExtn(COMASuplVer2SuplInitExtn*& aVer2Extn)
    {
    if(iOptionalMask & (1<<(3)))
        {
        aVer2Extn    = iVer2InitExtn; 
        return KErrNone;
        }
    else
        {
        return KErrOMASuplParamNotSet;
        }

    }
// -----------------------------------------------------------------------------
// COMASuplInitVer2::SetVer2Notification(COMASuplNotificationVer2* aNotificationV2) 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASuplInitVer2::SetVer2Notification(COMASuplNotificationVer2* aNotificationV2)
    {
    if(iNotificationV2)
        {
        delete iNotificationV2;
        iNotificationV2 = NULL;
        }
    iNotificationV2 = aNotificationV2;
    if(iNotificationV2)
           iOptionalMask |= LEFTSHIFT(4);
 
    }

// -----------------------------------------------------------------------------
// COMASuplInitVer2::GetVer2Notification(COMASuplNotificationVer2*& aNotificationV2)const 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplInitVer2::GetVer2Notification(COMASuplNotificationVer2*& aNotificationV2)const
    {
    if(iOptionalMask & (1<<(4)))
        {
        aNotificationV2    = iNotificationV2; 
        return KErrNone;
        }
    else
        {
        return KErrOMASuplParamNotSet;
        }
    }
