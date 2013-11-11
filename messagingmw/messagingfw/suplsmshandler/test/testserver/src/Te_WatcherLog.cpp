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
// A test simulator class for the CTe_WatcherLog class
// 
//

/**
 @file
 @internalComponent
 @prototype
*/

#include "Te_WatcherLog.h"

/**
 * Static factory method for creating an instance of the CTe_WatcherLog class.
 * 
 * @return An instance of the class. The calling application becomes the owner of the returned instance
 * and is responsible its disposal. 
 * 
 * @leave If a error happens, it leaves with KErrNoMemory code.
 */
CTe_WatcherLog* CTe_WatcherLog::NewL()
	{
	return new(ELeave) CTe_WatcherLog();
	}

/**
 * Private default constructor
 */
CTe_WatcherLog::CTe_WatcherLog()
	{
	//Intentionally left blank
	}

/**
 * A stub implementation for the CWatcherLog::Printf method.
 * 
 * @see CWatcherLog::Printf
 */	
void CTe_WatcherLog::Printf(TRefByValue< const TDesC > /*aFmt*/,...)
	{
	//Intentionally left blank
	}

/**
 * A stub implementation for the CWatcherLog::Printf method.
 * 
 * @see CWatcherLog::Printf
 */
void CTe_WatcherLog::Printf(TRefByValue< const TDesC8 > /*aFmt*/,...)
	{
	//Intentionally left blank
	}

/**
 * A stub implementation for the CWatcherLog::IsLogging method.
 * 
 * @return Always returns ETrue.
 * 
 * @see CWatcherLog::IsLogging
 */
TBool CTe_WatcherLog::IsLogging() const
	{
	//Intentionally left blank
	return ETrue;
	}

