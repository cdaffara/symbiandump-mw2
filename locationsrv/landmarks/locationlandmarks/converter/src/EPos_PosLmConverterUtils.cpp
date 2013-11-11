/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Helper class for using strings/Uri etc.
*
*
*/


// INCLUDE FILES
#include <utf.h>
#include <uriutils.h>
#include <EPos_Landmarks.h>
#include "EPos_PosLmConverterUtils.h"
#include "EPos_LmConverter.h"

// CONSTANTS
_LIT(KLineFeed,     "\r");
_LIT(KNewLine,      "\n");
_LIT(KTab,          "\t");
_LIT(KSlash,        "/");

_LIT(KLt,           "<");
_LIT(KGt,           ">");
_LIT(KAmp,          "&");
_LIT(KApos,         "'");
_LIT(KQuot,         "\"");

_LIT(KLtXmlStr,     "&lt;");
_LIT(KGtXmlStr,     "&gt;");
_LIT(KAmpXmlStr,    "&amp;");
_LIT(KAposXmlStr,   "&apos;");
_LIT(KQuotXmlStr,   "&quot;");

_LIT(KPosNaN,       "NaN");
_LIT(KPosInfinity,  "INF");
_LIT(KPosNegativeInfinity, "-INF");

_LIT8(KNaN,       "NaN");
_LIT8(KInfinity,  "INF");
_LIT8(KNegativeInfinity, "-INF");

