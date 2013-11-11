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
* Description:  Tests cancellation of GetSuplInfoL() in different scenarios
*
*/

#ifndef C_COMASUPLGETSUPLINFOCANCELTEST_H
#define C_COMASUPLGETSUPLINFOCANCELTEST_H

#include <epos_comasuplinforequestlist.h>
#include "epos_comasuplgetsuplinfomanager.h"
//#include "epos_comasuplpostestercategory.h"
//epos_comasuplgetsuplinfocanceltest.h
class COMASuplPosHandlerBase;
class COMASuplPosSessionBase;

/** Constant used for logging name of test*/
_LIT8(KGetSuplInfoCancelTest, "---------------GetSuplInfoL Cancel Test---------------");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoCancelTestComplete, "Info: GetSuplInfoL Cancel  Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoCancelTestCancel, "Info: GetSuplInfoL Cancel Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KGetSuplInfoCancelTestAborted, "Error: GetSuplInfoL Cancel Test Aborted");

/**
 *  Tests GetSuplInfoL() function
 *
 *  Tests GetSuplInfoL() function after Initializing the session and 
 *  after closing the session. Contents of request List can be controlled
 *  through arguments in CreateRequestList().
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1
 */
class COMASuplGetSuplInfoCancelTest : public COMASuplGetSuplInfoManager
	{
	public:

		static COMASuplGetSuplInfoCancelTest* NewL(
								COMASuplPosHandlerBase& aPosHandler
								, COMASuplPosTesterCategory* aCallBack = NULL );
		virtual ~COMASuplGetSuplInfoCancelTest();

	private:
		void ConstructL();
		COMASuplGetSuplInfoCancelTest( COMASuplPosHandlerBase& aPosHandler
									, COMASuplPosTesterCategory* aCallBack );
	public:
		/**
	     * Ends session specified by iSessionBase
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void EndSession(const TDesC8& aInfo);

		/**
	     * Tests cancellation of GetSuplInfoL() after Ending the session.
	     * GetSuplInfoL() is not requested before cancel request.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
	    void CancelWithoutRequest( TRequestStatus& aStatus
	    						, TExecutionSequence aNextinSeq );
	    
	    /**
	     * Cancels GetSuplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
	    void CancelGetInfoRequest(const TDesC8& aInfo);
	    
	    /**
	     * Writes log based on completion status of GetSuplInfoL().
	     *
	     * @since S60 v3.1
	     * @return void
	     */
	    void ProcessRequestComplete();

	public:
		//derived form MOMASuplTestObserver through COMASuplPosTesterCategory
		/**
	     * From COMASuplPosTesterCategory
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1
	     */
		void StartTestingL();
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
		
		/**
	     * If Session initialization completes with KErrNone, calls 
	     * CancelGetSuplInfo
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessInitComplete();
		
		/**
	     * Requests for GetSuplInfoL() and immediately cancels it.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessGetSuplInfoAndCancelL();
		
		/**
	     * When GetSuplInfoL completes this function is executed. It provides
	     * information about completion status of GetSuplInfoL.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessGetSuplInfoRequestComplete();
		
		/**
	     * Ends session specified by iSessionBase and requests for CancelGetsuplInfo.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessEndSessionAndCancel();
	};
#endif