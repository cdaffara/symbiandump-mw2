/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*           Miscellaneous text related utility methods.
*
*/



// ========== INCLUDE FILES ================================

#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include <s32file.h>

#include <charconv.h>
#include <txtrich.h>

#include <cmsvattachment.h>
#include <cmsvmimeheaders.h>
#include <mmsvattachmentmanager.h>

#include "msgtextutils.h"

// ========== LOCAL CONSTANTS AND MACROS ===================

/* Reddy - Moved ot header
// Default charsets
const TUint KDefaultCharConvCharset = KCharacterSetIdentifierAscii;
const TUint KDefaultMIBCharset = KCharacterSetMIBEnumUsAscii;
*/

const TInt KMaxSampleLengthForAutoDetection = 512;
const TInt KCharsetPluginArrayGranularity = 10;

const TUint KCharParagraphSeparator = 0x2029;
const TUint KCharLineFeed = 0x0a;
const TUint KCharCarriageReturn = 0x0d;
const TUint KCharAsciiDot = 0x2e;
const TUint KCharAsciiMax = 0x7f;

const TInt KMaxFileNameLength = 8;      // must be at least 4!
const TInt KMaxFileExtensionLenght = 5; // includes dot
const TInt KMaxFileTotalLength = 12;

_LIT8( KReplaceChar8, "_" );
_LIT16( KReplaceChar16, "_" );

// ========== MEMBER FUNCTIONS =============================


// ---------------------------------------------------------
// CMsgTextUtils::NewL
// ---------------------------------------------------------
//
EXPORT_C CMsgTextUtils* CMsgTextUtils::NewL( RFs& aFs )
    {
    CMsgTextUtils* data = new ( ELeave ) CMsgTextUtils( aFs );
    CleanupStack::PushL( data );
    data->ConstructL();
    CleanupStack::Pop();
    return data;
    }


// ---------------------------------------------------------
// CMsgTextUtils::CMsgTextUtils
//
// Constructor
// ---------------------------------------------------------
//
CMsgTextUtils::CMsgTextUtils( RFs& aFs )
    : iFs( aFs )
    {
    }


// ---------------------------------------------------------
// CMsgTextUtils::ConstructL
// ---------------------------------------------------------
//
void CMsgTextUtils::ConstructL()
    {
    // Nothing to do. Reserved for future use.
    }

// ---------------------------------------------------------
// CMsgTextUtils::~CMsgTextUtils
//
// Destructor.
// ---------------------------------------------------------
//
CMsgTextUtils::~CMsgTextUtils()
    {
    delete iCharConv;
    }

// ---------------------------------------------------------
// CMsgTextUtils::ConvertToBuffer8LC
// 
// ---------------------------------------------------------
//
EXPORT_C HBufC8* CMsgTextUtils::ConvertToBuffer8LC(
        const TDesC& aText,
        TUint aCharacterSetIdentifier )
    {
    TInt origLength = aText.Length();
    TInt maxLength = origLength;
    HBufC8* resultBuffer = HBufC8::NewLC( maxLength ); 
    if ( !iCharConv )
        {
        iCharConv = CCnvCharacterSetConverter::NewL();
        }

    if ( iCharConv->PrepareToConvertToOrFromL( aCharacterSetIdentifier, iFs ) !=
        CCnvCharacterSetConverter::EAvailable )
        {
        User::Leave( KErrNotSupported );
        }

    iCharConv->SetReplacementForUnconvertibleUnicodeCharactersL( KReplaceChar8 );

    TBuf8<40> outputBuffer;
    TPtrC16 remainderOfUnicode( aText );

    FOREVER
        {
        const TInt doneAll = iCharConv->ConvertFromUnicode( outputBuffer, remainderOfUnicode );

        if ( doneAll == CCnvCharacterSetConverter::EErrorIllFormedInput )
            {
            User::Leave( KErrCorrupt );
            }
        else if ( doneAll < 0 )
            {
            // For future expansion of errors
            User::Leave( KErrGeneral );
            }
        else
            { // lint
            }

        // Make sure outputBuffer fits into resultBuffer
        while ( resultBuffer->Length() + outputBuffer.Length() > maxLength )
            {
            // Increase resultBuffer length by origLength
            maxLength += origLength;
            resultBuffer = resultBuffer->ReAllocL( maxLength );
            CleanupStack::Pop(); // resultBuffer
            CleanupStack::PushL(resultBuffer);
            }
        // Append to result
        resultBuffer->Des().Append( outputBuffer );

        if ( doneAll == 0 )
            {
            return resultBuffer; // All converted
            }

        remainderOfUnicode.Set( remainderOfUnicode.Right( doneAll ));
        }
    }       

