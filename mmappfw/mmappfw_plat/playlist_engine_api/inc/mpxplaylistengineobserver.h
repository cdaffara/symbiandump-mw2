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
* Description:  Playlist engine observer
*
*/


#ifndef MMPXPLAYLISTENGINEOBSERVER_H
#define MMPXPLAYLISTENGINEOBSERVER_H


#include <mpxmedia.h>

/**
*  A playlist engine client must implement this
*  observer interface for InternalizePlaylistL and/or ExternalizePlaylistL callback.
*
*  @lib mpxplaylistengine.lib
*/
NONSHARABLE_CLASS(MMPXPlaylistEngineObserver) 
    {
    public:
    
        /**
        *  Callback when Internalize Playlist request is completed. This operation cannot
        *  be a long running task as this may be invoked from the RunL of a plugin.
        *
        *  Parameters are only available for the duration of this method. If a client
        *  requires them to be available beyond the completion of this method, a copy
        *  should be made.
        *
        * @param aPlaylist a playlist parsed from the playlist file. Depending on the
        *        content of the playlist, not all info for a media will be available;
        *        only the available info from playlist file is populated, e.g. for
        *        m3u playlists, the only available information is URI of the media and
        *        its title if it's in extended format.
        *
        *        This media will contain the following attributes if no error has occured:
        *
        *            EMPXMediaGeneralUri for the playlist
        *            EMPXMediaGeneralType, which has a value of EMPXItem
        *            EMPXMediaGeneralCategory, which has a value of EMPXPlaylist
        *            EMPXMediaArrayContents
        *            EMPXMediaArrayCount
        *
        *        When an error occurs, this is NULL.
        *
        *        Client takes over the ownership of this CMPXMedia object.
        *
        *  @param aError error code   
        *
        *  @param aCompleted a flag that indicates if internalize playlist command has
        *         been fully processed. EFalse if not. In this case, there will be
        *         subsequent calls to HandlePlaylistL for the same playlist with the
        *         segmented results. ETrue if there are no more results to be sent to
        *         the observer.
        */
        virtual void HandlePlaylistL(CMPXMedia* aPlaylist,
                                     const TInt aError,
                                     const TBool aCompleted) = 0;

        /**
        *  Callback when Externalize Playlist request is completed. This operation cannot
        *  be long running task as this may be in invoked from the RunL of a plugin.
        *
        *  Parameters are only available for the duration of this method. If a client
        *  requires them to be available beyond the completion of this method, a copy
        *  should be made.
        *
        *  @param aPlaylistUri URI for the playlist which has been externalized.
        *  @param aError error code    
        */
        virtual void HandlePlaylistL(const TDesC& aPlaylistUri,
                                     const TInt aError) = 0;        
    };

#endif      // MMPXPLAYLISTPLUGINOBSERVER_H


