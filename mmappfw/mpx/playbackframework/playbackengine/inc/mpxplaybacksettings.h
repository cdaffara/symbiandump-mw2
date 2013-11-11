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
* Description:  Playback settings.
*
*/



#ifndef CMPXPLAYBACKSETTINGS_H
#define CMPXPLAYBACKSETTINGS_H

//  INCLUDES
#include <e32base.h>

// CLASS DECLARATION

/**
*  CMPXPlaybackSettings.
*  Playback settings
*
*  @lib mpxplaybackengine.dll
*  @since 3.0
*/
NONSHARABLE_CLASS(CMPXPlaybackSettings) : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMPXPlaybackSettings* NewL();

        /**
        * Destructor.
        */
        virtual ~CMPXPlaybackSettings();

    public: // New functions

        // Central repository is used for settings


    private:

        /**
        * C++ default constructor.
        */
        CMPXPlaybackSettings();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
    };

#endif      // CMPXPLAYBACKSETTINGS_H

// End of File
