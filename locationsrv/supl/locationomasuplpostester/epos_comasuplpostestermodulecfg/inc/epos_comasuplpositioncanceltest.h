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
* Description:  Tests cancellation of GetPositionL
*
*/

#ifndef C_COMASUPLPOSITIONCANCELTEST_H
#define C_COMASUPLPOSITIONCANCELTEST_H

#include "epos_comasuplpostestercategory.h"
#include "epos_comasuplpositionvelocitymanager.h"

class COMASuplPosHandlerBase;
class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplInfoRequestList;
class COMASuplInfoRequestList;
class COMASuplMessageFileReader;

/** Constant used for logging name of test*/
_LIT8(KPositionCancelTest,"---------------GetPosition Cancellation Test---------------");

/** Constant used for logging information about test*/
_LIT8(KPositionCancelTestComplete, "Info: GetPosition Cancellation Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KPositionCancelTestCancel, "Info: GetPosition Cancellation Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KPositionCancelTestAborted, "Error: GetPosition Cancellation Test Aborted");

/**
 *  Tests GetPositionL in different scenarios.
 *
 *  It tests the process of reading messages from a message file and sending
 *  the POS payload created from the message file, to the Message plugin.
 *  When all the messages are sent, it asks the Message plugin for Position
 *  and cancels the request.
 *
 *  @lib epos_comasuplpostestermodulecfg
 *  @since S60 v3.1u
 */
class COMASuplPositionCancelTest : public COMASuplPositionVelocityManager
	{
	public:
		static COMASuplPositionCancelTest* NewL(
							COMASuplPosHandlerBase* aPosHandler
							, TInt aGetPositionReqCount
							, TFileName& aFileName
							, COMASuplPosTesterCategory* aCallBack = NULL);

		virtual ~COMASuplPositionCancelTest();
	
	private:
		COMASuplPositionCancelTest(
									COMASuplPosHandlerBase* aPosHandler
									, TInt aGetPositionReqCount
									, TFileName& aFileName
									, COMASuplPosTesterCategory* aCallBack);
		void ConstructL();

	public:
		//derived form MOMASuplTestObserver through COMASuplPosTesterCategory
		/**
	     * From COMASuplPosTesterCategory
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1u
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
	public:
		/**
	     * Cancels GetPositionL. Calls CancelGetPosition()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void CancelGetPositionRequest();
		
		/**
	     * Sets iCancelRequested to ETrue
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void CancellationRequested();

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
		
		/*
		 * ETrue if the Position request has been canceled.
		 */
		TBool iCancelRequested;
		};
#endif