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
* Description:  
*     Provides functionality to write trace output to debug output (serial
*     port in ARMI case) or predefined file.
*
*/



// INCLUDE FILES
#include   <bautils.h>
#include   "epos_csupltrace.h"

// CONSTANTS
#ifdef _DEBUG
_LIT(KPosTraceLogFile, "epostracelog.txt");
_LIT(KPosTraceLogDir, "epos"); // Relative to C:\Logs
const TInt KMaxTraceLength = 1024;
#endif

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

CSuplTrace::CSuplTrace()
    {
    }

// Two-phased constructor.
EXPORT_C CSuplTrace* CSuplTrace::NewL()
    {
    CSuplTrace* self = new (ELeave) CSuplTrace;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CSuplTrace* CSuplTrace::NewL(
    const TDesC& aTraceLogDir,
    const TDesC& aTraceLogFile)
    {
    CSuplTrace* self = new (ELeave) CSuplTrace;
    CleanupStack::PushL(self);
    self->ConstructL(aTraceLogDir, aTraceLogFile);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CSuplTrace::~CSuplTrace()
    {
#ifdef _DEBUG
    delete iTraceDir;
    delete iTraceFile;
#endif
    }

// EPOC default constructor can leave.
void CSuplTrace::ConstructL()
    {
#ifdef _DEBUG
    iTraceDir = KPosTraceLogDir().AllocL();
    iTraceFile = KPosTraceLogFile().AllocL();
#endif
    }


#ifdef _DEBUG

void CSuplTrace::ConstructL(
    const TDesC& aTraceLogDir,
    const TDesC& aTraceLogFile)
    {
    iTraceDir = aTraceLogDir.AllocL();
    iTraceFile = aTraceLogFile.AllocL();
    }

#else

void CSuplTrace::ConstructL(
    const TDesC& /*aTraceLogDir*/,
    const TDesC& /*aTraceLogFile*/)
    {
    }

#endif

// ---------------------------------------------------------
// CSuplTrace::TraceL
// (other items were commented in a header).
// ---------------------------------------------------------
//

#ifdef _DEBUG

EXPORT_C void CSuplTrace::TraceL(
    const TDesC& aDescription,
    const TDesC& aFilename,
    const TInt aLineNumber)
    {
    _LIT(KPosTraceLogFormatNormal , "[EPos/0x%LX]: %S (%d): %S");
    const TInt KPosTraceLogFormatNormalExtraChars = 40;

    RProcess process;

    HBufC* buf = HBufC::NewL(
        aDescription.Length() +
        aFilename.Length() +
        KPosTraceLogFormatNormalExtraChars);

    buf->Des().Format(KPosTraceLogFormatNormal,
        process.Id().Id(),
        &aFilename,
        aLineNumber,
        &aDescription);
    
    RFileLogger::Write( *iTraceDir, *iTraceFile, EFileLoggingModeAppend, *buf);
    RDebug::RawPrint(*buf);
    
    delete buf;
    }

EXPORT_C void CSuplTrace::Log( 
    const TDesC& aTraceLogDir,
    const TDesC& aTraceLogFile,
    TRefByValue<const TDesC> aFmt, ... )
    {
    //coverity[var_decl]
    VA_LIST list;
    VA_START( list, aFmt );

    RProcess process;
    HBufC* buf = HBufC::New( KMaxTraceLength );
    if ( buf )
    	{
    	TPtr ptr( buf->Des() );
        _LIT( KPrefix, "[EPos/0x%LX]: ");
        ptr.Format( KPrefix, process.Id().Id() );
        //coverity[uninit_use_in_call]
	    ptr.AppendFormatList( aFmt, list );
	
	    RDebug::RawPrint( ptr );
	    RFileLogger::Write( aTraceLogDir, aTraceLogFile, EFileLoggingModeAppend, ptr );

	    delete buf;
    	}
    }

#else

EXPORT_C void CSuplTrace::TraceL(
    const TDesC& /*aDescription*/,
    const TDesC& /*aFilename*/,
    const TInt /*aLineNumber*/)
    {
    }

EXPORT_C void CSuplTrace::Log( 
    const TDesC& /*aTraceLogDir*/,
    const TDesC& /*aTraceLogFile*/,
    TRefByValue<const TDesC> /*aFmt*/, ... )
    {
    }

#endif

//  End of File