// ---------------------------------------------------------
// CMsgTextUtils::ConvertToBuffer16LC
// 
// ---------------------------------------------------------
//
EXPORT_C HBufC* CMsgTextUtils::ConvertToBuffer16LC(
        const TDesC& aText,
        TUint aCharacterSetIdentifier )
    {
    HBufC8* resultBuffer = ConvertToBuffer8LC( aText, aCharacterSetIdentifier );

    HBufC* finalresult = HBufC::NewL( resultBuffer->Length() );
    finalresult->Des().Copy( *resultBuffer );
    CleanupStack::PopAndDestroy(); // resultbuffer
    CleanupStack::PushL( finalresult );
        
    return finalresult;
    }
    
// ---------------------------------------------------------
// CMsgTextUtils::ConvertToFileL
// 
// ---------------------------------------------------------
//
EXPORT_C void CMsgTextUtils::ConvertToFileL(
        const TDesC& aText,
        RFile& aFile,
        TUint aCharacterSetIdentifier )
    {
    // Takes ownership of "aFile".
    RFileWriteStream writer( aFile );
    writer.PushL();

    if ( !iCharConv )
        {
        iCharConv = CCnvCharacterSetConverter::NewL();
        }

    if ( iCharConv->PrepareToConvertToOrFromL( aCharacterSetIdentifier, iFs )
        != CCnvCharacterSetConverter::EAvailable )
        {
        User::Leave( KErrNotSupported );
        }

    iCharConv->SetReplacementForUnconvertibleUnicodeCharactersL( KReplaceChar8 );

    TBuf8<128> outputBuffer;
    TPtrC16 remainderOfUnicodeText( aText );
    FOREVER
        {
        TInt doneAll = iCharConv->ConvertFromUnicode(
            outputBuffer, remainderOfUnicodeText );
        if ( doneAll == CCnvCharacterSetConverter::EErrorIllFormedInput )
            {
            User::Leave( KErrCorrupt );
            }
        else if ( doneAll < 0 ) // future-proof against "TError" expanding
            {
            User::Leave( KErrGeneral );
            }
        else
            { // lint
            }

        writer.WriteL( outputBuffer );
            
        if ( doneAll == 0 )
            {
            // All of aText has been converted and handled
            writer.CommitL();
            writer.Pop();
            writer.Close();
            return;
            }
        remainderOfUnicodeText.Set( remainderOfUnicodeText.Right( doneAll ) );
        }
    }

