/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizer for MP3 files.
*
*/



#ifndef CMP3AUDIOPLAYCONTROLLERRECOGNIZER_H
#define CMP3AUDIOPLAYCONTROLLERRECOGNIZER_H

// INCLUDES
#include <e32base.h>
#include <apmrec.h>
#include <apmstd.h>

// CLASS DECLARATION

/**
* Concrete implementation of the recognizer API used to recognize MP3 files.
*
*  @lib MP3AudioPlayControllerRecognizer.lib
*  @since 3.0
*/
class CMP3AudioPlayControllerRecognizer : public CApaDataRecognizerType
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMP3AudioPlayControllerRecognizer* NewL();

        /**
        * Destructor.
        */
        virtual ~CMP3AudioPlayControllerRecognizer();

        /**
        * Creates recognizer
        * @since 3.0
        * @param  none
        * @return recognizer instance
        */
		static CApaDataRecognizerType* CreateRecognizerL();

    private: // Functions from base classes

        /**
        * From CApaDataRecognizerType
        * Returns one of the SupportedDataType
        * @since 3.0
        * @param aIndex - depending on the file extension
        * @return Supported DataType
        */
        TDataType SupportedDataTypeL( TInt aIndex ) const;

        /**
        * From CApaDataRecognizerType
        * Returns the buffersize needed to scan
        * @since 3.0
        * @param  none
        * @return BufferSize if needed
        */
		TUint PreferredBufSize();

        /**
        * From CApaDataRecognizerType
        * Do the recognition of the file using the name of the
        * the file or the buffer to scan
        * @since 3.0
        * @param aName    Name of the file
        * @param aBuffer  Buffer that shall be scanned for recognition
        * @return void
        */
		void DoRecognizeL( const TDesC& aName, const TDesC8& aBuffer );

    private:

        /**
        * C++ default constructor.
        */
        CMP3AudioPlayControllerRecognizer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};


#endif	// CMP3AUDIOPLAYCONTROLLERRECOGNIZER_H
