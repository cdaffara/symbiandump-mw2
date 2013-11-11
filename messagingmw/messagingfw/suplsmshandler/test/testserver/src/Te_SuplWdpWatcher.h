// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// A test wrapper over the CSuplWdpWatcher class.
// 
//

#if (!defined __TE_LBSSUPLWDPWATCHER__)
#define __TE_LBSSUPLWDPWATCHER__

#include "LbsSuplWdpWatcher.h"

/**
A test wrapper over the CSuplWdpWatcher class.

@see CSuplWdpWatcher
*/
class CTe_LbsSuplWdpWatcher : public CLbsSuplWdpWatcher
	{
private:
	/** The flag for enabling/disabling asserts for the CLbsSuplWdpWatcher class */
	static TBool iEnableAsserts;
	
public:
	inline static TBool AssertsEnabled();
	inline static void EnableAsserts();
	inline static void DisableAsserts();
	
private:
	//This constructor is only for avoiding gcce error
	CTe_LbsSuplWdpWatcher(TInt aPriority, CWatcherLog& aLog) : 
		CLbsSuplWdpWatcher(aPriority, aLog){};
	
	};
	
	
inline TBool CTe_LbsSuplWdpWatcher::AssertsEnabled()	
	{
	return iEnableAsserts;
	}

inline void CTe_LbsSuplWdpWatcher::EnableAsserts()
	{
	iEnableAsserts = ETrue;
	}

inline void CTe_LbsSuplWdpWatcher::DisableAsserts()
	{
	iEnableAsserts = EFalse;
	}

#endif //__TE_LBSSUPLWDPWATCHER__
