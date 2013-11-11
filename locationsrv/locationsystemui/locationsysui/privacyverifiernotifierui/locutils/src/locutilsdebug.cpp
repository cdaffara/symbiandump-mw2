/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains debugging functions
*
*/



// INCLUDE FILES
#include "locutilsdebug.h"
#include <flogger.h>
#include <e32svr.h>
// CONSTANTS

/// Folder where the log resides
_LIT( KLocLogFolder, "Verifier" );

/// The name of the log file
_LIT( KLocLogFileName, "LocVerifier" );

/// The format in which the time is formatted in log
_LIT( KLocLogTimeFormat, "%02d.%02d:%02d:%06d ");

/// The length of the string produced by KLocLogTimeFormat
const TInt KLocLogTimeFormatLength = 16;

/// How many characters a log line can contain
const TInt KLocLogLineLength = 256;

// ========================== OTHER EXPORTED FUNCTIONS =========================


// -----------------------------------------------------------------------------
// Debug
// Generates a log file if c:\logs\locationsysui\ folder exists
// -----------------------------------------------------------------------------
//
EXPORT_C void Debug( TRefByValue<const TDesC> aText, ... )
    {    
    VA_LIST args;
    VA_START( args, aText );
    
    TBuf<KLocLogLineLength> buf;
    buf.FormatList( aText, args );

    #ifdef _DEBUG
    RDebug::Print(buf);
    #endif

    RFileLogger logger;
    
    TInt ret=logger.Connect();
    if (ret==KErrNone)
        {
        logger.SetDateAndTime( EFalse,EFalse );
        logger.CreateLog( KLocLogFolder, KLocLogFileName, EFileLoggingModeAppend );       
        TBuf<KLocLogTimeFormatLength> timeStamp;
        TTime now;
        now.HomeTime();
        TDateTime dateTime;
        dateTime = now.DateTime();
        timeStamp.Format( KLocLogTimeFormat, 
            dateTime.Hour(), dateTime.Minute(),
            dateTime.Second(), dateTime.MicroSecond() );
        buf.Insert( 0, timeStamp );

        logger.Write(buf);
        }

    logger.Close();

    VA_END( args );
    }

//  End of File  
