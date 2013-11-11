/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains definitions of the queue item class.
*
*/


#ifndef __MS60AUDIOSOURCEOBSERVER_H__
#define __MS60AUDIOSOURCEOBSERVER_H__

// INCLUDES

#include <e32std.h>

/**
*  This is the S60 Audio Base Source observer class.
*
*  @since 3.1
*/
class MS60AudioBaseSourceObserver
	{
	public:
        /**
        * Used to Indiacate the Seek Support changed
        * @since Series 60 3.1
        */
		virtual void SeekSupportChanged() = 0;
	};

#endif // __MS60AUDIOSOURCEOBSERVER_H__

//  End of File
