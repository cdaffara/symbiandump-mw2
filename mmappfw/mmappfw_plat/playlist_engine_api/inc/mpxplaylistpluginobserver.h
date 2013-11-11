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
* Description:  Playlist plugin observer
*
*/


#ifndef MMPXPLAYLISTPLUGINOBSERVER_H
#define MMPXPLAYLISTPLUGINOBSERVER_H


#include <mpxmedia.h>

/**
*  This interface is not visible to the playlist
*  engine client. This observer interface is used internally by a plugin for request
*  completion notification.
*
*  CMPXPlaylistEngine implements this interface. When a plugin completes a
*  InternalizePlaylistL or ExternalizePlaylistL request, the appropriate
*  HandlePlaylistL is called by the plugin to notify CMPXPlaylistEngine.
*
*  @lib mpxplaylistengine.lib
*/
NONSHARABLE_CLASS(MMPXPlaylistPluginObserver) 
    {
    public:

        /**
        *  Callback for InternalizePlaylistL.
        *
        *  @since S60 3.2.3
        *  @param aPlaylist a list of media parsed from the playlist file. NULL
        *         if there is an error. client takes ownership of this object.
        *  @param aError error code    
        *  @param aCompleted a flag that indicates if internalize playlist command has
        *         been fully processed. EFalse if not. In this case, there will be
        *         subsequent calls to HandlePlaylistL for the same playlist with the
        *         segmented results.ETrue if there are no more results to be sent to
        *         the observer.        
        */
        virtual void HandlePlaylistL(CMPXMedia* aPlaylist,
                                     const TInt aError,
                                     const TBool aCompleted) = 0;

        /**
        *  Callback for ExternalizePlaylistL.
        *
        *  @since S60 3.2.3
        *  @param aPlaylistUri URI for the playlist which has been externalized
        *  @param aError error code    
        */
        virtual void HandlePlaylistL(const TDesC& aPlaylistUri,
                                     const TInt aError) = 0;        
    };

#endif      // MMPXPLAYLISTPLUGINOBSERVER_H


