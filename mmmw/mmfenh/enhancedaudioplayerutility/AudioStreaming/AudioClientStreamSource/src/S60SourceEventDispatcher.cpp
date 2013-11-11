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
* Description:  Base class definition of the S60 Audio Stream Player aysnc callback
*
*/

#include <e32svr.h>
#include "S60SourceEventDispatcher.h"


CS60SourceEventDispatcher* CS60SourceEventDispatcher::NewL(MS60AudioStreamSourceObserver& aObserver)
	{
	return new(ELeave) CS60SourceEventDispatcher(aObserver);
	}

CS60SourceEventDispatcher::CS60SourceEventDispatcher(MS60AudioStreamSourceObserver& aObserver) :
	CActive(CActive::EPriorityHigh),
	iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CS60SourceEventDispatcher::~CS60SourceEventDispatcher()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60SourceEventDispatcher::~CS60SourceEventDispatcher"));
#endif
	Cancel();
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::SeekSupportChanged
// Seek Support changed Callback from the Source
// -----------------------------------------------------------------------------
//
void CS60SourceEventDispatcher::SeekSupportChanged()
	{
	iSourceEvent = ESeekSupportChanged;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::BufferProcessed
// BufferProcessed Callback from the Source
// -----------------------------------------------------------------------------
//
void CS60SourceEventDispatcher::BufferProcessed(CClientAudioBuffer& aBuffer, TInt aBufferStatus)
	{
	iSourceEvent = EBufferProcessed;
	iBuffer = &aBuffer;
	iBufferStatus = aBufferStatus;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::RunL
// BufferProcessed Callback from the Source
// -----------------------------------------------------------------------------
//
void CS60SourceEventDispatcher::RunL()
	{
	switch (iSourceEvent)
		{
		case ESeekSupportChanged:
			iObserver.SeekSupportChanged();
			break;
		case EBufferProcessed:
			iObserver.BufferProcessed(*iBuffer, iBufferStatus);
			break;

		}
	}

void CS60SourceEventDispatcher::DoCancel()
	{
	// Nothing to cancel
	}
