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
* Description: Landmark recognizer class.
*
*
*/


#ifndef CPOSLMRECOGNIZER_H
#define CPOSLMRECOGNIZER_H

//  INCLUDES
#include <apmrec.h> // CApaDataRecognizerType
#include <f32file.h>

// FORWARD DECLARATIONS
class CPosLmSimpleParser;

// CLASS DECLARATION

/**
* Landmark recognizer class. Plug-in
* code that implements the CApaDataRecognizerType
* interface used to recognize a specified data format.
* Can examine sample data supplied in a buffer, and return,
* if recognized, the data type of that data.
* A data type is also commonly known as a MIME type.
*/

class CPosLmRecognizer : public CApaDataRecognizerType
    {

    public: // Constructors and destructor

        /**
        * C++ default constructor.
        * Initialise the internal data member iCountDataTypes
        * with the number of MIME types that will be recognized.
        * Set the recognizer priority.
        */
        CPosLmRecognizer();


        static CApaDataRecognizerType* CreateRecognizerL();

    public: // Functions from base classes

        /**
        * Returns the size of the data buffer that will be
        * passed to the recognition function (used by the
        * recognition framework) see DoRecognizeL().
        * @return Size of the data buffer.
        */
        TUint PreferredBufSize();

        /**
        * Returns the MIME type that the recognizer is able
        * to manage (used by the recognition framework).
        * @param aIndex The index of the MIME type to return.
        * @return A MIME type.
        */
        TDataType SupportedDataTypeL(
        /* IN */ TInt aIndex
        ) const;

        /**
        * The recognition function. The result of the recognition
        * is stored in the iConfidence data member.
        * @param aName The name of the file that contain the
        *   data to analyze. Not used in this implementation.
        * @param aBuffer The data buffer
        * \see PreferredBufSize()
        */
        void DoRecognizeL(
        /* IN */ const TDesC& aName,
        /* IN */ const TDesC8& aBuffer
        );

    private:

        /**
        * The data recognition function. This functions checks
        * if the provided data includes specified tags.
        * @param[in] aBuffer The data buffer to check.
        */
        void CheckBufferL( const TDesC8& aBuffer );

        /**
        * This functions opens file given as input and calls
        * recognition from file.
        * @param[in] aFileName The name of file to recognize
        */
        void CheckFileL( const TDesC& aFileName );

        /**
        * This function checks if the provided file includes specified tags.
        * @param[in] aFile The file to check.
        */
        void CheckFileL( const RFile& aFile );

        /**
        * This function uses a parser to check for lmx tags.
        * @param[in] aParser The parser to use.
        */
        TInt CheckXmlContent( CPosLmSimpleParser& aParser );

        /** Checks XML content for given root tag and namespace 
         * @param[in] aParser content controller
         * @param[in] aRootTag Search for this root tag
         * @param[in] aNamespace Search for this namespace
         * @param[in] aMimeType Choose this MIME if root tag is found */
        TInt CheckXmlContent( 
                CPosLmSimpleParser& aParser,
                const TDesC8& aRootTag,
                const TDesC8& aNamespace,
                const TDesC8& aMimeType );

        /** Skip amy comment tags */ 
        void SkipComments( CPosLmSimpleParser& aParser );
    
        /** Check for XML prolog */ 
        TInt CheckXmlProlog( CPosLmSimpleParser& aParser );
        
        /** Last resort in recognition: file extension checking */
        void CheckFileExtension( const TDesC& aFilename );

    private:
        TBool iXmlPrologFound;
        TBool iIsNotXmlData;
    };

#endif      // CPOSLMRECOGNIZER_H

// End of File
