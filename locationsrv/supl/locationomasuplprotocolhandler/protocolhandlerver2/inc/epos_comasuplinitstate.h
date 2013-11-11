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
* Description:   class for handling SUPL_INIT message...
*
*/



#ifndef C_COMASUPLINITSTATE_H
#define C_COMASUPLINITSTATE_H

#include <e32base.h>

#include <lbs/epos_cposnetworkprivacy.h>
#include <lbs/epos_cpossuplprivacyrequestinfo.h>
#include <lbs/epos_mposverificationobserver.h>

#include "epos_comasuplinitver2.h"
#include "epos_momasupltimeoutnotifier.h"
#include "epos_comasuplstate.h"

// Forward Declarartion
class COMASuplNotification;
class COMASuplTimeoutTimer;
class MOMASuplPrivacyObserver;
/**
 *  COMASuplInitState handles all SUPL_INIT related processing.  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class COMASuplInitState : public COMASuplState,
						  public MPosVerificationObserver
{

	public :  // Constructor 

		/**
		* NewL Method.
		* @since S60  v3.1u
		* @param aMsgTimeout,timeout value for SUPL_INIT
		* @return Instance of COMASuplInitState
		*/ 
		static COMASuplInitState* NewL(TInt aMsgTimeout,const TDesC8& aHSLPAddress,
										MOMASuplPrivacyObserver &aPrivacyObserver,TInt aSupportedMode, CPosNetworkPrivacy*	iNetworkPrivacy);

		/**
		* Destructor.
		*/
		~COMASuplInitState();

	public:		//MPosVerificationObserver

		/**
		* Called when a call to CPosNetworkPrivacy::VerifyLocationRequestL()
		* completes, in which this observer object was passed as a parameter.
		*
		* @param aRequestId The request id which was returned by
		* 					CPosNetworkPrivacy::VerifyLocationRequestL().
		* @param aCompletionCode @p KErrNone if the location request is
		* 		 accepted, @p KErrAccessDenied if the location request is rejected
		* 		 or any of the system wide error codes.
		*/
		void HandleVerifyComplete(TInt /*aRequestId*/, TInt aCompletionCode);

	public:		//COMASuplState
		/**
		* Timeout Notifier Method.
		* @since S60  v3.1u
		* @param None
		* @return None
		*/
		void TimerExpiredL();

		/**
		* ProcessMessage Method,Process SUPL_INIT message...
		* @since S60  v3.1u
		* @param None
		* @return system wide error
		*/ 
		virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 
		/**
		* GetPosMethod Method,returns the positioning method...
		* @
		* @param None
		* @return TOMASuplPosMethod
		*/
		TOMASuplPosMethod GetPosMethod();

		/**
		* IsTriggerSession Method,returns whether trigger session or not 
		* @
		* @param None
		* @return TOMASuplPosMethod
		*/
	    TBool IsTriggerSession();	

		/**
		* GetTriggerType Method,returns whether trigger session or not 
		* @
		* @param None
		* @return TOMASuplPosMethod
		*/
	    COMASuplTriggeredStart::TOMASuplTrigType GetTriggerType();	
	    
	    void GetRequestorIDAndName(TInt& aRequestorId, TDes& aRequestorName);
	    
	    void GetPrivacyStatus(TBool& aPrivacyOverride);
	    
	    void GetSuplPrivacyRequestInfo(CPosSUPLPrivacyRequestInfo*& asuplReqInfo);
	    
	    void SetSessionId(TInt aSessionId);
	    
	private:
		/**
		* HandlePrivacy - Handles the privacy information
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/
		TInt HandlePrivacyL();
		/**
		* MakeGSMNotificationL - Handles the privacy notification
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 
		TInt MakeGSMNotificationL();

		/**
		* MakeGSMVerificationL - Handles the privacy verification
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 
		TInt MakeGSMVerificationL(CPosNetworkPrivacy::TRequestDecision aTimeOutStrategy);

		/**
		* Requestor - Retrievs requestor Information
		* @since S60  v3.1u
		* @param requestor
		* @return error code
		*/ 					 
		TInt RequestorL(HBufC*& aRequestor);

		/**
		* Client - Retrievs client Information
		* @since S60  v3.1u
		* @param clientname
		* @return error code
		*/ 	
		TInt ClientL(HBufC*& aClient);	

	private:

		/**
		* CheckForVersion - Checks for SUPL version
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 	
		TInt CheckForVersion();
		
		/**
		* CheckForModeSupport - Checks Modes
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 	
		TInt CheckForModeSupport();
		
		/**
		* CheckForModeSupport - Checks for the existance of SET Session ID
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 	
		
		TInt CheckForSetSessionID();
		
		/**
		* CheckForSLPSessionID - Checks for the existance of SET Session ID
		* @since S60  v3.1u
		* @param None
		* @return error code
		*/ 	
		TInt CheckForSLPSessionID();

		/**
		* ConverttoUniCode - Converts to Unicode
		* @since S60  v3.1u
		* @param clientname - Unicode String
		* @param aRequestor -  UTF8,UCS2, UTF& String
		* @return error code
		*/ 	
		TInt ConverttoUniCodeL(HBufC*& aUniCode,HBufC8& aRequstor);							  
		
		/**
		* CheckForPositioningMethod - Checks whether positionmethod is NoPosition
		* @since S60  v3.1u
		* @param None
		* @return None
		*/
		void CheckForPositioningMethod();
		
		/**
		* CheckForExtensionParameters - Extracts SUPL 2.0 extension params 
		* @since S60  v3.1u
		* @param None
		* @return None
		*/
		void CheckForExtensionParameters();
		
		/**
		* GsmDefaultDecode - Unpacks the data of  7-bit format to 8-bit
		* @since S60  v3.1u
		* @param aIn, input  string, represented in 7-bit
		* @param aOut,output string, represented in 8-bit
		* @return None
		*/
		void GsmDefaultDecode( const TDesC8& aIn, TDes8& aOut );
		
		/**
		* FormUCS2String - Forms the UCS2 string from the string which contains the characters 
		* @since S60  v3.1
		* @param aIn, input  string, represented in 8-bit
		* @param aOut,output string, represented in UCS2
		* @return None
		*/
		void FormUCS2String( const TDesC8& aIn, TDes& aOut );
		
	private : 

		/**
		* Constuctor Method.
		* @since S60  v3.1u
		* @param aMsgTimeout,Timeout for message
		* @return None
		*/ 
		COMASuplInitState(TInt aMsgTimeout,const TDesC8& aHSLPAddress,
						MOMASuplPrivacyObserver &aPrivacyObserver,TInt aSupportedMode, CPosNetworkPrivacy*	iNetworkPrivacy);

		/**
		* ConstructL Method, 2nd phase construction
		* @since S60  v3.1u
		* @param None
		* @return None
		*/
		void ConstructL();
		
		void ReadRequestIDFeatureFlagL();
		

	private: //Data

		COMASuplInitVer2* iSuplInit;

		CPosNetworkPrivacy*	iNetworkPrivacy;

		// RequestId. Used by POS Message Plugin API.
		TInt	iGSMRequestId;

		//Type of requestinfo
		CPosSUPLPrivacyRequestInfo::TIdType iIdType;

		//Notification
		COMASuplNotification* iNotification;

        //Notification mode
        COMASuplVer2SuplInitExtn::TOMASuplNotificationMode iNotificationMode;

        //Trigger type
        COMASuplTriggeredStart::TOMASuplTrigType iTriggerType;

        //Historic reportings supported
        TOMASuplHistoricReporting iHistRepSupported;
		
		//Completion Code
		TInt iCompletionCode;
		
		const TDesC8& iHSLPAddress;
		
		// Privacy Observer 
		MOMASuplPrivacyObserver &iPrivacyObserver;
		
		TBool iNoPosition;
		//Supported Mode of SET
		TInt iSupportedMode;
		TOMASuplPosMethod iPosMethod;
        TBool iTriggerSessionFlag;
        
        
        
        RBuf iRequestorName;     //Vijay
        
        TBool iPrivacyOverride;  //Vijay
        
        CPosSUPLPrivacyRequestInfo* iSuplReqInfo; //Vijay
        
        TBool iIsNotificationOnly;//APE Centric
        
        TBool iRequestIDFeatureFlag;//APE Centric
        
        TInt iSETSessionUniqueId;
};

#endif //C_COMASUPLINITSTATE_H
