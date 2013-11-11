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
* Description:  Player plugin observer
*
*/


#ifndef MMPXMPLAYBACKPLUGINOBSERVER_H
#define MMPXMPLAYBACKPLUGINOBSERVER_H

// INCLUDES
#include <mpxplaybackengineobserver.h>

// FORWARD DECLARATIONS

// CONSTANTS

// CLASS DECLARATION


/**
*  Observer interface for playback plugin.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXPlaybackPluginObserver) : public MMPXPlaybackEngineObserver
    {
public:
    /** 
    *  Event for observer.
    *
    *  @deprecated Use CMPXMessage definition instead.
    */ 
    enum TEvent 
        { 
        EPInitialised,   // 0 async response for Initialise, aData=duration
        EPPlaying,       // 1 async response for Play
        EPPlayComplete,  // 2
        EPPaused,        // 3 async response for Pause
        EPStopped,       // 4 async response for Stop
        EPClosed,        // 5 async response for Close, or Close when stop. aData=command id
        EPSetComplete,   // 6 async response for SetL aData=propoerty
        EPActive,        // 7 aData=ETrue/EFalse
        EPSupportedFeaturesChanged,  // 8 aData=supported featured
        EPDurationChanged,           // 9 aData=duration in ms.
        EPDownloadPositionChanged,   // 10
        EPDownloadStateChanged,      // 11
        EPDownloadCmdPauseDownload,  // 12
        EPDownloadCmdResumeDownload, // 13
        EPDownloadCmdCancelDownload, // 14
        EPDownloadStarted,     // 15 aData=total expected bytes
        EPDownloadingUpdated,  // 16 aData=total bytes so far
        EPDownloadingCanceled, // 17 aData=total bytes so far
        EPDownloadingComplete, // 18 aData=total bytes downloaded
        EPSubPlayersChanged,   // 19
        EPPlayerUnavailable,   // 20
        EPVolumeChanged,       // 21 aData=volume level
        EPMuteChanged,         // 22 aData=mute value
        EPPluginSeeking,       // 23 Plugin transition to seeking state
        EPPositionChanged,     // 24 aData=new position
        EPBufferingStarted,     // 25 
        EPDownloadFileMoved
        };
public:

    /** 
    * Handle plugin event.
    *
    * @since S60 3.2.3
    * @param aEvent the event type
    * @param aData optional data
    * @param aError error code of the event
    * @deprecated Use method MMPXPlaybackPluginObserver::HandlePlaybackMessage
    *    (CMPXMessage* aMsg, TInt aErr) instead.
    */
    virtual void HandlePluginEvent(TEvent aEvent,TInt aData,TInt aError)=0;

    /** 
     * Handle message from plug-in.
     *
     * @since S60 3.2.3
     * @param aMsg message from the plug-in
     * @deprecated Use method MMPXPlaybackPluginObserver::HandlePlaybackMessage
    *    (CMPXMessage* aMsg, TInt aErr) instead.
    */
    virtual void HandlePlaybackMessage(const CMPXMessage& aMsg) 
        {
        HandlePlaybackMessage(const_cast<CMPXMessage*>(&aMsg), KErrNone);
        }
    
    /**
     * Handle message from plug-in.
     * TO DO: make it as pure virtual when previous callback removed.
     *
     * @since S60 3.2.3
     * @param aMsg message from the plug-in. Ownership not transferred.
     * @param aErr system error code. if aErr is not KErrNone, aMsg will not be 
     *             used
     */
    virtual void HandlePlaybackMessage(CMPXMessage* /*aMsg*/, 
                                       TInt /*aErr*/) {}
    
    
    /** 
     * Handle message from plug-in.
     *
     * @since S60 9.2
     * @param aMsg message from the plug-in
     * @return system error
    */
    virtual TInt HandlePlaybackSyncMessage (const CMPXMessage& /*aMsg*/) {return KErrNotSupported;}

    
    };

#endif      // MMPXMPLAYBACKPLUGINOBSERVER_H

// End of file