// ---------------------------------------------------------
// CMsgTextUtils::CharconvIdToMibIdL
// 
// ---------------------------------------------------------
//
EXPORT_C TUint CMsgTextUtils::CharconvIdToMibIdL( TUint aCharconvCharsetId )
    {
    // Switch-case is here for performance optimization
    TUint charset = 0;
    switch ( aCharconvCharsetId )
        {
        case 0:
            // Symbian OS native charset
            charset = KCharacterSetMIBEnumIso10646Ucs2;
            break;
        case KCharacterSetIdentifierAscii:
            charset = KCharacterSetMIBEnumUsAscii;
            break;
        case KCharacterSetIdentifierUtf8:
            charset = KCharacterSetMIBEnumUtf8;
            break;
        case KCharacterSetIdentifierIso88591:
            charset = KCharacterSetMIBEnumISO_8859_1;
            break;
        case KCharacterSetIdentifierIso88592:
            charset = KCharacterSetMIBEnumISO_8859_2;
            break;
        case KCharacterSetIdentifierIso88593:
            charset = KCharacterSetMIBEnumISO_8859_3;
            break;
        case KCharacterSetIdentifierIso88594:
            charset = KCharacterSetMIBEnumISO_8859_4;
            break;
        case KCharacterSetIdentifierIso88595:
            charset = KCharacterSetMIBEnumISO_8859_5;
            break;
        case KCharacterSetIdentifierIso88596:
            charset = KCharacterSetMIBEnumISO_8859_6;
            break;
        case KCharacterSetIdentifierIso88597:
            charset = KCharacterSetMIBEnumISO_8859_7;
            break;
        case KCharacterSetIdentifierIso88598:
            charset = KCharacterSetMIBEnumISO_8859_8;
            break;
        case KCharacterSetIdentifierIso88599:
            charset = KCharacterSetMIBEnumISO_8859_9;
            break;
        case KCharacterSetIdentifierIso885910:
            charset = KCharacterSetMIBEnumISO_8859_10;
            break;
        case KCharacterSetIdentifierIso885913:
            charset = KCharacterSetMIBEnumISO_8859_13;
            break;
        case KCharacterSetIdentifierIso885914:
            charset = KCharacterSetMIBEnumISO_8859_14;
            break;
        case KCharacterSetIdentifierIso885915:
            charset = KCharacterSetMIBEnumISO_8859_15;
            break;
        case KCharacterSetIdentifierUtf7:
            charset = KCharacterSetMIBEnumUtf7;
            break;
        case KCharacterSetIdentifierCodePage1252:
            charset = KCharacterSetMIBEnumCodePage1252;
            break;
        // Chinese charsets
        case KCharacterSetIdentifierGb2312:
            charset = KCharacterSetMIBEnumGb2312;
            break;
        case KCharacterSetIdentifierHz:
            charset = KCharacterSetMIBEnumHz;
            break;
        case KCharacterSetIdentifierGbk:
            charset = KCharacterSetMIBEnumGbk;
            break;
        case KCharacterSetIdentifierBig5:
            charset = KCharacterSetMIBEnumBig5;
            break;
        // Japanese charsets
        case KCharacterSetIdentifierShiftJis:
            charset = KCharacterSetMIBEnumShiftJis;
            break;
        case KCharacterSetIdentifierIso2022Jp:
            charset = KCharacterSetMIBEnumIso2022Jp;
            break;
        case KCharacterSetIdentifierJis:
            charset = KCharacterSetMIBEnumJis;
            break;
        case KCharacterSetIdentifierEucJpPacked:
            charset = KCharacterSetMIBEnumEucJpPacked;
            break;
        default:
            {
            if ( !iCharConv )
                {
                iCharConv = CCnvCharacterSetConverter::NewL();
                }
            charset = iCharConv->ConvertCharacterSetIdentifierToMibEnumL(
                aCharconvCharsetId, iFs );
            if ( charset == 0 )
                {
                charset = KDefaultMIBCharset;
                }
            break;
            }
        }
    return charset;
    }

