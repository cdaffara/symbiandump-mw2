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
* Description:   class for handling SUPL_TRIGGER_STOP message...
*
*/



#ifndef C_COMASUPLTRIGGERSTOPSTATE_H
#define C_COMASUPLTRIGGERSTOPSTATE_H

#include <e32base.h>

#include "epos_comasupltriggeredstop.h"
#include "epos_comasuplstate.h"

class COMASuplAsnHandlerBase;

/**
 *	COMASuplTriggerStopState handles all SUPL_END related processing.
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */


class COMASuplTriggerStopState : public COMASuplState
{
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param COMASuplEnd::TOMASuplStatusCode,Status code,if any error 
		* @return Instance of COMASuplTriggerStopState
		*/ 
		static COMASuplTriggerStopState* NewL(COMASuplEnd::TOMASuplStatusCode,
                                              COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param None
		* @return Instance of COMASuplTriggerStopState
		*/ 
		static COMASuplTriggerStopState* NewL(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
		/**
		* NewL Method.
		* @since S60 3.1u
		* @param None
		* @return Instance of COMASuplTriggerStopState
		*/ 
		static COMASuplTriggerStopState* NewL(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);

		/**
		* Destructor.
		*/
		~COMASuplTriggerStopState();
		
    public:		//COMASuplState
    
   	/**
		* GenerateMessageL Method,Generates SUPL_END message...
		* @since S60 3.1u
		* @param None
		* @return system wide error
		*/ 
		virtual TInt GenerateMessageL();

   	/**
		* EncodeMessageL Method,Encodes SUPL_END message...
		* @since S60 3.1u
		* @param aSuplVersion, version of SUPL
		* @param aSessionId, sessionid of session.
		* @param aError, error code if any error occures while conversion.
		* @return encoded data, HBufC8*
		*/ 
		virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,COMASuplSessionID* aSessionId,
									   TInt &aError);
		
    /**
		* CancelOperation  Method,Cancels Operation...which are going on...
		* @since S60 3.1u
		* @param None
		* @return None
		*/ 
		virtual void CancelOperation();

    /**
		* ProcessMessage Method,Process SUPL_END message...
		* @since S60 3.1u
		* @param aDecodedMessage, COMASuplAsnMessageBase ...decoded message
		* @return system wide error
		*/ 
		virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 
		
		/**
		* StartTimer Method, Starts timer...timeout value is depend on message type.
		* @since S60 3.1u
		* @param None
		* @return None
		*/ 
		void StartTimer();
		
 	   /**
		* ComputeSuplEndVer Method,Computes the Verof SUPL INIT
		* in case of Network Initiated Location requests.
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		
		TBool ComputeSuplEndVerL();

 	   /**
		* GetStatusCode Method, returns the current value stored in the status code
		* @since S60 v3.2
		* @param None
		* @return statusCode
		*/
        COMASuplEnd::TOMASuplStatusCode GetStatusCode();

	private:
	
 	   /**
		* LogReceivedPosition Method,Logs position information
		* @since S60 v3.1
		* @param None
		* @return None
		*/
		void PrintHex(const TDesC8& aBuffer,TInt aLine);
		
	protected : 
	
		//Constuctor
		COMASuplTriggerStopState(COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
		//Constuctor
		COMASuplTriggerStopState(TInt aMsgTimeout, COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		/**
		* Constuctor
		* @since S60 3.1u
		* @param COMASuplEnd::TOMASuplStatusCode,Status code,if any error 
		* @return None
		*/ 
		COMASuplTriggerStopState(COMASuplEnd::TOMASuplStatusCode aStatusCode,COMASuplAsnHandlerBase* aOMASuplAsnHandlerBase);
		
    /**
    * ConstructL Method, 2nd phase construction
    * @since S60 3.1u
    * @param None
    * @return None
    */
		void ConstructL();

	private: //Data
	    COMASuplEnd::TOMASuplStatusCode iStatusCode;
	    TBool iSetStatusCode;
		COMASuplAsnHandlerBase* iOMASuplAsnHandlerBase;
};

#endif //C_COMASUPLTRIGGERSTOPSTATE_H
