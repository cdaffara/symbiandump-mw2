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
* Description:  Playback message
*
*/



#ifndef TMPXPLAYBACKMESSAGE_H
#define TMPXPLAYBACKMESSAGE_H


#include <e32base.h>
#include <mpxmessage.h>
/**
* Playback message type.
*
* @lib mpxcommon.lib
*/
class TMPXPlaybackMessage : public TMPXMessage
    {
public:
    enum TEvent
        {
        ENoEvent,
        EError, // EError must be 1. iData is error originating from player, iType=event type
        ECommandReceived, // iType=TMPXPlaybackCommand
        ECommandComplete, // iType=TMPXPlaybackCommand
        EPropertyChanged, // iType=TMPXPlaybackProperty; iData=new value
        EStateChanged, // iType=TMPXPlaybackState
        ESongCorrupt,
        ESongContainerChanged, // Need to call MMPXMedia::CollectionPathL() to get new path
        EInitializeComplete, // iData = ETrue if init was to start playback, else EFalse.
        ESongChanged, // iData=new index; need to query to get new properties (e.g. duration, title etc.)
        EPlayerChanged, // Need to call MMPXPlaybackUtility::CurrentPlayer() to get new features
        EActivePlayerChanged, // iData=active/inactive (ETrue/EFalse); iType (ETrue,rebind to  ActivePlayerMode)
        ESubPlayersChanged, // Need to query to get new list of sub players
        EPlayerSelectionChanged, // Need to query MMPXPlayerManager::GetSelectionL()
        EDownloadStarted, // iData=total expected bytes
        EDownloadUpdated, // iData=total bytes so far
        EDownloadComplete, // iData=total bytes downloaded
        EDownloadPositionChanged, // iData=total bytes so far
        EDownloadStateChanged, // iData=TMPXPlaybackPdDownloadState
        EDownloadCmdPauseDownload, // iData=transaction ID
        EDownloadCmdResumeDownload, // iData=transaction ID
        EDownloadCmdCancelDownload, // iData=transaction ID
        EAccessoryChanged, // iType=TAccessoryMode
        EMediaChanged, // When the song/playlist hasn't, but data may become available
        ESkipping, //skipping event, iData=+1, skipping forward, -1, skipping backward
        ESkipEnd, //skipping end
        EPlayerUnavailable,
        EPlaylistUpdated, // playlist updated
        EReachedEndOfPlaylist, // playback or skipping forward has reached end of playlist
        EDownloadFileMoved
        };
public:
    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    *  @param aEvent event
    *  @param aType event type
    *  @param aData optional data
    */
    inline TMPXPlaybackMessage(TEvent aEvent,TInt aType = 0,TInt aData = 0);

    /**
    *  C++ copy constructor.
    *
    *  @since S60 3.2.3
    *  @param aMsg source message
    */
    inline TMPXPlaybackMessage(const TMPXPlaybackMessage& aMsg);

    /**
    *  Overloaded assignment operator.
    *
    *  @since S60 3.2.3
    *  @param aMsg source message
    */
    inline TMPXPlaybackMessage& operator=(const TMPXPlaybackMessage& aMsg);

    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    */
    inline TMPXPlaybackMessage();
    };

#include "mpxplaybackmessage.inl"

#endif // TMPXPLAYBACKMESSAGE_H
