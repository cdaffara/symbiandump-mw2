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
* Description:   Timeout Observer
*
*/



#ifndef M_MOMASUPLDIALOGTIMEOUTTIMER_H
#define M_MOMASUPLDIALOGTIMEOUTTIMER_H

#include <e32base.h>
/**
 *  This class specifies the function to be called when a timeout occurs.
    Used in conjunction with COMASuplTimeoutTimer class   
 *  @lib eposomasuplprotocolhandler.lib
 *  @since S60 TB 9.1 
 */

class MOMASuplDialogTimeOutNotifier
    {
public:
    /**
    *TimerExpired,The function to be called when a timeout occurs
    * @since  S60 9.1 TB 
    * @param  None
    * @return None
    */
	virtual void DialogTimerExpiredL() = 0;
			
    };

#endif // M_MOMASUPLDIALOGTIMEOUTTIMER_H


