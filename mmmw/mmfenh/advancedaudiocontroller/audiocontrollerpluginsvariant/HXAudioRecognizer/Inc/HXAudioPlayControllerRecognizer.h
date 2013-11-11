/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Recognizer for Helix Audio files
*
*/


#ifndef CHXAUDIOPLAYCONTROLLERRECOGNIZER_H
#define CHXAUDIOPLAYCONTROLLERRECOGNIZER_H

#include <apmrec.h>  // base class defined here

//*** user include files go here:

//#include "?include_file"

//*** forward declarations go here:

//class ?forward_classname;

//*** external data types go here:

/**  ?description */
//extern ?data_type;

//*** global function prototypes (which should be very rare) go here:
/**
 * ?description
 *
 * @since S60 ?S60_version
 * @param ?arg1 ?description
 * @return ?description
 */
//?type ?function_name( ?arg_list );

//*** constants go here:

/**  ?description */
//const ?type ?constant_var = ?constant;

/**
 *  Concrete implementation of the recognizer API used to recognize Real Audio files.
 *
 *  @lib HXAudioPlayControllerRecognizer.lib
 *  @since S60 v3.2
 */
class CHXAudioPlayControllerRecognizer : public CApaDataRecognizerType
    {
public:

     static CHXAudioPlayControllerRecognizer* NewL();
     

        /**
        * Destructor.
        */
        virtual ~CHXAudioPlayControllerRecognizer();
     

    /**
     * Creates recognizer
     * @since 3.2
     * @param  none
     * @returns recognizer instance
     */
	static CApaDataRecognizerType* CreateRecognizerL();

private:

   	/**
     * From CApaDataRecognizerType
     * Returns one of the SupportedDataType
     * @since 3.2
     * @param aIndex - depending on the file extension
     * @return Supported DataType
     */
 	TDataType SupportedDataTypeL( TInt aIndex ) const;

    /**
     * From CApaDataRecognizerType
     * Returns the buffersize needed to scan
     * @since 3.2
     * @param  none
     * @return BufferSize if needed
     */
	TUint PreferredBufSize();

    /**
     * From CApaDataRecognizerType
     * Do the recognition of the file using the name of the
     * the file or the buffer to scan
     * @since 3.2
     * @param aName    Name of the file
     * @param aBuffer  Buffer that shall be scanned for recognition
     * @return void
     */
	void DoRecognizeL( const TDesC& aName, const TDesC8& aBuffer );

    /**
     * C++ default constructor.
     */
  	CHXAudioPlayControllerRecognizer();
        
    void ConstructL();
    
    TBool IsRealMimeTypeSupported(const TDesC& aMimeType);
    TBool IsASFTypeSupported(const TDesC& aMimeType);
private: // data

    };

//#include "?include_file.inl"


#endif // CHXAUDIOPLAYCONTROLLERRECOGNIZER_H