// ---------------------------------------------------------
// CMsgTextUtils::MibIdToCharconvIdL
// 
// ---------------------------------------------------------
//
EXPORT_C TUint CMsgTextUtils::MibIdToCharconvIdL( TUint aMibId )
    {
    // Switch-case is here for performance optimization
    TUint charset = 0;
    switch ( aMibId )
        {
        case KCharacterSetMIBEnumIso10646Ucs2:
        case KCharacterSetMIBEnumUTF16:
            //no conversion for Unicode
            charset = 0;
            break;
        case KCharacterSetMIBEnumUTF16BE:
            charset = KCharacterSetIdentifierUnicodeBig; 
            break;
        case KCharacterSetMIBEnumUTF16LE:
            charset = KCharacterSetIdentifierUnicodeLittle;  
            break;
        case KCharacterSetMIBEnumUsAscii:
            charset = KCharacterSetIdentifierAscii;
            break;
        case KCharacterSetMIBEnumUtf8:
            charset = KCharacterSetIdentifierUtf8;
            break;
        case KCharacterSetMIBEnumISO_8859_1:
            charset = KCharacterSetIdentifierIso88591;
            break;
        case KCharacterSetMIBEnumISO_8859_2:
            charset = KCharacterSetIdentifierIso88592;
            break;
        case KCharacterSetMIBEnumISO_8859_3:
            charset = KCharacterSetIdentifierIso88593;
            break;
        case KCharacterSetMIBEnumISO_8859_4:
            charset = KCharacterSetIdentifierIso88594;
            break;
        case KCharacterSetMIBEnumISO_8859_5:
            charset = KCharacterSetIdentifierIso88595;
            break;
        case KCharacterSetMIBEnumISO_8859_6:
            charset = KCharacterSetIdentifierIso88596;
            break;
        case KCharacterSetMIBEnumISO_8859_7:
            charset = KCharacterSetIdentifierIso88597;
            break;
        case KCharacterSetMIBEnumISO_8859_8:
            charset = KCharacterSetIdentifierIso88598;
            break;
        case KCharacterSetMIBEnumISO_8859_9:
            charset = KCharacterSetIdentifierIso88599;
            break;
        case KCharacterSetMIBEnumISO_8859_10:
            charset = KCharacterSetIdentifierIso885910;
            break;
        case KCharacterSetMIBEnumISO_8859_13:
            charset = KCharacterSetIdentifierIso885913;
            break;
        case KCharacterSetMIBEnumISO_8859_14:
            charset = KCharacterSetIdentifierIso885914;
            break;
        case KCharacterSetMIBEnumISO_8859_15:
            charset = KCharacterSetIdentifierIso885915;
            break;
        case KCharacterSetMIBEnumUtf7:
            charset = KCharacterSetIdentifierUtf7;
            break;
        case KCharacterSetMIBEnumCodePage1252:
            charset = KCharacterSetIdentifierCodePage1252;
            break;
        // Chinese charsets
        case KCharacterSetMIBEnumGb2312:
            charset = KCharacterSetIdentifierGb2312;
            break;
        case KCharacterSetMIBEnumHz:
            charset = KCharacterSetIdentifierHz;
            break;
        case KCharacterSetMIBEnumGbk:
            charset = KCharacterSetIdentifierGbk;
            break;
        case KCharacterSetMIBEnumBig5:
            charset = KCharacterSetIdentifierBig5;
            break;
        // Japanese charsets
        case KCharacterSetMIBEnumShiftJis:
            charset = KCharacterSetIdentifierShiftJis;
            break;
        case KCharacterSetMIBEnumIso2022Jp:
            charset = KCharacterSetIdentifierIso2022Jp;
            break;
        case KCharacterSetMIBEnumJis:
            charset = KCharacterSetIdentifierJis;
            break;
        case KCharacterSetMIBEnumEucJpPacked:
            charset = KCharacterSetIdentifierEucJpPacked;
            break;
        default:
            {
            if ( !iCharConv )
                {
                iCharConv = CCnvCharacterSetConverter::NewL();
                }
            charset = iCharConv->ConvertMibEnumOfCharacterSetToIdentifierL( aMibId, iFs );
            if ( charset == 0 )
                {
                charset = KDefaultCharConvCharset;
                }
            break;
            }
        }
    return charset;
    }

