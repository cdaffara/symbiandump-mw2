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
* Description:  Header file for supl 2.0 set capabilities
*
*/

// INCLUDE FILES

#include "epos_comasuplsetcapabilitiesver2.h"
#include "lbs/epos_tomasuplposprotocolversion.h"
#include "lbs/epos_eomasuplposerrors.h"

// -----------------------------------------------------------------------------
// TOMASuplPosTechnology::TOMASuplPosTechnology2()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplPosTechnology2::TOMASuplPosTechnology2():iIsVer2ExtSet(EFalse)
	{       
	}

// -----------------------------------------------------------------------------
// TOMASuplPosTechnology::GetVer2PosExtn()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplPosTechnology2::GetVer2PosExtn(TOMASuplVer2PosTechExt& aExtn)const 
    {
    if(iIsVer2ExtSet) //if GANSS pos methods are present this wil be set        
        {
        aExtn = iVer2Ext;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    
    }

// -----------------------------------------------------------------------------
// TOMASuplPosTechnology::SetVer2PosExtn()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplPosTechnology2::SetVer2PosExtn(TOMASuplVer2PosTechExt aExtn)
    {
    iVer2Ext = aExtn;
    iIsVer2ExtSet = ETrue;
    }

// -----------------------------------------------------------------------------
// TOMASuplPosProtocol2::TOMASuplPosProtocol2() ,Constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplPosProtocol2::TOMASuplPosProtocol2()
	{
	iIsVer2ExtnSet = EFalse;
	}

// -----------------------------------------------------------------------------
// TOMASuplPosProtocol2::GetVer2PosProtocolExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplPosProtocol2::GetVer2PosProtocolExtn(TOMASuplVer2ProtocolExt& aVer2Extn)const
    {
    if(iIsVer2ExtnSet)
        {
        aVer2Extn = iVer2Ext;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

// -----------------------------------------------------------------------------
// TOMASuplPosProtocol2::SetVer2PosProtocolExtn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplPosProtocol2::SetVer2PosProtoExtn(TOMASuplVer2ProtocolExt aVer2Extn)
    {
    iIsVer2ExtnSet = ETrue;
    iVer2Ext = aVer2Extn;
    }
// ============================ MEMBER FUNCTIONS OF COMASuplSETCapabilities ===============================

//
// ---------------------------------------------------------
//  constuctor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C COMASuplSETCapabilitiesVer2::COMASuplSETCapabilitiesVer2()
    {
    iPrefMethod = EOMAPrefMethodUnknown;
    iIsVer2SetCapExtnSet = EFalse;
    }

//
// ---------------------------------------------------------
//  destructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C COMASuplSETCapabilitiesVer2::~COMASuplSETCapabilitiesVer2()
    {
    }
    
//
// ---------------------------------------------------------
// Static Factory method to create the COMASuplHorizVelocity
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
EXPORT_C  COMASuplSETCapabilitiesVer2* COMASuplSETCapabilitiesVer2::NewL()
    {
    COMASuplSETCapabilitiesVer2* self = new(ELeave) COMASuplSETCapabilitiesVer2;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
	}           

//
// -------------------------------------------------------------
// Two Phase constructor
//
// (other items were commented in a header).
// -------------------------------------------------------------
//
EXPORT_C void COMASuplSETCapabilitiesVer2::ConstructL()
    {
    iPrefMethod = EOMANoPreferred;
	}           

// -----------------------------------------------------------------------------
// COMASuplSETCapabilities::PosTechnology
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASuplSETCapabilitiesVer2::GetSETCapabilities(TOMASuplPosTechnology2& aPosTechnology,
				  				 						TOMASuplPrefMethod2&    aPrefMethod,
				  				                        TOMASuplPosProtocol2&   aPosProtocol) const
    {
	aPosTechnology = iPosTechnology;
    aPrefMethod    = iPrefMethod;
	aPosProtocol   = iPosProtocol;
    }
// -----------------------------------------------------------------------------
// COMASuplSETCapabilities::PrefferedMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASuplSETCapabilitiesVer2::SetSETCapabilities(const TOMASuplPosTechnology2& aPosTechnology,
					  				 				const TOMASuplPrefMethod2&    aPrefMethod,
					  				 				const TOMASuplPosProtocol2&   aPosProtocol)
	{
	iPosTechnology = aPosTechnology;
    iPrefMethod    = aPrefMethod;
	iPosProtocol   = aPosProtocol;
	}

EXPORT_C TInt COMASuplSETCapabilitiesVer2::GetVer2SetCapExtn(TOMAVer2SetCapExtn& aCapExtn) const
    {
    if(iIsVer2SetCapExtnSet)
        {
        aCapExtn = iVer2SetCapExtn;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void COMASuplSETCapabilitiesVer2::SetVer2SetCapExtn(const TOMAVer2SetCapExtn aCapExtn) 
    {
    iIsVer2SetCapExtnSet = ETrue;
    iVer2SetCapExtn = aCapExtn;
    }
EXPORT_C TBool COMASuplSETCapabilitiesVer2::IsVer2SetCapExtnPresent()const
    {
    return iIsVer2SetCapExtnSet;
    }
// -----------------------------------------------------------------------------
// COMASuplSETCapabilitiesVer2::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//     
EXPORT_C COMASuplSETCapabilitiesVer2* COMASuplSETCapabilitiesVer2::CloneL()
	{

	COMASuplSETCapabilitiesVer2*	cloneCapabilities = COMASuplSETCapabilitiesVer2::NewL();	

	cloneCapabilities->SetSETCapabilities(iPosTechnology,iPrefMethod,iPosProtocol);
	if(iIsVer2SetCapExtnSet)
	    cloneCapabilities->SetVer2SetCapExtn(iVer2SetCapExtn);
	    
	return (cloneCapabilities);


	}


// -----------------------------------------------------------------------------
// TOMASuplVer2PosTechExt::TOMASuplVer2PosTechExt() ,Constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplVer2PosTechExt::TOMASuplVer2PosTechExt()
    {
    }


// -----------------------------------------------------------------------------
// TOMASuplSupported3GPP2PosProtoVer::TOMASuplSupported3GPP2PosProtoVer() ,Constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplSupported3GPP2PosProtoVer::TOMASuplSupported3GPP2PosProtoVer()
    {
    iRevNumber = 0;
    iPointReleaseNumber = 0;
    iInternalEditLevel = 0;
    }
// -----------------------------------------------------------------------------
// TPosProtocol::TIA801() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplSupported3GPP2PosProtoVer::GetTOMASuplSupported3GPP2PosProtoVer(TUint16& aRevNum,TInt& aPointReleaseNumber,TInt& aInternalEditLevel) const
    {
    aRevNum = iRevNumber;
    aPointReleaseNumber = iPointReleaseNumber;
    aInternalEditLevel = iInternalEditLevel;
    }
// -----------------------------------------------------------------------------
// TPosProtocol::RRLP() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplSupported3GPP2PosProtoVer::SetTOMASuplSupported3GPP2PosProtoVer(TUint16 aRevNumber,TInt aPointReleaseNumber,TInt aInternalEditLevel)
    {
    iRevNumber = aRevNumber;
    iPointReleaseNumber = aPointReleaseNumber;
    iInternalEditLevel = aInternalEditLevel;
    }    
// -----------------------------------------------------------------------------
// TOMASuplPosProtocolVersion3GPP2::TOMASuplPosProtocolVersion3GPP2() ,Constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplPosProtocolVersion3GPP2::TOMASuplPosProtocolVersion3GPP2()
    {
    }
// -----------------------------------------------------------------------------
// TPosProtocol::TIA801() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplPosProtocolVersion3GPP2::SetTOMASuplPosProtocolVersion3GPP2(TOMASuplSupported3GPP2PosProtoVer aVersion)
    {
    iSupProtoVer.Append(aVersion);
    }

EXPORT_C void TOMASuplPosProtocolVersion3GPP2::GetTOMASuplPosProtocolVersion3GPP2(RArray<TOMASuplSupported3GPP2PosProtoVer>& aSupProtoVer) const
    {
    aSupProtoVer = iSupProtoVer;
    }
// -----------------------------------------------------------------------------
// TOMASuplVer2ProtocolExt::TOMASuplVer2ProtocolExt() ,Constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplVer2ProtocolExt::TOMASuplVer2ProtocolExt()
    {
    }
// -----------------------------------------------------------------------------
// TPosProtocol::TIA801() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVer2ProtocolExt::GetTOMASuplVer2ProtocolExt(TOMASuplPosProtocolVersion& aRRLPVersion,TOMASuplPosProtocolVersion& aRRCVersion,TOMASuplPosProtocolVersion3GPP2& aTIA801Version) const
    {
    aRRLPVersion = iPosProtocolVersionRRLP;
    aRRCVersion = iPosProtocolVersionRRC;
    aTIA801Version = iPosProtocolVersionTIA801;
    }
// -----------------------------------------------------------------------------
// TPosProtocol::RRLP() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplVer2ProtocolExt::SetTOMASuplVer2ProtocolExt(TOMASuplPosProtocolVersion aRRLPVersion,TOMASuplPosProtocolVersion aRRCVersion,TOMASuplPosProtocolVersion3GPP2 aTIA801Version)
    {
    iPosProtocolVersionRRLP = aRRLPVersion;
    iPosProtocolVersionRRC = aRRCVersion;
    iPosProtocolVersionTIA801 = aTIA801Version;
    }

EXPORT_C TOMASuplReportingCap::TOMASuplReportingCap()
    {
    iOptionalMask = 0;
    }
EXPORT_C TInt TOMASuplReportingCap::GetReportMode(TOMASuplReportMode &aReportMode)const
    {
    if(iOptionalMask & (1<<1))
        {
        aReportMode = iReportMode;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
EXPORT_C TInt TOMASuplReportingCap::GetBatchReportMode(TOMASuplBatchReportMode &aBatchReportMode)const
    {
    if(iOptionalMask & 1)
        {
        aBatchReportMode = iBatchRepMode;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void TOMASuplReportingCap::SetReportMode(const TOMASuplReportMode aReportMode)
    {
    iOptionalMask |= (1<<1);
    iReportMode = aReportMode;
    }
EXPORT_C void TOMASuplReportingCap::SetBatchReportMode(const TOMASuplBatchReportMode aBatchReportMode)
    {
    iOptionalMask |= 1;
    iBatchRepMode = aBatchReportMode;
    }
EXPORT_C TOMASuplReportMode::TOMASuplReportMode()
    {
    iRealTime = EFalse;
    iQuasiRealTime = EFalse;
    iBatch = EFalse;
    }
EXPORT_C void TOMASuplReportMode::GetRealTime(TBool &aRealTime)const
    {
    aRealTime = iRealTime;
    }
EXPORT_C void TOMASuplReportMode::GetQuasiRealTime(TBool &aQuasiRealTime)const
    {
    aQuasiRealTime = iQuasiRealTime;
    }
EXPORT_C void TOMASuplReportMode::GetBatch(TBool &aBatch)const
    {
    aBatch = iBatch;
    }

EXPORT_C void TOMASuplReportMode::SetRealTime(const TBool aRealTime)
    {
    iRealTime = aRealTime;
    }
EXPORT_C void TOMASuplReportMode::SetQuasiRealTime(const TBool aQuasiRealTime)
    {
    iQuasiRealTime = aQuasiRealTime;
    }
EXPORT_C void TOMASuplReportMode::SetBatch(const TBool aBatch)
    {
    iBatch = aBatch;
    }


EXPORT_C TOMASuplBatchReportMode::TOMASuplBatchReportMode()
    {
    iOptionalMask = 0;
    }
EXPORT_C TInt TOMASuplBatchReportMode::GetReportPosition(TBool &aReportPosition)const
    {
    if(iOptionalMask & (1<<3))
        {
        aReportPosition = iReportPosition;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    
    }
EXPORT_C TInt TOMASuplBatchReportMode::GetReportMeasurement(TBool &aReportMeasurement)const
    {
    if(iOptionalMask & (1<<2))
        {
        aReportMeasurement = iReportMeasurements;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;       
    
    }
EXPORT_C TInt TOMASuplBatchReportMode::GetMaxPositions(TInt &aMaxPositions)const
    {
    if(iOptionalMask & (1<<1))
        {
        aMaxPositions = iMaxPositions;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;       
        
    }
EXPORT_C TInt TOMASuplBatchReportMode::GetMaxMeasurements(TInt &aMaxMeasurements)const
    {
    if(iOptionalMask & (1))
        {
        aMaxMeasurements = iMaxMeasurements;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;       
    
    }

EXPORT_C void TOMASuplBatchReportMode::SetReportPosition(const TBool aReportPosition)
    {
    iOptionalMask |= (1<<3);
    iReportPosition = aReportPosition;
    }
EXPORT_C void TOMASuplBatchReportMode::SetReportMeasurement(const TBool aReportMeasurement)
    {
    iOptionalMask |= (1<<2);
    iReportMeasurements = aReportMeasurement;
    }
EXPORT_C void TOMASuplBatchReportMode::SetMaxPositions(const TInt aMaxPositions)
    {
    iOptionalMask |= (1<<1);
    iMaxPositions = aMaxPositions;
    }
EXPORT_C void TOMASuplBatchReportMode::SetMaxMeasurements(const TInt aMaxMeasurements)
    {
    iOptionalMask |= (1);
    iMaxMeasurements = aMaxMeasurements;
    }
EXPORT_C TOMAVer2SetCapExtn::TOMAVer2SetCapExtn()
    {
    iIsServiceCapSet = EFalse;
    }
EXPORT_C void TOMAVer2SetCapExtn::SetServiceCaps(const TOMAServiceCapabilities aServiceCaps) 
    {
    iServiceCaps = aServiceCaps;
    iIsServiceCapSet = ETrue;
    }
EXPORT_C TInt TOMAVer2SetCapExtn::GetServiceCaps(TOMAServiceCapabilities& aServiceCaps) const
    {
    if(iIsServiceCapSet)
           {
           aServiceCaps = iServiceCaps;
           return KErrNone;
           }
       return KErrOMASuplParamNotSet;
      
    
    }
EXPORT_C TOMAServiceCapabilities::TOMAServiceCapabilities()
    {
    iOptionalMask = 0;
    }
EXPORT_C void TOMAServiceCapabilities::SetEventTriggerCapabilities(const TOMASuplEventTriggerCapabilities aEventTriggerCapabilities)
    {
    iOptionalMask |= (1<<1);
    iEventTriggerCapabilities = aEventTriggerCapabilities;
    }
EXPORT_C TInt TOMAServiceCapabilities::GetEventTriggerCapabilities(TOMASuplEventTriggerCapabilities& aEventTriggerCapabilities) const
    {
    if(iOptionalMask  & (1<<1))
        {
        aEventTriggerCapabilities = iEventTriggerCapabilities;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
EXPORT_C void TOMAServiceCapabilities::SetSuplServiceSupported(const TOMASuplServiceSupported aServicesSupported)
    {
    iOptionalMask |= (1<<2) ;
    iServicesSupported = aServicesSupported;
    }
EXPORT_C TInt TOMAServiceCapabilities::GetSuplServiceSupported(TOMASuplServiceSupported& aServicesSupported) const
    {
    if(iOptionalMask & (1<<2))
            {
            aServicesSupported = iServicesSupported;
            return KErrNone;
            }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void TOMAServiceCapabilities::SetSuplReportingCap(const TOMASuplReportingCap aReportingCapabilities)
    {
    iOptionalMask |= (1<<3);
    iReportingCapabilities = aReportingCapabilities;
    }
EXPORT_C TInt TOMAServiceCapabilities::GetSuplReportingCap(TOMASuplReportingCap& aReportingCapabilities) const
    {
    if(iOptionalMask & (1<<3))
        {
        aReportingCapabilities = iReportingCapabilities;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
    
EXPORT_C void TOMAServiceCapabilities::SetSuplSessionCapabilities(const TOMASuplSessionCapabilities aSessionCapabilities)
    {
    iOptionalMask |= 1;
    iSessionCapabilities = aSessionCapabilities;
    }
EXPORT_C TInt TOMAServiceCapabilities::GetSuplSessionCapabilities(TOMASuplSessionCapabilities& aSessionCapabilities) const
    {
    if(iOptionalMask & (1))
        {
        aSessionCapabilities = iSessionCapabilities;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
EXPORT_C TOMASuplSessionCapabilities::TOMASuplSessionCapabilities()
    {
    iOptionalMask = 0;
    }
EXPORT_C TInt TOMASuplSessionCapabilities::GetMaxNumberPeriodicSessions(TInt8& aMaxNumberPeriodicSessions) const
    {
    if(iOptionalMask & (1<<1))
        {
        aMaxNumberPeriodicSessions = iMaxNumberPeriodicSessions;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
}

EXPORT_C TInt TOMASuplSessionCapabilities::GetMaxNumberTotalSessions(TInt& aMaxNumberTotalSessions) const
    {
    if(iOptionalMask & (1<<2))
        {
        aMaxNumberTotalSessions = iMaxNumberTotalSessions;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C TInt TOMASuplSessionCapabilities::GetMaxNumberTriggeredSessions(TInt8& aMaxNumberTriggeredSessions) const
    {
    if(iOptionalMask & (1<<0))
        {
        aMaxNumberTriggeredSessions = iMaxNumberTriggeredSessions;
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void TOMASuplSessionCapabilities::SetMaxNumberTriggeredSessions(const TInt8 aMaxNumberTriggeredSessions) 
    {
    iOptionalMask |= (1<<0);
    iMaxNumberTriggeredSessions = aMaxNumberTriggeredSessions;
    }

EXPORT_C void TOMASuplSessionCapabilities::SetMaxNumberTotalSessions(const TInt aMaxNumberTotalSessions) 
    {
    iOptionalMask |= (1<<2);
    iMaxNumberTotalSessions = aMaxNumberTotalSessions;
    }

EXPORT_C void TOMASuplSessionCapabilities::SetMaxNumberPeriodicSessions(const TInt8 aMaxNumberPeriodicSessions) 
    {
    iOptionalMask |= (1<<1);
    iMaxNumberPeriodicSessions = aMaxNumberPeriodicSessions;
    }

EXPORT_C void TOMASuplServiceSupported::GetPeriodicTrigger(TBool& aPeriodicTrigger)const
    {
    aPeriodicTrigger = iPeriodicTrigger;
    }
EXPORT_C  void TOMASuplServiceSupported::GetAreaEventTrigger(TBool& aAreaEventTrigger)const
    {
    aAreaEventTrigger = iAreaEventTrigger;
    }
    
EXPORT_C void TOMASuplServiceSupported::SetPeriodicTrigger(TBool aPeriodicTrigger)
    {
    iPeriodicTrigger = aPeriodicTrigger;
    }
EXPORT_C void TOMASuplServiceSupported::SetAreaEventTrigger(TBool aAreaEventTrigger)
    {
    iAreaEventTrigger = aAreaEventTrigger;
    }
  
//  End of File  
