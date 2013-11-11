/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles test execution and monitors user input.
*
*/

#include <epos_comasuplposition.h>
#include <epos_comasuplpospayload.h>
#include <epos_comasuplvelocity.h>

#include "epos_tomasuplmatchdatatypes.h"
#include "epos_comasuplpostesterlogger.h"

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::PositionsMatch
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::PositionsMatch(COMASuplPosition& aPrev
													, COMASuplPosition& aCurr)
	{
	//iVelocityPresent
	TBool objectsMatch = EFalse;
	TInt errPrev = KErrGeneral;
	TInt errCurr = KErrGeneral;
	
	TOMASuplUtcTime utcTimePrev;
	TOMASuplUtcTime utcTimeCurr;
	TOMASuplPositionEstimate positionEstimatePrev;
	TOMASuplPositionEstimate positionEstimateCurr;
	COMASuplVelocity* velocityPrev = NULL; 
	COMASuplVelocity* velocityCurr = NULL;

	aPrev.GetPosition(utcTimePrev, positionEstimatePrev);//returns void
	aCurr.GetPosition(utcTimeCurr, positionEstimateCurr);//returns void
	
	errPrev = aPrev.GetVelocity(velocityPrev);
	errCurr = aCurr.GetVelocity(velocityCurr);
	if(errPrev != errCurr)
		{
		return EFalse;
		}
	if(errPrev == KErrNone)
		{
		//Velocity
		objectsMatch = MatchVelocity(*velocityPrev, *velocityCurr);
		if(!objectsMatch)
			{
			return EFalse;
			}
		}
	//utc time
	objectsMatch = MatchUtcTime(utcTimePrev, utcTimeCurr);
	if(!objectsMatch)
		{
		return EFalse;
		}
	//PositionEstimate
	objectsMatch = MatchPositionEstimate(positionEstimatePrev
									, positionEstimateCurr);
	if(!objectsMatch)
		{
		return EFalse;
		}
	
	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchUtcTime
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchUtcTime(TOMASuplUtcTime& aPrev
												, TOMASuplUtcTime& aCurr)
	{
	TDateTime prevTimeStamp;
	TDateTime currTimeStamp;
	TInt prevZoneCode = KNoValue;
	TInt currZoneCode = KNoValue;
	TInt prevZone = KNoValue;
	TInt currZone = KNoValue;
	aPrev.GetUtcTime(prevTimeStamp,prevZoneCode,prevZone);
	aCurr.GetUtcTime(currTimeStamp,currZoneCode,currZone);
	
	TTime prevTime(prevTimeStamp);
	TTime currTime(currTimeStamp);
	if(prevTime != currTime)
		{
		return EFalse;
		}
	
	if(prevZoneCode != currZoneCode)
		{
		return EFalse;
		}
	if(prevZone != currZone)
		{
		return EFalse;
		}

	return ETrue;	
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchPositionEstimate
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchPositionEstimate(
											TOMASuplPositionEstimate& aPrev
											, TOMASuplPositionEstimate& aCurr)
	{
	TInt errPrev = KErrGeneral;
	TInt errCurr = KErrGeneral;
	TBool objectMatch = EFalse;
	
	//MatchUnCertainity
	TOMASuplUncertainty uncertaintyPrev;
	TOMASuplUncertainty uncertaintyCurr;
	errPrev = aPrev.GetUncertainty(uncertaintyPrev);
	errCurr = aCurr.GetUncertainty(uncertaintyCurr);
	if(errPrev == errCurr)
		{
		if( KErrNone == errPrev)
			{
			objectMatch = MatchUnCertainity(uncertaintyPrev, uncertaintyCurr);
			if(!objectMatch)
				{
				return EFalse;
				}
			}
		
		}
	else
		{
		return EFalse;
		}
	
	//MatchConfidence
	TInt confidencePrev = KNoValue;
	TInt confidenceCurr = KNoValue;
	errPrev = aPrev.GetConfidence(confidencePrev);
	errCurr = aCurr.GetConfidence(confidenceCurr);
	if(errPrev == errCurr)
		{
		if(KErrNone == errPrev)
			{
			objectMatch = MatchConfidence(confidencePrev, confidenceCurr);
			if(!objectMatch)
				{
				return EFalse;
				}
			}
		}
	else
		{
		return EFalse;
		}
	
	//MatchAltitudeInfo
	TOMASuplAltitudeInfo altitudeInfoPrev;
	TOMASuplAltitudeInfo altitudeInfoCurr;
	errPrev = aPrev.GetAltitudeInfo(altitudeInfoPrev);
	errCurr = aCurr.GetAltitudeInfo(altitudeInfoCurr);
	if(errPrev == errCurr)
		{
		if(KErrNone == errPrev)
			{
			objectMatch = MatchAltitudeInfo(altitudeInfoPrev, altitudeInfoCurr);
			if(!objectMatch)
				{
				return EFalse;
				}
			}
		}
	else
		{
		return EFalse;
		}
	
	//Match LatitudeSign, Latitude and Longitude
	TOMASuplPositionEstimate::TOMASuplLatitudeSign latSignPrev;
	TInt latitudePrev = KNoValue;
	TInt longitudePrev = KNoValue;
	
	TOMASuplPositionEstimate::TOMASuplLatitudeSign latSignCurr;
	TInt latitudeCurr = KNoValue;
	TInt longitudeCurr = KNoValue;
	
	aPrev.GetPositionEstimate(latSignPrev, latitudePrev, longitudePrev);
	aCurr.GetPositionEstimate(latSignCurr, latitudeCurr, longitudeCurr);
	if(latSignPrev != latSignCurr)
		{
		return EFalse;
		}
	if(latitudePrev != latitudeCurr)
		{
		return EFalse;
		}
	if(longitudePrev != longitudeCurr)
		{
		return EFalse;
		}

	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchUnCertainity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchUnCertainity(
												TOMASuplUncertainty& aPrev
											, TOMASuplUncertainty& aCurr)
	{
	TInt uncertaintySemiMajorPrev = KNoValue;
    TInt uncertaintySemiMinorPrev = KNoValue;
    TInt orientationMajorAxisPrev = KNoValue;
    
    TInt uncertaintySemiMajorCurr = KNoValue;
    TInt uncertaintySemiMinorCurr = KNoValue;
    TInt orientationMajorAxisCurr = KNoValue;
    
    aPrev.GetUncertainty(uncertaintySemiMajorPrev, uncertaintySemiMinorPrev
    				, orientationMajorAxisPrev);
    aCurr.GetUncertainty(uncertaintySemiMajorCurr, uncertaintySemiMinorCurr
    				,orientationMajorAxisCurr);
    if(uncertaintySemiMajorPrev != uncertaintySemiMajorCurr)
    	{
    	return EFalse;
    	}
    if(uncertaintySemiMinorPrev != uncertaintySemiMinorCurr)
    	{
    	return EFalse;
    	}
    if(orientationMajorAxisPrev != orientationMajorAxisCurr)
    	{
    	return EFalse;
    	}
    return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchConfidence
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchConfidence(TInt aPrev, TInt aCurr)
	{
	if(aPrev != aCurr)
		{
		return EFalse;
		}
	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchAltitudeInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchAltitudeInfo(
										TOMASuplAltitudeInfo& aPrev
										, TOMASuplAltitudeInfo& aCurr)
	{
	TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirPrev;
	TOMASuplAltitudeInfo::TOMASuplAltitudeDirection altDirCurr;
    TInt altPrev = KNoValue;
    TInt altCurr = KNoValue;
    TInt altUncertaintyPrev = KNoValue;
    TInt altUncertaintyCurr = KNoValue;
    aPrev.GetAltitudeInfo(altDirPrev, altPrev, altUncertaintyPrev);
    aCurr.GetAltitudeInfo(altDirCurr, altCurr, altUncertaintyCurr);
    if(altDirPrev != altDirCurr)
    	{
    	return EFalse;
    	}
    if(altPrev != altCurr)
    	{
    	return EFalse;
    	}
    if(altUncertaintyPrev != altUncertaintyCurr)
    	{
    	return EFalse;
    	}
    return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchVelocity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchVelocity(COMASuplVelocity& aPrev
												, COMASuplVelocity& aCurr)
	{
	COMASuplHorizVelocity* horizVelocityPrev = aPrev.Velocity();
	COMASuplHorizVelocity* horizVelocityCurr = aCurr.Velocity();
	TOMASuplVelocityType veloTypePrev = horizVelocityPrev->VelType();
	TOMASuplVelocityType veloTypeCurr = horizVelocityCurr->VelType();

	if(veloTypePrev != veloTypeCurr)
		{
		return EFalse;
		}

	switch(veloTypePrev)
		{
		case EHorizVelocity:
			{
			return MatchHorizVelocity(horizVelocityPrev, horizVelocityCurr);
			}
		case EHorizAndVertVelocity:
			{
			COMASuplHorizAndVertVelocity* veloPrev = 
			static_cast<COMASuplHorizAndVertVelocity*>(horizVelocityPrev);
			
			COMASuplHorizAndVertVelocity* veloCurr = 
			static_cast<COMASuplHorizAndVertVelocity*>(horizVelocityCurr);
			
			return MatchHorizAndVertVelocity(veloPrev, veloCurr);
			
			}
		case EHorizUncertVelocity:
			{
			COMASuplHorizUncertVelocity* veloPrev = 
			static_cast<COMASuplHorizUncertVelocity*>(horizVelocityPrev);
			
			COMASuplHorizUncertVelocity* veloCurr = 
			static_cast<COMASuplHorizUncertVelocity*>(horizVelocityCurr);
			
			
			return MatchHorizUncertVelocity(veloPrev, veloCurr);
			
			}
		case EHorizAndVertUncertVelocity:
			{
			COMASuplHorizAndVertUncertVelocity* veloPrev = 
			static_cast<COMASuplHorizAndVertUncertVelocity*>(horizVelocityPrev);
			
			COMASuplHorizAndVertUncertVelocity* veloCurr = 
			static_cast<COMASuplHorizAndVertUncertVelocity*>(horizVelocityCurr);
			
			return MatchHorizAndVertUncertVelocity(veloPrev, veloCurr);
			}
		default:
			{
			return EFalse;
			}
		}
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchHorizVelocity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchHorizVelocity(
											COMASuplHorizVelocity* aPrev
											, COMASuplHorizVelocity* aCurr)
	{
	TUint16 bearingPrev = KNoValue16;
	TUint16 horSpeedPrev = KNoValue16;
	TUint16 bearingCurr = KNoValue16;
	TUint16 horSpeedCurr = KNoValue16;
	aPrev->GetHorizVel(bearingPrev, horSpeedPrev);
	aCurr->GetHorizVel(bearingCurr, horSpeedCurr);
	if(bearingPrev != bearingCurr)
		{
		return EFalse;
		}
	if(horSpeedPrev != horSpeedPrev)
		{
		return EFalse;
		}
	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchHorizAndVertVelocity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchHorizAndVertVelocity(
										COMASuplHorizAndVertVelocity* aPrev
									, COMASuplHorizAndVertVelocity* aCurr)
	{
	TUint16 bearingPrev = KNoValue16;
	TUint16 horSpeedPrev = KNoValue16;
	TUint8 verDirectPrev = KNoValue8;
	TUint8 verSpeedPrev = KNoValue8;
	TUint16 bearingCurr = KNoValue16;
	TUint16 horSpeedCurr = KNoValue16;
	TUint8 verDirectCurr = KNoValue8;
	TUint8 verSpeedCurr = KNoValue8;
	aPrev->GetHorizAndVertVel(bearingPrev, horSpeedPrev
							, verDirectPrev, verSpeedPrev);
	aCurr->GetHorizAndVertVel(bearingCurr, horSpeedCurr
							, verDirectCurr, verSpeedCurr);

	if(!MatchHorizVelocity(aPrev, aCurr))
		{
		return EFalse;
		}

	if(verDirectPrev != verDirectCurr)
		{
		return EFalse;
		}
	if(verSpeedPrev != verSpeedCurr)
		{
		return EFalse;
		}
	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchHorizUncertVelocity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchHorizUncertVelocity(
										COMASuplHorizUncertVelocity* aPrev
									, COMASuplHorizUncertVelocity* aCurr)
	{
	TUint16 bearingPrev = KNoValue16;
	TUint16 horSpeedPrev = KNoValue16;
	TUint8 uncertSpeedPrev = KNoValue8;
	
	TUint16 bearingCurr = KNoValue16;
	TUint16 horSpeedCurr = KNoValue16;
	TUint8 uncertSpeedCurr = KNoValue8;
	aPrev->GetHorizUncertVel(bearingPrev, horSpeedPrev, uncertSpeedPrev);
    aCurr->GetHorizUncertVel(bearingCurr, horSpeedCurr, uncertSpeedCurr);
    
    if(!MatchHorizVelocity(aPrev, aCurr))
		{
		return EFalse;
		}

    if(uncertSpeedPrev != uncertSpeedCurr)
    	{
    	return EFalse;
    	}
	return ETrue;
	}

// -----------------------------------------------------------------------------
// TOMASuplMatchDataTypes::MatchHorizAndVertUncertVelocity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool TOMASuplMatchDataTypes::MatchHorizAndVertUncertVelocity(
							COMASuplHorizAndVertUncertVelocity* aPrev
							, COMASuplHorizAndVertUncertVelocity* aCurr)
	{
	TUint8 horUncertSpeedPrev = KNoValue8;
	TUint8 verUncertSpeedPrev = KNoValue8;
	
	TUint16 bearingPrev = KNoValue16;
	TUint16 horSpeedPrev = KNoValue16;
	TUint8 verDirectPrev = KNoValue8;
	TUint8 verSpeedPrev = KNoValue8;
	
	TUint8 horUncertSpeedCurr = KNoValue8;
	TUint8 verUncertSpeedCurr = KNoValue8;
	
	TUint16 bearingCurr = KNoValue16;
	TUint16 horSpeedCurr = KNoValue16;
	TUint8 verDirectCurr = KNoValue8;
	TUint8 verSpeedCurr = KNoValue8;
	
	if(!MatchHorizAndVertVelocity(aPrev, aCurr))
		{
		return EFalse;
		}
	aPrev->GetHorizVertUncertVel(bearingPrev, horSpeedPrev, verDirectPrev
			, verSpeedPrev, horUncertSpeedPrev, verUncertSpeedPrev);

	aCurr->GetHorizVertUncertVel(bearingCurr, horSpeedCurr, verDirectCurr
			, verSpeedCurr, horUncertSpeedCurr, verUncertSpeedCurr);

	if(horUncertSpeedPrev != horUncertSpeedCurr)
		{
		return EFalse;
		}
	if(verUncertSpeedPrev != verUncertSpeedCurr)
		{
		return EFalse;
		}
	return ETrue;
	}
