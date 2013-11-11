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
* Description:   Class for doing Initilization of protocol handler operation.
*
*/



#ifndef C_COMASUPLDEINITIALIZER_H
#define C_COMASUPLDEINITIALIZER_H

#include <e32base.h>
#include "epos_csuplprotocolmanagerbase.h"   // interface
/**
 *  MCompleteDeInitialization ... observer for De-initilization completion.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 5.2
 */
class MCompleteDeInitialization
{
	public:
		/**
		* DeInitilizationCompletedL Method,called after request has been completed by Active Scheduler
		* @since S60 5.2
		* @param aError,Error code occured during initilization
		* @return None
		*/ 
		virtual void DeInitilizationCompletedL(TInt aError)=0;
		
		
};
/**
 *  Class for doing De-initilization of protocol handler.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 5.2
 */
class COMASuplTrace;

class COMASuplDeInitilizer : public CActive
{

public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 5.2
		* @param MCompleteDeInitilization
		* @return Instance of COMASuplDeInitilizer
		*/ 
		static COMASuplDeInitilizer* NewL( MCompleteDeInitialization& aObserver );
		
		/**
		* Destructor.
		*/
		~COMASuplDeInitilizer();
		
		void Deinitialize( CSUPLProtocolManagerBase* aOMASuplProtocolHandler );
		
private : 
	
				
		/**
		* Constuctor 
		* @since S60 5.2
		* @param aObserver, MCompleteDeInitilization
		* @return None
		*/
		COMASuplDeInitilizer(MCompleteDeInitialization& aObserver);
		
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
	    //Tracing utility
		COMASuplTrace* iTrace;
		
		
		//Observer for initilization			
		MCompleteDeInitialization&     iObserver; 
		CSUPLProtocolManagerBase* iOMASuplProtocolHandler;
		

      
};

#endif //C_COMASUPLDEINITIALIZER_H
