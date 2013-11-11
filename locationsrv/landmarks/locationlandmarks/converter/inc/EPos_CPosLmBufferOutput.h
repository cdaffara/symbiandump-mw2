/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:Buffer output for encoder.
*
*/


#ifndef CPOSLMBUFFEROUTPUT_H
#define CPOSLMBUFFEROUTPUT_H

//  INCLUDES
#include <s32mem.h>
#include "EPos_CPosLmOutput.h"

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Buffer output for encoder.
*/
class CPosLmBufferOutput : public CPosLmOutput
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param anExpandSize The granularity of the buffer: each segment
        *   contains (in addition to 16 bytes of overhead) this number of
        *   bytes for data.
        * @returns A new instance of this class.
        */
        static CPosLmBufferOutput* NewL(
        /* In  */   TInt anExpandSize
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmBufferOutput();

    public: // Functions from base classes

        /**
        * From CPosLmOutput.
        *
        * Ensures that any buffered data is written to the buffer. The buffer
        * is also compressed.
        */
        void CommitL();

        /*
        * From CPosLmOutput.
        *
        * @param aDesc A descriptor that should be written to the output.
        */
        void CheckLowDiskL(
        /* IN  */   const TDesC16& aDesc
        );

    public: // New functions

        /**
        * Get the output buffer.
        * The caller takes ownership of buffer.
        *
        * @return The output buffer.
        */
        CBufBase* BufferLC();

    private:

        /**
        * C++ default constructor.
        */
        CPosLmBufferOutput();

        /**
        * By default Symbian 2nd phase constructor is private.
        *
        * @param anExpandSize The granularity of the buffer: each segment
        *   contains (in addition to 16 bytes of overhead) this number of
        *   bytes for data.
        */
        void ConstructL(
        /* In  */   TInt anExpandSize
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmBufferOutput( const CPosLmBufferOutput& );
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmBufferOutput& operator=( const CPosLmBufferOutput& );

    private:    // Data

        TBool               iOwnsBuffer;
        CBufBase*           iBuffer;
        RBufWriteStream     iBufWriteStream;

    };

#endif      // CPOSLMBUFFEROUTPUT_H

// End of File
