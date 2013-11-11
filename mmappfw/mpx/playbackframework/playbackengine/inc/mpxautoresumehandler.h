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
* Description:  Auto resume handler
*
*/



#ifndef CMPXAUTORESUMEHANDLER_H
#define CMPXAUTORESUMEHANDLER_H

// INCLUDES
#include <e32base.h>
#include <mpxpskeyobserver.h>
#include <mpxplaybackframeworkdefs.h>

// FORWARD DECLARATIONS
class CMPXPlaybackEngine;
class CMPXPSKeyWatcher;

/**
*  CMPXAutoResumeHandler.
*  Class for processing call state events.
*
*  @since 3.0
*/
NONSHARABLE_CLASS(CMPXAutoResumeHandler) :  public CBase,
                                            public MMPXPSKeyObserver

    {
public:
    /**
    * Creates instance of the CMPXAutoResumeHandler class.
    *
    * @param aEngine reference to playback engine
    * @param aMixerSupport flag to support mixer
    * @return Returns the instance just created.
    */
    static CMPXAutoResumeHandler* NewL(CMPXPlaybackEngine& aEngine,
                                       TBool aMixerSupport);

    /**
    * Destructor.
    */
    virtual ~CMPXAutoResumeHandler();
    
    /**
    * Handle open file complete
    */
    void HandleOpenFileComplete();
    
    /**
    * Handle playback state change
    * @param aState new state    
    */
    void HandlePlaybackStateChange(TMPXPlaybackState aState);

    /**
    * Handle complete of playing a song
    * @aError error code
    */
    void HandlePlaybackComplete(TInt aError);

    /**
    * Resume timer cancel.
    */
    void CancelResumeTimer();

    /**
     * Set autoresume value
     */
    void SetAutoResume(TBool aAutoResume);

private: // constructors

    /**
    * C++ default constructor.
    *
    * @param aEngine reference to playback engine
    * @param aMixerSupport flag to support mixer
    */
    CMPXAutoResumeHandler(CMPXPlaybackEngine& aEngine, TBool aMixerSupport);

    /**
    * Symbian OS constructor.
    */
    void ConstructL();

private: // From base classes

    /**
    * From MMPXPSKeyObserver
    * Handle PS event
    *
    * @param aUid The UID that identifies the property category
    * @param aKey The property sub-key
    */
    void HandlePSEvent(TUid aUid, TInt aKey);

private:  // New functions

    /**
    * Handle call state or type change.
    */
    void DoHandleStateChangeL();

    /**
    * Check if we should go to pause in
    * given call state
    */
    TBool ShouldPause();

    /**
    * Handle resume timer callback
    */
    void HandleResumeTimerCallback();

    /**
    * Callback for resume timer.
    */
    static TInt ResumeTimerCallback(TAny* aPtr);

    /**
    * Checks if current player is remote: playback shouldn't be paused
    * in that case
    */
    TBool IsPlaybackRemote();
    
    /**
    * Handle change in voice command state. 
    */
    void DoHandleVoiceCmdChange();

private: // data
    CMPXPlaybackEngine&  iEngine;
    TBool iMixerSupport;
    TBool iPausedForCall;
    CMPXPSKeyWatcher* iStateObserver; // owned
    CMPXPSKeyWatcher* iTypeObserver;  // owned
    TTime iKErrDiedTime;
    TBool iAutoResume;

    // Timer for resume operation. Owned.
    CPeriodic* iResumeTimer;
    
    CMPXPSKeyWatcher* iVoiceCmdObserver;  // owned
    TBool iPausedForVoiceCmd;
    TBool iVoiceCmdResumeOngoing;
    };

#endif      // CMPXAUTORESUMEHANDLER_H   
            
// End of File
