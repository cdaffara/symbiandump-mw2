// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifdef _DEBUG
#undef _MSG_NO_LOGGING
#endif


#include <e32cons.h>
#include <flogger.h>
#include <e32svr.h>
#include <watcher.h>

#ifndef  _MSG_NO_LOGGING
_LIT(KWatcherLogDir, "watcher");
_LIT(KWatcherLogDirFull, "c:\\logs\\watcher");
_LIT(KWatcherLogDirConsole, "c:\\logs\\watcher\\cons");
_LIT(KWatcherLogFile, "watcher.txt");
_LIT(KWatcherConsoleTitle, "Messaging Watchers");
#endif

CWatcherLog* CWatcherLog::NewL(RFs& aFs)
	{
	CWatcherLog* self = new (ELeave) CWatcherLog(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CWatcherLog::CWatcherLog(RFs& aFs)
: CBase(), iFs(aFs)
	{
	}

CWatcherLog::~CWatcherLog()
	{
	delete iConsole;
	}

void CWatcherLog::ConstructL()
	{
#ifndef  _MSG_NO_LOGGING
	if (IsConsole())
		iConsole = Console::NewL(KWatcherConsoleTitle, TSize(KConsFullScreen, KConsFullScreen));
#endif
	}

#ifndef  _MSG_NO_LOGGING
EXPORT_C void CWatcherLog::Printf(TRefByValue<const TDesC> aFmt,...)
#else
EXPORT_C void CWatcherLog::Printf(TRefByValue<const TDesC> /*aFmt*/,...)
#endif
	{
#ifndef  _MSG_NO_LOGGING
	VA_LIST list;
	VA_START(list, aFmt);

	// Print to log file
	TBuf<KWatcherLogBuffer> buf;
	buf.FormatList(aFmt, list);

	// Write to log file
	RFileLogger::Write(KWatcherLogDir, KWatcherLogFile, EFileLoggingModeAppend, buf);

	// Write to console
	if (iConsole)
		{
		buf.Append('\n');
		iConsole->Printf(KWatcherStringFormat, &buf);
		}
#endif
	}

#ifndef  _MSG_NO_LOGGING
EXPORT_C void CWatcherLog::Printf(TRefByValue<const TDesC8> aFmt,...)
#else
EXPORT_C void CWatcherLog::Printf(TRefByValue<const TDesC8> /*aFmt*/,...)
#endif
	{
#ifndef  _MSG_NO_LOGGING
	VA_LIST list;
	VA_START(list, aFmt);
	
	// Print to log file
	TBuf8<KWatcherLogBuffer> buf;
	buf.FormatList(aFmt, list);
	
	// Write to log file
	RFileLogger::Write(KWatcherLogDir, KWatcherLogFile, EFileLoggingModeAppend, buf);
	
	// Write to console
	if (iConsole)
		{
		buf.Append('\n');
		iConsole->Printf(KWatcherStringFormat, &buf);
		}
#endif
	}

EXPORT_C TBool CWatcherLog::IsLogging() const
	{
#ifndef  _MSG_NO_LOGGING
	TUint att;
	return (iFs.Att(KWatcherLogDirFull, att) == KErrNone);
#else
	return EFalse;
#endif
	}


TBool CWatcherLog::IsConsole() const
	{
#ifndef  _MSG_NO_LOGGING
	TUint att;
	return (iFs.Att(KWatcherLogDirConsole, att) == KErrNone);
#else
	return EFalse;
#endif
	}
