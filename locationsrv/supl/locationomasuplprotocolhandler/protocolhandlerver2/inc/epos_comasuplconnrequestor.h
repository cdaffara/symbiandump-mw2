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



#ifndef C_COMASUPLCONNREQUESTOR_H
#define C_COMASUPLCONNREQUESTOR_H

#include <e32base.h>

#include "epos_csuplconnection.h"
#include "epos_comasupluicompletionobserver.h" 
#include "epos_momasupldialogtimeoutnotifier.h"

class CSuplCommunicationManager;
class MOMASuplConnObserver;
class CSuplSettings;
class COMASuplTrace;
class COMASuplFallBackHandler;
class COMASUPLProtocolManager2;
class COMASuplDialogTimer;
/**
 *  Class for doing asynchronous service  i.e. used for Sending Packet to Socket.	
 *
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1
 */
 
class COMASuplConnRequestor : public CActive
{

	enum TConnState
	    {
		  EConnecting,
		  EConnectionDone,
	      ESending,
	      EInitilizeSetting,
		  EInitialState
		};

	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since Series 60 3.1
		* @param MEventCompletionObserver
		* @return Instance of COMASuplAOOperation
		*/ 
		
		/*
		static COMASuplConnRequestor* NewL(CSuplCommunicationManager& aCommMgr, const TDesC8& aHostAddr,
										   TInt aIAPId, TUint aPort, MOMASuplConnObserver& aObserver);
		*/										   
		static COMASuplConnRequestor* NewL(CSuplCommunicationManager& aCommMgr,COMASUPLProtocolManager2& aProtoMgr,TUint aPort,MOMASuplConnObserver& aObserver);										   
		
		/**
		* Destructor.
		*/
		~COMASuplConnRequestor();
		
	public :		
	
		/**
		* CreateConnection Method. Creates the Socket required to
		* communicate with the SLP
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void CreateConnectionL(TBool aTriggerSession);
		
		/**
		 * CreateConnection Method Overload. Creates the Socket required to
		 * communicate with the SLP. Called in case of NI case.
		 * @since 
		 * @param None
		 * @return None
		 */ 
		void CreateConnectionL(TInt aDialogTimeOutDelay, TBool aTriggerSession);
		
		/**
		* SendPacket Method.
		* @since Series 60 3.1
		* @param aConnection, for sending Packet
		* @param aPacket, Packet
		* @return None
		*/ 
		void SendPacket(const TDesC8& aPacket);

		/**
		* OpenConnection Method.,It Connects to SLP
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void OpenConnection();
		
		/**
		* ListenToMessages Method.Start listening to Messages
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void ListenToMessages();
		
		/**
		* CloseConnection Method.Destroys the socket to SLP
		* @since Series 60 3.1
		* @param None
		* @return None
		*/ 
		void CloseConnection();
		
		/**
		* GetLocalIPAddress Method.
		* @since Series 60 3.1
		* @param aAddress, TInetAddr 
		* @return None
		*/ 
		TInt GetLocalIPAddress(TInetAddr& aAddress);
		
		/**
		* GetLocalIPAddress Method.
		* @since Series 60 3.1
		* @param aAddress, TInetAddr 
		* @return None
		*/ 
		void CancelReadOperation();
		
		/**
		* SetIAPID Method.
		* @since Series 60 3.1
		* @param aIAPID, 
		* @return None
		*/ 
		void SetIAPID(TInt aIAPID);

	/**
		* IsHslpGenerated Method,checks if Hslp address is NULL. 
		* @since S60 3.1
		* @param None
		* @return None
		*/ 
	
		void IsHslpGenerated(TBool aIsGenerated);

       /**
		* IsHslpAddrFromImsiUsed Method,checks if Hslp address is used for connection is from IMSI. 
		* @since S60 3.2
		* @param None
		* @return None
		*/ 
        TBool IsHslpAddrFromImsiUsed();
        
        void SetDefaultParametersL(const TDesC& aClientProvidedHSLPAddress,TBool aFallBackAllowed);	
        
        void GetUsedServerAddress(TDes& aHSLPAddressUsed);
        
        void SettingsUsageUICompletedL(TInt /*aError*/) {}
        void SettingsTimeOutUICompletedL(TInt /*aError*/) {}
        TBool ConvertIAPNameToIdL(const TDesC& aIAPName, TUint32& aIAPId);
        void UpdateNetInfo(const TTime& aLastTimeUsed,TInt& aMcc,TInt& aMnc,
							              TInt& aCid,TInt& aLac,TInt& aType,TInt& aErrorCode);
				
				void UseDefaultServerL();	            
				void UseServerL(const TDesC& aClientProvidedHSLPAddress);  
				void UpdateSLPListForHomeUsage(TBool aHomeNetwork);

        TUint GetPortNumber();
	private : 
		//Constuctor
		/*
		COMASuplConnRequestor(CSuplCommunicationManager& aCommMgr, 
							  TInt aIAPId, TUint aPort, MOMASuplConnObserver& aObserver);
		*/							  
											 
		COMASuplConnRequestor(CSuplCommunicationManager& aCommMgr,
							   COMASUPLProtocolManager2& aProtoMgr,
					  			TUint aPort,
					  		   MOMASuplConnObserver& aObserver);
		/**
		* By default Symbian 2nd phase constructor is private.
		*/
		//void ConstructL(const TDesC8& aHostAddr);
		void ConstructL();
		
	private :		
	
		
		/**
		* InitilizeSetting Method,Initilize Supl Settings. 
		* @since S60 3.1
		* @param None
		* @return None
		*/ 
		void InitilizeSetting();
		
		void SaveAccessPoint(const TDesC& aIapName);

		/**
		 * Dialog Timeout Notifier Method.
		 * @since Series 60 9.1 TB
		 * @param None
		 * @return None
		 */
		//virtual void DialogTimerExpiredL();	
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
	
			//Connection State
			TConnState iState;
			
			// Communication Manager Reference
			CSuplCommunicationManager& iCommMgr;
			
           // Reference to protocol manager to invoke settings UI
            COMASUPLProtocolManager2& iProtocolManager;


			
			// Connection Object
			CSuplConnection *iConnection;

			// A String buffer holding the IP Address in String Format
			TBuf <KMaxHostAddressLen> iHostAddress;
			
			// An unsigned integer hodling the port with which 
			// connection has to be made
			TUint iPort;
			
			// IAP ID
			TUint32 iIAPId;
			
			// TLS
			TBool iTls;
			
			// PSKTLS
			TBool iPskTls;
			
			// Active Object Observer
			MOMASuplConnObserver& iObserver;      
			
			//For trying for once with newly generated HSLP.
			TBool iTLSAuthenticationFailed;
			
			//SUPLSettings
			CSuplSettingsInternal *iSuplSettings;
			
			//Trace Utility
			COMASuplTrace* iTrace;
			TBool iIsHSLPGenerated;
            TBool iHslpAddrFromImsiUsed;
            
            TInt iLastConnectionError;
            
            COMASuplFallBackHandler* iFallBackHandler;
            
            TBool iIsSettingInitilized;
            
            TInt64 iCurrentSLPId;
            
            TBool iTriggerSession;

};

#endif //C_COMASUPLCONNREQUESTOR_H
