/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  handle callback messages return from testplaybackview
*
*/

#ifndef TESTPLAYBACKVIEWOBSERVER_H
#define TESTPLAYBACKVIEWOBSERVER_H


/* TestPlaybackView commands.  Refer TMPXPlaybackCommand if possible*/
enum TTestPlaybackViewCommand
    {
    ETPbViewCmdPlay,
    ETPbViewCmdPause,
    ETPbViewCmdStop,
    ETPbViewCmdNext,
    ETPbViewCmdPrevious,
    ETPbViewCmdStartSeekForward,
    ETPbViewCmdStartSeekBackward,
    ETPbViewCmdStopSeeking,
    ETPbViewCmdIncreaseVolume,
    ETPbViewCmdDecreaseVolume,
    ETPbViewCmdClose,
    ETPbViewCmdEnd,
    };
    
/* TestPlaybackView playback state.  Refer TMPXPlaybackState if possible*/    
enum TTestPlaybackState
    {
    ETPbStateNotInitialised, 
    ETPbStateInitialising,
    ETPbStatePlaying,    
    ETPbStatePaused,
    ETPbStateStopped,
    ETPbStateSeekingForward, 
    ETPbStateSeekingBackward,   
    ETPbStateShuttingDown,
    ETPbStateBuffering,
    ETPbStateDownloading,
    ETPbStatEnd,
    };

/**
 *  Handle callback message from CTestPlaybackView
 *
 *  This interface class serves as generalized message issued from 
 *  view class.
 *
 *  @lib testplaybackutility.lib
 *  @since S60 v3.0
 */
class MTestPlaybackViewObserver
    {
public:
    
    /**
    * Handle View received command
    * @param aCmd a command received in view
    */
    virtual void HandleUiCommandL(TTestPlaybackViewCommand aCmd) = 0;

    };


#endif // TESTPLAYBACKVIEWOBSERVER_H
