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
* Description:  Class for parsing start of landmarks exchange format file.
*
*/


#ifndef CPOSLMSIMPLEPARSER_H
#define CPOSLMSIMPLEPARSER_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>

// CLASS DECLARATION

/**
* Class for parsing start of landmarks exchange format file.
*/
NONSHARABLE_CLASS(CPosLmSimpleParser : public CBase)
    {

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * This constructor is used to parse from a file.
        */
    static CPosLmSimpleParser* NewLC(
        /* IN  */       const RFile& aFile,
        /* IN  */       TInt aBufferSize
        );

        /**
        * Two-phased constructor.
        *
        * This constructor is used to parse from a buffer.
        */
    static CPosLmSimpleParser* NewLC(
        /* IN  */       const TDesC8& aBuffer
        );

        /**
        * Destructor.
        */
        ~CPosLmSimpleParser();

    public: // Functions from base classes

        /**
        * Reads the next character from the input data.
        *
        * This method moves the read pointer to the next character.
        *
        * @param aChar On return, contains the next character.
        * @return @p KErrNone if successful, @p KErrEof if there is no more
        *   input data to parse, otherwise a system-wide error code.
        */
        TInt NextChar(
        /* OUT */       TUint8& aChar
        );

        /**
        * Parses the input until a certain string is found.
        *
        * The client can also pass an optional abort string to this method.
        * If the abort string is found before the other string, this method
        * will stop parsing. This is useful for instance for checking
        * whether a string is found before an xml tag end (">").
        *
        * This method moves the read pointer to the position after the string
        * or abort string whichever is found first.
        *
        * @param aString The string to search for.
        * @param aAbortString If this string is found, searching for
        *   @p aString is aborted.
        * @return @p KErrNone if the string was found, @p KErrEof if there is
        *   no more input data to parse, @p KErrNotFound if the string was not
        *   found before the abort string, otherwise a system-wide error code.
        */
        TInt FindString(
        /* IN  */       const TDesC8& aString,
        /* IN  */       const TDesC8& aAbortString = KNullDesC8
        );

        /**
        * Checks if the next characters to parse equals a certain string.
        *
        * This method does not increment the read pointer.
        *
        * @param aString The string to look for.
        * @return @p KErrNone if the string is found at the read pointer
        *   position, @p KErrNotFound if not found or otherwise a
        *   system-wide error code.
        */
        TInt CheckForStringNext(
        /* IN  */       const TDesC8& aString
        );

        /**
        * Moves the read pointer to the next character that is not a white
        * space, line feed, carrige return, form feed or a tab.
        */
        void SkipSpaces();
        
        /**
        * Moves the read pointer past any existing sequence of 
        * 0xEF 0xBB 0xBF, which is the Byte Order Mark for UTF-8
        * (see http://www.w3.org/TR/2004/REC-xml-20040204/#sec-guessing).
        */
        void SkipUTF8Marks();
        
        /** Unwinds parser to the beginning of content */
        void Restart();
        
    private:

        /**
        * C++ default constructor.
        */
        CPosLmSimpleParser();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN  */       const RFile& aFile,
        /* IN  */       TInt aBufferSize
        );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN  */       const TDesC8& aBuffer
        );

        /**
        * If the data in the buffer has been handled, this method reads new
        * data from the input to the buffer.
        *
        * The client can specify that a number of characters from the old
        * buffer should be included in the new buffer as well. This is needed
        * when searching for a string because the string may be split in two
        * when reading to the buffer.
        *
        * @param aNumReuseChar The number of characters to reuse in the new
        *   buffer.
        * @return @p KErrNone if successful, @p KErrEof if the end of the
        *   input has been reached, or otherwise a system-wide error code.
        */
        TInt ReadToBufferIfNeeded(
        /* IN  */       TInt aNumReuseChar = 0);

    private:  // Data

        const RFile* iFile;

        HBufC8* iBufferSpace;
        const TDesC8* iBuffer;

        TInt iBufferPtr;

        TInt iFileOffset;

        TBool iEndOfFile;
    };

#endif      // CPOSLMSIMPLEPARSER_H

// End of File
