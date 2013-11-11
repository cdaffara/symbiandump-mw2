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



#ifndef C_COMASUPLINITILIZEREQUESTOR_H
#define C_COMASUPLINITILIZEREQUESTOR_H

#include <e32base.h>
#include <epos_csuplsettings.h>

#include "epos_comasuplinitilizeobserver.h"

class COMASuplPosHandlerBase;
class COMASuplSettings;

/**
 *  Class for doing initilization of protocol handler.
 *  
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class COMASuplInitilizeRequestor : public CActive
{

public :  // Constructor 
	
		/**
		* NewL Method.
		* @since Series 60 3.1u
		* @param MEventCompletionObserver
		* @return Instance of COMASuplInitilizeRequestor
		*/ 
		static COMASuplInitilizeRequestor* NewL(COMASuplPosHandlerBase *aPosHandler,MCompleteInitilization& aObserver, COMASuplSettings* aOMASuplSettings);
		
		/**
		* Destructor.
		*/
		~COMASuplInitilizeRequestor();
		
public :    		
		/**
		* DoInitilization Method, starts initilzation
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/ 
		void DoInitilizationL();
		
		/**
		* GetIMSI Method, Retrieves IMSI from SIM
		* @since Series 60 3.1u
		* @param aIMSI, TDes
		* @return TInt, error code
		*/ 
		TInt GetIMSI(TDes& aIMSI);
		
private : 
	
		/**
		* CompleteSelf Method, request for self completion
		* @since Series 60 3.1u
		* @param None
		* @return None
		*/ 
		void CompleteSelf();
		
		/**
		* Constuctor 
		* @since Series 60 3.1u
		* @param aObserver, MCompleteInitilization
		* @return None
		*/
		COMASuplInitilizeRequestor(COMASuplPosHandlerBase *aPosHandler,MCompleteInitilization& aObserver,COMASuplSettings* aOMASuplSettings);
		
		/**
		* By default Symbian 2nd phase constructor is private.
		*/
		void ConstructL();
		
		/**
		* Checks for IMSI variance
		*/
		void SetVariant();
		
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
	
		enum TInitializeRequestorState
		    {
		    ENotInitialized = 0,
		    EInitializePosHandler,
		    EInitializeSuplSettings,
		    EInitializationComplete
		    };
		    
		//POS Handler   
		COMASuplPosHandlerBase*     iPosHandler;
		
		//Observer for initilization			
		MCompleteInitilization&     iObserver; 

        //Tracing utility
	    COMASuplTrace* iTrace;
		
		TInitializeRequestorState   iState;
		
		CSuplSettings*              iSuplSettings;
		
		TBool						iVariantEnabled;
		COMASuplSettings* iOMASuplSettings;
};

#endif //C_COMASUPLINITILIZEREQUESTOR_H
