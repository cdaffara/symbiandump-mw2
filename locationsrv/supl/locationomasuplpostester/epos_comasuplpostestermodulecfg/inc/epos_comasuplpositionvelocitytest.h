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
* Description:  For testing of GetPositionL and message transaction.
*
*/

#ifndef C_COMASUPLPOSITIONVELOCITYTEST_H
#define C_COMASUPLPOSITIONVELOCITYTEST_H

#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpositionvelocitymanager.h"
class COMASuplPosHandlerBase;
//class TFileName;
class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplInfoRequestList;
class COMASuplInfoRequestList;
class COMASuplMessageFileReader;

/** Constant used for logging name of test*/
_LIT8(KPositionVelocityTest,"---------------GetPosition Velocity Test---------------");

/** Constant used for logging information about test*/
_LIT8(KPositionVelocityTestComplete, "Info: GetPosition Velocity Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KPositionVelocityTestCancel, "Info: GetPosition Velocity Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KPositionVelocityTestAborted, "Error: GetPosition Velocity Test Aborted");

/**
 *  Tests message transaction and GetpositionL()
 *
 *  Processes instructions in the message file and requests for position
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplPositionVelocityTest : public COMASuplPositionVelocityManager
	{
	public:
		static COMASuplPositionVelocityTest* NewL(COMASuplPosHandlerBase* 
									aPosHandler
									, TInt aGetPositionReqCount
									, TFileName& aFileName
									, COMASuplPosTesterCategory* aCallBack = NULL);

		virtual ~COMASuplPositionVelocityTest();
	
	private:
		COMASuplPositionVelocityTest(COMASuplPosHandlerBase* 
									aPosHandler
									, COMASuplPosTesterCategory* aCallBack
									, TInt aGetPositionReqCount
									, TFileName& aFileName);
		void ConstructL();

	public:
		//derived form MOMASuplTestObserver through COMASuplPosTesterCategory
		/**
	     * From COMASuplPosTesterCategory
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void StartTestingL();

		/**
	     * From COMASuplPosTesterCategory
	     * Cancels testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void TestingCancelled(const TDesC8& aInfo);
		
		/**
	     * From COMASuplPosTesterCategory
	     * Completes testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void TestingComplete(const TDesC8& aInfo);
		
		/**
	     * From COMASuplPosTesterCategory
	     * Aborts testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void TestingAborted(const TDesC8& aError);

	public:
		//derived from CActive through COMASuplPosTesterCategory
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
	     * Handles leave from RunL
	     *
	     * @since S60 v3.1
	     * @param aError: Errorcode of leaving
	     */
		TInt  RunError( TInt aError );
	private:
		/**
	     * Pointer to the PosHandler.
	     * Not own.
	     */
		COMASuplPosHandlerBase* iPosHandler;

		/**
	     * Unique request id for session
	     */
		TInt iRequestID;
		};
#endif