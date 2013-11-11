/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the S60 Audio Player event monitor
*
*/

#ifndef __CPLAYEREVENTDISPATCHER_H__
#define __CPLAYEREVENTDISPATCHER_H__

#include <e32base.h>
#include <e32std.h>

#include "MS60AudioPlayerObserver.h"
#include "S60AudioPlayer.h"


/**
Active object utility class to allow the callback to be called asynchronously.
This should help prevent re-entrant code in clients of the mediaframework.
*/
class CPlayerEventDispatcher : public CActive

	{

	enum TPlayerEvent
		{
		EAddSourceComplete = 1,
		EStateChanged,
		EFastForwardSupportChanged,
		ERewindSupportChanged,
		EDurationChanged,
		};

	public:

        /**
        * Two-phased constructor.
        */
		static CPlayerEventDispatcher* NewL(MS60AudioPlayerObserver& aObserver);
		/**
        *	Destructor
        */
		virtual ~CPlayerEventDispatcher();

        /**
        * Callback for the App that AddSource is complete
        * @since Series 60 3.1
        * @param aError One of the Standard Error Codes
        */
		void AddSourceComplete(TInt aError);

        /**
        * Callback for the App that the Player State Changed
        * @since Series 60 3.1
        * @param aState Resulting State after the change
        * @param aError One of the Standard Error Codes
        */
		void StateChanged(TPlayerState aState, TInt aError);

        /**
        * Used to Indiacate the FastForward Support change
        * @since Series 60 3.1
        */
		void FastForwardSupportChanged();

        /**
        * Used to Indiacate the Rewind Support change
        * @since Series 60 3.1
        */
		void RewindSupportChanged();

        /**
        * Callback for the App that the Duration changed
        * @since Series 60 3.1
        * @param aDuration The New Duration After the Change
        */
		void DurationChanged(const TTimeIntervalMicroSeconds& aDuration);

	private:
		CPlayerEventDispatcher(MS60AudioPlayerObserver& aObserver);
		void ConstructL();
		void RunL();
		void DoCancel();

	private:

		MS60AudioPlayerObserver* iObserver;
		TInt iError;
		TTimeIntervalMicroSeconds iDuration;
		TPlayerEvent iPlayerEvent;
		TPlayerState iPlayerState;
	};

#endif //__CPLAYEREVENTDISPATCHER_H__

//  End of File
