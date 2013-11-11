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
* Description:  Header file for supl 2.0 triggered response  message
*
*/

#include "epos_comasupltriggeredresponse.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplstartver2.h"
#include "epos_suplconstants.h"
#include "epos_comasupllocationidver2.h"
#include  "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplsetcapabilitiesver2.h"

const TInt KSPCKeyLifeTimeShift = 1;
const TInt KSPCTIDShift = 2;
const TInt KSPCSetKeyShift = 3;
const TInt KSPCRepModeShift = 4;
const TInt KSupNwInfoShift = 5;
const TInt KSLPAddressShift = 6;
const TInt KTrigParamShift = 7;


EXPORT_C TOMASuplBatchRepConditions::TOMABatchRepCondType TOMASuplBatchRepConditions::GetBatchRepType()
    {
    return iBatchRepType;
    }
EXPORT_C TInt TOMASuplBatchRepConditions::NumOfIntervals()
    {
    return iNumOfIntervals;
    }
EXPORT_C TInt TOMASuplBatchRepConditions::NumOfMins()
    {
    return iNumOfMins;
    }
EXPORT_C void TOMASuplBatchRepConditions::SetBatchRepType(TOMABatchRepCondType aType)
    {
    iBatchRepType = aType;
    }
EXPORT_C void TOMASuplBatchRepConditions::SetNumOfIntervals(TInt aNoOfIntervals)
    {
    iNumOfIntervals = aNoOfIntervals;
    }
EXPORT_C void TOMASuplBatchRepConditions::SetNumOfMins(TInt aNoOfMins)
    {
    iNumOfMins = aNoOfMins;
    }


EXPORT_C void TOMASuplBatchRepType::GetBatchRepType(TBool& aRepPosition,TBool& aRepMeas,TBool& aInterMed,TBool& aDiscardOld)
    {
    aRepPosition = iReportPosition;
    aRepMeas = iReportMeasurements;
    aInterMed = iIntermediateReports;
    aDiscardOld = iDiscardOldest;
    }

EXPORT_C void TOMASuplBatchRepType::SetRepPos(TBool aRepPos)
    {
    iReportPosition = aRepPos;
    }
EXPORT_C void TOMASuplBatchRepType::SetRepMeas(TBool aRepMeas)
    {
    iReportMeasurements = aRepMeas;
    }
EXPORT_C void TOMASuplBatchRepType::SetInterRep(TBool aInterMedRep)
    {
    iIntermediateReports = aInterMedRep;
    }
    
EXPORT_C void TOMASuplBatchRepType::SetDiscOld(TBool aDisOld)
    {
    iDiscardOldest = aDisOld;
    }

EXPORT_C void TOMASuplReportingMode::GetRepMode(TOMASuplRepMode& aRepMode)
    {
    aRepMode = iRepMode;
    }
EXPORT_C void TOMASuplReportingMode::GetBatchRepType(TOMASuplBatchRepType& aBatchRepType)
    {
    aBatchRepType = iBatchRepType;
    }
EXPORT_C void TOMASuplReportingMode::GetBatchRepCond(TOMASuplBatchRepConditions& aBatchRepCond)
    {
    aBatchRepCond = iRepConditions;
    }

EXPORT_C void TOMASuplReportingMode::SetRepMode(TOMASuplRepMode aRepMode)
    {
    iRepMode = aRepMode;
    }
EXPORT_C void TOMASuplReportingMode::SetBatchRepType(TOMASuplBatchRepType aBatchRepType)
    {
    iBatchRepType = aBatchRepType;
    }
EXPORT_C void TOMASuplReportingMode::SetBatchRepCond(TOMASuplBatchRepConditions aBatchRepCond)
    {
    iRepConditions = aBatchRepCond;
    }

EXPORT_C COMASuplTriggeredResponse::COMASuplTriggeredResponse()
    {
    // No implementation required
    }

EXPORT_C COMASuplTriggeredResponse::~COMASuplTriggeredResponse()
    {
    if(iSuplSLPAddress)
        {
        delete iSuplSLPAddress;
        iSuplSLPAddress = NULL;
        }
    if(iSPCTID)
        {
        delete iSPCTID;
        iSPCTID = NULL;
        }
    }

