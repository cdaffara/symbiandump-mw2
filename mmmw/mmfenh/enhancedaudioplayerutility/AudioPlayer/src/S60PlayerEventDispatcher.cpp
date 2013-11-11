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
* Description:  S60 Audio Event Monitor implementation
*
*/



#include "S60AudioPlayer.h"
#include "S60PlayerEventDispatcher.h"


CPlayerEventDispatcher::~CPlayerEventDispatcher()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CPlayerEventDispatcher::~CPlayerEventDispatcher"));
#endif
	}

CPlayerEventDispatcher::CPlayerEventDispatcher(
	MS60AudioPlayerObserver& aObserver ) :
	CActive(CActive::EPriorityStandard),
	iObserver(&aObserver)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CPlayerEventDispatcher::CPlayerEventDispatcher"));
#endif
	CActiveScheduler::Add(this);
	}

CPlayerEventDispatcher* CPlayerEventDispatcher::NewL(MS60AudioPlayerObserver& aObserver)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CPlayerEventDispatcher::NewL"));
#endif

	CPlayerEventDispatcher* self = new(ELeave) CPlayerEventDispatcher(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPlayerEventDispatcher::ConstructL()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CPlayerEventDispatcher::ConstructL"));
#endif
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::AddSourceComplete
// AddSource complete Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//

void CPlayerEventDispatcher::AddSourceComplete(TInt aError)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CPlayerEventDispatcher::AddSourceComplete"));
#endif
	iError = aError;
	iPlayerEvent = EAddSourceComplete;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::StateChanged
// State changed Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//

void CPlayerEventDispatcher::StateChanged(
	TPlayerState aState,
	TInt aError )
	{
	iPlayerState = aState;
	iPlayerEvent = EStateChanged;
	iError = aError;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, iError);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::FastForwardSupportChanged
// FastForward Support changed Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//

void CPlayerEventDispatcher::FastForwardSupportChanged()
	{
	iPlayerEvent = EFastForwardSupportChanged;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::RewindSupportChanged
// Rewind Support changed Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//

void CPlayerEventDispatcher::RewindSupportChanged()
	{
	iPlayerEvent = ERewindSupportChanged;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::DurationChanged
// Duration changed Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//
void CPlayerEventDispatcher::DurationChanged(const TTimeIntervalMicroSeconds& aDuration)
	{
	iDuration = aDuration;
	iPlayerEvent = EDurationChanged;
	if (!IsActive())
		{
		TRequestStatus* s = &iStatus;
		SetActive();
		User::RequestComplete(s, KErrNone);
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::RunL
// RunL for the PlayerEvent Dispatcher
// -----------------------------------------------------------------------------
//

void CPlayerEventDispatcher::RunL()
	{
	switch (iPlayerEvent)
		{
		case EAddSourceComplete:
			iObserver->AddSourceComplete(iError);
			break;
		case EStateChanged:
			iObserver->StateChanged(iPlayerState, iError);
			break;
		case ERewindSupportChanged:
			iObserver->RewindSupportChanged();
			break;
		case EFastForwardSupportChanged:
			iObserver->FastForwardSupportChanged();
			break;
		case EDurationChanged:
			iObserver->DurationChanged(iDuration);
			break;
		}
	}

// -----------------------------------------------------------------------------
// CPlayerEventDispatcher::DoCancel
// Duration changed Callback from the AudioPlayer
// -----------------------------------------------------------------------------
//
void CPlayerEventDispatcher::DoCancel()
	{
	// Nothing to cancel
	}