// ---------------------------------------------------------
// ConvertParagraphSeparatorsLC
// ---------------------------------------------------------
//
EXPORT_C HBufC* CMsgTextUtils::ConvertParagraphSeparatorsLC( const TDesC& aText )
    {
    TInt position;
    TPtrC ptr;

    TInt numberOfSeparators = 0;

    position = 0;
    ptr.Set( aText.Mid( 0 ) ); 
    while ( position != KErrNotFound && position < aText.Length() )
        {
        ptr.Set( ptr.Mid( position ) ); 
        position = ptr.Locate( TChar( KCharParagraphSeparator ) );
        if ( position != KErrNotFound )
            {
            numberOfSeparators++;
            position++; // point past separator just found
            }
        }

    HBufC* convertedText = HBufC::NewLC( aText.Length() + numberOfSeparators );
    ptr.Set( aText.Mid( 0 ) );
    TInt start = 0;
    position = aText.Locate( TChar( KCharParagraphSeparator ) );
    while ( position != KErrNotFound && start < aText.Length() )
        {
        ptr.Set( aText.Mid( start ) ); 
        position = ptr.Locate( TChar( KCharParagraphSeparator ) );
        if ( position != KErrNotFound )
            {
            convertedText->Des().Append( ptr.Left( position ) );
            convertedText->Des().Append( TChar( KCharCarriageReturn ) );
            convertedText->Des().Append( TChar( KCharLineFeed ) );
            start = start + position + 1; // point past separator
            }
        }
    // append what is left after last separator has been found
    if ( start < aText.Length() )
        {
        convertedText->Des().Append( aText.Mid( start ) );
        }

    return convertedText;
    }

// ---------------------------------------------------------
// CMsgTextUtils::TrimAndRemoveNonAlphaDigit
// 
// NOTE: This is intended for small strings. With long 
// strings if could be more efficient to to seek replacable
// chars than to loop thru every char.
// ---------------------------------------------------------
//
EXPORT_C void CMsgTextUtils::TrimAndRemoveNonAlphaDigit( TDes& aString )
    {
    aString.Trim();
    for (TInt i = aString.Length(); --i >= 0 ;)
        {
        TChar c = (TChar) aString[i];
        // Allow dots i.e. "." because they are allowed in URI and
        // because filename are created from content-location
        // so this will result to "filename.txt" instead of "filename_txt"
        if ( aString[i] > KCharAsciiMax ||
            ( aString[i] != KCharAsciiDot && !c.IsAlphaDigit() ) )
            {
            aString.Replace( i, 1, KReplaceChar16 );
            }
        }
    }

// ---------------------------------------------------------
// UTF8Size
// ---------------------------------------------------------
//
EXPORT_C TInt CMsgTextUtils::UTF8Size( TPtrC aText )
    {
    TInt count = 0;
    TInt sizeInBytes = 0;
    TUint16 charValue;
    while ( count < aText.Length() )
        {
        charValue = aText[count];
        if ( charValue < 0x80 )
            {
            sizeInBytes += 1;
            }
        else if ( charValue < 0x800 )
            {
            sizeInBytes += 2;
            }
        else //if ( charValue < 0x10000 )
            {
            sizeInBytes += 3;
            }
        count++;
        }
    return sizeInBytes;
    }

// ---------------------------------------------------------
// ConvertLineBreaksL
// ---------------------------------------------------------
//
EXPORT_C void CMsgTextUtils::ConvertLineBreaksL( CRichText& aText, TInt aMode )
    {
    if ( aMode & ECRLFtoLF )
        {
        DoConvertCRLFL( aText );
        }
    }

// ---------------------------------------------------------
// DoConvertCRLFL
// ---------------------------------------------------------
//
void CMsgTextUtils::DoConvertCRLFL( CRichText& aText )
    {
    TInt i( 0 );
    TInt documentLength( aText.DocumentLength() );
    
    while ( i < documentLength )
        {
        if ( aText.Read( i, 1 ).Locate( KCharCarriageReturn ) != KErrNotFound )
            {
            if ( i < documentLength - 1 &&
                 aText.Read( i + 1, 1 ).Locate( KCharLineFeed ) != KErrNotFound )
                {
                // "CR+LF" -> "LF"
                aText.DeleteL( i, 1 );
                documentLength--;
                }
            }
        i++;
        }
    }

