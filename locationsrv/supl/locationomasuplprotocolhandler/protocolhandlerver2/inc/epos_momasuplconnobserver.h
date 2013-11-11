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
* Description:   Class for observing connection states
*
*/



#ifndef M_MOMASUPLCONNOBSERVER_H
#define M_MOMASUPLCONNOBSERVER_H

#include <e32base.h>

/**
 *  Observer class for Packet Send
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class MOMASuplConnObserver
{
	public:	
		/**
		* OperationComplete Method.
		* @since S60 3.1u
		* @param aErrorCode, error code,if some error occurs during asynchronous operation
		* @return None
		*/ 
		virtual void OperationCompleteL(TInt aErrorCode) = 0;		
};

#endif //M_MOMASUPLCONNOBSERVER_H
