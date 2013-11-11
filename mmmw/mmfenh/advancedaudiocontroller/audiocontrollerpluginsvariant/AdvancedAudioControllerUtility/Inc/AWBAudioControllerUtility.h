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
* Description:  Class definition for the AWB utility functions.
*
*/



#ifndef CAWBAUDIOCONTROLLERUTILITY_H
#define CAWBAUDIOCONTROLLERUTILITY_H

//  INCLUDES
#include "AdvancedAudioUtility.h"

// FORWARD DECLARATIONS
class CMMFDataBuffer;

// CLASS DEFINITIONS

/**
*  This class provides AWB utility functions.
*
*  @lib CAWBAudioControllerUtility.lib
*  @since 3.0
*/
class CAWBAudioControllerUtility : public CAdvancedAudioUtility
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CAWBAudioControllerUtility* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CAWBAudioControllerUtility();

    public: // Functions from base classes

		/**
        * From CAdvancedAudioUtility
		* Returns the byte position of the first frame.
		* @param aBuf Pointer to the data
		* @param aBufLen Length of the data
		* @return byte position of the first frame
		*/
		TInt SeekSync( CMMFDataBuffer* aBuf, TInt aBufLen );

		/**
        * From CAdvancedAudioUtility
		* Returns frame info.
		* @param aBuf Pointer to the data
		* @param aBufLen Length of the data
		* @param aInfo Contains the frame info on return
		* @return frame length
		*/
		TInt FrameInfo( const TUint8* aBuf, TInt aBufLen, TAudioFrameInfo& aInfo );

		/**
        * From CAdvancedAudioUtility
		* Returns frame header size.
		* @param none
		* @return frame header size
		*/
		TInt FrameHeaderSize();

		/**
		* From CAdvancedAudioUtility
        * Scans header information
		* @param aDataBuf
		* @return void
		*/
		void ScanHeaderL(CMMFDataBuffer* aDataBuf);

	private:

        /**
        * C++ default constructor.
        */
		CAWBAudioControllerUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

	private:	// Data

    };

#endif 		// CAWBAUDIOCONTROLLERUTILITY_H

// End of File
