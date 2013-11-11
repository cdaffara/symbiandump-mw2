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
* Description:  MPX parameter for commandline/message
*
*/


#ifndef CMPXPARAMETER_H
#define CMPXPARAMETER_H

// INCLUDES
#include <e32base.h>

enum TMPXPathType
    {
    EMPXTypeCollectionPath,
    EMPXTypeCollectionPlaylist
    };

enum TMPXCmdFwdType
    {
    EMPXCmdFwdNone,
    EMPXCmdFwdStandAlone,
    EMPXCmdFwdNowPlaying
    };

// FORWARD DECLARATIONS
class CMPXCollectionPath;

// CLASS DECLARATION
/**
* Usage:
* - To open a specific view, set iType to the view type.
*
* - To open a specific path, set iCollectionPath, music player will select
*   the best matching view to display the result.
*
* - using iCollectionPath/iCollectionPlaylist + iType is supported, but not recommended
*   since music player already selects the best matching view to display the result.
*
* - If the client wants the command to be processed by a specific music player instance
*   they should set iCmdForward to a value defined by TMPXCmdFwdType. The default forwarding is to
*   stand alone musicplayer (it'll be launched if it doesn't exist).
*
*  @lib mpxcommon.lib
*/
class CMPXParameter : public CBase
    {
    public: // Constructors and destructor

        /**
        * C++ Constructor.
        *
        * @since S60 3.2.3
        */
        IMPORT_C CMPXParameter();

        /**
         * Destructor.
         *
         * @since S60 3.2.3
         */
        IMPORT_C virtual ~CMPXParameter();

    public: // New funtions

        /**
         * Externalize a object of this class to steam.
         *
         * @since S60 3.2.3
         * @param aStream write stream
         */
        IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

        /**
         * Internalize a object of this class from steam.
         *
         * @since S60 3.2.3
         * @param aStream read stream
         * @leave KErrNotSupported The collection path is not valid,
         */
        IMPORT_C void InternalizeL( RReadStream& aStream );

    public: // data member
        CMPXCollectionPath*     iCollectionPath; // OWNED
        TMPXPathType            iPathType;
        TUid                    iType;
        TInt                    iCmdForward; // TMPXCmdFwdType

    private:
        TInt                    iVersion;
    };

#endif // CMPXPARAMETER_H

// End of File

