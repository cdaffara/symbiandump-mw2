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
* Description:  A view displaying basic playback state and feature
*
*/

#ifndef TESTPLAYBACKVIEW_H
#define TESTPLAYBACKVIEW_H

/* INCLUDES */
#include "testBaseview.h"
#include "testplaybackviewobserver.h"

/* DATA TYPE */
_LIT(KTxtTestPlaybackView, "TestPlaybackView");

/* FORWARD DECLARATION*/
class MTestPlaybackViewObserver;

/**
 *  UI of playback test
 *
 *  @lib testplaybackutility.lib
 *  @since S60 v3.0
 */
class CTestPlaybackView : public CTestBaseView
    {
public:
    
    IMPORT_C static CTestPlaybackView* NewL(CConsoleMain* aConsoleMain,
                                            CTestBaseView* aParent,
                                            const TDesC& aName,
                                            CStifLogger* aLogger);

    IMPORT_C virtual ~CTestPlaybackView();
    
    /**
     * Add observer
     * @since S60 v3.0
     * @param aObs an observer to add
     */
    IMPORT_C void AddObserver(MTestPlaybackViewObserver* aObs);
     
     /**
    * Update current index in the playlist
    * @param aIndex current index
    * @param aTotalItem total index
    */
    IMPORT_C void UpdateItemIndex(TInt aIndex, TInt aTotalItem);
    
    /**
    * Update position in the song
    * @param aPosition position
    * @param aDuration duration
    */
    IMPORT_C void UpdateProgress(TInt aPosition, TInt aDuration);
    
    /**
    * Update volume
    * @param aVolume current volume
    * @param @aMaxVolume maximum volume
    */
    IMPORT_C void UpdateVolume(TInt aVolume, TInt aMaxVolume);
    
    /**
    * Update artist and song name
    * @param aArtist artist name
    * @param aSongName song name
    */
    IMPORT_C void UpdateSongInfo(const TDesC& aArtist, const TDesC& aSongName);
    
    /**
    * Handles state changes in playbackutility
    */
    IMPORT_C void StateChanged(TTestPlaybackState aState);

// from base class CTestBaseView
    /**
     * Initialization of a view before first time activation
     */
    void InitializeViewL();
    
    /**
     * Cleanup the child view before deactivate/destroy view
     */
    void CleanupViewL();
    
    /**
     * Display this view 
     */
    void DisplayViewL();
        
    /** 
     * Process keypresses on this view - called by ConsoleMain
     * @param aSelection, key pressed
     * @param aContinue, whether to continue or stop scheduler
     */
    void SelectL( TKeyCode aSelection, TBool& aContinue );
        
    /** 
     * Update display from timer - called by ConsoleMain
     */
    void TimerUpdate();
    
    
protected:  //protected class
    class TPlaybackStatus
        {
    public:
        TBool iBackwardSeek;
        TBool iForwardSeek;
        TBool iPlaying;
        TBool iPause;
        TInt iVolume;
        TInt iMaxVolume;
        TInt iProgressPos;
        TInt iProgressDur;
        TInt iItemIndex;
        TInt iMaxItems;
        };

private:

    CTestPlaybackView(CConsoleMain* aConsoleMain,
                      CTestBaseView* aParent,
                      const TDesC& aName,
                      CStifLogger* aLogger);

    void ConstructL();
    
    /**
    * The following functions redraw the playback view
    * base one iCurrentStatus except song info
    */
    void RedrawIndex();
    void RedrawProgress();
    void RedrawVolume();
    void RedrawPlayerStatue();
    
private:    //data
    /**
     * Screen size
     */
    TSize iDisplaySize;
    
    /**
     * Player status
     */
    TPlaybackStatus iCurrentStatus;
    
    /**
     * Send user command to View observer
     * Not own.
     */
    MTestPlaybackViewObserver* iTestPlaybackViewObs;
    
    /**
     * Logger purpose
     * Not own.
     */
    CStifLogger* iLogger;
    
    
    };

#endif      //TESTPLAYBACKVIEW_H

