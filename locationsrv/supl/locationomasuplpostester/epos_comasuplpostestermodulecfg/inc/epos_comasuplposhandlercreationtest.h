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
* Description:  Tests creation and initialization of POS Handler.
*
*/

#ifndef C_COMASUPLPOSHANDLERCREATIONTEST_H
#define C_COMASUPLPOSHANDLERCREATIONTEST_H

#include <e32base.h>

#include "epos_comasupltesthandler.h"
#include "epos_comasuplpostestercategory.h"


class COMASuplPosHandlerBase;
class COMASuplPosTesterLogger;

/** Constant used for logging name of test*/
_LIT8(KPosHandlerCreationTest, "---------------POS Handler Creation Test---------------");

/** Constant used for logging information about test*/
_LIT8(KPosHandlerCreationComplete, "Info: POS Handler Creation Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KPosHandlerCreationCancel, "Info: POS Handler Creation Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KPosHandlerCreationAborted, "Error: POS Handler Creation Test Aborted");

/**
 *  Tests the creation of PosHandler.
 *
 *  Creates a Poshandler and initializes it.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplPosHandlerCreationTest : public COMASuplPosTesterCategory
	{
		enum TExecutionSequence
			{
			EStartTest = 0,
			EInitializeHandler,
			EInitComplete
			};
	public:
		static COMASuplPosHandlerCreationTest* NewL(const TUid& aUid
							, COMASuplPosTesterCategory* aCallBack = NULL);

		virtual ~COMASuplPosHandlerCreationTest();

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
	     * @param aError Error code of leaving
	     */
		TInt RunError(TInt aError);
	private:
		void ConstructL();		
		COMASuplPosHandlerCreationTest(const TUid& aUid
									, COMASuplPosTesterCategory* aCallBack);

	private:
		/**
	     * Points to an object of COMASuplPosHandlerBase which is created
	     * and initialized
	     * Own.
	     */
		COMASuplPosHandlerBase* iPosHandler;
		/**
		 * Uid of Implementation to be used for testing. 
		 */
		TUid iUid;
		
		TExecutionSequence iNextInSeq;
	};

#endif