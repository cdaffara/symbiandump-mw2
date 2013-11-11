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
* Description:  Tests reading message file, preparing the payload and
*  sending the Payload to the POS Message plugin.
*
*/

#ifndef C_COMASUPL_MSGFILEREADERTEST_H
#define C_COMASUPL_MSGFILEREADERTEST_H

#include "epos_comasuplpostestercategory.h"
#include <e32cmn.h>
#include <ecom/implementationinformation.h>

class COMASuplPosTesterLogger;
class COMASuplTestHandler;
class COMASuplPosPayload;
class COMASuplMessageFileReader;
class MOMASuplObserver;
class COMASuplMessageInfo;

/** Constant used for logging name of test*/
_LIT8(KMessageReaderTest, "---------------Message Reader Test---------------");

/** Constant used for logging information about test*/
_LIT8(KMessageReaderTestComplete, "Info: Message Reader Test Complete.");

/** Constant used for logging information about test*/
_LIT8(KMessageReaderTestCancel, "Info: Message Reader Test Canceled.");

/** Constant used for logging information about test*/
_LIT8(KMessageReaderTestAborted, "Error: Message Reader Test Aborted");

/**
 *  Tests reading of mesage file
 *
 *  Tests reading message file, preparing the payload and
 *  sending the Payload to the POS Message plugin.
 *
 *  @lib ?library
 *  @since S60 v3.1u
 */
class COMASuplMsgFileReaderTest : public COMASuplPosTesterCategory
	{
	/**  Controls the execution sequence in RunL */
	enum TExecutionSequence
		{
		EStartTest = 0,
		EPosProcessMessage,
		EPosSessionTestComplete
		};
	public:
		static COMASuplMsgFileReaderTest* NewL(
					TFileName& aMsgFile
					, COMASuplPosTesterCategory* aCallBack = NULL);
					
		virtual  ~COMASuplMsgFileReaderTest();
		
		void InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo);

	
	public:
		/**
	     * From COMASuplPosTesterCategory.
	     * First function executed. Testing starts from this function,
	     *
	     * @since S60 v3.1u
	     */
		void StartTestingL();
	public:
		/**
	     * From COMASuplPosTesterCategory.
	     * Function executes on completion of any asynchrous request from
	     * this object.
	     *
	     * @since S60 v3.1u
	     */
		void RunL();
		
		/**
	     * From COMASuplPosTesterCategory.
	     * Function executes on Canceling any asynchronous request.
	     *
	     * @since S60 v3.1u
	     */
		void DoCancel();
		
		/**
	     * From COMASuplPosTesterCategory.
	     * Function executes when RunL leaves
	     *
	     * @since S60 v3.1u
	     */
		TInt  RunError(TInt aError);
	private:
		COMASuplMsgFileReaderTest(COMASuplPosTesterCategory* aCallBack);
		void ConstructL(TFileName& aMsgFile);
	private:
		/**
	     * Decides which case will be executed in RunL
	     */
		TExecutionSequence iNextInSeq;

		 /**
    	  * Points to array of payload created from message file.
          * Own.
          */
		RPointerArray<COMASuplMessageInfo>* iMsgInfoList;
		
		 /**
    	  * Points to message file reader object.
          * Own.
          */
		COMASuplMessageFileReader* iMsgFileReader;
		
		/**
    	  * Holds the name of message file.
          */
		TFileName iMsgFileName;
	};
#endif