EXPORT_C COMASuplTriggeredResponse* COMASuplTriggeredResponse::NewLC()
    {
    COMASuplTriggeredResponse* self = new (ELeave)COMASuplTriggeredResponse();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

EXPORT_C COMASuplTriggeredResponse* COMASuplTriggeredResponse::NewL()
    {
    COMASuplTriggeredResponse* self=COMASuplTriggeredResponse::NewLC();
    CleanupStack::Pop(self); // self;
    return self;
    }

EXPORT_C void COMASuplTriggeredResponse::ConstructL()
    {
    iOptionalMask = 0;
    SetMessageType(COMASuplAsnMessageBase::ESUPL_TRIG_RESPONSE); 
    }

// -----------------------------------------------------------------------------
// COMASuplTriggeredStart::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplTriggeredResponse::Clone(const COMASuplAsnMessageBase* aTrigResp,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aTrigResp,aErrorCode));
    COMASuplTriggeredResponse* trigResp = (COMASuplTriggeredResponse*)aTrigResp;  
    
   
    TOMASuplPosMethod posMethod;
    trigResp->GetPosMethod(posMethod);
    SetPosMethod(posMethod);
    
    TOMASuplTriggerParams      triggerParams;
    if(trigResp->GetTriggerParams(triggerParams) == KErrNone)
        SetTriggerParams(triggerParams);
/*   
#if 0    
    TOMASuplSuppNetworkInformation supportedNetworkInformation;
    trigResp->GetSupNetworkInfo(supportedNetworkInformation);
    SetSupNetworkInfo(supportedNetworkInformation);
    
    TOMASuplReportingMode reportMode;
    trigResp->GetReportMode(reportMode);
    SetReportMode(reportMode);
    
    TOMASuplSPCSETKey sPCSetKey;
    trigResp->GetSPCSETKey(sPCSetKey);
    SetSPCSETKey(sPCSetKey);
    
    TInt sPCTKeyLifeTime;
    trigResp->GetSPCTKeyLifeTime(sPCTKeyLifeTime);
    SetSPCTKeyLifeTime(sPCTKeyLifeTime);
    
    COMASuplSLPAddress*    suplSLPAddress;
    trigResp->GetSLPAddress(suplSLPAddress);
    TBuf8<KIPv6Address>  iPvAddress;
    
    COMASuplSLPAddress* suplSLPAddressToSet = COMASuplSLPAddress::NewL();
    
    switch(suplSLPAddress->SLPAddressType())
        {
        case COMASuplSLPAddress::EIPv4Address:
            {
            suplSLPAddress->IPvAddress(iPvAddress);
            suplSLPAddressToSet->SetIPv4Address(iPvAddress);
            }
            break;
        case COMASuplSLPAddress::EIPv6Address:
            {
            suplSLPAddress->IPvAddress(iPvAddress);
            suplSLPAddressToSet->SetIPv6Address(iPvAddress);
            }
            break;
        case COMASuplSLPAddress::EFqdn:
            {
            HBufC8* getFqdn;
            suplSLPAddress->Fqdn(getFqdn);
            HBufC8* fqdn = HBufC8::NewL(getFqdn->Length());
            suplSLPAddressToSet->SetFqdn(fqdn);
            }
            break;
        }
    SetSLPAddress(suplSLPAddressToSet);
    
    COMASuplSPCTID* sPCTIDToSet =  COMASuplSPCTID::NewL();
    COMASuplSPCTID* getSPCTID = NULL;
    trigResp->GetSPCTId(getSPCTID);
    
    HBufC8* getFqdn = NULL;        
    TBuf8<KRANDStrLen> getRAND;
    getSPCTID->GetFqdn(getFqdn);
    getSPCTID->GetRAND(getRAND);
    
    HBufC8* setFqdn = HBufC8::NewL(getFqdn->Length());
    *setFqdn = *getFqdn;
    sPCTIDToSet->SetFqdn(setFqdn);
    sPCTIDToSet->SetRAND(getRAND);
    
    SetSPCTId(sPCTIDToSet);
#endif    
*/
    }