// -----------------------------------------------------------------------------
// RecognizeCharSetL
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMsgTextUtils::RecognizeCharSetL( RFs& aFs, RFile& aFile )
    {
    TUint charSet( 0 );
    
    HBufC8* sample = HBufC8::NewLC( KMaxSampleLengthForAutoDetection );
	TPtr8 sampleDes = sample->Des();
    
    TInt fileSize( 0 );
    User::LeaveIfError( aFile.Size( fileSize ) );
    User::LeaveIfError( aFile.Read( 0, sampleDes, fileSize > sampleDes.MaxLength()
        ? sampleDes.MaxLength() 
        : fileSize ) );


    // Check for Byte Order Mark (BOM)
    // U+FEFF ZERO WIDTH NON-BREAKING SPACE
    //
    // (from www.unicode.org FAQ)
    // - FE FF -> UTF-16, big-endian 
    // - FF FE -> UTF-16, little-endian 
    //
    if ( !sampleDes.Length() || 
        ( sampleDes.Size() >= 2 && 
        ( ( sampleDes[0] == 0xfe && sampleDes[1] == 0xff ) ||
        ( sampleDes[0] == 0xff && sampleDes[1] == 0xfe ) ) ) )
        {
        // Zero means no conversion
        charSet = 0;
        }
    else
        {
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* availableSets = 
            CCnvCharacterSetConverter::CreateArrayOfCharacterSetsAvailableLC( aFs );

        CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* toDetectSets =
            new ( ELeave ) CArrayFixFlat<CCnvCharacterSetConverter::SCharacterSet>( KCharsetPluginArrayGranularity );
        CleanupStack::PushL( toDetectSets );

        const TInt count( availableSets->Count() );
        for ( TInt i( 0 ); i < count; i++ )
            {
            switch ( availableSets->At( i ).Identifier() )
                {
                case KCharacterSetIdentifierSms7Bit:
                    // remove Sms7Bit from charset list
                    break;
                default:
                    toDetectSets->AppendL( availableSets->At( i ) );
                    break;
                }
            }

        TUint resultCharset( 0 );
        TInt confidence( 0 );
        CCnvCharacterSetConverter::AutoDetectCharacterSetL(
            confidence, resultCharset, *toDetectSets, sampleDes );
        charSet = resultCharset;
        
        CleanupStack::PopAndDestroy( 2 ); // availableSets, toDetectSets
        }
    CleanupStack::PopAndDestroy( sample );
    TInt seekPos(0);
    aFile.Seek( ESeekStart, seekPos );
    return charSet;
    }

