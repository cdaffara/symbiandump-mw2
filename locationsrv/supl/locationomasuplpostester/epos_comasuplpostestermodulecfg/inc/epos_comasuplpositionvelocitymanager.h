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
* Description:  Parent class for testing of GetPositionL and message transaction.
*
*/

#ifndef C_COMASUPLPOSITIONVELOCITYMANAGER_H
#define C_COMASUPLPOSITIONVELOCITYMANAGER_H

#include <e32base.h>

class COMASuplPositionVelocityTest;
class COMASuplInfoRequestList;
class COMASuplPosTesterCategory;
class COMASuplPosition;
class COMASuplPosSessionBase;
class COMASuplHorizVelocity;
class COMASuplHorizAndVertVelocity;
class TOMASuplUtcTime;
class TOMASuplPositionEstimate;
class TOMASuplUncertainty;
class TOMASuplAltitudeInfo;
class COMASuplVelocity;
class COMASuplMessageInfo;
class COMASuplPosPayload;
class COMASuplMessageFileReader;

/** Constant used for logging. */
_LIT8(KPosDifferent, "Error: Positions Retrieved in two Requests are different.");

/** Index of POS payload in request list*/
const TInt KPayLoadIndex = 2; //Index 0 is for velocity

class COMASuplPositionVelocityManager : public COMASuplPosTesterCategory
	{
	protected:
		/**  Controls the execution sequence in RunL. */
		enum TExecutionSequence
		{
		EStartTest = 0,
		ECancelBeforeGivingRequest,
		ERequestToInitializeSession,
		ERequestToInitializeSessionComplete,
		EProcessInstructions,
		EStartMsgTransaction,
		ERequestForInfo,
		ERequestForInfoComplete,
		ESendData,
		ESendDataComplete,
		ERequestForPosition,
		ERequestForPositionComplete
		};

	public:
		virtual ~COMASuplPositionVelocityManager();

	protected:
		COMASuplPositionVelocityManager(COMASuplPosTesterCategory* aCallBack
										, TFileName& aMsgFileName);

		void InitializeL(COMASuplTestHandler* aTestHandler
								, COMASuplPosTesterLogger& aLogger
								, TInt aTestNo);
	public:

		/**
	     * Create the request list. Leaves if total number of objects is negative.
	     *
	     * @since S60 v3.1
	     * @param aVelocityCount Number of velocity objects in the list.
	     * Default is 1.
	     * @param aPayLoadCount Number of payload objects in the list.
	     * Default is 1.
	     * @return COMASuplInfoRequestList
	     */
		COMASuplInfoRequestList& CreateRequestListL();

		/**
	     * Resets lists and requests for GetSuplInfoL. 
	     *
	     * @since S60 v3.1
	     * @param aStatus Request status
	     * @return void
	     */
		void RequestForInfoL();

		/**
	     * Processes completion of GetSuplInfoL. 
	     *
	     * @since S60 v3.1
	     * @param aStatus Request status
	     * @return System wide error code.
	     */
		void ProcessRequestCompleteL(TRequestStatus& aStatus);

		/**
	     * Processes completion of GetPositionL. 
	     *
	     * @since S60 v3.1
	     * @param aStatus Request status
	     * @return System wide error code.
	     */
		TInt ProcessGetPositionCompleteL(TRequestStatus& aStatus);

		/**
	     * Deletes previous position object and requests for GetPositionL. 
	     *
	     * @since S60 v3.1
	     * @param aStatus Request status
	     * @return void
	     */
		void RequestForPositionL(TRequestStatus& aStatus);

		/**
	     * Deletes iCreatedTypeSeq and iRequestList. Initializes them to NULL. 
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return void
	     */
		void ResetLists();

		/**
	     * Checks whether all the messages in the message file have been sent. 
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return EFalse if all the message are sent.
	     */
		TBool MoreMsgInMsgFileL();

		/**
	     * Checks whether the object returned in the list is last from 
	     * POS Message plugin.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return ETrue if the POS payload object is last.
	     */
		TBool LastMessage();

		/**
	     * Does necessary initialization for doing the same test
	     * on an existing session.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return void
	     */
		void InitializeForExistingSessionTest();

		/**
	     * Starts test on the existing session. This should be called after
	     * InitializeForExistingSessionTest().
	     * on an existing session.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return void
	     */
		void StartExistingSessionTest();

		/**
	     * Checks that the elements in list received are similar to the list sent
	     *
	     * @since S60 v3.1
	     * @param aDiffIndex Index of the difference. If there is no difference
	     * it is -1.
	     * @return ETrue if there is any difference else EFalse.
	     */
		TBool MatchLists(TInt& aDiffIndex);

		/**
	     * Sends a message to POS Message Plugin.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return void
	     */
		void SendPosMessageL(COMASuplPosPayload& aPayload);

		/**
	     * Provides next instruction read from message file.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return COMASuplMessageInfo which holds instruction and data.
	     */
		COMASuplMessageInfo* ReadNextMessageL();

		/**
	     * Identifies the type of instruction and processes it.
	     *
	     * @since S60 v3.1
	     * @param aMsgInfo Message instruction read from message file.
	     * @return void.
	     */
		void ProcessMsgInstructionL(COMASuplMessageInfo& aMsgInfo);

		/**
	     * Sends aPayLoad to POS Message plug-in.
	     *
	     * @since S60 v3.1
	     * @param aPayload Payload to be sent to POS Message plug+in.
	     * @return void.
	     */
		void ProcessSendL(COMASuplPosPayload& aPayload);

		/**
	     * Requests GetSuplInfoL() to POS Message plug-in.
	     *
	     * @since S60 v3.1
	     * @param aNextInSeq Case in RunL to be executed on completion of request.
	     * @return void.
	     */
		void ProcessWaitL(TExecutionSequence aNextInSeq);

		/**
	     * Reads next instruction and processes it. Uses ReadNextMessageL()
	     * and ProcessMsgInstructionL();
	     *
	     * @since S60 v3.1
	     * @param void.
	     * @return void.
	     */
		void ProcessNextInstructionL();
		
		/**
	     * Returns ETrue if the next instruction is wait
	     *
	     * @since S60 v3.1
	     * @param void.
	     * @return void.
	     */
		TBool IsNextInstructionWait();

		/**
	     * Moves to next "send" instruction. If there is no "send" instruction
	     * returns EFalse and sets aIndex to length of array, else reurns 
	     * ETrue and sets aIndex to the index of instruction array.
	     *
	     * @since S60 v3.1
	     * @param void.
	     * @return void.
	     */
		TBool MoveToNextSendInstruction();


	protected:

		/**
    	 * Holds the Type of objects in the sequence they are in the request list
    	 * Own.
    	 */
		CArrayFixFlat<TInt>* iCreatedTypeSeq;

		/**
    	 * Request list for GetSuplInfoL.
    	 * Own.
    	 */
		COMASuplInfoRequestList* iRequestList;

		/**
    	 * Position object to be sent in the GetPositionL
    	 */
		COMASuplPosition* iCurrentPosition;

		/**
    	 * Position object sent from the GetPositionL, which has completed.
    	 * Own.
    	 */
		COMASuplPosition* iPreviousPosition;

		/**
    	 * Pointer to POS Session.
    	 * Own.
    	 */
		COMASuplPosSessionBase* iSessionBase;

		/**
    	 * Maximum number of times GetPositionL is to be made. 
    	 * To be given by user.
    	 */
		TInt iMaxGetPositionReqCount;

		/**
    	 * Number of times GetPositionL has already been requested.
    	 */
		TInt iGetPositionRequested;

		/**
    	 * Number of message instructions executed.
    	 */
		TInt iMsgExecuted;

		/**
    	 * List of payload objects created by reading the message file.
    	 * Own.
    	 */
		RPointerArray< COMASuplMessageInfo >* iMsgInfoList;

		/**
    	 * Identifies the case to be executed when RunL is called.
    	 */
		TExecutionSequence iNextInSeq;

		/**
    	 * ETrue if the test which uses an already existing session has started.
    	 */
		TBool iExistingSessionTestStarted;
		
		/**
	     * Pointer to the Message File Reader.
	     * Own.
	     */
		COMASuplMessageFileReader* iMsgFileReader;
		
		/**
		 * Holds the name of message file.
		 */
		TFileName iMsgFileName;
		
	};
#endif