const TUint KPosXmlRealPoint = '.';

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// PosLmConverterUtils::IsTokenL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::IsTokenL(
    const TDesC& aDesC,
    TInt aCorrectLength)
    {
    if (aDesC.Find(KLineFeed) != KErrNotFound &&
        aDesC.Find(KNewLine) != KErrNotFound &&
        aDesC.Find(KTab) != KErrNotFound)
        {
        return EFalse;
        }

    // Check for leading, trailing or internal sequences of two or more spaces.
    HBufC* copy = aDesC.AllocLC();
    copy->Des().TrimAll();
    TInt trimmedLength = copy->Length();
    CleanupStack::PopAndDestroy(copy);
    if (trimmedLength != aDesC.Length())
        {
        return EFalse;
        }

    if (aDesC.Length() == aCorrectLength)
        {
        return ETrue;
        }
    return EFalse;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SeparateMediaLink
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmConverterUtils::SeparateMediaLink(
    const TDesC& aMediaLink,
    HBufC* aMimeType,
    HBufC* aUrl)
    {
    _LIT(KPosGenericUrlSeparator, "://");
    TInt result = SeparateMediaLink(
        KPosGenericUrlSeparator, aMediaLink, aMimeType, aUrl);
    if (result != KErrNone)
        {
        _LIT(KPosGeneralUrlSeparator, ":");
        result = SeparateMediaLink(
            KPosGeneralUrlSeparator, aMediaLink, aMimeType, aUrl);
        }

    if (*aMimeType == KSlash)
        {
        // Handle mime type "/" as a missing mime type.
        aMimeType->Des().Zero();
        }

    return result;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::ConcatenateMediaLinkLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* PosLmConverterUtils::ConcatenateMediaLinkLC(
    const TDesC& aMimeType,
    const TDesC& aUrl)
    {
    // Concat mimetype + "/" + url
    TInt mimeTypeLen = KSlash().Length();
    if (&aMimeType)
        {
        mimeTypeLen = aMimeType.Length();
        }
    ASSERT( &aUrl );
    TInt urlLen = aUrl.Length();

    HBufC* medialink = HBufC::NewLC(mimeTypeLen + KSlash().Length() + urlLen);
    if (&aMimeType)
        {
        medialink->Des().Append(aMimeType);
        }
    else
        {
        medialink->Des().Append(KSlash);
        }
    // Append delimiter
    medialink->Des().Append(KSlash);
    // Don't need to check aUrl since it is mandatory
    medialink->Des().Append(aUrl);

    return medialink;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::IsAnyUri
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::IsAnyUri(
    const TDesC& aDesC)
    {
    TUriParser uriParser;
    TInt errorCode = uriParser.Parse(aDesC);

    if (errorCode != KErrNone)
        {
        return EFalse;
        }

    if (!uriParser.IsSchemeValid())
        {
        return EFalse;
        }

    TPtrC16 uriNoFrag;
    uriParser.UriWithoutFragment(uriNoFrag);

    TBool hasInvalidChars = UriUtils::HasInvalidChars(uriNoFrag);

    return !hasInvalidChars;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::RealToDesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::RealToDesL(TReal aReal, TDes& aDes)
    {
    if (Math::IsNaN(aReal))
        {
        aDes = KPosNaN;
        }
    else if (Math::IsInfinite(aReal))
        {
        if (aReal > 0)
            {
            aDes = KPosInfinity;
            }
        else
            {
            aDes = KPosNegativeInfinity;
            }
        }
    else
        {
        TRealFormat realFormat;
        realFormat.iPoint = KPosXmlRealPoint;

        User::LeaveIfError(aDes.Num(aReal, realFormat));
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::DesToRealL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::DesToRealL(const TDesC& aDes, TReal32& aReal)
    {
    if (ConvertIfTRealSpecialValueL(aDes, aReal))
        {
        return;
        }

    TLex lexer(aDes);
    TInt status = lexer.Val(aReal, TChar(KPosXmlRealPoint));
    ValidateConversionL(lexer, status);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::DesToRealL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::DesToRealL(const TDesC& aDes, TReal64& aReal)
    {
    if (ConvertIfTRealSpecialValueL(aDes, aReal))
        {
        return;
        }

    TLex lexer(aDes);
    TInt status = lexer.Val(aReal, TChar(KPosXmlRealPoint));
    ValidateConversionL(lexer, status);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::DesToRealL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::DesToRealL(const TDesC8& aDes, TReal32& aReal)
    {
    if (ConvertIfTRealSpecialValueL(aDes, aReal))
        {
        return;
        }

    TLex8 lexer(aDes);
    TInt status = lexer.Val(aReal, TChar(KPosXmlRealPoint));
    ValidateConversionL(lexer, status);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::DesToRealL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::DesToRealL(const TDesC8& aDes, TReal64& aReal)
    {
    if (ConvertIfTRealSpecialValueL(aDes, aReal))
        {
        return;
        }

    TLex8 lexer(aDes);
    TInt status = lexer.Val(aReal, TChar(KPosXmlRealPoint));
    ValidateConversionL(lexer, status);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::DesToTUintL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::DesToTUintL(const TDesC& aDes, TUint16& aUint)
    {
    TLex lexer(aDes);
    TInt status = lexer.Val(aUint, EDecimal);
    ValidateConversionL(lexer, status);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SetNaN
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::SetNaN(TReal32& aReal)
    {
    TRealX floatX;
    floatX.SetNaN();
    aReal = floatX;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SetNaN
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::SetNaN(TReal64& aReal)
    {
    TRealX floatX;
    floatX.SetNaN();
    aReal = floatX;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SetInfinite
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::SetInfinite(TBool aNegative, TReal32& aReal)
    {
    TRealX floatX;
    floatX.SetInfinite(aNegative);
    aReal = floatX;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SetInfinite
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::SetInfinite(TBool aNegative, TReal64& aReal)
    {
    TRealX floatX;
    floatX.SetInfinite(aNegative);
    aReal = floatX;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::AssertL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::AssertL(TBool aCondition, TInt aErrorCode)
    {
    if (!aCondition)
        {
        User::Leave(aErrorCode);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
HBufC* PosLmConverterUtils::Utf8ToUnicodeLC( const TDesC8& aUtf8 )
    {
    HBufC* string = HBufC::NewLC( aUtf8.Length() );
    TPtr16 ptr = string->Des();
    PosLmConverterUtils::Utf8ToUnicodeL( aUtf8, ptr );
    return string;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::Utf8ToUnicodeL( const TDesC8& aUtf8, TDes16& aUnicode )
    {
    // Create a small output buffer
    TBuf16<30> output;
    // Create a buffer for the unconverted text
    TPtrC8 remUtf8(aUtf8);

    TBool convertingReady = EFalse;
    while (!convertingReady) // conversion loop
        {
        // Start conversion. When the output buffer is full, return the
        // number of characters that were not converted
        const TInt err = CnvUtfConverter::ConvertToUnicodeFromUtf8(
            output, remUtf8);

        // check to see that the descriptor isn't corrupt - leave if it is
        if (err == CnvUtfConverter::EErrorIllFormedInput)
            {
            User::Leave(KErrCorrupt);
            }
        else if (err < 0) // future-proof against "TError" expanding
            {
            User::Leave(KErrGeneral);
            }

        // Do something here to store the contents of the output buffer.
        if (aUnicode.Length() + output.Length() > aUnicode.MaxLength())
            {
            User::Leave(KErrNoMemory);
            }
        aUnicode.Append(output);

        // Finish conversion if there are no unconverted characters in the
        // remainder buffer
        if (err == 0)
            {
            convertingReady = ETrue;
            }
        else
            {
            // Remove the converted source text from the remainder buffer.
            // The remainder buffer is then fed back into loop
            remUtf8.Set(remUtf8.Right(err));
            }
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::EscapeStringToXmlLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* PosLmConverterUtils::EscapeStringToXmlLC(
    const TDesC& aStrToEscape)
    {
    HBufC* string = HBufC::NewLC(CalculateEscapedStringSize(aStrToEscape));
    string->Des().Copy(aStrToEscape);

    // Must start with &, because when escaping other characters & will be
    // added to the string.
    TPtr ptr = string->Des();
    EscapeString(ptr, KAmp, KAmpXmlStr);
    EscapeString(ptr, KLt, KLtXmlStr);
    EscapeString(ptr, KGt, KGtXmlStr);
    EscapeString(ptr, KApos, KAposXmlStr);
    EscapeString(ptr, KQuot, KQuotXmlStr);

    return string;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::OnlyWhiteSpaces
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::OnlyWhiteSpaces(
    const TDesC8& aStr)
    {
    for (TInt i=0; i<aStr.Length(); i++)
        {
        TChar ch = TChar(aStr[i]);
        if (!ch.IsSpace())
            {
            return EFalse;
            }
        }

    return ETrue;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::SeparateMediaLink
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmConverterUtils::SeparateMediaLink(
    const TDesC& aUrlSeparator,
    const TDesC& aMediaLink,
    HBufC* aMimeType,
    HBufC* aUrl)
    {
    _LIT(KPosMimeTypeFirstPart, "*/");
    _LIT(KPosMimeTypeSecondPart, "*");
    _LIT(KPosUrlFirstPart, "*?");
    _LIT(KPosUrlSecondPart, "*?");
    const TInt KMimeTypePatternLength = 5;
    const TInt KUrlPatternLength = 14;
    const TInt KFullPatternLength = 20;

    TBuf<KMimeTypePatternLength> mimeTypePattern;
    mimeTypePattern.Append(KPosMimeTypeFirstPart);
    mimeTypePattern.Append(KPosMimeTypeSecondPart);

    TBuf<KUrlPatternLength> urlPattern;
    urlPattern.Append(KPosUrlFirstPart);
    urlPattern.Append(aUrlSeparator);
    urlPattern.Append(KPosUrlSecondPart);

    TBuf<KFullPatternLength> fullPattern;
    fullPattern.Append(mimeTypePattern);
    fullPattern.Append(KSlash);
    fullPattern.Append(urlPattern);

    // Try matching full url pattern
    TInt offset = aMediaLink.Match(fullPattern);
    if (offset != KErrNotFound)
        {
        // The "*/" part
        *aMimeType = aMediaLink.Left(offset+1);

        // The "*/*?<aUrlSeparator>*?" part
        TPtrC remainder = aMediaLink.Mid(offset+1);

        TBuf<KFullPatternLength> remainderFullPattern;
        remainderFullPattern.Append(KPosMimeTypeSecondPart);
        remainderFullPattern.Append(KSlash);
        remainderFullPattern.Append(urlPattern);

        offset = remainder.Match(remainderFullPattern);
        if (offset == KErrNotFound)
            {
            return offset;
            }

        aMimeType->Des().Append(remainder.Left(offset)); // The "*" part
        *aUrl = remainder.Mid(offset+1); // The "*?<aUrlSeparator>*?" part
        }
    else
        {
        // Try matching url pattern
        offset = aMediaLink.Match(urlPattern);
        if (offset == KErrNotFound)
            {
            return offset;
            }

        // Make sure that the first part does not include mimeType pattern
        TPtrC firstPart = aMediaLink.Left(offset); // The "*?" part

        offset = firstPart.Match(mimeTypePattern);
        if (offset != KErrNotFound)
            {
            return KErrNotFound;
            }

        *aUrl = aMediaLink;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::EscapeString
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::EscapeString(
    TDes& aStrToEscape,
    const TDesC& aFromStr,
    const TDesC& aToStr)
    {
    TPtrC ptr(aStrToEscape);
    TInt lastPos = 0;

    // Find first occurence of the escape string.
    TInt pos = ptr.Find(aFromStr);

    while (pos != KErrNotFound)
        {
        aStrToEscape.Replace(pos+lastPos, aFromStr.Length(), aToStr);

        // Move ptr forward past the inserted aToStr in aStrToEscape
        pos += aToStr.Length();
        lastPos += pos;
        ptr.Set(aStrToEscape.Right(aStrToEscape.Length() - lastPos));

        // Find next occurence of the escape string.
        pos = ptr.Find(aFromStr);
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::CalculateEscapedStringSize
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmConverterUtils::CalculateEscapedStringSize(
    const TDesC& aStrToEscape)
    {
    return aStrToEscape.Length() +
        CalculateIncrease(aStrToEscape, KAmp, KAmpXmlStr) +
        CalculateIncrease(aStrToEscape, KLt, KLtXmlStr) +
        CalculateIncrease(aStrToEscape, KGt, KGtXmlStr) +
        CalculateIncrease(aStrToEscape, KApos, KAposXmlStr) +
        CalculateIncrease(aStrToEscape, KQuot, KQuotXmlStr);
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::CalculateIncrease
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt PosLmConverterUtils::CalculateIncrease(
    const TDesC& aStrToEscape,
    const TDesC& aFromStr,
    const TDesC& aToStr)
    {
    TPtrC ptr(aStrToEscape);
    TInt occurrences = 0;

    // Find first occurence of the escape string.
    TInt pos = ptr.Find(aFromStr);

    while (pos != KErrNotFound)
        {
        ++occurrences;

        // Move ptr forward past the found occurence.
        ptr.Set(ptr.Right(ptr.Length() - (pos + aFromStr.Length())));

        // Find next occurence of the escaped string.
        pos = ptr.Find(aFromStr);
        }

    // Return the increased (or decreased) size impact on the string to escape.
    return occurrences*(aToStr.Length() - aFromStr.Length());
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::ConvertIfTRealSpecialValueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::ConvertIfTRealSpecialValueL(
    const TDesC& aDes,
    TReal64& aReal)
    {
    if (aDes == KPosNaN)
        {
        SetNaN(aReal);
        return ETrue;
        }
    else if (aDes == KPosInfinity)
        {
        SetInfinite(EFalse, aReal);
        return ETrue;
        }
    else if (aDes == KPosNegativeInfinity)
        {
        SetInfinite(ETrue, aReal);
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::ConvertIfTRealSpecialValueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::ConvertIfTRealSpecialValueL(
    const TDesC& aDes,
    TReal32& aReal)
    {
    if (aDes == KPosNaN)
        {
        SetNaN(aReal);
        return ETrue;
        }
    else if (aDes == KPosInfinity)
        {
        SetInfinite(EFalse, aReal);
        return ETrue;
        }
    else if (aDes == KPosNegativeInfinity)
        {
        SetInfinite(ETrue, aReal);
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::ConvertIfTRealSpecialValueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::ConvertIfTRealSpecialValueL(
    const TDesC8& aDes,
    TReal64& aReal)
    {
    if (aDes == KNaN)
        {
        SetNaN(aReal);
        return ETrue;
        }
    else if (aDes == KInfinity)
        {
        SetInfinite(EFalse, aReal);
        return ETrue;
        }
    else if (aDes == KNegativeInfinity)
        {
        SetInfinite(ETrue, aReal);
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// PosLmConverterUtils::ConvertIfTRealSpecialValueL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool PosLmConverterUtils::ConvertIfTRealSpecialValueL(
    const TDesC8& aDes,
    TReal32& aReal)
    {
    if (aDes == KNaN)
        {
        SetNaN(aReal);
        return ETrue;
        }
    else if (aDes == KInfinity)
        {
        SetInfinite(EFalse, aReal);
        return ETrue;
        }
    else if (aDes == KNegativeInfinity)
        {
        SetInfinite(ETrue, aReal);
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::ValidateConversionL(const TLex& aLexer, TInt aStatus)
    {
    if (aStatus == KErrNone)
        {
        if (aLexer.Remainder() != KNullDesC)
            {
            User::Leave(KErrPosLmUnknownFormat);
            }
        }
    else if (aStatus == KErrGeneral)
        {
        User::Leave(KErrPosLmUnknownFormat);
        }
    else
        {
        User::Leave(aStatus);
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void PosLmConverterUtils::ValidateConversionL(const TLex8& aLexer, TInt aStatus)
    {
    if (aStatus == KErrNone)
        {
        if (aLexer.Remainder() != KNullDesC8 )
            {
            User::Leave(KErrPosLmUnknownFormat);
            }
        }
    else if (aStatus == KErrGeneral)
        {
        User::Leave(KErrPosLmUnknownFormat);
        }
    else
        {
        User::Leave(aStatus);
        }
    }
