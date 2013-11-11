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
* Description: Abstract base class for encoder output.
*
*/


#ifndef CPOSLMOUTPUT_H
#define CPOSLMOUTPUT_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class RWriteStream;

// CLASS DECLARATION

/**
*  Abstract base class for encoder output.
*
*/
class CPosLmOutput : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmOutput();

    public: // New functions

        /**
        * Writes the content of the 16 bit descriptor to this output.
        *
        * @param aDes A reference to a descriptor.
        */
        void WriteL(
        /* IN  */   const TDesC16& aDes
        );

        /**
        * Writes the content of the 16 bit descriptor to this output.
        * Deletes the descriptor (using CleanupStack::PopAndDestroy()).
        *
        * @param aDes A reference to a descriptor. Must have been pushed to the
        *   CleanupStack.
        */
        void WriteAndDeleteL(
        /* IN  */   TDesC16* aDes
        );

        /**
        * Writes a TReal value as a string to this output.
        *
        * @param aValue The value to be written.
        */
        void WriteTRealL(
        /* IN  */   TReal aValue
        );

        /**
        * Writes a TUint value as a string to this output.
        *
        * @param aValue The value to be written.
        */
        void WriteTUintL(
        /* IN  */   TUint aValue
        );

        /**
        * Ensures that any buffered data is written to this output.
        */
        virtual void CommitL();

        /**
        * Optionally performs a low disk check by the sub class. Leaves with
        * KErrDiskFull if the disk is full.
        *
        * @param aDesc A descriptor that should be written to the output.
        */
        virtual void CheckLowDiskL(
        /* IN  */   const TDesC16& aDesc
        ) = 0;

    protected:  // New functions

        /**
        * C++ default constructor.
        */
        CPosLmOutput();

        /**
        * Symbian 2nd phase constructor.
        *
        * @param aWriteStream The write stream to be used by this output.
        */
        void BaseConstructL(
        /* IN  */   RWriteStream& aWriteStream
        );

    private:

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmOutput( const CPosLmOutput& );
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmOutput& operator=( const CPosLmOutput& );

    private:    // Data

        RWriteStream*   iWriteStream;  // No ownership
    };

#endif      // CPOSLMOUTPUT_H

// End of File
