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
* Description:   Class for doing asynchronous service.
*
*/



#ifndef C_COMASUPLPOSREQUESTOR_H
#define C_COMASUPLPOSREQUESTOR_H

#include <e32base.h>
#include "lbs/epos_tomasuplallowedcapabilities.h"

class COMASuplPosSessionBase;
class COMASuplInfoRequest;
class COMASuplInfoRequestList;
class COMASuplTrace;
class COMASuplSETCapabilities;
class MOMASuplMsgStateObserver;
class COMASuplPosition;

class TOMASuplAllowedCapabilities;
class TOMASuplPositioningMethod;

/**
	Class for doing asynchronous service  i.e. used for POS Handler.	
*/

class COMASuplPOSRequestor : public CActive
{
		  
	public:
		enum TPOSRequestType
			{
				EPOS_SESSION_INITIALIZE = 0,
				EPOS_GET_SUPL_INFO,
				EPOS_GET_POSITION,
				EPOS_IDEAL
			};	  	
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param 
		* @return Instance of COMASuplPOSRequestor
		*/ 
		static COMASuplPOSRequestor* NewL(MOMASuplMsgStateObserver* aObserver,COMASuplPosSessionBase* aPOSSession);
		
		/**
		* Destructor.
		*/
		~COMASuplPOSRequestor();
		
	public :
	
		/**
		* GetSuplInfoL Method.
		* @since S60 3.1u
		* @param None
		* @return Error code if function leaves otherwise KErrNone
		*/ 
		TInt GetSuplInfoL();
		
		/**
		* GetPositionL Method.
		* @since S60 3.1u
		* @param position information
		* @return Error code if function leaves otherwise KErrNone
		*/ 
		TInt GetPositionL(COMASuplPosition* aPosition);
		
		/**
		* CancelRequest Method,Cancels any outstanding request if any
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/ 
		void CancelRequest();
		
		COMASuplInfoRequestList* GetOMASuplInfoRequestList();
		
		void AppendInfoRequest(COMASuplInfoRequest* aSuplInfoRequest);
		
		TInt InitilizePOSSessionL(TInt aRequestId);
		
		/**
		* SetObserver Sets the observer that gets back data
		* from POS Message plugin
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 
		
		void SetObserver(MOMASuplMsgStateObserver* aObserver);
		
		/**
		* CreateListL Creates the Info Request List 
		* from POS Message plugin
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 
		void CreateListL();
	
		/**
		* DestroyList Destroys the Info Request List 
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 	
		void DestroyList();
		
		/**
		* SetPosMethodAndAllowedCapabilities send allowed capability and POS Method to POS Plug-in 
		* @since 
		* @param aPOSAllowedCapabilities The allowed capabilities
		* @param aPOSPositioningMethod   The prefered positioning method
		* @return None
		*/
        void SetPosMethodAndAllowedCapabilities(TOMASuplAllowedCapabilities& aPOSAllowedCapabilities,COMASuplPosSessionBase::TOMASuplPositioningMethod aPOSPositioningMethod);
        /**
         * SetSLPAddressUsed send server address currently used for this session into the POS Plug-in 
         * @since 
         * @param aServerAddress The server address that is being used for this session         
         * @return None
         */
        void SetSLPAddressUsed(const TDesC& aServerAddress);
	private : 
		//Constuctor
		COMASuplPOSRequestor(MOMASuplMsgStateObserver* aObserver,COMASuplPosSessionBase* aPOSSession);
		
		/**
		* By default Symbian 2nd phase constructor is private.
		*/
		void ConstructL();
		

		
  protected :  // Functions from CActive
      /**
      * From CActive 
      * To handle request completion
      */
      void RunL();
      
      /**
      * From CActive 
      * To cancel the asynchronous requests
      */
   		void DoCancel();  
      /**
      * From CActive 
      * To handle errors in asynchronous requests
      */
   		TInt RunError(TInt aError);
     
	private : //Data
	
		MOMASuplMsgStateObserver* iObserver;
		
		//POS Session ... 
		//will not have ownership with this class
		COMASuplPosSessionBase* iPOSSession;
		
		// Info Requestor List
		COMASuplInfoRequestList* iOMASuplInfoRequestList; 
		
		TPOSRequestType iPOSSessionRequestType;
		
		COMASuplPosSessionBase::TOMASuplPositioningMethod iPositioningMethod;
		
		TOMASuplAllowedCapabilities iAllowedCapabilities;
		
    //Trace Utility
    COMASuplTrace* iTrace;
};

#endif

