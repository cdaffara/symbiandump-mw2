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
* Description:  Tests for thread creation & Process priority.
*
*/

#ifndef C_COMASUPLTHREADPROCESSTEST_H
#define C_COMASUPLTHREADPROCESSTEST_H

#include "epos_comasuplpostestercategory.h"

/**
 *  Tests for thread creation & Process priority.
 *
 *  Provides the number of threads which were created but not killed.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplThreadProcessTest:public COMASuplPosTesterCategory
	{
	public:
		static COMASuplThreadProcessTest* NewL(
								COMASuplPosTesterCategory* aCallBack = NULL);
		~COMASuplThreadProcessTest();
	private:
		COMASuplThreadProcessTest(COMASuplPosTesterCategory* aCallBack);
		void ConstructL();
		
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
		
		/**
	     * Gets number of threads associated with the current process,
	     * name of process and priority of the process.
	     *
	     * @since S60 v3.1
	     * @param aThreadCount Gets the number of threads associated with
	     * current process
	     * @param aPriority Priority of current process.
	     * @param aProcessName Name of current process.
	     * @return void
	     */
		void ReadStatusL(TInt& aThreadCount, TProcessPriority& aPriority
								, TFullName& aProcessName);

		/**
	     * Gets number of threads associated with the a process,
	     *
	     * @since S60 v3.1
	     * @param aProcessName Name of process whose associated threads 
	     * are to be counted.
	     * @return TInt number of threads associated with aProcessName.
	     */
		void CountThreadsL(const TFullName& aProcessName, TInt& aThreadCount);

		/**
	     * Appends '*' before and after aProcessName.
	     *
	     * @since S60 v3.1
	     * @param aSearchString A descriptor containing "*aProcessName*"
	     * @param aProcessName Name of the process.
	     * @return void
	     */
		void PrepareSearchStringL(HBufC*& aSearchString
								, const TFullName& aProcessName);

		/**
	     * Stores number of threads associated with the current process,
	     * name of process and priority of the process before starting tests..
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void StatusBeforeTestsL();

		/**
	     * Stores number of threads associated with the current process,
	     * name of process and priority of the process after starting tests..
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void StatusAfterTestsL();

		/**
	     * Matches number of threads associated with the a process,
	     * name of process and priority of the process after starting tests
	     * with the same stored before starting tests.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		TBool MatchAndLogStatus();
		
	private:
		/*
		 * Number of threads before executing test.
		 */
		TInt iThreadCountBeforeTests;

		/*
		 * Number of threads after executing test.
		 */
		TInt iThreadCountAfterTests;
		
		/*
		 * Priority of process before executing test.
		 */
		TProcessPriority  iProcessPriorityBeforeTests;
		
		/*
		 * Priority of process after executing test.
		 */
		TProcessPriority  iProcessPriorityAfterTests;
		
		/*
		 * Name of process before executing test.
		 */
		TFullName iProcessNameBeforeTests;
		
		/*
		 * Name of process after executing test.
		 */
		TFullName iProcessNameAfterTests;
	};
#endif