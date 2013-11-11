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




#ifndef MSGTEXTUTILS_H
#define MSGTEXTUTILS_H

//  INCLUDES

#include <e32base.h>
#include <f32file.h>
#include <charconv.h>
#include <cmsvattachment.h>

// CONSTANTS

// Character sets' MIB enumerations
const TUint KCharacterSetMIBEnumUtf8 = 106;
const TUint KCharacterSetMIBEnumUsAscii = 3;
const TUint KCharacterSetMIBEnumIso10646Ucs2 = 1000; // 1000
const TUint KCharacterSetMIBEnumUTF16BE = 1013; // 1013
const TUint KCharacterSetMIBEnumUTF16LE = 1014; // 1014
const TUint KCharacterSetMIBEnumUTF16 = 1015; // 1015

const TUint KCharacterSetMIBEnumISO_8859_1 = 4;
const TUint KCharacterSetMIBEnumISO_8859_2 = 5;
const TUint KCharacterSetMIBEnumISO_8859_3 = 6;
const TUint KCharacterSetMIBEnumISO_8859_4 = 7;
const TUint KCharacterSetMIBEnumISO_8859_5 = 8;
const TUint KCharacterSetMIBEnumISO_8859_6 = 9;
const TUint KCharacterSetMIBEnumISO_8859_7 = 10;
const TUint KCharacterSetMIBEnumISO_8859_8 = 11;
const TUint KCharacterSetMIBEnumISO_8859_9 = 12;
const TUint KCharacterSetMIBEnumISO_8859_10 = 13;
const TUint KCharacterSetMIBEnumISO_8859_13 = 109;
const TUint KCharacterSetMIBEnumISO_8859_14 = 110;
const TUint KCharacterSetMIBEnumISO_8859_15 = 111;

const TUint KCharacterSetMIBEnumUtf7 = 1012;
const TUint KCharacterSetMIBEnumCodePage1252 = 2252;

// Chinese charsets
const TUint KCharacterSetMIBEnumGb2312 = 2025;
const TUint KCharacterSetMIBEnumHz = 2085;
//const TUint KCharacterSetMIBEnumGb12345 =
const TUint KCharacterSetMIBEnumGbk = 113;
const TUint KCharacterSetMIBEnumBig5 = 2026;

// Japanese charset
const TUint KCharacterSetMIBEnumShiftJis = 17;
const TUint KCharacterSetMIBEnumIso2022Jp = 39;
//const TUint KCharacterSetMIBEnumIso2022Jp1 =
const TUint KCharacterSetMIBEnumJis = 16;
const TUint KCharacterSetMIBEnumEucJpPacked = 18;

// Default charsets - moved from cpp
const TUint KDefaultCharConvCharset = KCharacterSetIdentifierAscii;
const TUint KDefaultMIBCharset = KCharacterSetMIBEnumUsAscii;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CRichText;
class MMsvAttachmentManager;

// CLASS DECLARATION

