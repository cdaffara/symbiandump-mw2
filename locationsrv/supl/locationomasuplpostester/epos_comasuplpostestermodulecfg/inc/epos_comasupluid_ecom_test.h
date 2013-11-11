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
* Description:  Tests loading of correct implementation.
*
*/

#ifndef C_COMASUPLUID_ECOM_TEST_H
#define C_COMASUPLUID_ECOM_TEST_H

#include <e32cmn.h>
#include <ecom/implementationinformation.h>

#include "epos_comasuplpostestercategory.h"

class COMASuplPosTesterLogger;
class COMASuplTestHandler;

/** Constant used for logging name of test*/
_LIT8(KUid_ECom_Test, "---------------Uid ECom Test---------------");

/** Constant used for logging information about test*/
_LIT8(KUid_ECom_TestComplete, "Info: Uid ECom Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KUid_ECom_TestCancel, "Info: Uid ECom Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KUid_ECom_TestAborted, "Error: Uid ECom Test Aborted");


/**
 *  Tests loading of correct implementation.
 *
 *  Loads the implementation provided in the argument & matches its uid.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1
 */
class COMASuplUid_ECom_Test : public COMASuplPosTesterCategory
	{
	public:
		static COMASuplUid_ECom_Test* NewL(
								CImplementationInformation& aImplInfo
								, COMASuplPosTesterCategory* aCallBack = NULL);

		virtual ~COMASuplUid_ECom_Test();
		
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
	     * Handles leave in RunL
	     *
	     * @since S60 v3.1
	     */
		TInt RunError(TInt aError);

	private:
		COMASuplUid_ECom_Test(CImplementationInformation& aImplInfo
							, COMASuplPosTesterCategory* aCallBack);
		void ConstructL();
	private:
		/**
	     * Implementation to be loaded
	     * Not own.
	     */
		CImplementationInformation* iImplInfo;
	};
#endif