EXPORT_C void COMASuplTriggeredResponse::GetPosMethod(TOMASuplPosMethod& aPosMethod)const
    {
    aPosMethod = iPosMethod;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetTriggerParams(TOMASuplTriggerParams& aTriggerParams)const
    {
    if(iOptionalMask & (1<<KTrigParamShift))
        {
        aTriggerParams = iTriggerParams;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetSLPAddress(COMASuplSLPAddress*& aAddress)const
    {
    if(iOptionalMask & (1<<KSLPAddressShift))
        {
        if(iSuplSLPAddress)
            aAddress = iSuplSLPAddress;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetReportMode(TOMASuplReportingMode& aReportMode)const
    {
    if(iOptionalMask & (1<<KSPCRepModeShift))
        {
        aReportMode = iReportMode;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetSupNetworkInfo(TOMASuplSuppNetworkInformation& aSupNwInfo)const  
    {
    if(iOptionalMask & (1<<KSupNwInfoShift))
        {
        aSupNwInfo = iSupportedNetworkInformation;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetSPCSETKey(TOMASuplSPCSETKey& aSupSPCSETKey)const
    {
    if(iOptionalMask & (1<<KSPCSetKeyShift))
        {
        aSupSPCSETKey = iSPCSetKey;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetSPCTId(COMASuplSPCTID*& aSupSPCTId)const
    {
    if(iOptionalMask & (1<<KSPCTIDShift))
        {
        if(iSPCTID)
            aSupSPCTId = iSPCTID;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredResponse::GetSPCTKeyLifeTime(TInt& aKeyLifeTime)const
    {
    if(iOptionalMask & (1<<KSPCKeyLifeTimeShift))
        {
        aKeyLifeTime = iSPCTKeyLifeTime;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }


EXPORT_C void COMASuplTriggeredResponse::SetPosMethod(TOMASuplPosMethod aPosMethod)
    {
    iPosMethod = aPosMethod;
    }
EXPORT_C void COMASuplTriggeredResponse::SetTriggerParams(TOMASuplTriggerParams aTriggerParams)
    {
    iOptionalMask |= (1<<KTrigParamShift);
    iTriggerParams = aTriggerParams;
    }
EXPORT_C void COMASuplTriggeredResponse::SetSLPAddress(COMASuplSLPAddress* aAddress)
    {
    if(iSuplSLPAddress)
        {
        delete iSuplSLPAddress;
        iSuplSLPAddress = NULL;
        }
    iOptionalMask |=  (1<<KSLPAddressShift);
    iSuplSLPAddress = aAddress;
    }
EXPORT_C void COMASuplTriggeredResponse::SetSupNetworkInfo(TOMASuplSuppNetworkInformation aSupNwInfo)
    {
    iOptionalMask |= (1<<KSupNwInfoShift);
    iSupportedNetworkInformation = aSupNwInfo;
    }
EXPORT_C void COMASuplTriggeredResponse::SetReportMode(TOMASuplReportingMode aReportMode)
    {
    iOptionalMask |= (1<<KSPCRepModeShift);
    iReportMode = aReportMode;
    }
EXPORT_C void COMASuplTriggeredResponse::SetSPCSETKey(TOMASuplSPCSETKey aSupSPCSETKey)
    {
    iOptionalMask |= (1<<KSPCSetKeyShift);
    iSPCSetKey = aSupSPCSETKey;
    }
EXPORT_C void COMASuplTriggeredResponse::SetSPCTId(COMASuplSPCTID* aSupSPCTId)
    {
    if(iSPCTID)
        {
        delete iSPCTID;
        iSPCTID = NULL;
        }
    iOptionalMask |= (1<<KSPCTIDShift);
    iSPCTID = aSupSPCTId;
    }
EXPORT_C void COMASuplTriggeredResponse::SetSPCTKeyLifeTime(TInt aKeyLifeTime)
    {
    iOptionalMask |= (1<<KSPCKeyLifeTimeShift);
    iSPCTKeyLifeTime = aKeyLifeTime;
    }
