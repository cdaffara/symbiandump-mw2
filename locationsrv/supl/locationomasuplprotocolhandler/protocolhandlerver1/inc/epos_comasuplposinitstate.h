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
* Description:   class for handling SUPL_POS_INIT message...
*
*/



#ifndef C_COMASUPLPOSINITSTATE_H
#define C_COMASUPLPOSINITSTATE_H

#include <e32base.h>

#include "epos_comasuplstartstate.h"
#include "epos_comasuplsession.h"
#include "epos_comasuplasnbase.h"

class COMASuplSETCapabilities;
class RMobilePhone;
class COMASuplPOSRequestor;
class COMASuplReqAsstData;
class COMASuplPosition;
class COMASuplPosPayload;		
class COMASuplVelocity;

/**
 *  COMASuplPosInitState handles all SUPL_POS_INIT related processing.  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class COMASuplPosInitState : public COMASuplStartState
{
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 v3.1u
		* @param aCapabilities,Allowed Capabilities
		* @param aMobilePhone,
		* @param aMsgTimeout,Timeout for message
		* @return Instance of COMASuplPosInitState
		*/ 
		static COMASuplPosInitState* NewL(COMASuplSETCapabilities* aCapabilities,
										  RMobilePhone& aMobilePhone,TInt aMsgTimeout,
										  COMASuplPOSRequestor* aPosRequestor,TInt aAllowedCapabilities,
										  COMASuplSession::TOMASuplReqType aRequestType, TOMASuplPosMethod aPosMethod,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase,
										  HBufC8* aEncodedSuplInit = NULL,
										  HBufC8* aHSLPAddress = NULL);
		
		/**
		* Destructor.
		*/
		~COMASuplPosInitState();
		
    public:		//COMASuplState
    
    /**
		* GenerateMessageL Method,Generates message...which message to be generate will be depend on derived class implementation
		* @since S60 v3.1u
		* @param None
		* @return System wide error
		*/ 
		virtual TInt GenerateMessageL();

    	/**
		* EncodeMessageL Method,Encodes message...which message to be generate will be depend on derived class implementation
		* @since S60  v3.1u
		* @param aSuplVersion, version of SUPL
		* @param aSessionId, sessionid of session.
		* @param aError, error code if any error occures while conversion.
		* @return encoded data, HBufC8*
		*/ 
		virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,
									   COMASuplSessionID* aSessionId,TInt &aError);
									 
	   /**
		* GetPosParamsL,Retrive Optional parameters for
		* SUPL POS INIT Message from POS Message plug-in
		* @since S60  v3.1u
		*/ 
		virtual void GetPosParamsL();
		
		void CancelOperation();
		
	public: // From MOMASuplMsgStateObserver
	
		/**
		* OperationCompleteL,Notifies about complaetion of operation with or without error.
		*@since Series 60 3.1u
		*@param aError,error code occured while completion of operation.
		*/
		virtual void OperationCompleteL(TInt aError);
		
		/**
		* GetPositionComplete,Notifies about complaetion of operation with or without error.
		*@since Series 60 3.1u
		*@param None
		*/
		void GetPositionComplete(TInt /*aError*/);
		
		
	public: //From MOMASuplETelObserver

	    /**
	     * RequestCompleted Method, Location Id request for comparision completed succesfully
	     * @since S60 v3.1u
	     * @param aLocationId, all location related information
	     * @param aErrorCode, Failure reason code        
	     * @return None
	     */
	    void ComparisionLocationIDRequestCompletedL(COMASuplLocationId* aLocationId,TInt aErrorCode);
		
	public:	
		/**
		* IsGenerationInProgress,returns status of generation of message
		*@since Series 60 3.1u
		*@param None
		@retun TBool
		*/
		TBool IsGenerationInProgress();
		
		/**
		*GetRemainingPospayLoadL,Gets the remaining Pospayload (i.e.more than 8 kb...used for sending with SUPL_POS) 
		*Callee gets ownership....
		*@since Series 60 3.1
		*@param None
		@retun COMASuplPosPayload*
		*/
		
		COMASuplPosPayload* GetRemainingPospayLoadL();

		/**
		*IsPosPayloadPresent,Checks if POSPayload is present in POSINIT
		*Callee gets ownership....
		*@since Series 60 3.1
		*@param None
		@retun TBool
		*/
		TBool IsPosPayloadPresent();
		
	private : 
	
	    //Constuctor
		COMASuplPosInitState(COMASuplSETCapabilities* aCapabilities,RMobilePhone& aMobilePhone,
							 TInt aMsgTimeout, COMASuplPOSRequestor* aPosRequestor,COMASuplSession::TOMASuplReqType aRequestType,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
        /**
        * ConstructL Method, 2nd phase construction
        * @since S60  v3.1u
        * @param None
        * @return None
        */
		void ConstructL(TInt aAllowedCapabilities, TOMASuplPosMethod aPosMethod, HBufC8* aEncodedSuplInit = NULL, HBufC8 *aHSLPAddress = NULL);
		
		/**
        * ValidateSetCapabilities Method to validate the capabilities of
        * the SET against the Capabilities retrieved during the construction
        * of SUPL POS INIT
        * @since S60  v3.1u
        * @param None
        * @return None
        */
		TInt ValidateSetCapabilities();

	
		/**
		* ComputeCapabilities Method,Computes capabilities.. does SETCaps = SETCaps & AllowedCaps
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		void ComputeCapabilities();
		
		/**
		* UpdateCapabilities modfies capability if 
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		void UpdateCapabilities(); 
		/**
		* ComputeSuplInitVerL Method,Computes the Verof SUPL INIT
		* in case of Network Initiated Location requests.
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		
		TBool ComputeSuplInitVerL();
        /**
		* StartTimer Method, Starts timer...
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/ 
		void StartTimer();
		
        /**
		* StripPospayLoadL Method, Strips Pospayload data if it exceeds more than 8KB
		* @since Series 60 3.1
		* @param  None
		* @return None
		*/ 
		void StripPospayLoadL();		

	private://Logging
		void LogPosPayloadData();
		void LogSETCapabilities();
		void LogPosition();
		void LogReqAssistanceData();
		void LogVelocity(COMASuplVelocity* velocity);
		void LogPacket(const TDesC8& aPacket);
		void PrintHex(const TDesC8& aBuffer,TInt aLine);
	private : //Data
	
		
		COMASuplSETCapabilities *iCurSetCapabilities;
		
		COMASuplSETCapabilities* iOldCapabilities;
		
		COMASuplReqAsstData *iReqAsstData;
		
		COMASuplPosition *iPosition;
		
		COMASuplPosPayload *iPosPayload;
		
		COMASuplVelocity *iVelocity;
				
		COMASuplSession::TOMASuplReqType iRequestType;
			
		HBufC8 *iEncodedSuplInit;
		
		HBufC8 *iHSLPAddress;
		
		TBuf8<64> iRes;
		
		TBool iVerPresent;

        TBool iPosPayloadPresent;
        
        COMASuplPosPayload* iRemainingPospayload;
        
        //Flag to indicate that the position data has to be ignored
        TBool   iIgnorePosData;
        //Flag to indicate fetching of pos data was repeated due to change in cell id 
        TBool   iRepeatedPosDataFetch;

    	COMASuplPosSessionBase::TOMASuplPositioningMethod iPosMethod;
};


#endif // C_COMASUPLPOSINITSTATE_H
