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
* Description:  Playlist plugin interface
*
*/


#ifndef MPXPLAYLISTPLUGIN_H
#define MPXPLAYLISTPLUGIN_H

#include <ecom/ecom.h>
#include <charconv.h>
#include <mpxmedia.h>

// FORWARD DECLARATIONS
class MMPXPlaylistPluginObserver;

/**
*  CMPXPlaylistPlugin: Defines the playlist plugin interface.
*
*  A playlist plugin is instantiated and owned by CMPXPlaylistPluginHandler.
*  But the client of the services a playlist plugin provides is
*  CMPXPlaylistEngine.
*
*/
NONSHARABLE_CLASS(CMPXPlaylistPlugin): public CBase
    {

public:

    /**
    * Factory method to instantiate a playlist plugin with the matching UID
    *
    * @param aUid uid of interface implementation
    * @param aObserver a playlist plugin observer
    * @param aFs a handle to file session
    * @param aTopCharacterSet top character set for the current locale
    * @param aAvailableCharacterSet available character set in the sytem
    * @return constructed object
    */
    inline static CMPXPlaylistPlugin* NewL(
        TUid aUid, 
        MMPXPlaylistPluginObserver& aObserver,
        RFs& aFs,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aTopCharacterSet,
        const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>& aAvailableCharacterSet);

    /**
    * Destructor
    */
    virtual ~CMPXPlaylistPlugin();
    
public: 

    /** 
    * Internalize a playlist. A MMPXPlaylistPluginObserver::HandlePlaylistL
    * callback will be generated on completion.
    *
    * Parameters are guaranteed by the caller (i.e. CMPXPlaylistEngine) to
    * exist until plugin completes this request.
    *
    * @param aStatus caller's request status
    * @param aPlaylistUri URI of the playlist to be internalized
    */
    virtual void InternalizePlaylistL(
        TRequestStatus& aStatus,
        const TDesC& aPlaylistUri) = 0;

    /** 
    * Externalize a playlist. a MMPXPlaylistPluginObserver::HandlePlaylistL
    * callback will be generated on completion.
    *
    * Parameters are guaranteed by the caller (i.e. CMPXPlaylistEngine) to
    * exist until plugin completes this request.
    *
    * @param aStatus caller's request status
    * @param aPlaylist a playlist to be externalized. This media contains
    *        the following attributes:
    *
    *            KMPXMediaGeneralTitle
    *            KMPXMediaGeneralType:
    *               This contains the value of EMPXItem
    *            KMPXMediaGeneralCategory:
    *               This contains the value of EMPXPlaylist
    *            KMPXMediaArrayContents
    *            KMPXMediaArrayCount
    *
    *        Each media item in the playlist is guaranteed to contain the
    *        following attributes:
    *
    *            KMPXMediaGeneralType:
    *               This contains the value of EMPXItem
    *            KMPXMediaGeneralCategory:
    *               This contains the value of EMPXSong
    *
    *            The rest of attributes may or may not be present. Client
    *            is recommended to call RequiredAttributes before making
    *            a request to export the playlist and provide attributes
    *            required for each media item in the playlist. However,
    *            not every client will be able to comply and supply the
    *            required attributes, specifically client may store the
    *            information about the media item in a memory card and
    *            memory card has been removed from the device. Each
    *            playlist plugin MUST be capable of handling the media
    *            item containing only the URI in addition to its type and
    *            category.
    *
    * @param aFilePath File path for where the playlist should be placed
    */
    virtual void ExternalizePlaylistL(
        TRequestStatus& aStatus,
        const CMPXMedia& aPlaylist,
        const TDesC& aFilePath) = 0;

    /**
    * Required attributes for the media items in the playlist in order to
    * externalize them to a playlist. Client is recommended to call this
    * method and provide the required attributes for each media item in
    * the playlist before externalizing the playlist.
    *
    * @return an array of attributes required in order to externalize
    * a playlist media into a playlist file.
    */
    virtual const TArray<TMPXAttribute> RequiredAttributes() const = 0;

    /**
    * Optional attributes for the media items in the playlist for externalizing
    * them to a playlist.
    *
    * @return an array of attributes which are optional when externalizing
    * a playlist media into a playlist file
    */
    virtual const TArray<TMPXAttribute> OptionalAttributes() const = 0;
    
    /**
    * Returns the file extension the plugin handles
    *
    * @return file extension which includes the period.
    */
    virtual const TDesC& FileExtension() const = 0;

    /**
    * Cancel request
    */        
    virtual void Cancel() = 0;
    
protected:

    MMPXPlaylistPluginObserver*  iObserver; // not owned
    RFs*                         iFs;       // not owned
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iTopCharacterSet;      // not owned
    const CArrayFix<CCnvCharacterSetConverter::SCharacterSet>* iAvailableCharacterSet;// not owned

private:

    // identifies the actual plugin instance
    TUid iDtorKey;
    };

#include "mpxplaylistplugin.inl"

#endif      // MPXPLAYLISTPLUGIN_H


