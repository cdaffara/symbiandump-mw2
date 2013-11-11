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
* Description: File output for encoder.
*
*/


#ifndef CPOSLMFILEOUTPUT_H
#define CPOSLMFILEOUTPUT_H

//  INCLUDES
#include <f32file.h>
#include <s32file.h>
#include "EPos_CPosLmOutput.h"

// FORWARD DECLARATIONS
class CPosLmDiskUtilities;

// CLASS DECLARATION

/**
*  File output for encoder.
*/
class CPosLmFileOutput : public CPosLmOutput
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @param aOutputFile The file name to write the encoded data to.
        * @returns A new instance of this class.
        */
        static CPosLmFileOutput* NewL(
        /* IN */        const TDesC&  aOutputFile
        );

        /**
        * Destructor.
        */
        virtual ~CPosLmFileOutput();

    public: // Functions from base classes

        /*
        * From CPosLmOutput.
        *
        * @param aDesc A descriptor that should be written to the output.
        */
        void CheckLowDiskL(
        /* IN  */   const TDesC16& aDesc
        );

    private:

        /**
        * C++ default constructor.
        */
        CPosLmFileOutput();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
        /* IN */        const TDesC&  aOutputFile
        );

        // Prohibit copy constructor if not deriving from CBase.
        CPosLmFileOutput( const CPosLmFileOutput& );
        // Prohibit assigment operator if not deriving from CBase.
        CPosLmFileOutput& operator=( const CPosLmFileOutput& );

    private:    // Data

        RFs                  iFs;
        RFileWriteStream     iFileWriteStream;
        CPosLmDiskUtilities* iDiskUtilities;
        TChar                iDbDrive;

    };

#endif      // CPOSLMFILEOUTPUT_H

// End of File
