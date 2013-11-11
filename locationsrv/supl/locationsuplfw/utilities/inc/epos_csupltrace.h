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



#ifndef CPOSTRACE_H
#define CPOSTRACE_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <bautils.h>

#if defined _DEBUG
#include <flogger.h>
#endif

// CONSTANTS
// Default filename that is traced
_LIT(KPosTraceDefaultTracedFile, "<NO FILE>");

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*     Provides functionality to write trace output to debug output (serial
*     port in ARMI case) or predefined file.
*/
class CSuplTrace :public CBase
    {

    private:  // Constructors and destructor

        /**
        * C++ default constructor.
        */
        CSuplTrace();

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CSuplTrace* NewL();

        /**
        * Two-phased constructor.
        * Takes parameters for Log directory and file.
        * The Log directory is relative to C:\Logs
        */
        IMPORT_C static CSuplTrace* NewL(const TDesC& aTraceLogDir,
                                        const TDesC& aTraceLogFile);

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CSuplTrace();

    private:  // Constructors and destructor

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL(const TDesC& aTraceLogDir,
                        const TDesC& aTraceLogFile);

        // By default, prohibit copy constructor
        CSuplTrace( const CSuplTrace& );
        // Prohibit assigment operator
        CSuplTrace& operator= ( const CSuplTrace& );

    public: // New functions

        /**
        * Specify a trace
        * @param[in] aDescription Description of the trace
        * @param[in] aFilename may NOT specify __FILE__
        * @param[in] aLineNumber may specify __LINE__
        */
        IMPORT_C void TraceL(
            const TDesC& aDescription,
            const TDesC& aFilename = KPosTraceDefaultTracedFile,
            const TInt aLineNumber = 0 );

        /**
        * Static trace
        * @param[in] aTraceLogDir Trace log folder under c:\logs
        * @param[in] aTraceLogFile Trace log file under aTraceLogDir
        * @param[in] aFmt Format of the trace string followed by the 
        *                 list of parameters.
        */
        IMPORT_C static void Log( 
            const TDesC& aTraceLogDir,
            const TDesC& aTraceLogFile,
            TRefByValue<const TDesC> aFmt, ... );

    private:    // Data

#if defined _DEBUG
        RFileLogger  iLogger;
        
        HBufC*  iTraceDir;
        HBufC*  iTraceFile;
#endif

    };

#ifdef _DEBUG

#define LOG( a )             { CSuplTrace::Log( KTraceDir, KTraceFile, _L( a ) ); }
#define LOG1( a, b )         { CSuplTrace::Log( KTraceDir, KTraceFile, _L( a ), b ); }
#define LOG2( a, b, c )      { CSuplTrace::Log( KTraceDir, KTraceFile, _L( a ), b, c ); }
#define LOG3( a, b, c, d )   { CSuplTrace::Log( KTraceDir, KTraceFile, _L( a ), b, c, d ); }
#define LOG4( a, b, c, d, e )   { CSuplTrace::Log( KTraceDir, KTraceFile, _L( a ), b, c, d, e ); }

#else // _DEBUG

// Release version - no logging
#define LOG( a )
#define LOG1( a, b )
#define LOG2( a, b, c )
#define LOG3( a, b, c, d )
#define LOG4( a, b, c, d, e )

#endif      // _DEBUG

#endif      // CPOSTRACE_H

// End of File
