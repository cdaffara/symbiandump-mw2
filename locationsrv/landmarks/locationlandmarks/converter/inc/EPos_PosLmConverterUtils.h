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
* Description:Helper class for using strings/Uri etc.
*
*/



#ifndef POSLMCONVERTERUTILS_H
#define POSLMCONVERTERUTILS_H

//  INCLUDES
#include <e32base.h>
#include <e32math.h>
#include <EPos_Landmarks.h>

// CLASS DECLARATION

/**
*  Helper class for using strings/Uri etc.
*/
class PosLmConverterUtils
    {
    public: // New functions

        /**
        * Checks wheter string is a token of correct length or not.
        *
        * @param aDesC A token.
        * @param aCorrectLength A correct length.
        * @return Whether string is a token with correct length or not.
        */
        static TBool IsTokenL(
        /* IN  */   const TDesC& aDesC,
        /* IN  */   TInt aCorrectLength
        );

        /**
        * Separate a mediaLink into a mimetype and a url.
        *
        * @param aMediaLink A mediaLink to separate.
        * @param aMimeType A mimetype to return.
        * @param aUrl A url to return.
        * @return KErrNone if separation was ok, otherwise KErrNotFound.
        */
        static TInt SeparateMediaLink(
        /* IN  */   const TDesC& aMediaLink,
        /* OUT  */  HBufC* aMimeType,
        /* OUT  */  HBufC* aUrl
        );

        /**
        * Concatenate a mimetype and a Uri into a MediaLink.
        *
        * @param aMimeType A mimetype to concatenate.
        * @param aUrl A url to concatenate.
        * @return A string with format mimeType/Url.
        */
        static HBufC* ConcatenateMediaLinkLC(
        /* IN  */   const TDesC& aMimeType,
        /* IN  */   const TDesC& aUrl
        );

        /**
        * Checks if string is a correct Uri.
        *
        * @param aDesC A string to check.
        * @return Whether string is a valid Uri or not.
        */
        static TBool IsAnyUri(
        /* IN  */   const TDesC& aDesC
        );

        /**
        * Converts a TReal to a descriptor.
        *
        * @param aReal A real to convert.
        * @param aDes A converted string.
        */
        static void RealToDesL(
        /* IN   */  TReal aReal,
        /* OUT  */  TDes& aDes
        );

        /**
        * Converts a descriptor to a TReal32.
        *
        * @param aDes A string to convert.
        * @param aReal A converted real.
        */
        static void DesToRealL(
        /* IN   */  const TDesC& aDes,
        /* OUT  */  TReal32& aReal
        );

        /**
        * Converts a descriptor to a TReal64.
        *
        * @param aDes A string to convert.
        * @param aReal A converted real.
        */
        static void DesToRealL(
        /* IN   */  const TDesC& aDes,
        /* OUT  */  TReal64& aReal
        );
        
        /**
        * Converts a 8-bit descriptor to a TReal32.
        *
        * @param aDes A string to convert.
        * @param aReal A converted real.
        */
        static void DesToRealL(
        /* IN   */  const TDesC8& aDes,
        /* OUT  */  TReal32& aReal
        );

        /**
        * Converts a 8-bit descriptor to a TReal64.
        *
        * @param aDes A string to convert.
        * @param aReal A converted real.
        */
        static void DesToRealL(
        /* IN   */  const TDesC8& aDes,
        /* OUT  */  TReal64& aReal
        );
        
        

        /**
        * Converts a descriptor to a TUint.
        *
        * @param aDes A string to convert.
        * @param aUint A converted TUint.
        */
        static void DesToTUintL(
        /* IN   */  const TDesC& aDes,
        /* OUT  */  TUint16& aUint
        );

        /**
        * Sets TReal32 to NaN.
        *
        * @param aReal A real to set to NaN.
        */
        static void SetNaN(
        /* IN/OUT */ TReal32& aReal
        );

        /**
        * Sets TReal64 to NaN.
        *
        * @param aReal A real to set to NaN.
        */
        static void SetNaN(
        /* IN/OUT */ TReal64& aReal
        );

        /**
        * Sets TReal32 to infinite.
        *
        * @param aNegative ETrue, the value is a negative infinite
        *   EFalse, the value is a positive infinite.
        * @param aReal A real to set to infinite.
        */
        static void SetInfinite(
        /* IN  */       TBool aNegative,
        /* IN/OUT */    TReal32& aReal
        );

        /**
        * Sets TReal64 to infinite.
        *
        * @param aNegative ETrue, the value is a negative infinite
        *   EFalse, the value is a positive infinite.
        * @param aReal A real to set to infinite.
        */
        static void SetInfinite(
        /* IN  */       TBool aNegative,
        /* IN/OUT */    TReal64& aReal
        );

        /**
        * Assert expression, otherwise function will leave with error code.
        *
        * @param aBool A expression to evaluate.
        * @param aErrorCode A leavecode if expression is false.
        */
        static void AssertL(
        /* IN */    TBool aCondition,
        /* IN */    TInt aErrorCode = KErrPosLmUnknownFormat
        );

        /**
         * Converts a string from UTF-8 to Unicode.
         *
         * @param aUtf8 A string in UTF-8 format.
         * @return A unicode string.
         */
        static HBufC* Utf8ToUnicodeLC( const TDesC8& aUtf8 );
        
        /**
        * Converts a string from UTF-8 to Unicode.
        *
        * @param aUtf8 A string in UTF-8 format.
        * @param aUnicode A unicode string.
        */
        static void Utf8ToUnicodeL(
        /* IN */    const TDesC8& aUtf8,
        /* IN */    TDes16& aUnicode
        );

        /**
        * Escapes the string with characters not supported in XML.
        *
        * @param aStrToEscape The string to escape.
        * @return An escaped string.
        */
        static HBufC* EscapeStringToXmlLC(
        /* IN */        const TDesC& aStrToEscape
        );

        /**
        * Checks if a string only contains whitespaces or not.
        *
        * @param aStr The string to check.
        * @return ETrue if string only contain whitespaces, otherwise EFalse.
        */
        static TBool OnlyWhiteSpaces(
        /* IN */        const TDesC8& aStr
        );

    private:

        static TInt SeparateMediaLink(
        /* IN   */  const TDesC& aUrlSeparator,
        /* IN   */  const TDesC& aMediaLink,
        /* OUT  */  HBufC* aMimeType,
        /* OUT  */  HBufC* aUrl
        );

        static void EscapeString(
        /* IN/OUT */    TDes& aStrToEscape,
        /* IN */        const TDesC& aFromStr,
        /* IN */        const TDesC& aToStr
        );

        static TInt CalculateEscapedStringSize(
        /* IN */        const TDesC& aStrToEscape
        );

        static TInt CalculateIncrease(
        /* IN */        const TDesC& aStrToEscape,
        /* IN */        const TDesC& aFromStr,
        /* IN */        const TDesC& aToStr
        );

        static TBool ConvertIfTRealSpecialValueL(
        /* IN  */   const TDesC& aDes,
        /* OUT  */  TReal64& aReal
        );

        static TBool ConvertIfTRealSpecialValueL(
        /* IN  */   const TDesC& aDes,
        /* OUT  */  TReal32& aReal
        );
        
        static TBool ConvertIfTRealSpecialValueL(
        /* IN  */   const TDesC8& aDes,
        /* OUT  */  TReal64& aReal
        );

        static TBool ConvertIfTRealSpecialValueL(
        /* IN  */   const TDesC8& aDes,
        /* OUT  */  TReal32& aReal
        );

        static void ValidateConversionL(
        /* IN  */   const TLex& aLexer,
        /* IN  */   TInt aStatus
        );
       
        static void ValidateConversionL(
        /* IN  */   const TLex8& aLexer,
        /* IN  */   TInt aStatus
        );
    };

#endif      // POSLMCONVERTERUTILS_H

// End of File
