/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Debug macros
*
*/


#ifndef MNRP_DEBUG_H
#define MNRP_DEBUG_H

#include <e32base.h>

#ifdef _DEBUG

#include <e32svr.h>
#include <e32std.h>
#include <f32file.h>
#include <flogger.h>

_LIT(KLogFile, "mnlog.txt");
_LIT(KLogDirFullName, "c:\\logs\\");
_LIT(KLogDir, "mn");

inline void Prefix( TDes& aMessage )
    {
    RProcess process;
    _LIT( KPrefix, "[MnRefProvider/%ld]: ");
    aMessage.Format( KPrefix, process.Id().Id() );
    }

inline void Log( TRefByValue<const TDesC> aFmt, ... )
    {
    VA_LIST list;
    VA_START( list, aFmt );

    HBufC* buf = HBufC::New(512);
    if ( buf )
    	{
    	TPtr ptr( buf->Des() );
	    Prefix( ptr );
	    ptr.AppendFormatList( aFmt, list );

	    RDebug::RawPrint( ptr );
	    RFileLogger::Write( KLogDir, KLogFile, EFileLoggingModeAppend, ptr );

	    delete buf;
    	}
    }

#define LOG( a )             { Log( _L( a ) ); }
#define LOG1( a, b )         { Log( _L( a ), b ); }
#define LOG2( a, b, c )      { Log( _L( a ), b, c ); }
#define LOG3( a, b, c, d )   { Log( _L( a ), b, c, d ); }
#define LOG4( a, b, c, d, e )   { Log( _L( a ), b, c, d, e ); }

#else // _DEBUG

// Release version - no logging
#define LOG( a)
#define LOG1( a, b )
#define LOG2( a, b, c )
#define LOG3( a, b, c, d )
#define LOG4( a, b, c, d, e )

#endif // _DEBUG

inline void Panic( TInt aReason )
    {
    _LIT( KPanicCategory, "MnRefProvider" );
    LOG1("Panicking: %1", aReason);
    User::Panic( KPanicCategory, aReason );
    }

#endif // MNRP_DEBUG_H

