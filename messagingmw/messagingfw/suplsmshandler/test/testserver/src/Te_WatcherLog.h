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
// A test simulator class for the CWatcherLog class
// 
//

#ifndef __TE_WATCHERLOG_H__
#define __TE_WATCHERLOG_H__

/**
@file
@internalComponent
@prototype
*/

#include <e32base.h>
#include <watcher.h>

class CTe_WatcherLog;

/**
A test simulator class for the TWatcherParams class
*/
class TTe_WatcherParams
	{
public:
	/**
	Constructor. 
	
	@param aLog [In] A reference on the CTe_WatcherLog object
	*/
	inline TTe_WatcherParams(CTe_WatcherLog& aLog) : iLog(aLog) {};
	
	/** A reference on the CTe_WatcherLog object */
	CTe_WatcherLog& iLog;
	};
	
	
/**
A test simulator class for the CWatcherLog class
*/
class CTe_WatcherLog : public CBase
	{
public:
	static CTe_WatcherLog* NewL();
	
	void Printf(TRefByValue< const TDesC > aFmt,...);
	void Printf(TRefByValue< const TDesC8 > aFmt,...);
	TBool IsLogging() const;
	
private:
	CTe_WatcherLog();

	};

#endif //__TE_WATCHERLOG_H__
