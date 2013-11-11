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
* Description:  Parent class of all test classes.
*
*/

#ifndef C_COMASUPLPOSTESTERCATEGORY_H
#define C_COMASUPLPOSTESTERCATEGORY_H

#include <e32base.h>
#include <epos_momasuplobserver.h>
#include "epos_momasupltestobserver.h"

class COMASuplTestHandler;
class COMASuplPosTesterLogger;


/** Literal for User::panic when IsActive() is true while making an
  * asynchronous request. */
_LIT( KPanicSetActive, "Already Active" );

/** Identifies the status of testing of a test class */
enum TTestingStatus
	{
	ETestNotStarted = 0,
	ETestComplete,
	ETestCancelled,
	ETestAborted
	};

/**
 *  Parent class for all test classes.
 *
 *  Provides functionality common to all test classes.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplPosTesterCategory : public CActive , public MOMASuplTestObserver
									, public MOMASuplObserver
	{
	private:
		COMASuplPosTesterCategory();
	public:
		COMASuplPosTesterCategory(COMASuplPosTesterCategory* aCallBack);

		/**
	     * Provides test handler and logger to the object
	     *
	     * @since S60 3.1u
	     * @param  aTestHandler Pointer to 
	     * COMASuplTestHandler for handling cancellation request from user.
	     * @param aLogger Reference of logger for 
	     * logging any Info/warning/error
	     * @return void
	     */
		virtual void InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo = 0);
		
		/**
	     * From MOMASuplTestObserver
	     * Starts testing for the test class.
	     *
	     * @since S60 v3.1u
	     */
		virtual void StartTestingL() = 0;

		/**
	     * From MOMASuplTestObserver
	     * Cancels testing for the test class.
	     *
	     * @since S60 v3.1
	     */
		virtual void TestingCancelled(const TDesC8& aInfo);
		
		/**
	     * From MOMASuplTestObserver
	     * Completes testing for the test class.
	     *
	     * @since S60 v3.1
	     */
		virtual void TestingComplete(const TDesC8& aInfo);
		
		/**
	     * From MOMASuplTestObserver
	     * Aborts testing for the test class.
	     *
	     * @since S60 v3.1
	     */
		virtual void TestingAborted(const TDesC8& aError);
		
		/**
	     * Returns the result of testing.
	     *
	     * @since S60 v3.1
	     
	     * @return TTestingStatus Status of testing.
	     */
		TTestingStatus GetTestingStatus();
		
		/**
	     * Gets the number of info, warning and error message for 
	     * execution of the test class.
	     *
	     * @since S60 v3.1
	     * @param aInfo Number of info messages.
	     * @param aWarning Number of warning messages.
	     * @param aError Number of error messages.
	     * @return void
	     */
		void GetTestSummary(TInt& aInfo, TInt& aWarning, TInt& aError);
		
		/**
	     * Gets the number of info, warning and error message for 
	     * execution of the test class. It is used to update 
	     * summary for multiple session test.
	     *
	     * @since S60 v3.1
	     * @param aInfo Number of info messages.
	     * @param aWarning Number of warning messages.
	     * @param aError Number of error messages.
	     * @return void
	     */
		void UpdateSessionTestSummary(TInt& aInfo, TInt& aWarning
														, TInt& aError);

		
		/**
	     * From MOMASuplObserver
	     * Returns version.
	     *
	     * @since S60 v3.1
	     */
		TReal SuplVersion();
		
		/**
	     * From MOMASuplObserver
	     * Terminates session.
	     *
	     * @since S60 v3.1
	     */
		void TerminateSession();

		/**
	     * Creates a dummy asynchronous request
	     *
	     * @since S60 v3.1
	     * @param  aStatus: Gets the completion status of request
	     * @return void
	     */
		void DummyRequest(TRequestStatus& aStatus);

	protected:
		/** 
		 * Holds the testing ststus for test class.
		 */
		TTestingStatus iTestingStatus;
		
		/** 
		 * Contains number of info messages for the test.
		 */
		TInt iInfo;
		
		/** 
		 * Contains number of warning messages for the test.
		 */
		TInt iWarning;
		
		/** 
		 * Contains number of error messages for the test.
		 */
		TInt iError;
		
		/**
	     * Points to the logger.
	     * Not own.
	     */
		COMASuplPosTesterLogger* iLogger;
		
		/**
	     * Points to the Test handler.
	     * Not own.
	     */
		COMASuplTestHandler* iTestHandler;
		
		/**
	     * Points to another test class.
	     * Not own.
	     */
		COMASuplPosTesterCategory* iCallBack;

		/** 
		 * Contains session number for a test object.
		 */
		TInt iTestNo;
	};

#endif
