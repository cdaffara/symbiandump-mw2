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
* Description:  This class provides the main API for the Advanced Audio Controller
*				 Resource.
*
*/



#ifndef CADVANCEDAUDIORESOURCE_H
#define CADVANCEDAUDIORESOURCE_H

// INCLUDES
#include <f32file.h>
#include <barsc.h>
#include <barsread.h>

#include "AdvancedAudioResource.hrh"

// FORWARD DECLARATIONS
class TMetadataEntry;
class CMMFMetaDataEntry;
class CMMFClip;

// CLASS DECLARATION

/**
*  This class provides Advanced Audio Controller properties.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class TAapProperties
	{
	public:
		TInt iSharedBufferMaxNum;		// Max number of shared buffer
		TInt iSharedBufferMaxSize;		// Max buffer size
		TInt iSharedBufferMaxSizeForNonSeekableSrc; // Max buffer size for Non Seekable source like prog download
		TInt iSamplingRate;				// Default sampling rate
		TInt iStereoSupport;			// Default channel number
		TBool iMetaDataSupport;			// Meta Data support indicator
	};

/**
*  This class implements base class for all resource file handling classes.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioResource : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CAdvancedAudioResource();

        /**
        * C++ default constructor.
        */
        IMPORT_C CAdvancedAudioResource();

    public: // New functions

		/**
        * Returns the controller's properties for the current Codec.
        * @since 3.0
        * @param	-
        * @return	AAP properties
        */
		IMPORT_C const TAapProperties& PropertiesL();

		/**
        * Returns the configuration parameters for the current Codec.
        * @since 3.0
        * @param	-
        * @return	Configuration parameters
        */
        IMPORT_C const RArray<TInt>& CodecConfigParametersL();

	protected:

		/**
        * Reads the data from the resource file for the specified codec.
        * @since 3.0
        * @param	aResourceFileName - descriptor containing resource file name
        * @param	aResourceId - one of the constants defined in this file.
        * @return	-
        */
		IMPORT_C virtual void ReadResourceFileL( const TDesC& aResourceFileName, TInt aResourceId );

		/**
        * Reads the configuration parameters for the codec.
        * @since 3.0
        * @param	-
        * @return	-
        */
		IMPORT_C virtual void ReadCodecConfigParametersL() = 0;

 	protected:	// Data

		// File session
		RFs iFs;
		// Resource file
		RResourceFile iResourceFile;
		// Resource reader
		TResourceReader iResourceReader;
		// Buffer where resource is read
		HBufC8* iResourceBuf;
		// Used in case of LEAVE during read
		TBool iResourceOpen;

		// AAP properties
		TAapProperties iProperties;
		// Codec configuration parameters
		RArray<TInt> iCodecConfigParams;
	};

#endif      // CADVANCEDAUDIORESOURCE_H

// End of File
