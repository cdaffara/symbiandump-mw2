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
* Description:    DEBUG utilities, for DEBUG version only
*
*/




// INCLUDE FILES
#include <e32std.h>
#include <e32base.h>

#ifdef _DEBUG
#include <flogger.h>
#include <e32debug.h>
#include <apparc.h>
#include <bautils.h>
#endif  // _DEBUG

#include "simpledebugutils.h"

// ================= MEMBER FUNCTIONS =======================

#ifdef _DEBUG

//**********************************
// TSimpleLogger
//**********************************
const TInt KLogBufferLength = 256;

// ---------------------------------------------------------
// TSimpleLogger::Log
// ---------------------------------------------------------
//
void TSimpleLogger::Log(TRefByValue<const TDesC> aFmt,...) //lint !e960
    {
    VA_LIST list;
    VA_START(list, aFmt); //lint !e960

    // Print to log file
    TBuf<KLogBufferLength> buf;
    buf.FormatList(aFmt, list);

    _LIT(KLogDir, "simple");
    _LIT(KLogFile, "simple.txt");
    // Write to log file
    RFileLogger::Write(KLogDir, KLogFile, EFileLoggingModeAppend, buf);
    RDebug::RawPrint( buf );

    }

// ---------------------------------------------------------
// TSimpleLogger::Dump
// ---------------------------------------------------------
//
void TSimpleLogger::Dump( const TDesC8& aData, RFs& aFs, TInt aMode )
    {
    if ( !aData.Length() )
        {
        return;
        }

    _LIT(KLogDir, "c:\\logs\\simple\\");

    TParse myParse;

    // generate unique file name
    TFileName fileName;
    fileName = KLogDir;
    if( BaflUtils::PathExists( aFs, fileName ) )
        {
        _LIT( KRelatedRcv, "Receive.xml");
        _LIT( KRelatedSnt, "Sent.xml");
        if ( aMode )
            {
            myParse.Set( fileName, &KRelatedRcv, NULL );
            }
        else
            {
            myParse.Set( fileName, &KRelatedSnt, NULL );
            }
        fileName = myParse.FullName();
        if ( CApaApplication::GenerateFileName( aFs, fileName ))
            {
            return;
            }

        // write data into file
        RFile file;
        TInt xmlError = file.Create( aFs, fileName, EFileWrite | EFileShareExclusive );
        if ( xmlError == KErrNone )
            {
            file.Write( aData );
            file.Flush();
            }

        // done - close file
        file.Close();

        Log( _L("Dump: LogFile = %S"), &fileName );
        }
    }
#endif  // _DEBUG


//**********************************
// CSimpleSettingFile
//**********************************

CSimpleSettingFile::~CSimpleSettingFile()
    {
    if ( iOpen )
        {
        iReader.Close();
        }
    iOpen = EFalse;
    }

CSimpleSettingFile::CSimpleSettingFile( RFs& aFs)
: iFs( aFs ),
  iOpen( EFalse )
    {
    }
    
#ifdef _DEBUG

CSimpleSettingFile* CSimpleSettingFile::NewL(
    RFs& aFs )
    {
    CSimpleSettingFile* self = new (ELeave) CSimpleSettingFile( aFs );
    return self;
    }

void CSimpleSettingFile::OpenL(TDesC& aResFile)
    {
    // open a file
    iFileName = aResFile;

    TInt myError = iReader.Open( iFs,
                             iFileName,
                             EFileShareReadersOnly );

    User::LeaveIfError( myError );
    iOpen = ETrue;
    }

TPtrC8 CSimpleSettingFile::KeyValueL( const TDesC8& aKey )
    {

    TPtrC8 myKey;
    TPtrC8 myValue;
    TBool getIt(EFalse);
    TInt err = 0;

    // Reset the reader
    OpenL( iFileName );

    // Start to search
    while ( !getIt)
        {
        TRAP ( err, ReadRowL( myKey, myValue ));
        if ( err != KErrNone )
            {
            User::Leave( KErrNotFound );
            }
        if ( !myKey.CompareF( aKey ))
            {
            return myValue;
            }
        }
    return TPtrC8();
    }

void CSimpleSettingFile::ReadRowL( TPtrC8& aKey, TPtrC8& aValue )
    {
    // READ ONE ROW
    TChar delim( 10 );
    iReader.ReadL( iRowBuffer, delim);
    TInt length = iRowBuffer.Length();
    if ( length > 2 )
        {
        // DROP CR+LF FROM THE END OF LINE
        iRowBuffer.Delete( length - 2, 2 );

        TInt pos = 0;
        pos = iRowBuffer.Find( _L8("=") );
        if ( pos > 0 )
            {
            aKey.Set( iRowBuffer.Left( pos ) );
            // Cut off separator
            aValue.Set( iRowBuffer.Mid( pos + 1 ) );
            }
        }
    } 
#endif  // _DEBUG   

//  End of File

