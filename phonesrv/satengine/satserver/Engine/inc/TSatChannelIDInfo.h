/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Information class for Channel IDs.
*
*/


#ifndef TSATCHANNELIDINFO_H
#define TSATCHANNELIDINFO_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION

/**
*  Container of Channel ID information
*
*  @lib SatEngine.lib
*  @since Series 60 3.0
*/
class TSatChannelIdInfo
    {
    public:

        /**
        * Constructor.
        * @param aChannelId Long version of channel ID
        * @param aShort Short version of channel ID
        */
        TSatChannelIdInfo( TInt aChannelId, TUint8 aShort );

        /**
        * Returns long channel ID
        * @return Long channel ID, range RSat::KChannel1-RSat::KChannel7
        */
        TInt ChannelId() const;

        /**
        * Returns short channel ID
        * @return Short channel id, range 1-7
        */
        TUint8 ShortChannelId() const;

        /**
        * Indicates is this channel ID in use
        * @return TBool indicating the availability of this channel ID
        */
        TBool IsReserved() const;

        /**
        * Reserves channel ID
        */
        void ReserveChannel();

        /**
        * Releases channel ID
        */
        void ReleaseChannel();

    private:

        // Long channel ID, range RSat::KChannel1 - RSat::KChannel7
        TInt    iChannelId;

        // Short channel ID, range 1 - 7
        TUint8  iShortChannelId;

        // Indicates is this channel in use of free
        TBool   iActive;

    };

#endif      // TSATCHANNELIDINFO_H

// End of File
