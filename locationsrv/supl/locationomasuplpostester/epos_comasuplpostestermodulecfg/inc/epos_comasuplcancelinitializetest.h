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
* Description:  Tests the behaviour of InitializeL and CancelInitialize.
*
*/

#ifndef C_COMASUPLCANCELINITIALIZETEST_H
#define C_COMASUPLCANCELINITIALIZETEST_H

#include "epos_comasuplpostestercategory.h"

class COMASuplPosHandlerBase;
class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplPosSessionBase;

/** Constant used for logging name of test*/
_LIT8(KCancelInitializeTest,"---------------CancelInitialize Test---------------");

/** Constant used for logging information about test*/
_LIT8(KCancelInitializeTestCanceled, " Info: CancelInitialize Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KCancelInitializeTestComplete, "Info: CancelInitialize Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KCancelInitializeTestAborted, "Error: CancelInitialize Test Aborted.");

/** Constant used for logging information about test*/
_LIT8(KInitializeWarning, "Warning: Session initialization completed before canceling");

/** Constant used for logging information about test*/
_LIT8(KInitializeInfo, "Info: Session initialization Canceled successfully.");

/**
 *  Tests the behaviour of InitializeL and CancelInitialize.
 *
 *  Logs a warning if CancelInitialize is unable to cancel the request.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplCancelInitializeTest : public COMASuplPosTesterCategory
	{
	private:
		/**  Controls the series of execution of asynchronous functions */
		enum TExecutionSequence
			{
			EStartTest = 0,
			EInitializeAndCancel,
			EHandlerInitAndCancelComplete,
			EHandlerInitComplete,
			ECreateNewSession,
			ECancelBeforeInitializingSession,
			EInitializeSessionAndCancel,
			ESessionInitializationComplete,
			};
	public:
		static COMASuplCancelInitializeTest* NewL(const TUid& aUid
								, COMASuplPosTesterCategory* aCallBack = NULL);

		~COMASuplCancelInitializeTest();
	private:
		COMASuplCancelInitializeTest(const TUid& aUid
									, COMASuplPosTesterCategory* aCallBack);

		void ConstructL();
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
	     * Handles leave from RunL
	     *
	     * @since S60 v3.1
	     * @param aError: Error code of leaving
	     */
		TInt  RunError( TInt aError );

	private:
		/**
		 * Pointer to the loaded POS Handler.
		 */
		COMASuplPosHandlerBase* iPosHandler;

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
	     * Contains the implementation of the implementation, 
	     * selected by the user.
	     */
		TUid iUid;
	};
#endif