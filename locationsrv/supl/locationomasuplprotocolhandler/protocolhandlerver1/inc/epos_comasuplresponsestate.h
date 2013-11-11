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
* Description:   class for handling SUPL_RESPONSE message...
*
*/



#ifndef C_COMASUPLRESPONSESTATE_H
#define C_COMASUPLRESPONSESTATE_H

#include <e32base.h>

#include "epos_comasuplstate.h"

class COMASuplSETCapabilities;

/**
 *  COMASuplResponseState handles all SUPL_RESPONSE related processing.   
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */


class COMASuplResponseState : public COMASuplState
{
	public :  // Constructor 
	
		/**
		* NewL Method.
		* @since S60 v3.1u
		* @param None
		* @return None
		* @return Instance of COMASuplResponseState
		*/ 
		static COMASuplResponseState* NewL();
		
		/**
		* Destructor.
		*/
		~COMASuplResponseState();
		
    public:		//COMASuplState
		
	 	/**
		* ProcessMessage Method,Process SUPL_RESPONSE message...
		* @since S60 v3.1u
		* @param aDecodedMessage,decoded ASN messagge.
		* @return system wide error
		*/ 
		virtual TInt ProcessMessageL(COMASuplAsnMessageBase* aDecodedMessage); 
		
		/**
		* GetPosMethod gives the positioning Method...
		* @since 
		* @param None.
		* @return TOMASuplPosMethod
		*/ 
		TOMASuplPosMethod GetPosMethod();
	protected : 
	
		//Constuctor
		COMASuplResponseState();
	
		/**
		* ConstructL Method, 2nd phase construction
		* @since S60 v3.1u
		* @param None
		* @return None
		*/
		void ConstructL();

	protected: //Data
	TOMASuplPosMethod iPosMethod;
		
};

#endif //C_COMASUPLRESPONSESTATE_H
