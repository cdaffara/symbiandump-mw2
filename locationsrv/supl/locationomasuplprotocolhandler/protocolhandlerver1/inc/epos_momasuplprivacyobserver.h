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
* Description:   Class for observing privacy states
*
*/



#ifndef M_MOMASUPLPRIVACYOBSERVER_H
#define M_MOMASUPLPRIVACYOBSERVER_H

#include <e32base.h>

/**
 *  Observer class for Privacy Checking
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 v3.1u
 */

class MOMASuplPrivacyObserver
{
	public:	
		/**
		* PrivacyCheckCompleteL Method.
		* @since S60 3.1u
		* @param aErrorCode, error code - Specifying whether accepted or rejected
		* @return None
		*/ 
		virtual void PrivacyCheckCompleteL(TInt aCompletionCode) = 0;		
};

#endif //M_MOMASUPLPRIVACYOBSERVER_H
