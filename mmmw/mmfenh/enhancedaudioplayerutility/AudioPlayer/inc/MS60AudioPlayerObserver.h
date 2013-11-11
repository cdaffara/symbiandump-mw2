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


#ifndef __MS60AUDIOPLAYEROBSERVER_H__
#define __MS60AUDIOPLAYEROBSERVER_H__

// INCLUDES
#include <e32std.h>
#include <e32base.h>

enum TPlayerState
	{
	EPlayerIdle,
	EPlayerPlaying,
	EPlayerPaused,
	EPlayerStopped,
	EPlayerFastForwarding,
	EPlayerRewinding
	};

// CLASS DECLARATION

/**
*  This is the S60 Audio Player observer class.
*  @since 3.1
*/
class MS60AudioPlayerObserver
	{
	public:
        /**
        * Callback for the App that AddSource is complete
        * @since Series 60 3.1
        * @param aError One of the Standard Error Codes
        */
		virtual void AddSourceComplete(TInt aError) = 0;

        /**
        * Callback for the App that the Player State Changed
        * @since Series 60 3.1
        * @param aState Resulting State after the change
        * @param aError One of the Standard Error Codes
        */
		virtual void StateChanged(TPlayerState aState, TInt aError) = 0;

        /**
        * Used to Indiacate the FastForward Support change
        * @since Series 60 3.1
        */
		virtual void FastForwardSupportChanged() = 0;

        /**
        * Used to Indiacate the Rewind Support change
        * @since Series 60 3.1
        */
		virtual void RewindSupportChanged() = 0;

        /**
        * Callback for the App that the Duration changed
        * @since Series 60 3.1
        * @param aDuration The New Duration After the Change
        */
		virtual void DurationChanged(const TTimeIntervalMicroSeconds& aDuration) = 0;
	};

#endif // __MS60AUDIOPLAYEROBSERVER_H__

//  End of File