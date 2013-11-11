/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Holds instructions and data from message file
*
*/

#ifndef C_COMASUPLMESSAGEINFO_H
#define C_COMASUPLMESSAGEINFO_H

#include <e32base.h>

class COMASuplPosPayload;

/** Constant .*/
_LIT8(KSendOp,"send");

/** Constant .*/
_LIT8(KWaitOp,"wait");

/**
 *  Contains instructions along with data read from message file.
 *
 *  Message file contains instructions along with POS payload type and
 *  pos payload. This class provides a container to hold these information
 *  in the form of an object.
 *
 *  @lib PosTesterModuleCfg.lib
 *  @since S60 v3.1
 */
class COMASuplMessageInfo : public CBase
	{
	public:

	enum TOMASuplOperationType
		{
		ESend = 0,
		EWait
		};

	public:
		static COMASuplMessageInfo* NewL(TOMASuplOperationType aOpType
									, COMASuplPosPayload& aPayload);

		~COMASuplMessageInfo();
	
	private:
		COMASuplMessageInfo(TOMASuplOperationType aOpType);
		void ConstructL(COMASuplPosPayload& aPayload);
	public:
		/**
     	 * Holds instruction, ESend or EWait 
     	 */
		TOMASuplOperationType iOpType;
		
		/**
     	 * Pointer to a copy of payload
     	 * Own. 
     	 */
		COMASuplPosPayload* iPayload;
	};
#endif