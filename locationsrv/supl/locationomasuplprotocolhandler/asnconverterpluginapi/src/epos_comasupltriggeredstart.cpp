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
* Description:  Header file for supl 2.0 triggered start  message
*
*/

#include "epos_comasupltriggeredstart.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplstartver2.h"
#include "epos_suplconstants.h"
#include "epos_comasupllocationidver2.h"
#include  "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplsetcapabilitiesver2.h"

const TInt KQopShift = 5;
const TInt KVerShift = 4;
const TInt KTrigTypeShift = 3;
const TInt KTrigParamsShift = 2;
const TInt KRepCapShift = 1;


EXPORT_C COMASuplTriggeredStart::COMASuplTriggeredStart()
    {
    iOptionalMask = 0;
    }

EXPORT_C COMASuplTriggeredStart::~COMASuplTriggeredStart()
    {
    // SetCapabilities*/
    if(iSETCapabilities)
        {
        delete iSETCapabilities;
        iSETCapabilities = NULL;
        }
    // Location Id*/
    if(iLocationId)
        {
        delete iLocationId;
        iLocationId = NULL;
        }
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

EXPORT_C COMASuplTriggeredStart* COMASuplTriggeredStart::NewLC()
    {
    COMASuplTriggeredStart* self = new (ELeave)COMASuplTriggeredStart();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

EXPORT_C COMASuplTriggeredStart* COMASuplTriggeredStart::NewL()
    {
    COMASuplTriggeredStart* self=COMASuplTriggeredStart::NewLC();
    CleanupStack::Pop(self); // self;
    return self;
    }

EXPORT_C void COMASuplTriggeredStart::ConstructL()
    {
    SetMessageType(COMASuplAsnMessageBase::ESUPL_TRIG_START);            
    }

// -----------------------------------------------------------------------------
// COMASuplTriggeredStart::SetECellId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplTriggeredStart::SetECellId(TOMASuplECellInfo& aECId)
    {
    iMmCellInfo = aECId;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggeredStart::GetECellId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplTriggeredStart::GetECellId(TOMASuplECellInfo& aECId)
    {
    aECId = iMmCellInfo;
    }
EXPORT_C void COMASuplTriggeredStart::GetSETCapabilities(COMASuplSETCapabilitiesVer2*& aSETCapabilities) const
    {
    if(iSETCapabilities)
        aSETCapabilities = iSETCapabilities;
    }
EXPORT_C void COMASuplTriggeredStart::GetLocationId(COMASuplLocationIdVer2*& aLocationId) const
    {
    if(iLocationId)
        aLocationId = iLocationId;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetQop(TOMASuplQop& aSuplQop) const
    {
    if(iOptionalMask & (1<<KQopShift))
        {
        aSuplQop = iSuplQop;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetVer(TDes8& aVer)const
    {
    if( iOptionalMask & (1<<KVerShift) )
        {
        if(aVer.MaxLength() >= iVer.Length())
            aVer = iVer;
        else
            return KErrArgument;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C void COMASuplTriggeredStart::GetMultipleLocIds(COMASuplMultipleLocIds*& aMultipleLocIds)const
    {
    if(iMultipleLocIds)
        aMultipleLocIds = iMultipleLocIds;
    }
EXPORT_C void COMASuplTriggeredStart::GetThirdParty(COMASuplThirdPartyIds*& aThirdparty)const
    {
    if(iThirdparty)
        aThirdparty = iThirdparty;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetTriggerType(TOMASuplTrigType&  aTrigType)const
    {
    if(iOptionalMask & (1<<KTrigTypeShift))
        {
        aTrigType = iTrigType;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetTriggerParams(TOMASuplTriggerParams& aTriggerParams)const
    {
    if(iOptionalMask & (1<<KTrigParamsShift))
        {
        aTriggerParams = iTriggerParams;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C void COMASuplTriggeredStart::GetPosition(COMASuplPosition*& aPosition)const
    {
    if(iPosition)
        aPosition = iPosition;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetReportingCaps(TOMASuplReportingCap& aReportingCap)const
    {
    if(iOptionalMask & (1<<KRepCapShift))
        {
        aReportingCap = iReportingCap;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt COMASuplTriggeredStart::GetCauseCode(TOMASuplTrigStartCauseCode& aCauseCode)const
    {
    if(iOptionalMask & (1))
        {
        aCauseCode = iCauseCode;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
EXPORT_C void COMASuplTriggeredStart::SetSETCapabilities(COMASuplSETCapabilitiesVer2* aSETCapabilities)
    {
    if(iSETCapabilities)
        {
        delete iSETCapabilities;
        iSETCapabilities = NULL;
        }
    iSETCapabilities = aSETCapabilities;
    }
EXPORT_C void COMASuplTriggeredStart::SetLocationId(COMASuplLocationIdVer2* aLocationId)
    {
    if(iLocationId)
        {
        delete iLocationId;
        iLocationId = NULL;
        }
    iLocationId = aLocationId;
    }
EXPORT_C void COMASuplTriggeredStart::SetQop(TOMASuplQop aSuplQop)
    {
    iOptionalMask |= (1<<KQopShift);
    iSuplQop = aSuplQop;
    }
EXPORT_C void COMASuplTriggeredStart::SetVer(TDes8& aVer)
    {    
    iOptionalMask |= 1<<KVerShift; //set the bit to indicate version field is set
    iVer = aVer;
    }
EXPORT_C void COMASuplTriggeredStart::SetMultipleLocIds(COMASuplMultipleLocIds* aMultipleLocIds)
    {
    if(iMultipleLocIds)
        {
        delete iMultipleLocIds;
        iMultipleLocIds = NULL;
        }
    iMultipleLocIds = aMultipleLocIds;
    }
EXPORT_C void COMASuplTriggeredStart::SetThirdParty(COMASuplThirdPartyIds* aThirdparty)
    {
    if(iThirdparty)
        {
        delete iThirdparty;
        iThirdparty = NULL;
        }
    iThirdparty = aThirdparty;
    }
EXPORT_C void COMASuplTriggeredStart::SetTriggerType(TOMASuplTrigType  aTrigType)
    {
    iOptionalMask |= (1<<KTrigTypeShift);
    iTrigType = aTrigType;
    }
    
EXPORT_C void COMASuplTriggeredStart::SetTriggerParams(TOMASuplTriggerParams aTriggerParams)
    {
    iOptionalMask |= (1<<KTrigParamsShift);
    iTriggerParams = aTriggerParams;
    }
EXPORT_C void COMASuplTriggeredStart::SetPosition(COMASuplPosition* aPosition)
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    iPosition = aPosition;
    }
EXPORT_C void COMASuplTriggeredStart::SetReportingCaps(TOMASuplReportingCap       aReportingCap)
    {
    iOptionalMask |= (1<<KRepCapShift);
    iReportingCap = aReportingCap;
    }
EXPORT_C void COMASuplTriggeredStart::SetCauseCode(TOMASuplTrigStartCauseCode aCauseCode)
    {
    iOptionalMask |= (1);
    iCauseCode = aCauseCode;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggeredStart::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplTriggeredStart::Clone(const COMASuplAsnMessageBase* aTrigStart,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aTrigStart,aErrorCode));
    COMASuplTriggeredStart* trigStart = (COMASuplTriggeredStart*)aTrigStart;
    
    TOMASuplQop suplQop;
    if(trigStart->GetQop(suplQop) == KErrNone)
        SetQop(suplQop);
    
    COMASuplTriggeredStart::TOMASuplTrigStartCauseCode causeCode;
    if(trigStart->GetCauseCode(causeCode) == KErrNone)
        SetCauseCode(causeCode);
    if(trigStart->ieCID)
        {
        TOMASuplECellInfo mmCellInfo;
        trigStart->GetECellId(mmCellInfo);
        SetECellId(mmCellInfo);
        ieCID = trigStart->ieCID;
        }
    
    COMASuplSETCapabilitiesVer2* sETCapabilities = NULL; 
    trigStart->GetSETCapabilities(sETCapabilities);
    if(sETCapabilities)
        TRAP(aErrorCode,iSETCapabilities = sETCapabilities->CloneL());

    COMASuplLocationIdVer2* locationId = NULL; 
    trigStart->GetLocationId(locationId);
    if(locationId)
        TRAP(aErrorCode,SetLocationId(locationId->CloneL()));
 
    TBuf8<KVer> ver;
    trigStart->GetVer(ver);
    SetVer(ver);
    

    COMASuplMultipleLocIds* multipleLocIds = NULL;
    trigStart->GetMultipleLocIds(multipleLocIds);
    if(multipleLocIds)
        TRAP(aErrorCode,SetMultipleLocIds(multipleLocIds->CloneL()));
    
    COMASuplThirdPartyIds* thirdparty = NULL; 
    trigStart->GetThirdParty(thirdparty);
    if(thirdparty)
        TRAP(aErrorCode,SetThirdParty(thirdparty->CloneL()));
    
    TOMASuplTrigType trigType; 
    if(trigStart->GetTriggerType(trigType) == KErrNone)
        SetTriggerType(trigType);

    TOMASuplTriggerParams      triggerParams;
    if(trigStart->GetTriggerParams(triggerParams) == KErrNone)
        SetTriggerParams(triggerParams);
    
    COMASuplPosition*          position; //optional parameter
    trigStart->GetPosition(position);
    if(position)
        TRAP(aErrorCode,SetPosition(static_cast <COMASuplPosition*> (position->CloneL())));
        
    TOMASuplReportingCap       reportingCap; 
    if(trigStart->GetReportingCaps(reportingCap) == KErrNone)
        SetReportingCaps(reportingCap);    
    
    }
EXPORT_C TOMASuplTriggerParams::TOMASuplTriggerParams()
    {
    iEventType = ETOMASuplTrigParamTypeUnknown;
    }
EXPORT_C void TOMASuplTriggerParams::SetAreaEventParams(const TOMASuplAreaEventParams areaEventParams)
    {
    iEventType = ETOMASuplAreaEventParams;
    iAreaEventParams = areaEventParams;
    }
EXPORT_C void TOMASuplTriggerParams::SetPeriodicParams(const TOMASuplPeriodicParams periodicEventParams)
    {
    iEventType = ETOMASuplPeriodicParams;
    iPeriodicEventParams = periodicEventParams;
    }

EXPORT_C TInt TOMASuplTriggerParams::GetAreaEventParams(TOMASuplAreaEventParams& areaEventParams)const
    {
    if(iEventType == ETOMASuplPeriodicParams)
        {
        areaEventParams = iAreaEventParams;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt TOMASuplTriggerParams::GetPeriodicParams(TOMASuplPeriodicParams& periodicEventParams)const
    {
    if(iEventType == ETOMASuplPeriodicParams)
        {
        periodicEventParams = iPeriodicEventParams;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void TOMASuplTriggerParams::GetEventType(TOMASuplTriggerParamsType& aType)const
    {
    aType = iEventType;
    }
EXPORT_C TOMASuplPeriodicParams::TOMASuplPeriodicParams()
    {
    iIsStartTimeSet = EFalse;
    }
EXPORT_C void TOMASuplPeriodicParams::GetSuplPeriodicParams(TInt32& aNoOfFixes,TInt32& aIntervalBetFixes)const
    {
    aNoOfFixes = numberOfFixes;
    aIntervalBetFixes = intervalBetweenFixes;
   
    }
EXPORT_C TInt TOMASuplPeriodicParams::GetStartTimeSet(TInt32& aStartTime)const
    {
    if(iIsStartTimeSet)
        {
        aStartTime = startTime;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
EXPORT_C void TOMASuplPeriodicParams::SetSuplPeriodicParams(const TInt32 aNoOfFixes,const TInt32 aIntervalBetFixes)
    {
    numberOfFixes = aNoOfFixes;
    intervalBetweenFixes = aIntervalBetFixes;
    }
EXPORT_C void TOMASuplPeriodicParams::SetStartTimeSet(TInt32 aStartTime)
    {
    iIsStartTimeSet = ETrue;
    startTime = aStartTime;
    }
    
// -----------------------------------------------------------------------------
// COMASuplStart::SetSuplStart() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplTriggeredStart::SetSuplTriggeredStart(COMASuplSETCapabilitiesVer2* aSuplSETCaps,
        COMASuplLocationIdVer2* aSuplLocationId,
        TBool	aeCID)
    {
    delete iLocationId;
    iLocationId = NULL;

    delete iSETCapabilities;
    iSETCapabilities = NULL;

    iSETCapabilities = aSuplSETCaps;
    iLocationId      = aSuplLocationId;
    ieCID            = aeCID;
    }	
