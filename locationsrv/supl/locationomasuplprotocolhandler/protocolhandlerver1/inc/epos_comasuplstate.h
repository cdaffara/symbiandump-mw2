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
* Description:   Base class for all State class(SUPL message) 
*
*/



#ifndef M_MOMASUPLMSGSTATEOBSERVER_H
#define M_MOMASUPLMSGSTATEOBSERVER_H

#include <e32base.h>
#ifdef _DEBUG        
#define PRINT_MESSAGE
#endif        

	/** 
	MOMASuplMsgStateObserver Notifies about complaetion of operation with or without error.
	*  @lib eposomasuplprotocolhandler.lib
	*  @since S60 v3.1u
	*/
	
class MOMASuplMsgStateObserver
    {
public:

		/**
		* OperationCompleteL,Notifies about complaetion of operation with or without error.
		*@since Series 60 3.1M
		*@param aError,error code occured while completion of operation.
		*/
    virtual void OperationCompleteL(TInt aError) = 0;
    /**
		* GetPositionComplete,Notifies about complaetion of get position operation with or without error.
		*@since Series 60 3.1u
		*@param aError,error code occured while completion of operation.
		*/
    virtual void GetPositionComplete(TInt aError) = 0;
    };

#endif // M_MOMASUPLMSGSTATEOBSERVER_H


/////////////////////////////////////////////////////////////////////////////////////



#ifndef C_COMASUPLSTATE_H
#define C_COMASUPLSTATE_H

#include <e32base.h>
#include "epos_comasuplasnmessagebase.h"

#include "epos_momasupltimeoutnotifier.h"

/**
* Base class for all State class(SUPL message)     
*  @lib eposomasuplprotocolhandler.lib
*  @since S60 v3.1u
*/


class COMASuplTimeoutTimer;
class COMASuplTrace;
class COMASuplSessionID;
class MOMASuplMsgStateObserver;

class COMASuplState : public CBase,public MOMASuplTimeOutNotifier
{

public:	

	//Type of message
	
		enum TSuplMessageType
			{
				ESUPL_NONE = 0,
				ESUPL_START ,	
				ESUPL_RESPONSE,
				ESUPL_POS,
				ESUPL_POS_INIT,
				ESUPL_INIT,
				ESUPL_END
			};
		
public : 
			/**
			* Destructor.
			*/
			~COMASuplState();
			
			protected:		
			// Constructor 
			COMASuplState();

public :			

			/**
			* GenerateMessageL Method,Generates message...which message to be generate will be depend on derived class implementation
			* @since Series 60 3.1M
			* @param None
			* @return system wide error code
			*/ 
			virtual TInt GenerateMessageL();
			
			/**
			* CancelOperation Method,Cancels operations if it is ongoing
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			virtual void CancelOperation();
			
			/**
			* EncodeMessageL Method,Encodes message...which message to be generate will be depend on derived class implementation,
			* ownership will be transfer to callee
			* @since Series 60 3.1M
			* @param aSuplVersion, version of SUPL
			* @param aSessionId, sessionid of session.
			* @param aError, error code if any error occures while conversion.
			* @return encoded data, HBufC8*
			*/ 
			virtual HBufC8* EncodeMessageL(TOMASuplVersion &aSuplVersion,
							   COMASuplSessionID* aSessionId,TInt &aError);
			/**
			* ProcessMessage Method,Processes message...which message to be generate will be depend on derived class implementation
			* @since Series 60 3.1M
			* @param aDecodedMessage, message to be process
			* @return Systemwide error code
			*/ 
			virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 

public :

			/**
			* BaseConstructL Method,Base class Construction
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			void BaseConstructL();
			
			/**
			* GetSuplState Method, returns type of message
			* @since Series 60 3.1M
			* @param None
			* @return COMASuplState::ESuplMessageType
			*/ 
			COMASuplState::TSuplMessageType GetSuplState();
			
			/**
			* StartTimer Method, Starts timer...timeout value is depend on message type.
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			virtual void StartTimer();
			
			/**
			* StopTimer Method, Stops timer...
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			void StopTimer();
			
			/**
			* SetMsgStateObserver Method, Sets observer message completion handling 
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			void SetMsgStateObserver(MOMASuplMsgStateObserver* aMsgStateObserver);
			
			/**
			* SetVersion Method, Sets current version of SUPL 1.0.0
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/ 
			void SetVersion(TOMASuplVersion& aOMASuplVersion);

public: // MOMASuplTimeOutNotifier

			/**
			* Timeout Notifier Method.
			* @since Series 60 3.1M
			* @param None
			* @return None
			*/
			virtual void TimerExpiredL();
			

protected : //Data

		//Timeout Timer
		COMASuplTimeoutTimer* iTimer;
		
		//Trace Utility
		COMASuplTrace* iTrace;
		
		TSuplMessageType iCurrentState;
		
		//Time out for message
		TInt iMessageTimeOut;
		
		//	Observer for message handling completion	
		MOMASuplMsgStateObserver* iMsgStateObserver;
		
		//Supl Version		
		TOMASuplVersion iOMASuplVersion;

};

#endif //C_COMASUPLSTATE_H

