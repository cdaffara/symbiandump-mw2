/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/


#include <e32base.h>
#include <bautils.h>
#include <syslangutil.h>
#include <PathInfo.h>
#include <data_caging_path_literals.hrh>
#include <mdsplaylisttopcharacterset.rsg>

#include "harvesterm3uplaylistparser.h"

#include "mdsutils.h"
#include "harvesterlog.h"

_LIT( KMDSM3ULineChange, "\n" );
_LIT( KMDSM3UTagExtm3u, "#EXTM3U" );
_LIT (KMDSM3UTagExtinf, "#EXTINF:" );
_LIT( KMDSM3UPoint, ",");
_LIT( KMDSM3UTagExt, "#");
_LIT( KMDSM3UAbsPath, ":\\");

const TInt KMDSM3UCarriageReturn = 13;
const TInt KMDSM3UNoOffset = 0;
const TInt KPlaylistSampleLength = 10000; 
const TUint KUnicodeBOM = 0xFEFF;
const TInt KPlaylistMaxSampleLength = 130000;
const TInt KMinimumConfidenceRequired = 75;
const TInt KMDSM3UPlaylistMaxItemCount = KMaxTInt;
const TInt KPathStartingChars = 3;

// MODULE DATA STRUCTURES
enum TMDSM3UPlaylistLineType
    {
    EMDSM3UPlaylistLineTypeExtinf = 1,
    EMDSM3UPlaylistLineTypePath = 2,
    EMDSM3UPlaylistLineTypeNotSupported = 3,
    EMDSM3UPlaylistLineTypeCorrupted = 4
    };

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::NewL
// -----------------------------------------------------------------------------
//
CHarvesterM3UPlaylistParser* CHarvesterM3UPlaylistParser::NewL( RFs& aFs,
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aAvailableCharacterSet,
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aTopCharacterSet )
    {
    CHarvesterM3UPlaylistParser* self = new ( ELeave ) CHarvesterM3UPlaylistParser(
            aFs, aAvailableCharacterSet, aTopCharacterSet );

	return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CHarvesterM3UPlaylistParser::~CHarvesterM3UPlaylistParser()
    {
    Reset();
    }


// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::CHarvesterM3UPlaylistParser
// -----------------------------------------------------------------------------
//
CHarvesterM3UPlaylistParser::CHarvesterM3UPlaylistParser( RFs& aFs,
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aAvailableCharacterSet,
        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* aTopCharacterSet ) 
    :iFs( aFs ), iAvailableCharacterSet( aAvailableCharacterSet ),
     iTopCharacterSet( aTopCharacterSet ), iEndLineNumber( KMDSM3UPlaylistMaxItemCount )
    {
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ParseL
// -----------------------------------------------------------------------------
//
TBool CHarvesterM3UPlaylistParser::ParseL( const TDesC& aFileName,
		RPointerArray<HBufC>& aUriArray )
    {
    iPlaylistFilePath.Set( aFileName );
    ReadPlaylistFileToBufferL();
    ParsePlaylistBufferL( aUriArray, iInvalidItems );
    
    // If at the moment, we know that there is at least one error parsing
    // with auto detect encoding, we don't need to proceed until end of
    // file anymore, this playlist file is concluded to be corrupted
    if ( iInvalidItems > 0 )
        {
        aUriArray.Reset();
        Reset();
        User::Leave( KErrCorrupt );
        }
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ResetL
// -----------------------------------------------------------------------------
//
void CHarvesterM3UPlaylistParser::Reset()
    {
    delete iBuffer;
    iBuffer = NULL;
    delete iLine;
    iLine = NULL;
    iBufferPtr.Set( KNullDesC );
    iCurrentLineNumber = 0;
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ReadPlaylistFileToBufferL
// -----------------------------------------------------------------------------
//
void CHarvesterM3UPlaylistParser::ReadPlaylistFileToBufferL()
    {
#ifdef _DEBUG
    WRITELOG1( "Before reading playlist to buffer: heap size = %d", User::Heap().Size() );
#endif
    
    TEntry entry;
    User::LeaveIfError( iFs.Entry( iPlaylistFilePath, entry ) );
    
    HBufC* buffer = HBufC::NewLC( entry.iSize );
    TPtr ptr = buffer->Des();

    HBufC8* buf8 = HBufC8::NewLC( entry.iSize );
    TPtr8 ptr8 = buf8->Des();

    // Read the first KPlaylistSampleLength bytes of the file
    TInt sampleLength( KPlaylistSampleLength );
    if( sampleLength > entry.iSize )
        {
        sampleLength = entry.iSize;
        }
    User::LeaveIfError( iFs.ReadFileSection(
                            iPlaylistFilePath, 0, ptr8, sampleLength ) );

    // auto detect character encoding
    TUint charSetId( 0 );
    TInt error = DetectCharacterSetL( *buf8, *iTopCharacterSet, charSetId );
    WRITELOG2("Encoding detected using top character set is 0x%x, error %d", charSetId, error);
    
    // when we fail to detect the encoding, use all available character set in the
    // system to try again. If that also fails, abandon the operation.
    if ( error )
        {
        User::LeaveIfError( DetectCharacterSetL( *buf8, *iAvailableCharacterSet, charSetId ) );
        WRITELOG1( "Encoding detected using available character set is 0x%x", charSetId );     
        }

    // read the whole file if the sample taken isn't the whole file
    if ( sampleLength != entry.iSize )
        {
        User::LeaveIfError( iFs.ReadFileSection(
                                iPlaylistFilePath, 0, ptr8, entry.iSize) );
        }
   
    // perform character conversion using the selected encoding
    TInt state( CCnvCharacterSetConverter::KStateDefault );
    TInt numOfUnconvertibleChars( 0 );
    CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
    charSetConv->PrepareToConvertToOrFromL( charSetId, *iAvailableCharacterSet, iFs );
    TInt retVal = charSetConv->ConvertToUnicode( ptr, *buf8, state, numOfUnconvertibleChars );
    User::LeaveIfError( retVal );

    // try again if the character set wasn't detected using the whole file
    if( (retVal > 0 || numOfUnconvertibleChars > 0) && (sampleLength != entry.iSize) )
        {
        WRITELOG3( "retVal = %d, numOfUnconvertibleChars = %d, entry.iSize = %d",
                retVal, numOfUnconvertibleChars, entry.iSize );  
        numOfUnconvertibleChars = 0;
        retVal = 0;
        User::LeaveIfError( DetectCharacterSetL( *buf8, *iAvailableCharacterSet, charSetId ) );
        charSetConv->PrepareToConvertToOrFromL( charSetId, *iAvailableCharacterSet, iFs );
        retVal = charSetConv->ConvertToUnicode( ptr, *buf8, state, numOfUnconvertibleChars );
        }
        
    if ( retVal > 0 || numOfUnconvertibleChars > 0 )
        {
        WRITELOG2( "Unable to find character encoding for the playlist file. retVal = %d, numOfUnconvertibleChars = %d",
                    retVal, numOfUnconvertibleChars );     
        User::Leave( KErrNotSupported );
        }
    
    // remove the byte order mark (BOM) character prepended at the beginning
    // of the stream if encoded with unicode as per Unicode section 2.4
    if ( (charSetId == KCharacterSetIdentifierUnicodeLittle ||
         charSetId == KCharacterSetIdentifierUnicodeBig) &&
        ptr.Length() > 0 &&
        ptr[0] == KUnicodeBOM )
        {
        ptr.Delete( 0,1 );
        }
        
    iBuffer = buffer;
    iBufferPtr.Set( *iBuffer );
    
    CleanupStack::PopAndDestroy (2, buf8 ); // charSetConv & buf8    
    CleanupStack::Pop( buffer );
        
    // brand new buffer which hasn't been read, reset iCurrentLineNumber and
    // iEndLineNumber
    iCurrentLineNumber = 0;

#ifdef _DEBUG
    WRITELOG1( "After reading playlist to buffer: heap size = %d", User::Heap().Size() );
#endif
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::DetectCharacterSetL
// -----------------------------------------------------------------------------
//
TInt CHarvesterM3UPlaylistParser::DetectCharacterSetL(
            const TDesC8& aSample,
            const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aCharacterSet,
            TUint& aCharSetId)
    {
    // CCnvCharacterSetConverter::ConvertibleToCharSetL hangs if sample is too big
    if ( aSample.Size() > KPlaylistMaxSampleLength )
        {
        User::Leave( KErrNotSupported );
        }
        
    TInt confidence( 0 );
    TInt highestConfidence( 0 );
    TUint charSetId( 0 );
    TUint highestConfidencecharSetId( 0 );

    CCnvCharacterSetConverter* charSetConv = CCnvCharacterSetConverter::NewLC();
    TInt count = aCharacterSet.Count();
    for ( TInt i=0; i < count; i++ )
        {
        charSetId = aCharacterSet.At(i).Identifier();
        charSetConv->ConvertibleToCharSetL( confidence, charSetId, aCharacterSet, aSample );
        if ( confidence > highestConfidence )
            {
            highestConfidence = confidence;
            highestConfidencecharSetId = charSetId;
            }
        }
    CleanupStack::PopAndDestroy( charSetConv );
    WRITELOG2( "CMPXM3uPlaylistImporter::DetectCharacterSetL :-> Confidence[%d] CharSetId[0x%x]",
            confidence, aCharSetId );
    if ( highestConfidence == 0 || highestConfidence < KMinimumConfidenceRequired )
        {
        return KErrNotFound;
        }
    else
        {
        aCharSetId = highestConfidencecharSetId;
        return KErrNone;
        }
    }
    
// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ParsePlaylistBufferL
// -----------------------------------------------------------------------------
//
void CHarvesterM3UPlaylistParser::ParsePlaylistBufferL(
			RPointerArray<HBufC>& aPlaylist,
            TInt& aInvalidItemCount)
    {
    // Read and process all the lines in the file
    //
    // the order of the following conditions is important. ReadNextLineL
    // should be called last to avoid skipping one line
    while ( iCurrentLineNumber < iEndLineNumber &&
               aPlaylist.Count() < KMDSM3UPlaylistMaxItemCount &&
               ReadNextLineL() )
        {
        ProcessLineL( aPlaylist, aInvalidItemCount );  
        }

    if ( aPlaylist.Count() == KMDSM3UPlaylistMaxItemCount )
        {
        Reset();
        User::Leave( KErrOverflow );
        }
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ReadNextLineL
// -----------------------------------------------------------------------------
//
TBool CHarvesterM3UPlaylistParser::ReadNextLineL()
    {
    // iBuffer should exist when this function is called
    __ASSERT_DEBUG( iBuffer, User::Leave( KErrBadDescriptor ) );

    if ( !iBufferPtr.Length() )
        {
        return EFalse;
        }

    delete iLine;
    iLine = NULL;

    // Try to find line change
    TInt offset = iBufferPtr.FindF( KMDSM3ULineChange );
  
    if( offset == KErrNotFound )
        {
        // No line change was found --> last line had no line change
        iLine = iBufferPtr.AllocL();
        // Set iBufferPtr to the end of buffer
        iBufferPtr.Set( iBufferPtr.Right(0) );
        }
    else
        {
        // Found line change
        TInt length( offset );
        if ( (offset > KMDSM3UNoOffset) && 
            (iBufferPtr[length - 1] == KMDSM3UCarriageReturn) )
            {
            --length;
            }

        iLine = iBufferPtr.Left(length).AllocL();

        // Move past the line feed
        iBufferPtr.Set( iBufferPtr.Mid(++offset) );
        }

    // Remove leading and trailing space characters from iLine's data.
    TPtr ptr = iLine->Des();
    ptr.Trim();

    iCurrentLineNumber++;
    return ETrue;
    }
    
// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ProcessLineL
// -----------------------------------------------------------------------------
//
void CHarvesterM3UPlaylistParser::ProcessLineL(
			RPointerArray<HBufC>& aPlaylist,
            TInt& aInvalidItemCount)
    {
    if ( iCurrentLineNumber == 1 ) // first line
        {
        // Check whether the file is in the extented format
        TInt offset = iLine->Find( KMDSM3UTagExtm3u );
        if( offset == KErrNotFound || offset != KMDSM3UNoOffset ||
            iLine->Length() != KMDSM3UTagExtm3u().Length() )
            {
            // The file is not in the extented format
            iExtendedFormat = EFalse;
            }
        else
            {
            // The file is in the extented format
            iExtendedFormat = ETrue;
            return;
            }        
        }
    
    // Parse line and then decide what to do with it
    switch( ParseLineL( iItem, aInvalidItemCount ) )
        {
        case EMDSM3UPlaylistLineTypeExtinf:
            // Continue to next round
            break;
            
        case EMDSM3UPlaylistLineTypePath:
            {
            // Line was a path => add item to playlist
            aPlaylist.AppendL( iItem.AllocL() );
            }
            break; 
                       
        case EMDSM3UPlaylistLineTypeNotSupported:
        case EMDSM3UPlaylistLineTypeCorrupted:
        default:
            {
            iItem = KNullDesC;
            }
            break;
        }
    }
    
// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ParseLineL
// -----------------------------------------------------------------------------
//
TInt CHarvesterM3UPlaylistParser::ParseLineL(
            TFileName& aItem,
            TInt& aInvalidItemCount)
    {
    __ASSERT_DEBUG( iLine, User::Leave(KErrAbort) );

    if( !iLine->Length() )
        {
        // Empty line => line is invalid
        return EMDSM3UPlaylistLineTypeNotSupported;
        }

    if( iExtendedFormat )
        {
        // File is in the extented format => check whether there is extented
        // info in this line.
        TInt offset = iLine->Find( KMDSM3UTagExtinf );
        if( offset != KErrNotFound && offset == KMDSM3UNoOffset )
            {
            offset = iLine->Find( KMDSM3UPoint );

            if( offset != KErrNotFound )
                {
                return EMDSM3UPlaylistLineTypeExtinf; // line type extinf
                }    
            }
        }

    // File is not in the extented format or supported info not found from this
    // line.
    switch( iLine->Find(KMDSM3UTagExt) )
        {
        case KMDSM3UNoOffset:
            // Unsupported extended info tag found from this line
            return EMDSM3UPlaylistLineTypeNotSupported;
            
        case KErrNotFound:
        default:
            // Extended info not found from the beginning of line => line is
            // a path.
            {
            // Get absolute path
            TInt error( KErrNone );
            HBufC* uri = ParseAbsolutePathLC( *iLine, error );
        
            if( error )
                {
                CleanupStack::PopAndDestroy( uri );
                ++aInvalidItemCount;
                return EMDSM3UPlaylistLineTypeCorrupted;
                }

            aItem = uri->Des();

            CleanupStack::PopAndDestroy( uri );
            
            return EMDSM3UPlaylistLineTypePath; // line type path
            }
        }
    }

// -----------------------------------------------------------------------------
// CHarvesterM3UPlaylistParser::ParseAbsolutePathLC
// -----------------------------------------------------------------------------
//
HBufC* CHarvesterM3UPlaylistParser::ParseAbsolutePathLC(
            const TDesC& aPath,
            TInt& aError)
    {
    HBufC* path = NULL;
    
    TBool isAbsolute( EFalse );
    
    if( aPath.Length() > KPathStartingChars && 
        !aPath.Mid(1, 2).CompareF( KMDSM3UAbsPath ) ) // magic: the 2nd and 3rd chars
                                               // are always ":\"
                                               // for absolute paths
        {
        isAbsolute = ETrue;
        }

    if( aPath.Length() > KMaxFileName ) // Test if path is too long
        {
        aError = KErrCorrupt;
        }
    else if( isAbsolute )
        {
        aError = KErrNone;
        aError = iFs.IsValidName( aPath ) ? KErrNone : KErrBadName;
        path = aPath.AllocLC();
        }
    else
        {
        // Given path could be relative => create absolute path and test it
        // Playlist file path
        TParse playlistPath;
        playlistPath.Set( iPlaylistFilePath, NULL, NULL );
        // Path to the folder, where playlist file is located to
        TPtrC currentFolder = playlistPath.DriveAndPath();
        // Create absolute path
        path = HBufC::NewLC( currentFolder.Length() + aPath.Length() );

        TPtr tmpPtr( path->Des() );
        tmpPtr = currentFolder;
        tmpPtr += aPath;

        aError = iFs.IsValidName(*path) ? KErrNone : KErrBadName;
        }
    
    // It is possible that a song exists in the filesystem but isn't added to
    // the database because it's not a supported type. If such song is included
    // in a playlist, it will be added to the database when the playlist is added.
    // Because of this, we cannot rely on whether the song exists in the database
    // to conclude whether the song is a broken link. We need to check for file
    // existence here. For the unsupported songs included in the playlist, they
    // will then be marked as corrupted when user initiates playback of those
    // songs.
    if ( !aError &&
         !BaflUtils::FileExists(iFs, *path) )
        {
        aError = KErrPathNotFound;
        }

    return path;
    }

// End of file