// ---------------------------------------------------------
// GetSafeAttachmentNameLC
// ---------------------------------------------------------
//
EXPORT_C HBufC* CMsgTextUtils::GetSafeAttachmentNameLC(
        MMsvAttachmentManager& aManager,
        const TDesC& aFileName,
        TMsvAttachmentId aAttachmentId,
        TBool aContentLocation )
    {
    TBuf<KMaxFileNameLength> name;
    TBuf<KMaxFileExtensionLenght> ext;
    TBuf<KMaxFileTotalLength> candidate;
    TParsePtrC parse( aFileName );

    ext.Copy( parse.Ext().Left( KMaxFileExtensionLenght ) );
    CMsgTextUtils::TrimAndRemoveNonAlphaDigit( ext );

    TInt nameLen = Min( KMaxFileNameLength,
        ( KMaxFileTotalLength - ext.Length() ) );
    name.Copy( parse.Name().Left( nameLen ) );    
    CMsgTextUtils::TrimAndRemoveNonAlphaDigit( name );

    candidate.Copy( name );
    candidate.Append( ext );

    TBool safeFound = EFalse;
    TInt count = aManager.AttachmentCount();
    TInt i = 0;
    while ( !safeFound && i < count )
        {
        safeFound = ETrue;
        for ( TInt ii = 0; ii < count && safeFound; ii++ )
            {
            CMsvAttachment* attachment = aManager.GetAttachmentInfoL( ii );
            CleanupStack::PushL( attachment );
            if ( aContentLocation )
                {
                CMsvMimeHeaders* msvMime = CMsvMimeHeaders::NewLC();
                msvMime->RestoreL( *attachment );            
                if ( attachment->Id() != aAttachmentId &&
                    !candidate.CompareF( msvMime->ContentLocation() ) )
                    {
                    safeFound = EFalse;
                    }
                CleanupStack::PopAndDestroy( msvMime );
                }
            else
                {
                if ( attachment->Id() != aAttachmentId &&
                    !candidate.CompareF( attachment->AttachmentName() ) )
                    {
                    safeFound = EFalse;
                    }
                }
            CleanupStack::PopAndDestroy( attachment );
            }
        i++;
        if ( !safeFound )
            {
            name.Zero();
            name.Copy( parse.Name().Left( nameLen - 3 ) );
            CMsgTextUtils::TrimAndRemoveNonAlphaDigit( name );
            TBuf<3> num;
            num.NumFixedWidth( i, EDecimal, 3 );
            name.Append( num );
            candidate.Zero();
            candidate.Copy( name );
            candidate.Append( ext );
            }
        }

    // It is guaranteed that we always find a safe candidate:
    // - either safeFound == ETrue, or
    // - we've got "count + 1st" candidate while there are
    //   "count" existing attachments.
    HBufC* safeFileName = candidate.AllocLC();
    return safeFileName;
    }

// ---------------------------------------------------------
// from: TBool CImRecvConvert::IsIllegalChar(const TUint aChar)
// ---------------------------------------------------------
//
LOCAL_C TBool IsIllegalChar(const TUint aChar)
    {
    return (
        aChar == '*'  ||
        aChar == '\\' ||
        aChar == '<'  ||
        aChar == '>'  ||
        aChar == ':'  ||
        aChar == '.'  ||
        aChar == '"'  ||
        aChar == '/'  ||
        aChar == '|'  ||
        aChar == '?'  ||
        aChar == CEditableText::EParagraphDelimiter  ||
        aChar == CEditableText::ELineBreak  ||
        aChar <  ' ' );
    }

// ---------------------------------------------------------
// GetFileNameFromBuffer
// ---------------------------------------------------------
//
EXPORT_C void CMsgTextUtils::GetFileNameFromBuffer(
    TFileName& aFileName,
    const TDesC& aBuffer,
    TInt aMaxLength,
    const TDesC* aExt /*= NULL*/ )
    {
    if ( aExt != NULL )
        {
        aMaxLength -= aExt->Length();
        }

    TInt len = aBuffer.Length();
    TInt max = Min( len, aMaxLength );

    //__ASSERT_DEBUG( max > 0, Panic( EMsgZeroLength ) );

    aFileName.Zero();

    TInt cc = 0;
    TUint ch;
    TUint ch1 = 0;
    TBool spaces = EFalse;
    for ( TInt i = 0; i < len && cc < max; i++ )
        {
        ch = aBuffer[i];

        // ignore spaces from beginning of the buffer until first
        // non-space is encountered.
        if ( !spaces && ch != ' ' )
            {
            spaces = ETrue;
            }

        if ( i > 0 )
            {
            ch1 = aBuffer[i - 1];
            }

        // strip illegal chars away.
        // checks also if previous and current chars are '.'
        if ( spaces && ! IsIllegalChar( ch ) )
            {
            if ( !( i > 0 && ch == '.' && ch1 == '.' ) )
                {
                aFileName.Append( ch );
                cc++;
                }
            }
        }

    aFileName.Trim();

    // If filename is empty at this point, do not append extension either.
    // Instead, empty filename is returned so that caller can use whatever
    // default s/he has for it.
    if ( aFileName.Length() > 0 && aExt != NULL )
        {
        aFileName.Append( *aExt );
        }
    }

