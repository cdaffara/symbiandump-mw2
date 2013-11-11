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
* Description:  Tests creation of single POS session.
*
*/

#ifndef C_COMASUPLPOSSESSIONCREATIONTEST_H
#define C_COMASUPLPOSSESSIONCREATIONTEST_H

#include <e32cmn.h>
#include <ecom/implementationinformation.h>

#include "epos_comasuplpostestercategory.h"

class COMASuplPosTesterLogger;
class COMASuplTestHandler;
class COMASuplPosSessionBase;
class COMASuplPosHandlerBase;

/** Constant used for logging name of test*/
_LIT8(KPosSessionCreationTest, "---------------POS Session Creation Test---------------");

/** Constant used for logging information about test*/
_LIT8(KPosSessionCreationTestComplete, "Info: POS Session Creation Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KPosSessionCreationTestCancel, "Info: POS Session Creation Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KPosSessionCreationTestAborted, "Error: POS Session Creation Test Aborted");


/**
 *  Tests creation of single POS session.
 *
 *  It creates a POS handler, initializes it, creates a pos session and 
 *  initializes it.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplPosSessionCreationTest : public COMASuplPosTesterCategory
	{
	/**  Controls the case execution in RunL */
	enum TExecutionSequence
		{
		EStartTest = 0,
		EInitializeHandler,
		ECreatePosSession,
		EInitializeSession,
		EPosSessionTestComplete
		};
	public:
		static COMASuplPosSessionCreationTest* NewL(
								CImplementationInformation& aImplInfo
								, COMASuplPosTesterCategory* aCallBack = NULL);
		virtual ~COMASuplPosSessionCreationTest();
		
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
	     * @param aError Errorcode of leaving
	     */
		TInt  RunError(TInt aError);
	private:
		COMASuplPosSessionCreationTest(CImplementationInformation& aImplInfo
									, COMASuplPosTesterCategory* aCallBack);
		void ConstructL();
	private:
		//Not to be deleted in destructor
		/**
		 * Pointer to the implementation to be tested for session creation.
		 * Not own.
		 */
		CImplementationInformation* iImplInfo;
		
		/**
		 * Identifies the casr to be executed when RunL executes.
		 */
		TExecutionSequence iNextInSeq;
		
		/**
		 * Pointer to COMASuplPosSessionBase objected created.
		 * Own.
		 */
		COMASuplPosSessionBase* iSessionBase;
		
		/**
		 * Pointer to COMASuplPosHandlerBase objected created.
		 * Own.
		 */
		COMASuplPosHandlerBase* iPosHandler;
		
		/**
		 * Pointer to MOMASuplObserver.
		 * Not own.
		 */
		MOMASuplObserver* iSuplObserver;
		
		/**
		 * Unique Id used to identify session.
		 */
		TInt iRequestID;
	};
#endif