/**
* CMsgTextUtils - Miscellaneous UI indenependent utilities.
*
* @lib UniDataUtils.lib
* @since 3.1
*/
class CMsgTextUtils : public CBase
    {
    public:  // Enums
    
        enum TConversionModeFlags
            {
            ECRLFtoLF = 0x1
            };
     
    public:  // Constructors and destructor

        /**
        * Constructor.
        *
        * @since    3.1
        * @param    aFs     IN Fileserver session.
        * @return   Pointer to instance
        */
        IMPORT_C static CMsgTextUtils* NewL( RFs& aFs );

        /**
        * Destructor.
        *
        * @since 3.1
        */
        virtual ~CMsgTextUtils();

        /**
        * ConvertToBufferLC
        * Converts text to given character set.
        * Uses underscores ('_') as replacement characters
        * 
        * @since    3.1
        * @param    aText   IN Text to be converted
        * @param    aCharacterSetIdentifier IN CharConv character
        *                                      set identifier
        * @return   Converted text 8-bit buffer in cleanup stack
        */
        IMPORT_C HBufC8* ConvertToBuffer8LC(
            const TDesC& aText,
            TUint aCharacterSetIdentifier );

        /**
        * ConvertToBufferLC
        * Converts text to given character set.
        * Uses underscores ('_') as replacement characters
        * 
        * @since    3.1
        * @param    aText   IN Text to be converted
        * @param    aCharacterSetIdentifier IN CharConv character
        *                                      set identifier
        * @return   Converted text 16-bit buffer in cleanup stack
        */
        IMPORT_C HBufC* ConvertToBuffer16LC(
            const TDesC& aText,
            TUint aCharacterSetIdentifier );

        /**
        * ConvertToFileL
        * Writes text to file with given character set encoding.
        * 
        * @since    3.1
        * @param    aText   IN Text to be converted
        * @param    aFile   IN Open file handle (writeable)
        *                      Takes ownership!
        * @param    aCharacterSetIdentifier IN CharConv character
        *                                      set identifier
        */
        IMPORT_C void ConvertToFileL(
            const TDesC& aText,
            RFile& aFile,
            TUint aCharacterSetIdentifier );

        /**
        * CharconvIdToMibIdL
        * Converts between id's.
        * 
        * @since    3.1
        * @param    aCharconvCharsetId  IN charconv id
        * @return   MIB enumeration
        */
        IMPORT_C TUint CharconvIdToMibIdL( TUint aCharconvCharsetId );

        /**
        * MibIdToCharconvIdL
        * Converts between id's.
        * 
        * @since    3.1
        * @param    aMibId  IN MIB enumeration
        * @return   Charconv id
        */
        IMPORT_C TUint MibIdToCharconvIdL( TUint aMibId );
        
        /**
        * ConvertParagraphSeparatorsLC
        * Convert Unicode Paragraph separators (0x2029) to CRLF (0x000d 0x000a)
        * 
        * @since    3.1
        * @param aText Text to convert
        * @return Pointer to converted buffer in cleanup stack.
        */
        IMPORT_C static HBufC* ConvertParagraphSeparatorsLC( const TDesC& aText );

        /**
        * TrimAndRemoveNonAlphaDigit
        * Removes spaces from start and end of the string. 
        * Replaces all non-alpha-digit characters with underscore "_".
        * Replaces all non-ASCII characters with underscore "_"
        * 
        * @since    3.1
        * @param    aString IN/OUT string to be modified
        */
        IMPORT_C static void TrimAndRemoveNonAlphaDigit( TDes& aString );

        /** 
        * Gets byte-size of UTF8 formatted text
        *
        * @since    3.1
        * @param aText
        * @return Size of text in bytes
        */
        IMPORT_C static TInt UTF8Size( TPtrC aText );
        
        /**
        * Performs wanted linebreak converts.
        *
        * @since 3.1
        * @param aText
        * @param aMode Flag indicating to 
        */
        IMPORT_C static void ConvertLineBreaksL( CRichText& aText, TInt aMode );

        /**
        * Tries to recognize character set for given text file. 
        * 
        * @param    aFs         IN File Server session.
        * @param    aFile       IN (Text) file to be recognized
        * @return   CharConv id of the character set
        */
        IMPORT_C static TUint CMsgTextUtils::RecognizeCharSetL( RFs& aFs, RFile& aFile );

        /*
        * GetSafeAttachmentNameLC
        * Generates safe file name (or content location) for an attachment
        * Same message must not include several attachments with the same
        * file name (or content location).
        *
        * Converts non-ascii and non-alpha-digit characters to underscore.
        *
        * Generates max 8 characters long file names.
        * Replaces last 3 charaters with running number
        * if file name (content location) already exists:
        *
        * 1st: "FileName.txt" -> "FileName.txt"
        * 2nd: "FileName.txt" -> "FileN001.txt"
        * 3rd: "FileName.txt" -> "FileN002.txt"
        * etc...
        *
        * @since    3.2
        * @param    aManager    IN reference to attachment manager
        * @param    aFileName   IN/OUT, in: original file name
        *                               out: safe file name
        * @param    aAttachmentId    IN ID of the attachment,
        *                               0 if no attachment exists yet.
        * @param    aContentLocation, if EFalse compares attachment names
        *                             if ETrue compares "ContentLocation" mime headers
        */
        IMPORT_C static HBufC* GetSafeAttachmentNameLC(
            MMsvAttachmentManager& aManager,
            const TDesC& aFileName,
            TMsvAttachmentId aAttachmentId,
            TBool aContentLocation );

        /**
        * Generates a legal filename from given text buffer.
        * @param aFileName  OUT: generated filename.
        * @param aBuffer    text buffer.
        * @param aMaxLength maximum length for filename.
        * @param aExt       optional file extension.
        */
        IMPORT_C static void GetFileNameFromBuffer(
            TFileName&      aFileName,
            const TDesC&    aBuffer,
            TInt            aMaxLength,
            const TDesC*    aExt = NULL );

        /**
        * ConvertToUnicodeL
        * Converts 8-bit data buffer(in given character set) to unicode buffer.
        * Uses underscores ('_') as replacement characters
        * 
        * @since    5.0
        * @param    aText   IN Text(8-bit descriptor) to be converted
        * @param    aCharacterSetIdentifier IN CharConv character set
        *                                      identifier of input 8-bit descriptor
        * @return   Converted text 16-bit buffer
        */
        IMPORT_C HBufC16* ConvertToUnicodeL( const TDesC8& aText, TUint aCharacterSetIdentifier );

        /**
        * ConvertPtrToDesC16
        * Converts 8-bit data buffer to 16-bit data buffer.
        * (similar to pointer typecast in C).
        * 
        * @since    5.0
        * @param    aFromBuff8   IN 8-bit descriptor  
        * @param    aToBuff16    OUT 16-bit descriptor( memory allocated by caller)
        * @return   void
        */
        IMPORT_C void ConvertPtrToDesC16( const TDes8& aFromBuff8, TDes16& aToBuff16);


    protected:

        /**
        * C++ constructor
        * 
        * @since    3.1
        * @param    aFs     IN Fileserver session.
        */
        CMsgTextUtils( RFs& aFs );

        /** 
        * ConstructL
        * Symbian OS constructor
        * 
        * @since    3.1
        */
        void ConstructL();
    
    private: // data
    
        static void DoConvertCRLFL( CRichText& aText );
        
    protected:  // data

        RFs& iFs;
        CCnvCharacterSetConverter* iCharConv;

    };

#endif   // MSGTEXTUTILS_H

// End of file
