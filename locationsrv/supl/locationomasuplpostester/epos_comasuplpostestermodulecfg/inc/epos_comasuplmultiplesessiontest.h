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
* Description:  Header file for Multiple session test
*
*/

#ifndef C_COMASUPLMULTIPLESESSIONTEST_H
#define C_COMASUPLMULTIPLESESSIONTEST_H
#include <e32base.h>

#include "epos_comasuplpostestercategory.h"

class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplPosHandlerBase;


/** Constant used for logging name of test*/
_LIT8(KMultipleSessionTest, "---------------Multiple Session Test---------------");

/** Constant used for logging information about test*/
_LIT8(KMultipleSessionComplete, "Info: Multiple Session Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KMultipleSessionCancel, "Info: Multiple Session Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KMultipleSessionAborted, "Error: Multiple Session Test Aborted");

/**
 *  Controls multiple session test.
 *
 *  It runs various tests. these tests include:
 *  PosSessionCreationTest, CancelInitializeTest, PositionTest
 *  GetSuplInfoTest, GetSuplInfoCancelTest, MsgFileReaderTest
 *  SessionTest, PositionVelocityTest and PositionCancelTest.
 *
 *  @lib epos_comasuplpostestermodulecfg
 *  @since S60 v3.1u
 */
class COMASuplMultipleSessionTest : public COMASuplPosTesterCategory
	{
	public:
		static COMASuplMultipleSessionTest* NewL(
							COMASuplPosHandlerBase* aPosHandler
							, CImplementationInformation& aImplInfo
							, TInt aSessionCount
							, COMASuplPosTesterCategory* aCallBack = NULL);

		virtual ~COMASuplMultipleSessionTest();

	private:
		void ConstructL();

		COMASuplMultipleSessionTest(COMASuplPosHandlerBase* aPosHandler
									, CImplementationInformation& aImplInfo
									, TInt aSessionCount
									, COMASuplPosTesterCategory* aCallBack);
	public:
		/**
	     * From COMASuplPosTesterCategory
	     * Provides test handler and logger to the object
	     *
	     * @since S60 v3.1u
	     * @param aTestHandler: Provides a pointer to 
	     * COMASuplTestHandler for cleanup after test is performed
	     * @param aLogger: Provides a pointer to 
	     * COMASuplPosTesterLogger to enable logging
	     */
		void InitializeL( COMASuplTestHandler* aTestHandler
							, COMASuplPosTesterLogger& aLogger 
							, TInt aTestNo = 0);
		/**
	     * From COMASuplPosTesterCategory
	     * Starts testing for the test class
	     *
	     * @since S60 v3.1u
	     */
		void StartTestingL();
		
	public:
		/**
	     * From COMASuplPosTesterCategory
	     * Handles completion of asynchronous request
	     *
	     * @since S60 v3.1u
	     */
		void RunL();
		
		/**
	     * From COMASuplPosTesterCategory
	     * Handles cancellation of asynchronous request
	     *
	     * @since S60 v3.1u
	     */
		void DoCancel();
		
		/**
	     * From COMASuplPosTesterCategory
	     * Handles leave from RunL, aborts testing.
	     *
	     * @since S60 v3.1u
	     * @param aError Error code of leaving
	     */
		TInt RunError(TInt aError);

		/**
	     * From COMASuplPosTesterCategory
	     * Cancels testing for the test class
	     *
	     * @since S60 v3.1u
	     */
		void TestingCancelled(const TDesC8& aInfo);

		/**
	     * From COMASuplPosTesterCategory
	     * Completes testing for the test class
	     *
	     * @since S60 v3.1u
	     */
		void TestingComplete(const TDesC8& aInfo);

		/**
	     * From COMASuplPosTesterCategory
	     * Aborts testing for the test class
	     *
	     * @since S60 v3.1u
	     */
		void TestingAborted(const TDesC8& aError);
	public:
		/**
	     * Adds GetSuplInfoCancelTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddGetSuplInfoCancelTestL();

		/**
	     * Adds GetSuplInfoTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddGetSuplInfoTestL();

		/**
	     * Adds MsgFileReaderTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddMsgFileReaderTestL();

		/**
	     * Adds PosSessionCreationTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddPosSessionCreationTestL();

		/**
	     * Adds PositionVelocityTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddPositionVelocityTestL();

		/**
	     * Adds PositionCancelTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddPositionCancelTestL();

		/**
	     * Adds CancelInitializeTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddCancelInitializeTestL();

		/**
	     * Adds PositionTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddPositionTestL();
		
		/**
	     * Adds SessionTest to the list of tests to be executed.
	     *
	     * @since S60 v3.1u
	     * @return void
	     */
		void AddSessionTestL();

	private:
		/**
	     * Reference to an object of COMASuplPosHandlerBase initialized.
	     */
		COMASuplPosHandlerBase& iPosHandler;

		/**
	     * Reference to an the implementation to be tested.
	     */
		CImplementationInformation& iImplInfo;

		/**
	     * Number of sessions for each test.
	     */
		TInt iSessionCount;

		/**
	     * Array of test classes.
	     */
		RPointerArray<COMASuplPosTesterCategory> iTestList;

		/**
	     * Total numbers of tests complete/aborted.
	     */
		TInt iTestExecuted;

		/**
	     * Total number of tests, it is equal to length of iTestList.
	     */
		TInt iTotalTests;
	};
#endif