// ---------------------------------------------------------
// CMsgTextUtils::ConvertToUnicodeL
// Converts input 8-bit data buffer (in given foreign charset type) to unicode buffer.
// ---------------------------------------------------------
EXPORT_C HBufC16* CMsgTextUtils::ConvertToUnicodeL( const TDesC8& aText, TUint aCharacterSetIdentifier )
    {
    TInt origLength = aText.Length();
    TInt maxLength = origLength;

    if ( !iCharConv )
        {
        iCharConv = CCnvCharacterSetConverter::NewL();
        }
    
    //buffer to hold target data and return to caller
    HBufC* resultBuffer = HBufC::NewL( maxLength );
    _LIT8( KReplaceChar8, "_" );
    
    if ( iCharConv->PrepareToConvertToOrFromL( aCharacterSetIdentifier, iFs ) !=
        CCnvCharacterSetConverter::EAvailable )
        {
        User::Leave( KErrNotSupported );
        }

    //TODO:: need to set endianess if required.
    //TODO:: below line is not necessary i guess???
    //The missing character is simply replaced by the Unicode character which represents unknown characters (0xFFFD)
    //iCharConv->SetReplacementForUnconvertibleUnicodeCharactersL( KReplaceChar8 );

    TBuf16<40> outputBuffer;
    TPtrC8 remainderOfinputBuf ( aText );
    //TODO: KStateDefault header include
    TInt aState = 0; // KStateDefault;

    FOREVER
        {
        const TInt doneAll = iCharConv->ConvertToUnicode( outputBuffer, remainderOfinputBuf, aState );

        if ( doneAll == CCnvCharacterSetConverter::EErrorIllFormedInput )
            {
            User::Leave( KErrCorrupt );
            }
        else if ( doneAll < 0 )
            {
            // For future expansion of errors
            User::Leave( KErrGeneral );
            }
        else
            { // lint
            }

        // Make sure outputBuffer fits into resultBuffer
        while ( resultBuffer->Length() + outputBuffer.Length() > maxLength )
            {
            // Increase resultBuffer length by origLength
            maxLength += origLength;
            resultBuffer = resultBuffer->ReAllocL( maxLength );
            }
        // Append to result
        resultBuffer->Des().Append( outputBuffer );

        if ( doneAll == 0 )
            {                       
            /* Testing */
            /* start --- 
            resultBuffer = resultBuffer->ReAllocL( maxLength + 20);
            resultBuffer->Des().Append(_L("Korean Enc:415-5434"));
             ---- End */            
            return resultBuffer; // All converted
            }
        //TODO:: set the buffer correctly w.r.t length, Remember this is HBUFC16
        remainderOfinputBuf.Set( remainderOfinputBuf.Right( doneAll ));
        }
    }

// ---------------------------------------------------------
// CMsgTextUtils::ConvertPtrToDesC16
// Converts input 8-bit data buffer to 16-bit data buffer.
// ---------------------------------------------------------
EXPORT_C void CMsgTextUtils::ConvertPtrToDesC16( const TDes8& aFromBuff8, TDes16& aToBuff16 )
    {
    TInt i = 0;
    TInt j = 0;

    aToBuff16.FillZ( aToBuff16.MaxSize()/2 ); //MaxSize returns in bytes, hence devide by 2
    while( j < (aFromBuff8.Length() - 1) ) 
        {
        aToBuff16[i] = aFromBuff8[j++] & 0xff; // low byte
        aToBuff16[i++] |= (aFromBuff8[j++] << 8); // high byte
        }
    aToBuff16.SetLength(i);
    }

//  End of File
