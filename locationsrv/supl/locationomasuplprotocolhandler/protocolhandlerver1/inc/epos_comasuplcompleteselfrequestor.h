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
* Description:   Class for doing self complete operation.
*
*/



#ifndef C_COMASUPLSELFCOMPLETEREQUESTOR_H
#define C_COMASUPLSELFCOMPLETEREQUESTOR_H

#include <e32base.h>

/**
 *  MCompleteSelfRequest ... observer for self completion.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */
class MCompleteSelfRequest
{
	public:
		/**
		* RequestCompleted Method,called after request has been completed by Active Scheduler
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 
		virtual void RequestCompletedL() = 0;
		
		/**
		* StartInvalidSessionL Method is invoked by the active object
		* when there is a session request with invalid session ID
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 
		virtual void StartInvalidSessionL() = 0;
		/**
		* StartInvalidSessionL Method is invoked by the active object
		* when there is a session request with invalid session ID
		* @since Series 60 3.1M
		* @param None
		* @return None
		*/ 
		virtual void HandleInvalidSLPSession() = 0;
		
};

/**
 *  Class for doing self complete operation.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class COMASuplCompleteSelfRequestor : public CActive
{
	// Enum definition
	enum TOMASuplCompleteStatus
	{
		ESUPL_PACKET_RECEIVE,
		ESUPL_INVALID_SESSIONID,
		ESUPL_INVALID_SLP_SESSIONID
	};
	
	public :  // Constructor 
		/**
		* NewL Method.
		* @since Series 60 3.1u
		* @param MEventCompletionObserver
		* @return Instance of COMASuplCompleteSelfRequestor
		*/ 
		static COMASuplCompleteSelfRequestor* NewL(MCompleteSelfRequest& aObserver);
		
		/**
		* Destructor.
		*/
		~COMASuplCompleteSelfRequestor();
		
	public :		
		/**
		* CompleteSelf Method, request for self completion
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/ 
		void CompleteSelf();
		
		/**
		* CompleteInvalidSession for completing the request in case of invalid session ID
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/
		void CompleteInvalidSession();
		
		/**
		* CompleteInvalidSLPSession for completing the request in case of invalid session ID
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/
		void CompleteInvalidSLPSession();
		
	private : 
		/**
		* Constuctor 
		* @since Series 60 3.1u
		* @param aObserver, MCompleteSelfRequest
		* @return None
		*/

		COMASuplCompleteSelfRequestor(MCompleteSelfRequest& aObserver);
		
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

     
	private : //Data
			MCompleteSelfRequest& iObserver;    
			TOMASuplCompleteStatus iCompStatus;
};

#endif //C_COMASUPLSELFCOMPLETEREQUESTOR_H
