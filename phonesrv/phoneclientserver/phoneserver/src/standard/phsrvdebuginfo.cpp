/*
* Copyright (c) 2002-2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Debug information implementation.
*
*/


// INCLUDE FILES

#include    <e32std.h>                  // tls
#include    <e32svr.h>                  // rdebug
#include    <f32file.h>                 // file and FS
#include    <utf.h>                     // converter
#include    "phsrvdebuginfo.h" // this 
#include    "kphsrvconfigure.h" // defines 


// Initialize
#ifdef __PHSRV_DEBUG_INFO__

// CONSTANTS
// max length with number and extension is 25!
_LIT( KPhSrvDebugFileName, "C:\\PhSrvDebug");
_LIT( KPhSrvDebugFileExt, ".txt" );
const TInt KPhSrvDebugFileNameLength( 25 );

#ifdef __PHSRV_DEBUG_WRITE_LOG__
_LIT( KPhSrvDebugEOF, "\n" );
#endif // _PHSRV_DEBUG_WRITE_LOG__




//----------------------------------------------------------------------------
// PhSrvDebugInfo::Print
//----------------------------------------------------------------------------
//
void PhSrvDebugInfo::Print(TInt/* aArea*/,const TDesC& aText )
{
    


            // LOG TO FILE
#ifdef __PHSRV_DEBUG_WRITE_LOG__

            TBuf8<128> toFile;
            CnvUtfConverter::ConvertFromUnicodeToUtf8( toFile, aText.Left(126) );
            toFile.Append( KPhSrvDebugEOF );
            
            PhSrvDebugInfo::CPhSrvDebugData* data = NULL;
    if( !data )
        {
        data = CreateData();
        }
    if( data )
        {
        data->CreateFile();           
        data->WriteFile( toFile );
        }
#endif  // __PHSRV_DEBUG_WRITE_LOG__
#ifdef __PHSRV_PRINT_DEBUG_INFO__
            RDebug::Print( aText );
#endif // __PHSRV_PRINT_DEBUG_INFO__
          //  }   
      //  }
    }


//----------------------------------------------------------------------------
// PhSrvDebugInfo::CreateDataL
//----------------------------------------------------------------------------
//
PhSrvDebugInfo::CPhSrvDebugData* PhSrvDebugInfo::CreateData()
    {
    PhSrvDebugInfo::CPhSrvDebugData* data = 
        new PhSrvDebugInfo::CPhSrvDebugData();

    return data;
    }


//----------------------------------------------------------------------------
// PhSrvDebugInfo::CPhSrvDebugData::~CPhSrvDebugData
//----------------------------------------------------------------------------
//
PhSrvDebugInfo::CPhSrvDebugData::~CPhSrvDebugData()
    {
    ReleaseFile();
    }


//----------------------------------------------------------------------------
// PhSrvDebugInfo::CPhSrvDebugData::CreateFileL
//----------------------------------------------------------------------------
//
void PhSrvDebugInfo::CPhSrvDebugData::CreateFile()
    {
    if ( !IsFile() )
        {
        if ( !iFs ) 
            iFs = new RFs();
        if ( !iFs )
            return;
        if ( iFs->Connect() != KErrNone )
            return;

        TBuf<KPhSrvDebugFileNameLength> name;
        TInt err = KErrGeneral;
        TInt i = 0;
            
        if ( !iFile )
            {
            name.Zero();
            name.Append( KPhSrvDebugFileName );
            name.AppendNum( i );
            name.Append( KPhSrvDebugFileExt );
            
            iFile = new RFile();
            if ( !iFile )
                return;
            err = iFile->Create( *iFs, name, 
                EFileStreamText|EFileWrite|EFileShareAny );
            }

       if( err != KErrNone )
            {
            err = iFile->Open( *iFs, name, 
            EFileStreamText|EFileWrite|EFileShareAny );
            }
                    
        }
    }


void PhSrvDebugInfo::CPhSrvDebugData::ReleaseFile()
    {
    if ( iFile )
        {
        iFile->Close();
        delete iFile;
        iFile = NULL;
        }
    if ( iFs )
        {
        iFs->Close();
        delete iFs;
        iFs = NULL;
        }
    }


TBool PhSrvDebugInfo::CPhSrvDebugData::IsFile() const
    {
    return ( iFile != NULL );
    }


void PhSrvDebugInfo::CPhSrvDebugData::WriteFile(const TDesC8& aDes)
    {
    if ( IsFile() )
        {
        TInt place =0 ;
        iFile->Seek( ESeekEnd,place );
        iFile->Write( place, aDes );
        iFile->Flush();
        }
    }


#endif



//  End of File
