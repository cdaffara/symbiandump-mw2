/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  RingToneRecognizer
*
*/

#ifndef RECRT_H
#define RECRT_H

// INCLUDES
#include <apmrec.h>

// CONSTANTS
const TInt KRingingToneRecognizerImplementationUid = 0x101F8828;


/**
*  CApaRingingToneRecognizer.
*  Recognizer plug-in for ringing tones.
*/
class CApaRingingToneRecognizer : public CApaDataRecognizerType
    {
    public:  // Constructor and destructor
        /**
        * C++ default constructor.
        */
        CApaRingingToneRecognizer();

        /**
        * Destructor.
        */
        virtual ~CApaRingingToneRecognizer();

    public: // New functions

    static CApaDataRecognizerType* CreateRecognizerL();

    public: // Functions from base classes
        /**
        * From CApaDataRecognizerType
        */
        TUint PreferredBufSize();

        /**
        * From CApaDataRecognizerType
        */
        TDataType SupportedDataTypeL(TInt aIndex) const;

    private: // Functions from base classes
        /**
        * From CApaDataRecognizerType
        */
        void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
    };

#endif // RECRT_H
