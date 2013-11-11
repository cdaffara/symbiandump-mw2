/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is responsible for getting MMF file information
*
*/



#ifndef CMPXFILEINFOUTILITY_H
#define CMPXFILEINFOUTILITY_H

// INCLUDES
#include <e32base.h>
#include <e32hashtab.h>
#include <mmf/common/mmfcontroller.h>
#include <mmf/common/mmfcontrollerframework.h>

// CLASS DECLARATION

/**
*  File info utility class
*/
class CMPXFileInfoUtility : public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor: Normal C++ constructor & ConstructL.
        */
        static CMPXFileInfoUtility* NewL();

        /**
        * Two-phased constructor: Normal C++ constructor & ConstructL.
        */
        static CMPXFileInfoUtility* NewLC();

        /**
        * Destructor
        * Destroy the object and release all memory objects
        */
        virtual ~CMPXFileInfoUtility();

    public: // New functions

        /**
        * Open a file using audio controller.
        * The file is opened based on MIME type given mime type.
        * If MIME type is not provided, the file is opened by file handle
        * @param aFile, file handle
        * @param aMimeType, mime type of the file, by default it is KNullDesC
        */
        void OpenFileL(RFile& aFile,
                                const TDesC& aMimeType = KNullDesC);
        /**
        * Reset file info utility
        */
        void Reset();

        /**
        * Get the duration of a song
        * @return duration with micro seconds
        */
        TTimeIntervalMicroSeconds Duration();

        /**
        * Get the bit rate of a song
        * @return the bit rate of the song
        */
        TUint BitRate();

        /**
        * Get the sample rate of a song
        * @return the sample rate of the song
        */
        TUint SampleRate();

    private:

        /**
        * C++ default constructor
        */
        CMPXFileInfoUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Find a controller
        * The controller is searched by MIME type. If MIME type is empty,
        * the controller is searched by file name
        * @param aFileName file name
        * @param aMimeType, mime type of the file
        * @param aUid uid of the controller
        */
        void FindController( const TDesC& aFileName,
                             const TDesC& aMimeType,
                             TUid& aUid);

        /**
        * Open a controller
        * @param aUid uid of the controller
        */
        void OpenControllerL(const TUid& aUid);

        /**
        * Create an array to hold all the controller plugins that support audio
        */
        void CreateAudioFormatsArrayL();

    private:         // Member variables

        // Controller for music file
        RPtrHashMap<TUint32, RMMFController> iMMFControllers;
        RMMFController *iController;
        TUint32 iCurrentControllerUid;
        TUint32 iUid1;
        TUint32 iUid2;
        
        RMMFControllerImplInfoArray iAudioControllers;
    };

#endif // CMPXFILEINFOUTILITY_H

// End of File

