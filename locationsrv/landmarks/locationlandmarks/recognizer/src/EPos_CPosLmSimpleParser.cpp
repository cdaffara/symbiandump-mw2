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
* Description:  Landmark iterator.
*
*/


#include    "EPos_CPosLmSimpleParser.h"

#ifdef _DEBUG
_LIT(KPosLmRecognizerPanicCategory, "LmExcRec");
enum TPosLmRecognizerPanics
    {
    EPosLmTooManyCharactersReused = 1 /**
        It's only allowed to reuse up to half the buffer size. */
    };
#endif

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmSimpleParser* CPosLmSimpleParser::NewLC(
    const RFile& aFile, TInt aBufferSize)
    {
    CPosLmSimpleParser* self = new (ELeave) CPosLmSimpleParser();
    CleanupStack::PushL(self);
    self->ConstructL(aFile, aBufferSize);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmSimpleParser* CPosLmSimpleParser::NewLC(
  const TDesC8& aBuffer)
    {
    CPosLmSimpleParser* self = new (ELeave) CPosLmSimpleParser();
    CleanupStack::PushL(self);
    self->ConstructL(aBuffer);
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmSimpleParser::CPosLmSimpleParser()
    : iFile(NULL), iBuffer(NULL), iBufferPtr(0), iFileOffset(0),
    iEndOfFile(EFalse)
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmSimpleParser::ConstructL(const RFile& aFile, TInt aBufferSize)
    {
    iFile = &aFile;
    iBufferSpace = HBufC8::NewL(aBufferSize);
    iBuffer = iBufferSpace;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmSimpleParser::ConstructL(const TDesC8& aBuffer)
    {
    iBuffer = &aBuffer;
    // The whole buffer is kept in memory so we have reached EOF.
    iEndOfFile = ETrue;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmSimpleParser::~CPosLmSimpleParser()
    {
    delete iBufferSpace;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmSimpleParser::NextChar(TUint8& aChar)
    {
    TInt err = ReadToBufferIfNeeded();
    if (err)
        {
        return err;
        }

    aChar = (*iBuffer)[iBufferPtr++];
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmSimpleParser::FindString(
    const TDesC8& aString, const TDesC8& aAbortString)
    {
    // Make sure that the buffer contains more characters than
    // aString and aAbortString do.
    TInt maxStringLength =
        Max(aString.Length(), aAbortString.Length());
    TInt err = ReadToBufferIfNeeded(maxStringLength);

    while (!err)
        {
        // buf is a descriptor pointing to the data left to parse in the
        // buffer.
        TPtrC8 buf = iBuffer->Mid(iBufferPtr);
        TInt stringPos = buf.Find(aString);

        TInt abortStringPos = KErrNotFound;
        if (aAbortString.Length() != 0)
            {
            abortStringPos = buf.Find(aAbortString);
            }

        if (stringPos != KErrNotFound && (abortStringPos == KErrNotFound ||
           stringPos + aString.Length() < abortStringPos +
           aAbortString.Length()))
            {
            // aString was found before any occurance of aAbortString. Update
            // the read pointer to point to the first position after aString.
            iBufferPtr += stringPos + aString.Length();
            return KErrNone;
            }
        else if (abortStringPos != KErrNotFound)
            {
            // aAbortString was found. Abort the search and set the read
            // pointer to point to the first position after aAbortString.
            iBufferPtr += abortStringPos + aAbortString.Length();
            return KErrNotFound;
            }
        else
            {
            // Neither aString or aAbortString was found in the buffer. Read
            // new data from the input to the buffer and continue searching.
            iBufferPtr = iBuffer->Length() - maxStringLength;
            err = ReadToBufferIfNeeded(maxStringLength);
            }
        }

    return err;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmSimpleParser::CheckForStringNext(const TDesC8& aString)
    {
    // Make sure that the buffer contains more characters than
    // aString does.
    TInt err = ReadToBufferIfNeeded(aString.Length());
    if (err)
        {
        return err;
        }

    TPtrC8 buf = iBuffer->Mid(iBufferPtr);
    if ( buf.Length() >= aString.Length() )
        {
        if ( aString.Compare( buf.Left( aString.Length() ) ) == 0 )
            {
            return KErrNone;
            }
        else
            {
            return KErrNotFound;
            }
        }
    else
        {
        return KErrEof; // not enough data in buffer to check aString
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmSimpleParser::SkipSpaces()
    {
    TInt err = KErrNone;
    TUint8 chr = 0;
    do
        {
        err = NextChar(chr);
        } while (!err && TChar(chr).IsSpace());

    if (!err)
        {
        iBufferPtr--;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmSimpleParser::SkipUTF8Marks()
    {
    TInt err = KErrNone;
    TUint8 chr = 0;
    
    // Filter the UTF-8 Byte Order Marks
    // "EF BB BF"
    err = NextChar(chr);
    if (!err && chr != 0xEF)
        {
        iBufferPtr-=1;
        }
    else
        {
        err = NextChar(chr);
        if (!err && chr != 0xBB)
            {
            iBufferPtr-=2;
            }
        else
            {
            err = NextChar(chr);
            if (!err && chr != 0xBF)
                {
                iBufferPtr-=3;
                }
            }
        }
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmSimpleParser::ReadToBufferIfNeeded(TInt aNumReuseChar)
    {
    // If there are not enough characters left in the buffer, read new data
    // from the input.
    if (iBufferPtr >= iBuffer->Length() - aNumReuseChar)
        {
        if (iEndOfFile)
            {
            // Last read operation encountered the end of the file, or
            // the input is a buffer.
            return KErrEof;
            }

        TPtr8 buf = iBufferSpace->Des();
        __ASSERT_DEBUG(aNumReuseChar <= buf.MaxLength() / 2,
                        User::Panic(KPosLmRecognizerPanicCategory,
                        EPosLmTooManyCharactersReused));

        // Read data from the input to the buffer.
        TInt err = iFile->Read(Max(0, iFileOffset - aNumReuseChar), buf);
        if (err)
            {
            return err;
            }
        if (buf.Length() == 0)
            {
            return KErrEof;
            }

        if (buf.Length() != buf.MaxLength())
            {
            // The end of the file was encountered.
            iEndOfFile = ETrue;
            }

        iFileOffset += buf.Length() - aNumReuseChar;
        iBufferPtr = 0;
        }
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmSimpleParser::Restart()
    {
    iBufferPtr = 0;
    if ( iFile )
        {
        iFileOffset = 0;
        iFile->Seek( ESeekStart, iFileOffset );
        iEndOfFile = 0;
        }
    if ( iBufferSpace )
        {
        iBufferSpace->Des().Zero();
        }
    }
