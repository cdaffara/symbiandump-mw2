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
* Description:  Buffer commands
*
*/


#ifndef CMPXPLAYBACKCMDBUFFER_H
#define CMPXPLAYBACKCMDBUFFER_H


#include "mpxplaybackcommandbufferobserver.h"

/**
*  Special processing for media keys, e.g. buffering a quick
*  succession of keys and calling back with the net result after
*  a specific period of time
*/
NONSHARABLE_CLASS(CMPXPlaybackCmdBuffer) : public CTimer
    {
public:

    /**
    * Two-phased constructor
    *
    * @param aObserver observer of the media key
    * @return object of constructed
    */
    static CMPXPlaybackCmdBuffer* NewL(MMPXPlaybackCmdBufferObserver& aObserver);

public:
    /**
    *  Is this a command controlled by media keys
    *
    *  @param aCmd playback command
    *  @return ETrue if the command is controlled by media keys
    */
    TBool CommandForBuffering(TMPXPlaybackCommand aCmd) const;

    /**
    *  Buffer a command, along with the current state and position in the
    *  track
    *
    *  @param aCmd play command
    *  @param aState play state
    *  @param aPos position
    */
    void BufferCommandL(TMPXPlaybackCommand aCmd,TMPXPlaybackState aState,TInt aPos);

    /**
    *  Called by client when finished processing command
    */
    void CompleteCommand();

    /**
    *  Any command buffered
    *
    *  @return ETrue buffered commands available
    */
    TBool IsCommandBuffered() const;

    /**
    *  Clear all command
    */
    void ClearCommands();

private:
    /**
    *  From CActive
    *  Handles an active object's request completion event
    */
    void RunL();

    /**
    *  From CActive
    *  Handles a leave occurring in the request completion event handler RunL()
    *
    *  @param aError error code
    *  @return system error code
    */
    TInt RunError(TInt aError);

private:

    /**
    *  C++ constructor
    */
    CMPXPlaybackCmdBuffer(MMPXPlaybackCmdBufferObserver& aObserver);

private:
    MMPXPlaybackCmdBufferObserver& iObserver;
    TMPXPlaybackCmdInfo iEvent;
    TBool iIsCommandBuffered;
    TBool iEventProcessing;
    TTimeIntervalMicroSeconds32 iBufferTime;
};

#endif // CMPXPLAYBACKCMDBUFFER_H
