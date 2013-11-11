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
* Description:  Tests for various Error conditions for GetPositionL
*
*/

#ifndef C_COMASUPLPOSITIONTEST_H
#define C_COMASUPLPOSITIONTEST_H

#include "epos_comasuplpostestercategory.h"

class COMASuplPosHandlerBase;
class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplPosition;

/** Constant used for logging name of test*/
_LIT8(KPositionTest,"---------------Position Test---------------");

/** Constant used for logging information about test*/
_LIT8(KPositionTestComplete, "Info: Position Test  Complete.");

/** Constant used for logging information about test*/
_LIT8(KPositionTestCancel, "Info: Position Test  Canceled.");

/** Constant used for logging information about test*/
_LIT8(KPositionTestAborted, "Error: Position Test  Aborted.");

/** Constant used for logging.*/
_LIT8(KAfterEndSessionI, "Info: After Ending session, GetPositionL completes with ");

/** Constant used for logging.*/
_LIT8(KAfterEndSessionW, "Warning: After Ending session, GetPositionL completes with ");

/** Constant used for logging.*/
_LIT8(KGetPositionLCompleteW, "Warning: GetPositionL completes with ");

/** Constant used for logging.*/
_LIT8(KGetPositionLInfo, "Info: Requesting GetPositionL Before \
Initializing session.");

/** Constant used for logging.*/
_LIT8(KGetPositionLSessionInitInfo, "Info: Requesting GetPositionL After \
Initializing session.");

/** Constant used for logging.*/
_LIT8(KGetPositionLSessionEndInfo, "Info: Requesting GetPositionL After \
Ending session.");

/**
 *  Tests for GetPositionL() in various scenarios.
 *
 *  Tests for GetPositionL() in various scenarios and logs warning if there 
 *  is a deviation from expected behaviour. 
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplPositionTest : public COMASuplPosTesterCategory
	{
	private:
		/**  Controls the series of execution of asynchronous functions */
		enum TExecutionSequence
			{
			EStartTest = 0,
			EGetPositionBeforeInitialize,
			EGetPositionCompleteBeforeInitialize,
			EInitializeSession,
			EGetPositionAfterInitialize,
			EGetPositionCompleteAfterInitialize,
			EEndSessionAndGetPosition,
			EGetPositionCompleteAfterEndSession,
			
			};
	public:
		static COMASuplPositionTest* NewL(COMASuplPosHandlerBase*
								 aPosHandler
								, COMASuplPosTesterCategory* aCallBack = NULL);
		virtual ~COMASuplPositionTest();
	private:
		COMASuplPositionTest(COMASuplPosHandlerBase* aPosHandler
							, COMASuplPosTesterCategory* aCallBack);
		void ConstructL();

	public:
		/**
	     * Deletes iPosition and creates a new object for it.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void InitializePositionL();

		/**
	     * Initializes Position object and requests for GerPositionL
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void RequestForPositionL();

	public:
		/**
	     * From COMASuplPosTesterCategory
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void StartTestingL();
	public:
		/**
	     * From COMASuplPosTesterCategory
	     * Handles completion of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void RunL();

		/**
	     * From COMASuplPosTesterCategory
	     * Handles cancellation of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void DoCancel();

		/**
	     * From COMASuplPosTesterCategory
	     * Handles leave from RunL, aborts testing.
	     *
	     * @since S60 v3.1
	     * @param aError: Errorcode of leaving
	     */
		TInt  RunError( TInt aError );
	private:
		/**
	     * Reference to the loaded POS Handler.
	     */
		COMASuplPosHandlerBase& iPosHandler;

		/**
	     * Pointer to session object 
	     * Own.
	     */
		COMASuplPosSessionBase* iSessionBase;

		/**
	     * Identifies the case to be executed when RunL is called
	     */
		TExecutionSequence iNextInSeq;

		/**
		 * Unique request id for session
		 */
		TInt iRequestID;

		/**
	     * Holds the Position object.
	     * Own.
	     */
		COMASuplPosition* iPosition;

	};
#endif