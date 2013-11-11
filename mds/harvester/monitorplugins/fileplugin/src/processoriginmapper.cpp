/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Maintains a list of process ids and matching origin*
*/

#include <e32base.h>
#include <f32file.h>

#include "processoriginmapper.h"
#include "harvesterlog.h"

// File header of the location/origin -mapping file.
_LIT8( KFileHeader, "Process UID / Origin -mapping table\n"
                    "\n"
                    "UID         Origin\n" );

// Location of the process/origin -mapping file.
_LIT( KDefaultMappingFile, "C:\\private\\200009F5\\mappings.db" );
_LIT( KDefaultMappingRomFile, "Z:\\private\\200009F5\\mappings.db" );

// Maximum line length read from a file.
// Thus also size of the line buffer and length of first file header line,
// which should be file's longest line.
const TInt KMaxLineLength = 36;

// Length of all file header lines put together.
const TInt KHeaderLength = 57;


//-----------------------------------------------------------------------------
// CProcessOriginMapper::NewL()
//-----------------------------------------------------------------------------
//
CProcessOriginMapper* CProcessOriginMapper::NewL()
    {
    CProcessOriginMapper* self = new (ELeave) CProcessOriginMapper();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
//-----------------------------------------------------------------------------
// CProcessOriginMapper::~CProcessOriginMapper()
// Destructor.
//-----------------------------------------------------------------------------
//
CProcessOriginMapper::~CProcessOriginMapper()
    {
    delete iLineBuffer;
    iLineBuffer = NULL;
    iProcessOriginMap.Close();
    iReadStream.Close();
    iWriteStream.Close();
    iFs.Close();
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::CProcessOriginMapper()
// Constructor.
//-----------------------------------------------------------------------------
//
CProcessOriginMapper::CProcessOriginMapper()
	: iLineBuffer( NULL ), iDefaultFileRead( EFalse )
    {
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::ConstructL()
// Second phase construction called by NewL().
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    iLineBuffer = HBufC8::NewL( KMaxLineLength + 4 );
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::RegisterProcessL()
// Add a process to the list.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::RegisterProcessL( const TUid& aProcessId,
    const TOrigin& aOrigin )
    {
    WRITELOG2( "CProcessOriginMapper::RegisterProcessL - processId: 0x%.8x, origin: %d", aProcessId.iUid, (TInt)aOrigin );
    const TInt index = FindProcess( aProcessId );
    if ( index < 0 ) // not found
        {
        TProcessOriginPair pair = { aProcessId, aOrigin };
        User::LeaveIfError( iProcessOriginMap.InsertInOrder(pair, TLinearOrder<TProcessOriginPair>(CProcessOriginMapper::Compare))); 
        }
    else if ( index < iProcessOriginMap.Count() )
        {
        iProcessOriginMap[index].iOrigin = aOrigin;
        }
    else
        {
        User::Leave( KErrUnknown );
        }
    
    if( !iDefaultFileRead )
    	{
    	WriteFileL();
    	}
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::UnregisterProcessL()
// Remove a process from the list.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::UnregisterProcessL( const TUid& aProcessId )
    {
    WRITELOG1( "CProcessOriginMapper::UnregisterProcessL - processId: 0x%.8x", aProcessId.iUid );
    const TInt index = FindProcess( aProcessId );
    if ( index >= 0 && index < iProcessOriginMap.Count() )
        {
        iProcessOriginMap.Remove( index );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    WriteFileL();
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::OriginL()
// Check a process from the list.
//-----------------------------------------------------------------------------
//
TOrigin CProcessOriginMapper::OriginL( const TUid& aProcessId )
    {
    WRITELOG1( "CProcessOriginMapper::OriginL - search for processId: 0x%.8x", aProcessId.iUid );
    TOrigin origin = MdeConstants::Object::EOther;
    const TInt index = FindProcess( aProcessId );
    if ( index >= 0 && index < iProcessOriginMap.Count() )
        {
        origin = iProcessOriginMap[index].iOrigin;
        }

    WRITELOG2( "CProcessOriginMapper::OriginL - processId: 0x%.8x, origin: %d", aProcessId.iUid, (TInt)origin );
    return origin;
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::ReadFileL()
// Read process id mappings from a file.
// This doesn't clear the list of currently registered mappings.
//-----------------------------------------------------------------------------
//
TInt CProcessOriginMapper::ReadFileL( const TDesC& aFile )
    {
    WRITELOG1( "CProcessOriginMapper::ReadFileL - START reading file %S", &aFile );
    if ( aFile.Length() <= 0 || aFile.Length() > KMaxFileName )
        {
        User::Leave( KErrBadName );
        }

    TInt err = iReadStream.Open( iFs, aFile, EFileRead | EFileStreamText );
    if ( err == KErrNotFound || err == KErrPathNotFound )
        {
        // if mappings file is not found, try to read from rom (Z) drive
        err = iReadStream.Open( iFs, KDefaultMappingRomFile, EFileRead | EFileStreamText );
        }
    User::LeaveIfError( err );

    // read / match file header

    const TChar KLineFeed = '\n';
    TPtr8 ptr( iLineBuffer->Des() );
    iReadStream.ReadL( ptr, KLineFeed );
    ptr.SetLength( KMaxLineLength-1 );
    TBufC8<KHeaderLength> headerBuf( KFileHeader );
    if ( headerBuf.Locate( KLineFeed ) != KMaxLineLength-1 ||
        iLineBuffer->Compare( headerBuf.Left(KMaxLineLength-1) ) != 0 )
        {
        User::Leave( KErrCorrupt );
        }
    iReadStream.ReadL( ptr, KLineFeed );  // skip the next two lines (header stuff)
    iReadStream.ReadL( ptr, KLineFeed );

    TUid processId = { 0 };
    TOrigin origin = MdeConstants::Object::EOther;

    // read and register pairs one by one
    // for-loop limits the maximum amount of loops
    TInt count = 0;
    for ( count = 0; count < KMaxMappingSize; count++ )  
        {
        TRAP( err, ReadProcessOriginPairL( processId, origin ) );
        if ( err == KErrEof )
            {
            // successful exit
            break;
            }
        else if ( err != KErrNone )
            {
            User::Leave( KErrCorrupt );
            }
        RegisterProcessL( processId, origin );
        }

    iReadStream.Release();
    WRITELOG2( "CProcessOriginMapper::ReadFileL - END reading file %S, count %d", &aFile, count );
    return count;
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::ReadFileL()
// Read process id mappings from the default file.
//-----------------------------------------------------------------------------
//
TInt CProcessOriginMapper::ReadFileL()
    {
    WRITELOG( "CProcessOriginMapper::ReadFileL - reading default file" );
    iDefaultFileRead = ETrue;
    const TInt count = ReadFileL( KDefaultMappingFile );
    iDefaultFileRead = EFalse;
    return count;
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::WriteFileL()
// Writes process id mappings to a file.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::WriteFileL( const TDesC& aFile )
    {
    WRITELOG1( "CProcessOriginMapper::WriteFileL - writing file %S", &aFile );
    if ( aFile.Length() <= 0 || aFile.Length() > KMaxFileName )
        {
        User::Leave( KErrBadName );
        }

    TInt err = iWriteStream.Replace( iFs, aFile,
            EFileWrite | EFileShareExclusive | EFileStreamText );
    User::LeaveIfError( err );
    CleanupClosePushL( iWriteStream );
    
    // write file header
    
    iWriteStream.WriteL( KFileHeader );
    
    TUid processId = { 0 };
    TOrigin origin = MdeConstants::Object::EOther;

    // write pairs one by one
    // for-loop limits the maximum amount of loops
    const TInt count = iProcessOriginMap.Count();
    for ( TInt i( 0 ); i < count; i++ )
        {
        processId = iProcessOriginMap[i].iProcessId;
        origin = iProcessOriginMap[i].iOrigin;
        TRAP( err, WriteProcessOriginPairL( processId, origin ) );
        if ( err != KErrNone )
            {
            User::Leave( KErrCorrupt );
            }
        }
    
    iWriteStream.CommitL();
    iWriteStream.Release();

    CleanupStack::PopAndDestroy( &iWriteStream );
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::WriteFileL()
// Write process id mappings to the default file.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::WriteFileL()
    {
    WRITELOG( "CProcessOriginMapper::WriteFileL - writing default file..." );
    WriteFileL( KDefaultMappingFile );
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::Clear()
// Clear mapping table in memory.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::Clear()
    {
    WRITELOG( "CProcessOriginMapper::Clear - clearing origin map db" );
    iProcessOriginMap.Reset();
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::Count()
// Return the count of currently registered mappings.
//-----------------------------------------------------------------------------
//
TInt CProcessOriginMapper::Count()
    {
    return iProcessOriginMap.Count();
    }


// PRIVATE METHODS

//-----------------------------------------------------------------------------
// CProcessOriginMapper::FindProcess()
// Find index by process id.
//-----------------------------------------------------------------------------
//
TInt CProcessOriginMapper::FindProcess( const TUid& aProcessId )
    {
    TInt low( 0 );
    TInt high( iProcessOriginMap.Count() );
    
    while( low < high )
        {
        TInt mid( (low+high)>>1 );
        
        const TInt compare( aProcessId.iUid - iProcessOriginMap[mid].iProcessId.iUid );
        if( compare == 0 )
            {
            return mid;
            }
        else if( compare > 0 )
            {
            low = mid + 1;
            }
        else
            {
            high = mid;
            }
        }

    return KErrNotFound;
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::ReadProcessOriginPairL()
// Read one related process/origin pair from a text file.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::ReadProcessOriginPairL( TUid& aProcessId,
    TOrigin& aOrigin )
    {    
    const TUint32 KMinTestRangeId = 0xE0000000;
    const TUint32 KMaxTestRangeId = 0xEFFFFFFF; 
    
    const TUint32 KMaxProcessId = 0xFFFFFFFF;
    const TChar KLineFeed = '\n';

    // read a line

    TPtr8 ptr( iLineBuffer->Des() );
    iReadStream.ReadL( ptr, KLineFeed );

    TUint32 tempId = 0;
    TInt16 tempOrigin = 0;
    
    // parse process id

    TLex8 parser( ptr );
    TInt err = parser.Val( tempId, EHex );
    if ( err == KErrGeneral )
        {
        User::Leave( KErrEof );
        }
    else if ( err != KErrNone ) 
        {
        User::Leave( err ); // might be KErrOverflow
        }

    // parse origin

    parser.SkipSpace();
    err = parser.Val( tempOrigin );
    if ( err == KErrGeneral )
        {
        User::Leave( KErrCorrupt );
        }
    else if ( err != KErrNone ) 
        {
        User::Leave( err ); // might be KErrOverflow
        }

    if ( tempOrigin < 0 )
        {
        User::Leave( KErrCorrupt );
        }
    if ( tempId >= KMinTestRangeId && 
         tempId <= KMaxTestRangeId )
        {
        User::Leave( KErrCorrupt );
        }
    else if( tempId > KMaxProcessId )
        {
        User::Leave( KErrCorrupt );
        }

    // set results
    
    aProcessId = TUid::Uid( tempId );
    aOrigin = TOrigin( tempOrigin );
    }

//-----------------------------------------------------------------------------
// CProcessOriginMapper::WriteProcessOriginPairL()
// Write one related process/origin pair to a file.
//-----------------------------------------------------------------------------
//
void CProcessOriginMapper::WriteProcessOriginPairL( TUid& aProcessId,
    TOrigin& aOrigin )
    {
    _LIT8( KDelimiter, "\t" );
    _LIT8( KLineFeed, "\n" );
    TPtr8 ptr( iLineBuffer->Des() );
    ptr.Num( aProcessId.iUid );
    iWriteStream.WriteL( ptr );
    iWriteStream.WriteL( KDelimiter );
    ptr.Num( aOrigin );
    iWriteStream.WriteL( ptr );
    iWriteStream.WriteL( KLineFeed );
    }

TInt CProcessOriginMapper::Compare(const TProcessOriginPair& aFirst, const TProcessOriginPair& aSecond)
    {
    return aFirst.iProcessId.iUid - aSecond.iProcessId.iUid;
    }

