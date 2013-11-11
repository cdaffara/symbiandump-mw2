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
* Description:  Definition of the S60 Audio Stream Source event dispatcher
*
*/

#ifndef __CS60SOURCEEVENTDISPATCHER_H__
#define __CS60SOURCEEVENTDISPATCHER_H__

#include <e32base.h>
#include <e32std.h>
#include "MS60AudioStreamSourceObserver.h"

/**
Active object utility class to allow the callback to be called asynchronously.
This should help prevent re-entrant code in clients of the mediaframework.
*/
class CS60SourceEventDispatcher : public CActive
	{

	enum TSourceEvent
		{
		ESeekSupportChanged,
		EBufferProcessed
		};

	public:

		static CS60SourceEventDispatcher* NewL(MS60AudioStreamSourceObserver& aObserver);
		virtual ~CS60SourceEventDispatcher();

		void SeekSupportChanged();
		void BufferProcessed(CClientAudioBuffer& aBuffer, TInt aStatus);

	private:

		CS60SourceEventDispatcher(MS60AudioStreamSourceObserver& aObserver);

		void RunL();
		void DoCancel();

	private:
		MS60AudioStreamSourceObserver& iObserver;
		TSourceEvent iSourceEvent;
		CClientAudioBuffer* iBuffer;
		TInt iBufferStatus;

	};

#endif // __CS60SOURCEEVENTDISPATCHER_H__

//  End of File
