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
* Description:  Matches Data types.
*
*/

#ifndef T_TOMASUPLMATCHDATATYPES_H
#define T_TOMASUPLMATCHDATATYPES_H
class COMASuplPosition;
class TOMASuplUtcTime;
class TOMASuplPositionEstimate;
class TOMASuplUncertainty;
class TOMASuplAltitudeInfo;
class COMASuplHorizVelocity;
class COMASuplHorizAndVertVelocity;
class COMASuplHorizUncertVelocity;
class COMASuplHorizAndVertUncertVelocity;

/**
 *  Matches Datatypes.
 *
 *  Provides static methods for comparing data types.
 *  
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1u
 */
class TOMASuplMatchDataTypes
	{
	public:
		/**
	     * Matches two COMASuplPosition objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool PositionsMatch(COMASuplPosition& aPrev
											, COMASuplPosition& aCurr);

		/**
	     * Matches two TOMASuplUtcTime objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchUtcTime(TOMASuplUtcTime& aPrev
												, TOMASuplUtcTime& aCurr);

		/**
	     * Matches two TOMASuplPositionEstimate objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchPositionEstimate(TOMASuplPositionEstimate& aPrev
										, TOMASuplPositionEstimate& aCurr);

		/**
	     * Matches two TOMASuplUncertainty objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchUnCertainity(TOMASuplUncertainty& aPrev
											, TOMASuplUncertainty& aCurr);

		/**
	     * Matches two Confidence objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchConfidence(TInt aPrev, TInt aCurr);

		/**
	     * Matches two TOMASuplAltitudeInfo objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchAltitudeInfo(TOMASuplAltitudeInfo& aPrev
											, TOMASuplAltitudeInfo& aCurr);

		/**
	     * Matches two COMASuplVelocity objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchVelocity(COMASuplVelocity& aPrev
												, COMASuplVelocity& aCurr);

		/**
	     * Matches two COMASuplHorizVelocity objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchHorizVelocity(COMASuplHorizVelocity* aPrev
											, COMASuplHorizVelocity* aCurr);

		/**
	     * Matches two COMASuplHorizAndVertVelocity objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchHorizAndVertVelocity(
										COMASuplHorizAndVertVelocity* aPrev
									, COMASuplHorizAndVertVelocity* aCurr);

		/**
	     * Matches two COMASuplHorizUncertVelocity objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchHorizUncertVelocity(
										COMASuplHorizUncertVelocity* aPrev
									 , COMASuplHorizUncertVelocity* aCurr);
		
		/**
	     * Matches two COMASuplHorizAndVertUncertVelocity objects.
	     *
	     * @since S60 v3.1
	     * @param aPrev 
	     * @param aCurr 
	     * @return ETrue if objects match, else EFalse.
	     */
		static TBool MatchHorizAndVertUncertVelocity(
								COMASuplHorizAndVertUncertVelocity* aPrev
							  , COMASuplHorizAndVertUncertVelocity* aCurr);
	};
#endif