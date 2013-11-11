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
* Description:  Handles test execution and monitors user input.
*
*/

#ifndef C_COMASUPLTESTHANDLER_H
#define C_COMASUPLTESTHANDLER_H

#include <e32base.h>
#include <e32cons.h>

class MOMASuplTestObserver;

/**
 *  Handles test execution.
 *
 *  Handles test execution and monitors user input. Cancels testing if 
 *  user chooses to cancel testing.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplTestHandler: public CActive
	{
	public:
		static COMASuplTestHandler* NewL( CConsoleBase& aConsole );
		virtual ~COMASuplTestHandler();
		
	    /**
	     * Provides pointer to test class.
	     *
	     * @since S60 v3.1
	     * @param aCancelObserver Reference to test 
	     * class for call back on test cancellation.
	     * @return void
	     */
		void InitializeL( MOMASuplTestObserver* aCancelObserver );
		

	    /**
	     * Starts testing for a test class.Ready to monitor user inputs.
	     *
	     * @since S60 v3.1
	     * @return void
	     */
		void StartTest();

	public:
		/**
	     * From CActive
	     * Handles completion of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void RunL();
		
		/**
	     * From CActive
	     * Handles cancellation of asynchronous request
	     *
	     * @since S60 v3.1
	     */
		void DoCancel();
		
		/**
	     * From CActive
	     * Handles leave from RunL, aborts testing.
	     *
	     * @since S60 v3.1
	     * @param aError Errorcode of leaving
	     */
		TInt RunError( TInt aError );

	private:
		void ConstructL();
		COMASuplTestHandler();
		COMASuplTestHandler( CConsoleBase& aConsole );

		/**
	     * Dummy request is used to start ActiveScheduler.
	     *
	     * @since S60 v3.1
	     * @param aStatus iStatus of Active object.
	     * @return void
	     */
		void DummyRequest( TRequestStatus& aStatus );

	private:
		/**
		 * Reference to the console to print message.
		 */
		CConsoleBase& iConsole;
		
		/**
		 * Points to the test class to respond to user input for cancellation.
		 * Not own.
		 */
		MOMASuplTestObserver* iCancelObserver;
		
		/**
		 * Boolean to identify if testing has started.
		 */
		TBool iTestStarted;
	};
#endif