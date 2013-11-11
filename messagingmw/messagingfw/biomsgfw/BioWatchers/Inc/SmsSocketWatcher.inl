// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// SmsWatcher.inl
// 
//

#include <biodb.h>

#ifndef SMSWATCHER_INL_
#define SMSWATCHER_INL_

inline void CBaseSmsActiveSocketWatcher::SetObserver(MBioWatcherObserver* aObserver)
	{
	iObserver = aObserver;
	}

inline TBool CBaseSmsActiveSocketWatcher::CanStoreMessage() const
	{
	return ETrue;
	}

inline TBool CBaseSmsActiveSocketWatcher::Visible() const
	{
	return ETrue;
	}

inline void CBaseSmsActiveSocketWatcher::WatcherComplete(TInt aError)
	{
	if (iObserver)
		iObserver->HandleWatcherComplete(*this, aError);
	}

inline TUid CBaseSmsActiveSocketWatcher::BioMsgUID() const
	{
	return iBioMsgUID;
	}


#endif
