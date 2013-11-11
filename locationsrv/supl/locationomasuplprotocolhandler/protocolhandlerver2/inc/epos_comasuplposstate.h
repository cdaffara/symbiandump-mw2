/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   class for handling SUPL_POS message...
*
*/



#ifndef EPOS_COMASUPLPOSSTATE_H
#define EPOS_COMASUPLPOSSTATE_H

#include <e32base.h>

#include <lbs/epos_comasuplpospayload.h>
#include "epos_comasuplstate.h"

class COMASuplPosSessionBase;
class COMASuplPOSRequestor;
class COMASuplPosPayload;
class COMASuplVelocity;
class COMASuplPos;
class COMASuplSETCapabilities;
class COMASuplAsnHandlerBase;

/**
 *  COMASuplPosState handles all SUPL_POS related processing.
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1
 */

class COMASuplPosState : public COMASuplState,public MOMASuplMsgStateObserver
{
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 v3.1
		* @param None
		* @return None
		* @return Instance of COMASuplPosState
		*/ 
		static COMASuplPosState* NewL(COMASuplPosSessionBase* aPOSSession,TInt aMsgTimeout,COMASuplSETCapabilities* aSETCapabilities,
																	COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
		/**
		* Destructor.
		*/
		~COMASuplPosState();
		
    public:		//COMASuplState
    
	  /**
		* GenerateMessageL Method,Generates SUPL_END message...
		* @since S60 v3.1
		* @param None
		* @return system wide error
		*/ 
		virtual TInt GenerateMessageL();

    /**
		* EncodeMessageL Method,Encodes SUPL_END message...
		* @since S60 v3.1
		* @param aSuplVersion, version of SUPL
		* @param aSessionId, sessionid of session.
		* @param aError, error code if any error occures while conversion.
		* @return encoded data, HBufC8*
		*/ 
		virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,
						COMASuplSessionID* aSessionId,TInt &aError);

    /**
		* CancelOperation Method,Cancels operations if it is ongoing
		* @since S60 v3.1
		* @param None
		* @return None
		*/ 
		virtual void CancelOperation();
		
    /**
		* ProcessMessage Method,Process SUPL_END message...
		* @since S60 v3.1u
		* @param None
		* @return system wide error
		*/ 
		virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 
		
		/**
		* StartTimer Method, Starts timer...
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		virtual void StartTimer();
		
		/**
		* OperationCompleteL Method,
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void OperationCompleteL(TInt aError);
		void GetPositionCompleteL(TInt aError);
		
   public:
       	/**
		* IsLastMessage Method,returns true if POS plugin has anything to sent to SLP
		* @since Series 60 3.1
		* @param None
		* @return TBool
		*/ 
		TBool IsLastMessage();
		
       	/**
		* IsGenerationInProgress Method,returns true if generation is in progress or not
		* @since Series 60 3.1
		* @param None
		* @return TBool
		*/ 
		TBool IsGenerationInProgress();
		
       	/**
		* IsSegmentationDone Method,checks for segmentation is done 
		* @since Series 60 3.1
		* @param None
		* @return TBool
		*/ 
		TBool IsSegmentationDone() const;
		
	   	/**
		* SetRemainingPosPayloadFromSuplPosInit Method,Sets remaining pospayload from SUPL_POS_INIT
		* @since Series 60 3.1
		* @param aRemainingPospayload, will give remaining pos payload
		* @return None
		*/ 
		void SetRemainingPosPayloadFromSuplPosInitL(COMASuplPosPayload* aRemainingPospayload);	
		
	private :
	
	   	/**
		* IterateSuplInfoListL Method,returns None
		* @since Series 60 3.1
		* @param 
		* @return 
		*/ 
		void IterateSuplInfoListL(COMASuplPos* aOMASuplPos,TInt &aError);	
		
	   	/**
		* PseudoSegmentationL Method,does segmentation of pospayload
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void PseudoSegmentationL();
		
	   	/**
		* LogPosPayLoadData Method,returns None
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void LogPosPayLoadData();
		
	   	/**
		* LogVelocity Method,returns None
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void LogVelocity();		
		
		TBool CheckPOSPayloadType(const COMASuplPosPayload *aPosPayload);
		void LogPacket(const TDesC8& aPacket);
	protected : 
	
		//Constuctor
		COMASuplPosState(COMASuplPosSessionBase* aPOSSession,TInt aMsgTimeout,COMASuplSETCapabilities* aSETCapabilities,
										 COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
	  /**
	  * ConstructL Method, 2nd phase construction
	  * @since S60 v3.1u
	  * @param None
	  * @return None
	  */
		void ConstructL();

	private: //Data
		
	//POS Session 
	COMASuplPosSessionBase* iPOSSession;

	//POS Requestor
	COMASuplPOSRequestor* iPOSRequestor;

	COMASuplPosPayload* iOMASuplPosPayLoad;

	COMASuplVelocity* iOMASuplVelocity;
	
	//SET capabilities
	COMASuplSETCapabilities* iSETCapabilities;

	//Error code from POS plugin		
	TInt iRequestError;
	
	TBool iGenerationStatus;
	
	//Will hold if segmented used for last time...
	TBool iIsLastPosPayloadSegmented;
	
	//Will hold remaining pospayload data...
	HBufC8* iRemainingPospayload;
	
	//Will hold pospayload type...
	COMASuplPosPayload::TOMASuplPosPayloadType iPosPayLoadType;
	
	TBool iIsAnyPosPayloadFromPosInit;
	
	COMASuplAsnHandlerBase* iOMASuplAsnHandlerBase;
		
};

#endif //C_COMASUPLPOSSTATE_H
