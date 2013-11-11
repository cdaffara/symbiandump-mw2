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
* Description:  Parent class of classes testing GetSuplInfo.
*
*/

#ifndef C_COMASUPLGETSUPLINFOMANAGER_H
#define C_COMASUPLGETSUPLINFOMANAGER_H

#include <e32base.h>

#include "epos_comasuplpostestercategory.h"

class COMASuplTestHandler;
class COMASuplPosTesterLogger;
class COMASuplPosHandlerBase;
class COMASuplPosSessionBase;

_LIT8(KMatchError, "Error: RequestList Mismatch at index ");

/**
 *  Parent class of classes testing GetSuplInfoL
 *
 *  It Provides basic common funcionality required for testing GetSuplInfoL.
 *
 *  @lib epos_comasuplpostestermodulecfg.lib
 *  @since S60 v3.1u
 */
class COMASuplGetSuplInfoManager : public COMASuplPosTesterCategory
	{
	protected:
		COMASuplGetSuplInfoManager(COMASuplPosHandlerBase& iPosHandler
									, COMASuplPosTesterCategory* aCallBack);

	protected:
		/**  Controls the series of execution of asynchronous functions */
		enum TExecutionSequence
			{
			EStartTest = 0,
			ECancelWithoutRequest,
			ERequestBeforeInitialize,
			ERequestBeforeInitializeComplete,
			EInitializeSession,
			EInitializationComplete,
			ERequestGetSuplInfo,
			EGetSuplInfoLRequestComplete,
			ERequestGetSuplInfoAndCancel,
			EEndSessionAndRequestGetsuplInfo,
			EEndSessionRequestComplete,
			EEndSessionAndCancel,
			ETestingComplete
			};
	public:
		virtual ~COMASuplGetSuplInfoManager();

		/**
	     * Creates request list of objects from number specified in argument.
	     * It validates arguments using ValidateAndSetArguments(TInt, TInt, 
	     * TInt, TInt). If total number of arguments is less than 1, 
	     * it leaves with KErrUnderflow.
	     *
	     * @since S60 v3.1u
	     * @param aSetCapReqCount: Specifies the number of 
	     * COMASuplSETCapabilities objects to be created in the request list.
	     * @param aReqAsstDataCount: Specifies the number of 
	     * COMASuplReqAsstData objects to be created in the request list.
	     * @param aPositionCount: Specifies the number of 
	     * COMASuplPosition objects to be created in the request list.
	     * @param aVelocityCount: Specifies the number of 
	     * COMASuplVelocity objects to be created in the request list.
	     * @param aPayLoadCount: Specifies the number of 
	     * COMASuplPosPayload objects to be created in the request list.
	     * @return COMASuplInfoRequestList&: Reference to the request list.
	     */
		COMASuplInfoRequestList& CreateRequestListL(TInt aSetCapReqCount 
										, TInt aReqAsstDataCount
										, TInt aPositionCount
										, TInt aVelocityCount
										, TInt aPayLoadCount);

		/**
	     * Creates request list. Elements in request list are determined by
	     * iSetCapReqCount, iReqAsstDataCount, iPositionCount, iVelocityCount
	     * and iPayLoadCount. It uses "COMASuplInfoRequestList& 
	     * CreateRequestListL(TInt, TInt, TInt, TInt, TInt)" for creating 
	     * list.
	     *
	     * @since S60 v3.1
	     * @return COMASuplInfoRequestList&: Reference to the created request list.
	     */
		COMASuplInfoRequestList& CreateRequestListL();
		
		/**
	     * If any of the arguments is more than 1, it is set to 1 otherwise 
	     * there is no change in the value. this value is stored in the 
	     * corresponding member variable for creating list by 
	     * CreateRequestListL().
	     *
	     * @since S60 v3.1
	     * @param aSetCapReqCount Number of COMASuplSETCapabilities objects 
	     * requested for preparing request list.
	     * @param aReqAsstDataCount Number of COMASuplReqAsstData objects 
	     * requested for preparing request list.
	     * @param aPositionCount Number of COMASuplPosition objects 
	     * requested for preparing request list.
	     * @param aVelocityCount Number of COMASuplVelocity objects 
	     * requested for preparing request list.
	     * @param aPayLoadCount Number of COMASuplPosPayload objects 
	     * requested for preparing request list.
	     * @return void
	     */
		void ValidateAndSetArguments(TInt& aSetCapReqCount 
										, TInt& aReqAsstDataCount
										, TInt& aPositionCount
										, TInt& aVelocityCount
										, TInt& aPayLoadCount );

		/**
	     * If aObjCount is more than 1, it is set to 1.
	     * If aObjCount is less than 0, it is set to 0. 
	     * Otherwise there is no change in the value. this value is stored in the 
	     * corresponding member variable for creating list by 
	     * CreateRequestListL().
	     *
	     * @since S60 v3.1
	     * @param aObjCount Number of objects 
	     * @param aMsg Log Info in case of value of aObjCount is changed.
	     * @param aForNextReq A member variable to hold the value
	     * for further create list requests.
	     * @return void
	     */
		void ValidateAndSetArguments(TInt& aObjCount, const TDesC8& aMsg
										, TInt& aForNextReq);

		/**
	     * Deletes iCreatedTypeSeq and iRequestList. Initializes them to NULL.
	     *
	     * @since S60 v3.1
	     * @param void
	     * @return void
	     */
		void ResetLists();

		/**
	     * Matches type and sequence of elements in iRequestList 
	     * with iCreatedTypeSeq.
	     *
	     * @since S60 v3.1
	     * @param aDiffIndex: Index of difference in two lists.
	     * -1 if lists are similar
	     * @return ETrue if Lists match else EFalse.
	     */
		TBool MatchLists(TInt& aDiffIndex);

	protected:
		/**
	     * Identifies the case to be executed when RunL is called
	     */
		TExecutionSequence iNextInSeq;
		
		/**
	     * Reference to the PosHandler
	     */
		COMASuplPosHandlerBase& iPosHandler;
		
		/**
	     * Pointer to session object 
	     * Own.
	     */
		COMASuplPosSessionBase* iSessionBase;
		
		/**
	     * Request list for GetSuplInfoL()
	     * Own.
	     */
		COMASuplInfoRequestList* iRequestList;
		
		/**
	     * Unique request id for session
	     */
		TInt iRequestID;
		
		/**
    	 * List of payload objects created by reading the message file.
    	 * Own.
    	 */
		CArrayFixFlat< TInt >* iCreatedTypeSeq;
		
		/**
	     * Holds validated number of COMASuplSETCapabilities objects. It is
	     * always less than 2.
	     */
		TInt iSetCapReqCount;
		
		/**
	     * Holds validated number of COMASuplReqAsstData objects. It is
	     * always less than 2.
	     */
		TInt iReqAsstDataCount;
		
		/**
	     * Holds validated number of COMASuplPosition objects. It is
	     * always less than 2.
	     */
		TInt iPositionCount;
		
		/**
	     * Holds validated number of COMASuplVelocity objects. It is
	     * always less than 2.
	     */
		TInt iVelocityCount;
		
		/**
	     * Holds validated number of COMASuplPosPayload objects. It is
	     * always less than 2.
	     */
		TInt iPayLoadCount;
	};
#endif