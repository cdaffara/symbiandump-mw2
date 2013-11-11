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


#ifndef __MS60AUDIOSTREAMSOURCEOBSERVER_H__
#define __MS60AUDIOSTREAMSOURCEOBSERVER_H__

// INCLUDES
#include "MS60AudioSourceObserver.h"
#include "S60ClientAudioBuffer.h"

/**
*  This is the S60 Audio Base Player observer class.
*
*  @since 3.1
*/
class MS60AudioStreamSourceObserver : public MS60AudioBaseSourceObserver
	{
	public:
        /**
        * Callback for the App that the Buffer is processed
        * @since Series 60 3.1
        * @param aBuffer The Buffer which was processed
        * @param aStatus Buffer Status after it was processed
        */
		virtual void BufferProcessed(CClientAudioBuffer& aBuffer, TInt aStatus) = 0;
	};

#endif // __MS60AUDIOSTREAMSOURCEOBSERVER_H__

//  End of File