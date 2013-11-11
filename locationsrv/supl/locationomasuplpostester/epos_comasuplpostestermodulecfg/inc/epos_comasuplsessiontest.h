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
* Description:  Tests POS session.
*
*/

#ifndef C_COMASUPLSESSIONTEST_H
#define C_COMASUPLSESSIONTEST_H

#include <epos_comasuplinforequestlist.h>

#include "epos_comasuplgetsuplinfomanager.h"

class COMASuplPosHandlerBase;
class COMASuplPosSessionBase;
class COMASuplPosition;

/** Constant used for logging name of test*/
_LIT8(KSessionTest, "---------------Session Test---------------");


/**
 *  Tests GetSuplInfoL() function
 *
 *  Tests GetSuplInfoL() function after Initializing the session and 
 *  after closing the session. Contents of request List can be controlled
 *  through arguments in CreateRequestList().
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1u
 */
class COMASuplSessionTest : public COMASuplGetSuplInfoManager
	{
	private:
		/**  Controls the series of execution of asynchronous functions */
		enum TExecutionSequence
			{
			EStartTest = 0,
			EGetSuplInfoSessionNotInitRequest,
			EGetSuplInfoSessionNotInitRequestComplete,
			EGetPositionSessionNotInitRequest,
			EGetPositionSessionNotInitRequestComplete,
			EInitializeSessionRequest,
			EinitializeSessionRequestComplete,
			EGetSuplInfoSessionInitRequest,
			EGetSuplInfoSessionInitRequestComplete,
			EGetPositionSessionInitRequest,
			EGetPositionSessionInitRequestComplete,
			EEndSessionRequest,
			EEndSessionRequestComplete,
			EGetSuplInfoSessionEndRequest,
			EGetSuplInfoSessionEndRequestComplete,
			EGetPositionSessionEndRequest,
			EGetPositionSessionEndRequestComplete,
			EExistingSessionTest,
			ETestingComplete
			};

		/** Identifies the current state of the POS Session */
		enum TSessionState
			{
			ESessionUninitialized = 0,
			ESessionInitialized,
			ESessionEnded
			};
	public:
		static COMASuplSessionTest* NewL(
								COMASuplPosHandlerBase& aPosHandler
							, COMASuplPosTesterCategory* aCallBack = NULL);
		virtual ~COMASuplSessionTest();
		
	private:
		void ConstructL();
		COMASuplSessionTest( COMASuplPosHandlerBase& aPosHandler
							, COMASuplPosTesterCategory* aCallBack );
	public:
		/**
	     * From COMASuplGetSuplInfoManager
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void StartTestingL();
	public:
		/**
	     * From COMASuplGetSuplInfoManager
	     * Handles completion of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void RunL();
		/**
	     * From COMASuplGetSuplInfoManager
	     * Handles cancellation of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void DoCancel();

		/**
	     * From COMASuplGetSuplInfoManager
	     * Handles leave from RunL, aborts testing.
	     *
	     * @since S60 v3.1
	     * @param aError: Errorcode of leaving
	     */
		TInt  RunError( TInt aError );

		/**
	     * Requests for GetSuplInfoL before initializing session
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void GetSuplInfoRequestL();

		/**
	     * Logs the completion status of GetsuplinfoL
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void GetSuplInfoRequestCompleteL();

		/**
	     * Initializes the POS session.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void InitializeSessionRequestL();

		/**
	     * Logs the completion status of session initialization.
	     * Leaves if session initialization is not complete with KErrNone.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void InitializeSessionRequestCompleteL();
		
		/**
	     * Resets Position object
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void COMASuplSessionTest::ResetPositionL();
		/**
	     * Requests for GetPositionL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void GetPositionRequestL();
		
		/**
	     * Logs the completion status of GetSplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void GetPositionRequestComplete();
			
		/**
	     * Ends session and requests for GetSuplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void EndSession();

	private:

		/**
		 * Controls the series of execution of asynchronous functions.
		 */
		TExecutionSequence iNextInSeq;

		/**
		 * Identifies the current state of the POS Session.
		 */
		TSessionState iSessionState;

		/**
		 * Holds an object of COMASuplPosition for GetPositionL.
		 * Own.
		 */
		COMASuplPosition* iPosition;

		/**
		 * Holds EFalse if the tests are not done on an existing session.
		 */
		TBool iExistingSessionTestDone;
	};
#endif