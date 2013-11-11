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
* Description:  Tests GetSuplInfoL() in different scenarios
*
*/

#ifndef C_COMASUPLGETSUPLINFOTEST_H
#define C_COMASUPLGETSUPLINFOTEST_H

#include <epos_comasuplinforequestlist.h>

#include "epos_comasuplgetsuplinfomanager.h"

class COMASuplPosHandlerBase;
class COMASuplPosSessionBase;

/** Constant used for logging name of test*/
_LIT8(KGetSuplInfoTest, "---------------GetSuplInfoL Test---------------");


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
class COMASuplGetSuplInfoTest : public COMASuplGetSuplInfoManager
	{

	public:
		static COMASuplGetSuplInfoTest* NewL(
									COMASuplPosHandlerBase& aPosHandler
								, COMASuplPosTesterCategory* aCallBack = NULL);
		virtual ~COMASuplGetSuplInfoTest();
		
	private:
		void ConstructL();
		COMASuplGetSuplInfoTest( COMASuplPosHandlerBase& aPosHandler
								, COMASuplPosTesterCategory* aCallBack );

		/**
	     * Ends session specified by iSessionBase
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void EndSession();
		
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
		void ProcessRequestBeforeInitializeL();

		/**
	     * Logs the completion status of GetsuplinfoL
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessRequestBeforeInitializeCompleteL();

		/**
	     * Initializes the POS session.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessInitializeSessionL();

		/**
	     * Logs the completion status of session initialization.
	     * Leaves if session initialization is not complete with KErrNone.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessInitializationCompleteL();
		
		/**
	     * Requests for GetSuplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessRequestGetSuplInfoL();
		
		/**
	     * Logs the completion status of GetSplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessGetSuplInfoComplete();
			
		/**
	     * Ends session and requests for GetSuplInfoL()
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessEndSessionAndRequestGetsuplInfoL();
		
		/**
	     * Logs completion status of GetSuplInfoL requested after 
	     * ending the session
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void ProcessEndSessionRequestComplete();
	};
#endif