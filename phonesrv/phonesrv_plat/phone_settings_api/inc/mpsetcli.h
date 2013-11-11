/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*    MPsetCli defines interface to get and set CLI settings from the network.
*
*
*/


#ifndef     MPSETCLI_H
#define     MPSETCLI_H

// INCLUDES
#include <e32base.h>

// CLASS DEFINITIONS 
/**
*  MPsetCli class is virtual class for CLI.
*  @lib phonesettings.lib
*  @since 1.0
*/
class   MPsetCli
    {
    public:

        /**
        * Shows COLP (Connected Line Identification Presentation) mode.
        */
        virtual void GetColpModeL() = 0;

        /**
        * Shows CLIP (Calling Line Identification Presentation) mode.
        */
        virtual void GetClipModeL() = 0;
        
        /**
        * Shows CLIR (Calling Line Identification Restriction) mode.
        */
        virtual void GetClirModeL() = 0;

        /**
        * Shows COLR (Connected Line Identification Restriction) mode.
        */
        virtual void GetColrModeL() = 0;

        /**
        * Shows CNAP (Calling Number Presentation) mode.
        * 
        */
        virtual void GetCnapL() = 0;

        /**
        * Cancels any (and all) request(s).
        */
        virtual void CancelAll() = 0;
    };
#endif // MPSETCLI_H
// end of file
