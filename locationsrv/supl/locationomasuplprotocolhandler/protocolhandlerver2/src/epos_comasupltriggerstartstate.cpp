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
* Description:   Class for handling SUPL_START message.
*
*/



#include <etelmm.h> 
#include <e32math.h>
#include <f32file.h>
#include <bautils.h>

#include "epos_comasuplsession2.h"
#include "lbs/epos_tomasuplallowedcapabilities.h"
#include "epos_comasupllocationid.h"
#include "epos_comasupllocationidver2.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "epos_comasuplsetcapabilitiesver2.h"
#include "epos_suplterminal.h"
#include "epos_suplterminalconstants.h"
#include "epos_suplterminalqop.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasuplsession2.h"
#include "epos_tomasuplqop.h"
#include "epos_comasuplasnmessagebase.h"
#include "epos_comasupltriggerstartstate.h"
#include "epos_comasupltriggeredstart.h"
#include "epos_comasuplposrequestor.h"
#include "epos_omasuplconstants.h"
#include "epos_comasupltrace.h"
#include "epos_comasuplasnbase.h"


_LIT(KTraceFileName,"SUPL_OMA_SESSION::EPos_COMASuplTriggerStartState.cpp");

TInt FindK2(TReal horAccuracy,TReal C, TReal x)
	{
		TReal r = horAccuracy;
		TReal numerator ;
		TReal denominator;
		Math::Log(numerator,( r/C + 1.0 ));
		Math::Log(denominator,( 1.0 + x ));
		TReal res = numerator/ denominator;
		TInt round = res; //To ceil it.
		TReal decimal = res - round;
		if(decimal >= 0.5)
			{
				res  = res  + 0.5;
			}
		return res;
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::COMASuplTriggerStartState
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStartState::COMASuplTriggerStartState(RMobilePhone& aMobilePhone,
									  TInt aMsgTimeout,COMASuplPOSRequestor* aPosRequestor,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase, TSuplTerminalPeriodicTrigger aTriggerParams, COMASuplSession::TOMASuplReqType aRequestType):
									  iMobilePhone(aMobilePhone),
									  iPosRequestor(aPosRequestor),
									  iOMASuplAsnHandlerBase(aOMASuplAsnHandlerBase),
                                      iTriggerParams(aTriggerParams),
                                      iRequestType(aRequestType)
    {
		iMessageTimeOut = aMsgTimeout * KSecond;   
		iCurrentState =  ESUPL_TRIG_START;
		iIsQoPPresent = EFalse;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::~COMASuplTriggerStartState
// -----------------------------------------------------------------------------
//
COMASuplTriggerStartState::~COMASuplTriggerStartState()
    {
    if(iTrace)
        {
        iTrace->Trace(_L("COMASuplTriggerStartState::~COMASuplTriggerStartState..."), KTraceFileName, __LINE__); 							
        iTrace->Trace(_L("deleting iLocationIDRequestor..."), KTraceFileName, __LINE__); 							
        }
    delete iLocationIDRequestor;

    if(iPosRequestor)
        iPosRequestor->DestroyList();
    else    	    	
        {   
        if(iTrace)
            iTrace->Trace(_L("deleting iSETCapabilities ..."), KTraceFileName, __LINE__); 							
        delete iSETCapabilities ;
        }

    if(iTrace)
        iTrace->Trace(_L("deleting iAllowedCapabilities ..."), KTraceFileName, __LINE__); 							
    delete iAllowedCapabilities;

    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::ConstructL(TInt aAllowedCapabilities)
    {
    	COMASuplState::BaseConstructL();
    	iTrace->Trace(_L("COMASuplTriggerStartState::ConstructL..."), KTraceFileName, __LINE__); 							
    	iLocationIDRequestor = COMASuplETel::NewL(*this,iMobilePhone);
    	iAllowedCapabilities = COMASuplSETCapabilities::NewL();

    	if(!iPosRequestor)
    		iSETCapabilities = COMASuplSETCapabilities::NewL();
    	
        iSETCapabilities2 = COMASuplSETCapabilitiesVer2::NewL();
    	FillAllowedCapabilities(aAllowedCapabilities);
    	iLocationId = COMASuplLocationIdVer2::NewL();
    	
#ifdef PRINT_MESSAGE		    	
    	if(iECId)
    		{
    		    iTrace->Trace(_L("Allowed Capabilities contains Enhance Cell Id."), KTraceFileName, __LINE__); 
    		}
    	else
    		{
    			 iTrace->Trace(_L("Allowed Capabilities does not contains Enhance Cell Id."), KTraceFileName, __LINE__);
    		}	
#endif    		
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
COMASuplTriggerStartState* COMASuplTriggerStartState::NewL(RMobilePhone& aMobilePhone,TInt aMsgTimeout,
											COMASuplPOSRequestor* aPosRequestor,
											COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,TInt aAllowedCapabilities, 
                                            TSuplTerminalPeriodicTrigger aTriggerParams,
                                            COMASuplSession::TOMASuplReqType aRequestType)
    {
	    COMASuplTriggerStartState* self =  new ( ELeave ) COMASuplTriggerStartState(aMobilePhone,aMsgTimeout,aPosRequestor,aOMASuplAsnHandlerBase, aTriggerParams, aRequestType);
	    CleanupStack::PushL( self );
	    self->ConstructL(aAllowedCapabilities);
	    CleanupStack::Pop(self);
	    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::GenerateMessage
// Generates SUPL_SATART message
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplTriggerStartState::GenerateMessageL() 
	{
		iTrace->Trace(_L("Start of COMASuplTriggerStartState::GenerateMessage"), KTraceFileName, __LINE__); 					
		iLocationIDRequestor->GetLocationID();
		return KErrNone;
	}
	
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::CancelOperation
// Cancels the generation if SUPL START
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::CancelOperation() 
	{
		iTrace->Trace(_L("Cancel of COMASuplTriggerStartState::CancelOperation..."), KTraceFileName, __LINE__); 					
		COMASuplState::CancelOperation();	
		iLocationIDRequestor->Cancel();
		if(iPosRequestor)
		{
			iPosRequestor->Cancel();
		}
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::EncodeMessageL
// Encodes message ...returns encoded data
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC8* COMASuplTriggerStartState::EncodeMessageL(TOMASuplVersion &aSuplVersion,
											COMASuplSessionID* aSessionId,TInt &aError)
	{
		iTrace->Trace(_L("Start of COMASuplTriggerStartState::EncodeMessageL"), KTraceFileName, __LINE__); 					
		iTrace->Trace(_L("*********Values in SUPL TRIGGER START*************"), KTraceFileName, __LINE__); 
		COMASuplState::EncodeMessageL(aSuplVersion,aSessionId,aError);
		
		aError = ValidateSetCapabilities(); 
		if(aError != KErrNone)
		{
			return NULL;
		}

		COMASuplAsnMessageBase* OMASuplMessageBase = iOMASuplAsnHandlerBase->CreateOMASuplMessageL(COMASuplAsnMessageBase::ESUPL_TRIG_START);
		CleanupStack::PushL(OMASuplMessageBase);
		COMASuplTriggeredStart* OMASuplStart  =  static_cast<COMASuplTriggeredStart*>(OMASuplMessageBase);
		
		iTrace->Trace(_L("Created OMASuplStart"), KTraceFileName, __LINE__); 							
		ComputeCapabilities();

		if(iSETCapabilities && iPosRequestor)
		{
						iTrace->Trace(_L("Filling capability for POS....."), KTraceFileName, __LINE__);
						
						TOMASuplPosTechnology posTech;
						COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
						TOMASuplPosProtocol posProtocol;
						TOMASuplPosProtocolVersion   posProtocolVersion;
						TOMASuplPosProtocolVersion3GPP2 posProtocolVersionTia;
						TOMASuplPosProtocolVersion   tempProtocolVersion;
						

						iSETCapabilities->GetSETCapabilities(posTech, prefMethod, posProtocol,posProtocolVersion);
						
						
						TBool aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA;
						posTech.GetPosTechnology(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA);
						
						iAllowedCapabilitiesforPOS.SetAllowedCapabilities(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA, ETrue, ETrue);
						iPosRequestor->SetPosMethodAndAllowedCapabilities(iAllowedCapabilitiesforPOS,COMASuplPosSessionBase::EPositionUndefined);
			
						TOMASuplPosTechnology2 posTechnology2; 
						COMASuplSETCapabilitiesVer2::TOMASuplPrefMethod2    prefMethod2 = (COMASuplSETCapabilitiesVer2::TOMASuplPrefMethod2)prefMethod;
			
			      posTechnology2.SetPosTechnology(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA);
			
			      TBool tia801, rrlp, rrc;
			      TOMASuplPosProtocol2   posProtocol2; 
			      
						posProtocol.GetPosProtocol(tia801, rrlp, rrc);
						
			      posProtocol2.SetPosProtocol(tia801, rrlp, rrc);
						
						TOMASuplVer2ProtocolExt posProtocolExt;
						
						if(rrlp)
						{
								posProtocolExt.SetTOMASuplVer2ProtocolExt(posProtocolVersion,tempProtocolVersion,posProtocolVersionTia);
								posProtocol2.SetVer2PosProtoExtn(posProtocolExt);
						}
						
						if(rrc)
						{
							 posProtocolExt.SetTOMASuplVer2ProtocolExt(tempProtocolVersion,posProtocolVersion,posProtocolVersionTia);								
							 posProtocol2.SetVer2PosProtoExtn(posProtocolExt);
						}
			if(iSETCapabilities2)
			      iSETCapabilities2->SetSETCapabilities(posTechnology2, prefMethod2, posProtocol2);
      
      }
      
		iTrace->Trace(_L("SetMessageBase"), KTraceFileName, __LINE__);        
		OMASuplStart->SetMessageBase(aSuplVersion,aSessionId); 

        // Add trigger capability to SET Capabilities
        // Services supported
        iTrace->Trace(_L("Setting trigger params"), KTraceFileName, __LINE__);
        TOMASuplServiceSupported servSupport;
        servSupport.SetPeriodicTrigger(ETrue);
        servSupport.SetAreaEventTrigger(EFalse);

        //Reporting mode, only real time is supported
        TOMASuplReportMode repMode;
        repMode.SetRealTime(ETrue);
        repMode.SetQuasiRealTime(EFalse);
        repMode.SetBatch(EFalse);

        // Set false all batch reporting params
        TOMASuplBatchReportMode batchRep;
        batchRep.SetReportPosition(EFalse);
        batchRep.SetReportMeasurement(EFalse);
        batchRep.SetMaxPositions(0);
        batchRep.SetMaxMeasurements(0);

        // Set reporting capabilites
        TOMASuplReportingCap reportCap;
        reportCap.SetReportMode(repMode);
        reportCap.SetBatchReportMode(batchRep);

        // Session capability
        TOMASuplSessionCapabilities sessionCaps;
        sessionCaps.SetMaxNumberTotalSessions(5);
        sessionCaps.SetMaxNumberPeriodicSessions(5);
        sessionCaps.SetMaxNumberTriggeredSessions(5);

        // Set service capability
        TOMAServiceCapabilities serviceCaps;
        serviceCaps.SetSuplServiceSupported(servSupport);
        serviceCaps.SetSuplReportingCap(reportCap);
        serviceCaps.SetSuplSessionCapabilities(sessionCaps);

        // Set SET Capability extn
        TOMAVer2SetCapExtn setCapsExtn;
        setCapsExtn.SetServiceCaps(serviceCaps);
		iTrace->Trace(_L("SetVer2SetCapExtn"), KTraceFileName, __LINE__);
        // Set SET Capabilities
		if(iSETCapabilities2)
		    iSETCapabilities2->SetVer2SetCapExtn(setCapsExtn);

	    iTrace->Trace(_L("SetSuplTriggeredStart"), KTraceFileName, __LINE__);
	    
			OMASuplStart->SetSuplTriggeredStart(iSETCapabilities2,iLocationId,iECId);
		
			if(iECId)
				{
					OMASuplStart->SetECellId(iMmCellInfo);
				}

        //Set service capabilities
        OMASuplStart->SetReportingCaps(reportCap);

#ifdef PRINT_MESSAGE		
		// SET Capabilities
		if(iSETCapabilities)
		{
			iTrace->Trace(_L("SET Capabilities ....."), KTraceFileName, __LINE__);
			
			TOMASuplPosTechnology posTech;
			COMASuplSETCapabilities::TOMASuplPrefMethod prefMethod;
			TOMASuplPosProtocol posProtocol;

			iSETCapabilities->GetSETCapabilities(posTech, prefMethod, posProtocol);
		
			TBuf <200> posTechStr;
			posTechStr.Append(_L("POS Technologies - "));
			TBool aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA;
			posTech.GetPosTechnology(aGpsSetAssisted, aGpsSetBased, autonomousGps, AFLT, EOTD, OTDOA);
			if(aGpsSetAssisted) posTechStr.Append(_L("AGPS SET Assisted  "));
			if(aGpsSetBased) posTechStr.Append(_L("AGPS SET Based  "));
			if(autonomousGps) posTechStr.Append(_L("Autonomous GPS  "));
			if(AFLT) posTechStr.Append(_L("AFLT  "));
			if(EOTD) posTechStr.Append(_L("EOTD  "));
			if(OTDOA) posTechStr.Append(_L("OTDOA  "));

			iTrace->Trace(posTechStr, KTraceFileName, __LINE__);

			TBuf <80> prefMethodStr;
			prefMethodStr.Append(_L("Preferred Method - "));

			switch(prefMethod)
			{
			case COMASuplSETCapabilities::EOMAAGpsSETAssitedPreferred:
				prefMethodStr.Append(_L("AGPS SET Assisted Preferred"));
				break;
			case COMASuplSETCapabilities::EOMAAGpsSETBasedPreferred:
				prefMethodStr.Append(_L("AGPS SET Based Preferred"));
				break;
			case COMASuplSETCapabilities::EOMANoPreferred:
				prefMethodStr.Append(_L("No Preference"));
				break;
			}
			
			iTrace->Trace(prefMethodStr, KTraceFileName, __LINE__);

			TBuf <80> posProtocolStr;
			posProtocolStr.Append(_L("POS Protocol - "));
			TBool tia801, rrlp, rrc;
			
			posProtocol.GetPosProtocol(tia801, rrlp, rrc);
			if(tia801) posProtocolStr.Append(_L("TIA 801  "));
			if(rrlp) posProtocolStr.Append(_L("RRLP  "));
			if(rrc) posProtocolStr.Append(_L("RRC  "));
			iTrace->Trace(posProtocolStr, KTraceFileName, __LINE__);
        }

		// SET Capabilities
		if(iSETCapabilities2)
        {

            TBool perTrg, areaEventTrg, relTime, qRelTime, batch, position, measure;
            TInt totSessn, maxPos, maxMeasure;
            TInt8 maxPrdSession, maxTrgSession;

            servSupport.GetPeriodicTrigger(perTrg);
            servSupport.GetAreaEventTrigger(areaEventTrg);
            if (perTrg)
			    iTrace->Trace(_L("Periodic Trigger - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Periodic Trigger - Not Supported"), KTraceFileName, __LINE__);

            if (areaEventTrg)
			    iTrace->Trace(_L("Area Event Trigger - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Area Event Trigger - Not Supported"), KTraceFileName, __LINE__);

            //Reporting mode, only real time is supported
            repMode.GetRealTime(relTime);
            repMode.GetQuasiRealTime(qRelTime);
            repMode.GetBatch(batch);
            if (relTime)
			    iTrace->Trace(_L("Real Time report - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Real Time report - Not Supported"), KTraceFileName, __LINE__);

            if (qRelTime)
			    iTrace->Trace(_L("Quasi Real Time report - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Quasi Real Time report - Not Supported"), KTraceFileName, __LINE__);

            if (batch)
			    iTrace->Trace(_L("Batch reporting - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Batch reporting - Not Supported"), KTraceFileName, __LINE__);


            // Set false all batch reporting params
			TBuf <80> trgStr;
            batchRep.GetReportPosition(position);
            batchRep.GetReportMeasurement(measure);
            batchRep.GetMaxPositions(maxPos);
            batchRep.GetMaxMeasurements(maxMeasure);

            if (position)
			    iTrace->Trace(_L("Position in batch report - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Position in batch report - Not Supported"), KTraceFileName, __LINE__);

            if (measure)
			    iTrace->Trace(_L("Measurement in batch report - Supported"), KTraceFileName, __LINE__);
            else
			    iTrace->Trace(_L("Measurement in batch report - Not Supported"), KTraceFileName, __LINE__);

            trgStr.Copy(_L("Max positions "));
            trgStr.AppendNum(maxPos);
            trgStr.Append(_L(" Max Measurement "));
            trgStr.AppendNum(maxMeasure);
						iTrace->Trace(trgStr, KTraceFileName, __LINE__);

            // Session capability
            sessionCaps.GetMaxNumberTotalSessions(totSessn);
            sessionCaps.GetMaxNumberPeriodicSessions(maxPrdSession);
            sessionCaps.GetMaxNumberTriggeredSessions(maxTrgSession);
		}

		// Location ID
		if(iLocationId)
		{
			iTrace->Trace(_L("Location ID ......."), KTraceFileName, __LINE__);
			
			TBuf<50> locationIDTypeStr;
			locationIDTypeStr.Append(_L("Location ID Type - "));
			COMASuplLocationIdVer2::TOMASuplCellInfoType cellInfoType;
			COMASuplLocationIdVer2::TOMASuplStatus status;			
			TInt refMNC, refMcc, refCI, refLAC;
			TInt err;

			cellInfoType = iLocationId->SuplCellInfoType();
			switch(cellInfoType)
			{	
			case COMASuplLocationIdVer2::EGSM:
				locationIDTypeStr.Append(_L("GSM"));
				iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);

				COMASuplGSMCellInfo *gsmCellInfo;

				err = iLocationId->SuplLocationId(gsmCellInfo, status);
				if(KErrNone == err)
				{
					TBuf <100> gsmCellInfoStr;
					gsmCellInfoStr.Append(_L("GSM Cell Info - "));
					gsmCellInfo->SuplGSMCellInfo(refMNC, refMcc, refCI, refLAC);
					gsmCellInfoStr.Append(_L("MNC - "));
					gsmCellInfoStr.AppendNum(refMNC);
					gsmCellInfoStr.Append(_L(" MCC - "));
					gsmCellInfoStr.AppendNum(refMcc);
					gsmCellInfoStr.Append(_L(" CI - "));
					gsmCellInfoStr.AppendNum(refCI);
					gsmCellInfoStr.Append(_L(" LAC - "));
					gsmCellInfoStr.AppendNum(refLAC);

					iTrace->Trace(gsmCellInfoStr, KTraceFileName, __LINE__);
				}
				break;

			case COMASuplLocationIdVer2::EWCDMA:
				locationIDTypeStr.Append(_L("WCDMA"));
				iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);

				COMASuplCellInfo *wcdmaCellInfo;

				err = iLocationId->SuplLocationId(wcdmaCellInfo, status);
				if(KErrNone == err)
				{
					TBuf <100> wcdmaCellInfoStr;
					wcdmaCellInfoStr.Append(_L("WCDMA Cell Info - "));
					wcdmaCellInfo->SuplCellInfo(refMNC, refMcc, refCI);
					wcdmaCellInfoStr.Append(_L("MNC - "));
					wcdmaCellInfoStr.AppendNum(refMNC);
					wcdmaCellInfoStr.Append(_L(" MCC - "));
					wcdmaCellInfoStr.AppendNum(refMcc);
					wcdmaCellInfoStr.Append(_L(" CI - "));
					wcdmaCellInfoStr.AppendNum(refCI);

					iTrace->Trace(wcdmaCellInfoStr, KTraceFileName, __LINE__);
				}

				break;
			}
			TBuf <60> locationIdStatusStr;
			locationIdStatusStr.Append(_L("Location ID Status - "));
			switch(status)
			{
			case COMASuplLocationIdVer2::EStale:
				locationIdStatusStr.Append(_L("Stale"));
				break;
			case COMASuplLocationIdVer2::ECurrent:
				locationIdStatusStr.Append(_L("Current"));
				break;
			case COMASuplLocationIdVer2::EUnknown:
				locationIdStatusStr.Append(_L("Unknown"));
				break;
			}
			iTrace->Trace(locationIdStatusStr, KTraceFileName, __LINE__);
		}
#endif

		if(iIsQoPPresent)
			{
				iTrace->Trace(_L("QoP ....."), KTraceFileName, __LINE__);
				TOMASuplQop Qop;
				TReal32 horAccuracy;
				TReal32 verAccuracy;
				TInt maxLocationAge;
				TInt delay;
				
				if(iClientQop.GetHorizontalAccuracy(horAccuracy) != KErrNotFound)
					{
						TReal C = 10.0;
						TReal x = 0.1; // From 3GPP GAD
						Qop.SetSuplQop(FindK2(horAccuracy,C,x));
						
#ifdef PRINT_MESSAGE
						TBuf <30> horAccStr;
						horAccStr.Append(_L("Horizontal Accuracy - "));
						horAccStr.AppendNum(horAccuracy);
						iTrace->Trace(horAccStr, KTraceFileName, __LINE__);
#endif
					}

				if(iClientQop.GetVerticalAccuracy(verAccuracy) != KErrNotFound)
					{
						TReal C = 45.0;
						TReal x = 0.025; // From 3GPP GAD
						Qop.SetVerticalAccuracy(FindK2(verAccuracy,C,x));
#ifdef PRINT_MESSAGE
						TBuf <32> vertAccStr;
						vertAccStr.Append(_L("Vertical Accuracy - "));
						vertAccStr.AppendNum(verAccuracy);
						iTrace->Trace(vertAccStr, KTraceFileName, __LINE__);
#endif
					}

				if(iClientQop.GetMaxLocationAge(maxLocationAge) != KErrNotFound)
					{
						Qop.SetMaxLocationAge(maxLocationAge);
#ifdef PRINT_MESSAGE
						TBuf <32> maxAgeStr;
						maxAgeStr.Append(_L("Maximum Location Age - "));
						maxAgeStr.AppendNum(maxLocationAge);
						iTrace->Trace(maxAgeStr, KTraceFileName, __LINE__);
#endif
					}
					
				if(iClientQop.GetDelay(delay) != KErrNotFound)
					{	
						// Calculate the delay to be sent to SLP from the
						// delay received from client lib
						
						// Client library gives 2^x and we need to send
						// x to SLP. Therefore, conversion is -
						// x = (log (2^x) / log 2)
						
						TReal clientDelay = delay;
						TReal slpDelay;
						TReal logClientDelay;
						TReal logTwo;
						
						const TReal two = 2;
						
						TInt err = Math::Log(logClientDelay, clientDelay);
						if(KErrNone == err)
						{
							err  = Math::Log(logTwo, two);
							if(KErrNone == err)
							{
								slpDelay = logClientDelay/logTwo;
								Qop.SetDelay(slpDelay);		
#ifdef PRINT_MESSAGE
						TBuf <32> delayStr;
						delayStr.Append(_L("Delay - "));
						delayStr.AppendNum(slpDelay);
						iTrace->Trace(delayStr, KTraceFileName, __LINE__);
#endif
							}
						}
						
					}
					
				OMASuplStart->SetQop(Qop);
			}

        if (iRequestType == COMASuplSession::ESUPL_TERMINAL)
            {                
							// Trigger parameters
						iTrace->Trace(_L("Trigger Params ..."), KTraceFileName, __LINE__); 							
            TOMASuplTriggerParams trgParams;
            TOMASuplPeriodicParams prdParams; 
            TUint fixes, interval, starttime;
           TInt errorCode= iTriggerParams.Get(fixes, interval, starttime);
            if(errorCode==KErrNone)
             {
#ifdef PRINT_MESSAGE	
		    		iTrace->Trace(_L("Periodic Trigger params ..."), KTraceFileName, __LINE__); 							
	          TBuf<128> buf;
            buf.Copy(_L("Fixes ="));
            buf.AppendNum(fixes);
            buf.Append(_L(" Interval ="));
            buf.AppendNum(interval);
            buf.Append(_L(" StartTime ="));
            buf.AppendNum(starttime);
            iTrace->Trace(buf,KTraceFileName, __LINE__); 
#endif

            prdParams.SetSuplPeriodicParams(fixes, interval);
            prdParams.SetStartTimeSet(starttime);
            trgParams.SetPeriodicParams(prdParams);

            OMASuplStart->SetTriggerType(COMASuplTriggeredStart::ETOMASuplPeriodicTrigType);
				    iTrace->Trace(_L("Trigger Type - Periodic ..."), KTraceFileName, __LINE__); 							
	          OMASuplStart->SetTriggerParams(trgParams);
	           }
            }
			
		iTrace->Trace(_L("Starting Encoding..."), KTraceFileName, __LINE__); 							
		
		HBufC8 *encodedBuffer = NULL;
		TInt error;
		TRAPD(err,encodedBuffer = iOMASuplAsnHandlerBase->EncodeL(OMASuplStart,error));
		CleanupStack::PopAndDestroy(OMASuplMessageBase);
		
		 if ( error == KErrNone && err == KErrNone )
		 	{
		 			iTrace->Trace(_L("Encoding Done..."), KTraceFileName, __LINE__); 							
		 			aError = KErrNone;
		 			SetPrefferedBack();
#ifdef PRINT_MESSAGE
                    RFile file;
	                RFs   fs;
	                TInt cErr=fs.Connect();
	                TInt fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
                    if (fErr == KErrNotFound || fErr == KErrPathNotFound)
                        {                             
                        TBool ret = BaflUtils::FolderExists(fs,_L("c:\\logs\\epos\\"));
                        if(!ret)
                        	{
                        		fs.MkDirAll(_L("c:\\logs\\epos\\"));
                        	}
                        file.Create(fs, _L("c:\\logs\\epos\\packet.txt"), EFileWrite|EFileShareAny);
	                    fErr = file.Open(fs,_L("c:\\logs\\epos\\packet.txt"),EFileWrite|EFileShareAny);
                        }
                    TInt aPos;
                    file.Seek(ESeekEnd, aPos);
                    _LIT8(KMsgType, "SUPL START");
                    file.Write(KMsgType);
                    file.Write(_L8("\r \n"));
	                file.Write(*encodedBuffer);
                    file.Write(_L8("\r \n"));
	                file.Close();
	                fs.Close();
#endif
					return encodedBuffer;
		 	}
		 else
		 	{
		 		iTrace->Trace(_L("Encoding Failed."), KTraceFileName, __LINE__); 
				aError = error;	
		 		delete encodedBuffer;
		 		encodedBuffer = NULL;
		 	}
		 	
		 	return NULL;
	}
 
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::LocationIDRequestCompleted
// Gets called when location request completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::LocationIDRequestCompletedL(COMASuplLocationId* aLocationId,
													TInt aErrorCode)
	{
		iTrace->Trace(_L("COMASuplTriggerStartState::LocationIDRequestCompleted..."), KTraceFileName, __LINE__); 								
		iGenerationStatus = EFalse;
		if(aErrorCode!=KErrNone)
			{
				iTrace->Trace(_L("COMASuplTriggerStartState::LocationRequestFailed"), KTraceFileName, __LINE__); 					
				if(iMsgStateObserver)
					{
						iMsgStateObserver->OperationCompleteL(aErrorCode);
						return;
					}
			}
			
		COMASuplGSMCellInfo* cellInfo;
        COMASuplLocationId::TOMASuplStatus status;
        TInt err =aLocationId->SuplLocationId(cellInfo, status);
        if(KErrNone != err)
        	return;

		TInt refMNC,refMCC,refCI,refLac;
		cellInfo->SuplGSMCellInfo(refMNC,refMCC,refCI,refLac);
		
        COMASuplGSMCellInfo* cellInfoClone = COMASuplGSMCellInfo::NewL();
        cellInfoClone->SetSuplGSMCellInfo(refMNC,refMCC,refCI,refLac);
		iLocationId->SetSuplLocationId(cellInfoClone, status);
				
		if(iECId)
			{
				iTrace->Trace(_L("COMASuplTriggerStartState::LocationIDRequestCompletedL...Retrive E-CellId"), KTraceFileName, __LINE__); 					
				iGenerationStatus = ETrue;
				iLocationIDRequestor->GetECellID();
			}
		else
			{	
				GetAssistceDataFromPluginL(aErrorCode);
			}	
		delete aLocationId;
		aLocationId=NULL;
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::LocationECellIdRequestCompletedL
// Gets called when E-Cell Id location request completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::LocationECellIdRequestCompletedL(TOMASuplECellInfo& aECId,TInt aErrorCode)
	{
		
		iGenerationStatus = EFalse;
		if(aErrorCode!=KErrNone)
			{
				iTrace->Trace(_L("COMASuplTriggerStartState::LocationECellIdRequestCompletedL...Failed...still continueing... "), KTraceFileName, __LINE__); 					
				iECId = EFalse; //Since getting E-Cell id is failed....
				aErrorCode = KErrNone;
#if 0					
				if(iMsgStateObserver)
				{
					iMsgStateObserver->OperationCompleteL(aErrorCode);
					return;
				}
#endif
			}
		else
			{
				iMmCellInfo = aECId;
			}			
			
		GetAssistceDataFromPluginL(aErrorCode);
	}
	
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::ComparisionLocationIDRequestCompletedL
// Gets called when location request for comparision completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::ComparisionLocationIDRequestCompletedL(COMASuplLocationId* aLocationId,
        TInt /*aErrorCode*/)
    {
    if( aLocationId )
    	{
    	delete aLocationId;
    	aLocationId=NULL;
    	}
    }
    
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::GetPosParamsL() 
// Gets called when location request completed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::GetPosParamsL() 
	{
		iSETCapabilities = COMASuplSETCapabilities::NewL();
		iSETCapabilities->SetType(COMASuplInfoRequest::EOMASuplSETCapabilities);
		
		iPosRequestor->CreateListL();
		iPosRequestor->AppendInfoRequest(iSETCapabilities);  //takes ownership
		iPosRequestor->SetObserver(this);
		User::LeaveIfError(iPosRequestor->GetSuplInfoL());
	}	

// -----------------------------------------------------------------------------
// COMASuplPosInitState::OperationCompleteL
// An observer method that indicates the completion of
// data retrieval from POS Message plugin
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::OperationCompleteL(TInt aError) 
	{
		iTrace->Trace(_L("COMASuplTriggerStartState::OperationCompleteL start"), KTraceFileName, __LINE__);
		if(iMsgStateObserver)
		{
			if(aError == KErrNone)
				{
					aError = ValidateSetCapabilities(); 	
				}
			iTrace->Trace(_L("COMASuplTriggerStartState::OperationCompleteL returning"), KTraceFileName, __LINE__);	
			iMsgStateObserver->OperationCompleteL(aError);
		}
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::Capabilities 
// Returns iCapabilities 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplSETCapabilities* COMASuplTriggerStartState::Capabilities()
	{
	return iSETCapabilities;
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::Capabilities
// Returns iCapabilities 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::GetPositionCompleteL(TInt /* err */)
	{
	/* 
	No Body
	*/
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::InitilizeAllowedCapabilities
// Retrives capability from Allowed capabilities & puts into iSETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::InitilizeAllowedCapabilities()
	{
	
	iTrace->Trace(_L("Start of COMASuplTriggerStartState::InitilizeAllowedCapabilities"), KTraceFileName, __LINE__); 															
	
	TOMASuplPosTechnology posTechnology;
	COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
	TOMASuplPosProtocol   posProtocol;
	
	TBool  GpsSETAssisted 	= ETrue;
	TBool  GpsSETBased 		= ETrue;
	TBool  AutonomousGps 	= ETrue;
	TBool  FLT 			 	= ETrue;

	TBool  eOTD 		 	= ETrue;
	TBool  oTDOA 		 	= ETrue;
	
	TBool TIA801 = ETrue; 
	TBool RRLP = ETrue;
	TBool RRC =  ETrue;

	posTechnology.SetPosTechnology(GpsSETAssisted,GpsSETBased,AutonomousGps,FLT,
													   eOTD,oTDOA);
	posProtocol.SetPosProtocol(TIA801,RRLP,RRC);
	prefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
	
	iAllowedCapabilities->SetSETCapabilities(posTechnology,prefMethod,posProtocol);	
	
	iTrace->Trace(_L("End of COMASuplTriggerStartState::InitilizeAllowedCapabilities"), KTraceFileName, __LINE__); 															
	}
	
// -----------------------------------------------------------------------------
// DoANDOperation
// For manipulating SET capabiliites
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool COMASuplTriggerStartState::DoANDOperation(TInt aFlag, TInt aCapability) 
	{
		if ((aFlag & aCapability) > 0) 
			return ETrue;
		else
			return EFalse;
	}
	
// -----------------------------------------------------------------------------
// COMASuplSession::FillAllowedCapabilities
// Retrives capability from Allowed capabilities & puts into iSETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::FillAllowedCapabilities(TInt aAllowedCapabilities)
	{
	
	iTrace->Trace(_L("Start of COMASuplTriggerStartState::FillAllowedCapabilities"), KTraceFileName, __LINE__); 															
	
	TOMASuplPosTechnology posTechnology;
	COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
	TOMASuplPosProtocol   posProtocol;
	
	TBool  GpsSETAssisted 	= DoANDOperation(aAllowedCapabilities,KGpsSETAssisted);
	TBool  GpsSETBased 		= DoANDOperation(aAllowedCapabilities,KGpsSETBased);
	TBool  AutonomousGps 	= DoANDOperation(aAllowedCapabilities,KAutonomousGps);
	TBool  FLT 			 	= DoANDOperation(aAllowedCapabilities,KAFLT);
		   iECId 		 	= DoANDOperation(aAllowedCapabilities,KECID);
	TBool  eOTD 		 	= DoANDOperation(aAllowedCapabilities,KEOTD);
	TBool  oTDOA 		 	= DoANDOperation(aAllowedCapabilities,KOTDOA);
	
	TBool TIA801 = EFalse;
	TBool RRLP = EFalse;
	TBool RRC =  EFalse;

	posTechnology.SetPosTechnology(GpsSETAssisted,GpsSETBased,AutonomousGps,FLT,
													   eOTD,oTDOA);
	posProtocol.SetPosProtocol(TIA801,RRLP,RRC);
	prefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
	
	TBuf<128> msg(_L("COMASuplTriggerStartState::FillAllowedCapabilities : "));
	msg.AppendNum(GpsSETAssisted);
	msg.Append(_L(", "));
	msg.AppendNum(GpsSETBased);
	msg.Append(_L(", "));
	msg.AppendNum(AutonomousGps);
	msg.Append(_L(", "));
	msg.AppendNum(FLT);
	msg.Append(_L(", "));
	msg.AppendNum(eOTD);
	msg.Append(_L(", "));
	msg.AppendNum(oTDOA);
	msg.Append(_L(", "));
	msg.AppendNum(iECId);
	msg.Append(_L(", "));
	msg.AppendNum(ETrue);
	
	iTrace->Trace(msg, KTraceFileName, __LINE__);
	iAllowedCapabilities->SetSETCapabilities(posTechnology,prefMethod,posProtocol);	
	
	iTrace->Trace(_L("End of COMASuplTriggerStartState::FillAllowedCapabilities"), KTraceFileName, __LINE__); 															
	
	if(iPosRequestor) 
		return;

		//For Time being.... //IF POS is not there 
		{
				TOMASuplPosTechnology posTechnology;
				COMASuplSETCapabilities::TOMASuplPrefMethod    prefMethod;
				TOMASuplPosProtocol   posProtocol;

				TBool  GpsSETAssisted 	= EFalse;
				TBool  GpsSETBased 		= EFalse;
				TBool  AutonomousGps 	= EFalse;
				TBool  FLT 			 	= EFalse;
 
				TBool  eOTD 		 	= EFalse;
				TBool  oTDOA 		 	= EFalse;

				TBool TIA801 = EFalse; 
				TBool RRLP = EFalse;
				TBool RRC =  EFalse;

				posTechnology.SetPosTechnology(GpsSETAssisted,GpsSETBased,AutonomousGps,FLT,
												   eOTD,oTDOA);
				posProtocol.SetPosProtocol(TIA801,RRLP,RRC);
				prefMethod = COMASuplSETCapabilities::EOMANoPreferred; 

				iSETCapabilities->SetSETCapabilities(posTechnology,prefMethod,posProtocol);	
		}

	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::ComputeCapabilities
// Does ANDing of SETCapabilities with AllowedCapabilities & stores in SETCapabilities
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::ComputeCapabilities()
{	
	iTrace->Trace(_L("Start of COMASuplTriggerStartState::ComputeCapabilities"), KTraceFileName, __LINE__); 															
	
	TOMASuplPosProtocol   SETposProtocol;
	COMASuplSETCapabilities::TOMASuplPrefMethod    SETprefMethod;
	TOMASuplPosTechnology SETposTechnology;
	
	TOMASuplPosProtocol   AllowedposProtocol;
	COMASuplSETCapabilities::TOMASuplPrefMethod    AllowedprefMethod;
	TOMASuplPosTechnology AllowedposTechnology;
	
	TBool  SETGpsSETAssisted,AllowedGpsSETAssisted;
	TBool  SETGpsSETBased,AllowedGpsSETBased;
	TBool  SETAutonomousGps,AllowedAutonomousGps;
	TBool  SETeOTD,AllowedeOTD;
	TBool  SEToTDOA,AllowedoTDOA;
	TBool SETFLT, AllowedFLT;	
	TBool SETTIA801,AllowedTIA801; 
	TBool SETRRLP,AllowedRRLP;
	TBool SETRRC,AllowedRRC;
	
	iSETCapabilities->GetSETCapabilities(SETposTechnology,SETprefMethod,SETposProtocol);	
	iAllowedCapabilities->GetSETCapabilities(AllowedposTechnology,AllowedprefMethod,AllowedposProtocol);	

	SETposTechnology.GetPosTechnology(SETGpsSETAssisted,SETGpsSETBased,SETAutonomousGps,SETFLT,
													   SETeOTD,SEToTDOA);
	SETposProtocol.GetPosProtocol(SETTIA801,SETRRLP,SETRRC);
 

	AllowedposTechnology.GetPosTechnology(AllowedGpsSETAssisted,AllowedGpsSETBased,AllowedAutonomousGps,AllowedFLT,
													   AllowedeOTD,AllowedoTDOA);
	SETposProtocol.GetPosProtocol(AllowedTIA801,AllowedRRLP,AllowedRRC);
	AllowedprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 

	SETGpsSETAssisted &= AllowedGpsSETAssisted;
	SETGpsSETBased &= AllowedGpsSETBased;
	SETAutonomousGps &= AllowedAutonomousGps;
	
	SETFLT &= AllowedFLT; 
	SETeOTD &=AllowedeOTD;
	SEToTDOA &=AllowedoTDOA;
	
	SETTIA801 &= AllowedTIA801; 
	SETRRLP &= AllowedRRLP;
	SETRRC &= AllowedRRC;
	
	//If client has restricted SETBased and still POS msg plugin sets SETBasedPreferred.....then 
	// it as EOMANoPreferred
	///Save this one ...need for next message comparisions....
	iPOSMsgSETprefMethod = SETprefMethod;
	
	if(SETprefMethod == COMASuplSETCapabilities::EOMAAGpsSETBasedPreferred && (!SETGpsSETBased ))
		{
			SETprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
		}
		
	if(SETprefMethod == COMASuplSETCapabilities::EOMAAGpsSETAssitedPreferred  && (!SETGpsSETAssisted))
		{
			SETprefMethod = COMASuplSETCapabilities::EOMANoPreferred; 
		}
	
	SETposTechnology.SetPosTechnology(SETGpsSETAssisted,SETGpsSETBased,SETAutonomousGps,SETFLT,
													   SETeOTD,SEToTDOA);
	
	SETposProtocol.SetPosProtocol(SETTIA801,SETRRLP,SETRRC);
	
	iSETCapabilities->SetSETCapabilities(SETposTechnology,SETprefMethod,SETposProtocol);	
	
	iTrace->Trace(_L("End of COMASuplTriggerStartState::ComputeCapabilities"), KTraceFileName, __LINE__); 															
}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::ValidateSetCapabilities
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt COMASuplTriggerStartState::ValidateSetCapabilities() 
	{
		if(!iPosRequestor)  //No need to check status if POS is not there
			{
				return KErrNone;  
			}
		
		return iSETCapabilities->Status();
	}
	
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::SetQop
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::SetQop(TSuplTerminalQop& aQop)
	{
		iIsQoPPresent = ETrue;
		iClientQop = aQop;
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::SetPrefferedBack
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::SetPrefferedBack()
	{
		TOMASuplPosProtocol2   posProtocol;
		COMASuplSETCapabilitiesVer2::TOMASuplPrefMethod2    prefMethod;
		TOMASuplPosTechnology2 posTechnology;
		if(iSETCapabilities2)
		iSETCapabilities2->GetSETCapabilities(posTechnology,prefMethod,posProtocol);	
		
	}
	
	
// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::GetAssistceDataFromPluginL
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::GetAssistceDataFromPluginL(TInt aErrorCode)
	{
		iTrace->Trace(_L("COMASuplTriggerStartState::GetAssistceDataFromPluginL start"), KTraceFileName, __LINE__);
		if(iPosRequestor)
			{
				TRAPD(err,GetPosParamsL());
				
				if(err != KErrNone && iMsgStateObserver)
				{
					iMsgStateObserver->OperationCompleteL(err);
				}
			}
		else
			{
				if(iMsgStateObserver)
				{
					iTrace->Trace(_L("COMASuplTriggerStartState::GetAssistceDataFromPluginL returning"), KTraceFileName, __LINE__);
					iMsgStateObserver->OperationCompleteL(aErrorCode);
				}
			}
		
	}

// -----------------------------------------------------------------------------
// COMASuplTriggerStartState::GetCurrentCellID
// Get Current Cell id
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplTriggerStartState::GetCurrentCellID(TCellIdInfo& aCurrentCellId,TInt& aType)
	{
		TInt lRefMNC = -1;
		TInt lRefMCC = -1;
		TInt lRefLAC = -1;
		TInt lRefCI  = -1;
		aType = 0;
		if(iLocationId)
		{
			iTrace->Trace(_L("Location ID ......."), KTraceFileName, __LINE__);
			
			TBuf<50> locationIDTypeStr;
			locationIDTypeStr.Append(_L("Location ID Type - "));
			COMASuplLocationIdVer2::TOMASuplCellInfoType cellInfoType;
			COMASuplLocationIdVer2::TOMASuplStatus status;			
			TInt err;

			cellInfoType = iLocationId->SuplCellInfoType();
			switch(cellInfoType)
			{	
				case COMASuplLocationIdVer2::EGSM:
					{
						locationIDTypeStr.Append(_L("GSM"));
						iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);
						COMASuplGSMCellInfo *gsmCellInfo;
						err = iLocationId->SuplLocationId(gsmCellInfo, status);
						if(KErrNone == err)
						{
							TBuf <128> gsmCellInfoStr;
							gsmCellInfoStr.Append(_L("GSM Cell Info:"));
							gsmCellInfo->SuplGSMCellInfo(lRefMNC, lRefMCC, lRefCI, lRefLAC);
							gsmCellInfoStr.Append(_L("MNC - "));
							gsmCellInfoStr.AppendNum(lRefMNC);
							gsmCellInfoStr.Append(_L(" MCC - "));
							gsmCellInfoStr.AppendNum(lRefMCC);
							gsmCellInfoStr.Append(_L(" CI - "));
							gsmCellInfoStr.AppendNum(lRefCI);
							gsmCellInfoStr.Append(_L(" LAC - "));
							gsmCellInfoStr.AppendNum(lRefLAC);
							iTrace->Trace(gsmCellInfoStr, KTraceFileName, __LINE__);
							aType = 0;
						}
						break;
					}
				case COMASuplLocationIdVer2::EWCDMA:
					{
						locationIDTypeStr.Append(_L("WCDMA"));
						iTrace->Trace(locationIDTypeStr, KTraceFileName, __LINE__);
						COMASuplCellInfo *wcdmaCellInfo;
						err = iLocationId->SuplLocationId(wcdmaCellInfo, status);
						if(KErrNone == err)
						{
							TBuf <128> wcdmaCellInfoStr;
							wcdmaCellInfoStr.Append(_L("WCDMA Cell Info:"));
							wcdmaCellInfo->SuplCellInfo(lRefMNC, lRefMCC, lRefCI);
							wcdmaCellInfoStr.Append(_L("MNC - "));
							wcdmaCellInfoStr.AppendNum(lRefMNC);
							wcdmaCellInfoStr.Append(_L(" MCC - "));
							wcdmaCellInfoStr.AppendNum(lRefMCC);
							wcdmaCellInfoStr.Append(_L(" CI - "));
							wcdmaCellInfoStr.AppendNum(lRefCI);
							iTrace->Trace(wcdmaCellInfoStr, KTraceFileName, __LINE__);
							aType = 1;
						}
						break;
					}	
		}
		
	aCurrentCellId.iCid = lRefCI;
	aCurrentCellId.iMNC = lRefMNC; 
	aCurrentCellId.iMCC = lRefMCC;
	aCurrentCellId.iLac = lRefLAC;
	}
}	//  End of File
