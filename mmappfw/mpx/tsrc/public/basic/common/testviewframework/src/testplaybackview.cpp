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

#include <stiflogger.h>
#include <e32const.h>
#include "testplaybackviewobserver.h"
#include "testplaybackview.h"

// CONSTANTS
_LIT(KSpacer,           " ");
_LIT(KPlay,             " Play-1 ");
_LIT(KPlaySel,          "(Play-1)");
_LIT(KBackward,         " <<-2 ");
_LIT(KForward,          " >>-3 ");
_LIT(KStop,             " Stop-4 ");
_LIT(KStopSel,          "(Stop-4)");
_LIT(KFastRewind,       " FR-5 ");
_LIT(KFastRewindSel,    "(FR-5)");
_LIT(KFastForward,      " FF-6 ");
_LIT(KFastForwardSel,   "(FF-6)");
_LIT(KPause,            " Pause-7 ");
_LIT(KPauseSel,         "(Pause-7)");
    
_LIT(KExit,             "Exit-Esc"  );

const TInt KSongInfoYOffset = 10;
const TInt KDisplayScreenWidthOffset = 10;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Symbian 2-phrase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTestPlaybackView* CTestPlaybackView::NewL(CConsoleMain* aConsoleMain,
                                            CTestBaseView* aParent,
                                            const TDesC& aName,
                                            CStifLogger* aLogger)
    {
    CTestPlaybackView* self = new (ELeave) CTestPlaybackView(aConsoleMain,
                                                             aParent,
                                                             aName,
                                                             aLogger);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CTestPlaybackView::~CTestPlaybackView()
    {
    }

// ---------------------------------------------------------------------------
// Add observer
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::AddObserver(MTestPlaybackViewObserver* aObs)
    {
    iTestPlaybackViewObs = aObs;
    }

// ---------------------------------------------------------------------------
// Update current index
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::UpdateItemIndex(TInt aIndex, TInt aTotalItem)
    {
    iCurrentStatus.iItemIndex = aIndex;
    iCurrentStatus.iMaxItems = aTotalItem;
    RedrawIndex();
    }
    
// ---------------------------------------------------------------------------
// Update position in the song
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::UpdateProgress(TInt aPosition, TInt aDuration)
    {
    iCurrentStatus.iProgressPos = aPosition;
    iCurrentStatus.iProgressDur = aDuration;
    RedrawProgress();
    }
    
// ---------------------------------------------------------------------------
// Update volume
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::UpdateVolume(TInt aVolume, TInt aMaxVolume)
    {
    iCurrentStatus.iVolume = aVolume;
    iCurrentStatus.iMaxVolume = aMaxVolume;
    RedrawVolume();
    }
    
// ---------------------------------------------------------------------------
// Update artist and song name
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::UpdateSongInfo(const TDesC& aArtist, 
                                       const TDesC& aSongName)
    {
    TInt y = iDisplaySize.iHeight/ 2 - 2;
    //erase previous text
    for(TInt i = 0; i < iDisplaySize.iWidth; i++)
        {
        iConsole->SetPos(i,y);
        iConsole->Printf(_L(" "));
        iConsole->SetPos(i,y+1);
        iConsole->Printf(_L(" "));
        }
    iConsole->SetPos(
                Max((iDisplaySize.iWidth - aArtist.Length())/2, 0), y++);
    iConsole->Printf(_L("%S"), &aArtist);
    iConsole->SetPos(
                Max((iDisplaySize.iWidth - aSongName.Length())/2, 0), y);
    iConsole->Printf(_L("%S"), &aSongName);
    //DisplayViewL();
    }

// ---------------------------------------------------------------------------
// Handles state changes in playbackutility
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestPlaybackView::StateChanged(TTestPlaybackState aState)
    {
    
    switch(aState)
        {
    case ETPbStateInitialising:
        UpdateSongInfo(KNullDesC, _L("Opening file"));
    case ETPbStateShuttingDown:
    case ETPbStateBuffering:
    case ETPbStateDownloading:
    case ETPbStateStopped:
    case ETPbStateNotInitialised:
        iCurrentStatus.iPlaying = iCurrentStatus.iForwardSeek 
        = iCurrentStatus.iBackwardSeek = iCurrentStatus.iPause = EFalse;
        break;
    case ETPbStatePaused:
        iCurrentStatus.iPlaying = iCurrentStatus.iForwardSeek 
        = iCurrentStatus.iBackwardSeek = EFalse;
        iCurrentStatus.iPause = ETrue;
        break;
    case ETPbStatePlaying:
        iCurrentStatus.iPause = iCurrentStatus.iForwardSeek 
        = iCurrentStatus.iBackwardSeek = EFalse;
        iCurrentStatus.iPlaying = ETrue;
        break;
    case ETPbStateSeekingForward:
        iCurrentStatus.iBackwardSeek = EFalse;
        iCurrentStatus.iForwardSeek = ETrue;
        break;
    case ETPbStateSeekingBackward:
        iCurrentStatus.iForwardSeek = EFalse;
        iCurrentStatus.iBackwardSeek = ETrue;
        break;
        }
    RedrawPlayerStatue();
    }
    
// ---------------------------------------------------------------------------
// From class CTestBaseView.
// Initialization of a view before first time activation
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::InitializeViewL()
    {
    iConsole->ClearScreen();
    RedrawProgress();
    RedrawVolume();
    UpdateSongInfo(KNullDesC, KNullDesC);
    RedrawPlayerStatue();
    }
    
// ---------------------------------------------------------------------------
// From class CTestBaseView.
// Cleanup the child view before deactivate/destroy view
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::CleanupViewL()
    {
    }

// ---------------------------------------------------------------------------
// From class CTestBaseView.
// Display this view 
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::DisplayViewL()
    {
    DisplayViewBase();
    RedrawIndex();
    RedrawProgress();
    RedrawVolume();
    RedrawPlayerStatue();
    }
    
// ---------------------------------------------------------------------------
// From class CTestBaseView.
// Process keypresses on this view - called by ConsoleMain
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::SelectL( TKeyCode aSelection, TBool& aContinue )
    {
    aContinue = ETrue;
    switch(aSelection)
        {
        case '1':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdPlay);
            break;
        case '2':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdPrevious);
            break;
        case '3':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdNext);
            break;
        case '4':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdStop);
            break;
        case '5':
            iTestPlaybackViewObs->HandleUiCommandL(iCurrentStatus.iBackwardSeek?
                                                  ETPbViewCmdStopSeeking : 
                                                  ETPbViewCmdStartSeekBackward);
            break;
        case '6':
            iTestPlaybackViewObs->HandleUiCommandL(iCurrentStatus.iForwardSeek?
                                                  ETPbViewCmdStopSeeking : 
                                                  ETPbViewCmdStartSeekForward);
            break;
        case '7':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdPause);
            break;
        case '8':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdDecreaseVolume);
            break;
        case '9':
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdIncreaseVolume);
            break;
        case EKeyEscape:
            iTestPlaybackViewObs->HandleUiCommandL(ETPbViewCmdClose);
            aContinue = EFalse;
            CurrentViewDoneL();
            break;
        }
    }

// ---------------------------------------------------------------------------
// From class CTestBaseView.
// Update display from timer - called by ConsoleMain
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::TimerUpdate()
    {
    
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CTestPlaybackView::CTestPlaybackView(CConsoleMain* aConsoleMain,
                                      CTestBaseView* aParent,
                                      const TDesC& aName,
                                      CStifLogger* aLogger)
    : CTestBaseView(aConsoleMain, aParent, aName)
    {
    iDisplaySize = iConsole->ScreenSize();
    iDisplaySize.iWidth = iDisplaySize.iWidth - KDisplayScreenWidthOffset;
    iLogger = aLogger;
    }

// ---------------------------------------------------------------------------
// Symbian 2-phrase constructor
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::ConstructL()
    {
    
    iCurrentStatus.iBackwardSeek = EFalse;
    iCurrentStatus.iForwardSeek = EFalse;
    iCurrentStatus.iPlaying = EFalse;
    iCurrentStatus.iPause = EFalse;
    iCurrentStatus.iVolume = 0;
    iCurrentStatus.iMaxVolume = 20; //magic number??
    iCurrentStatus.iProgressPos = 0;
    iCurrentStatus.iProgressDur = KMaxTInt;
    iCurrentStatus.iItemIndex = 0;
    iCurrentStatus.iMaxItems = 0;
    
    InitializeViewL();
    }


// ---------------------------------------------------------------------------
// Redraw index in view
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::RedrawIndex()
    {
    if(iCurrentStatus.iMaxItems <= 0)
        return;
    iConsole->SetPos(iDisplaySize.iWidth - 5, 3);   //##/## format
    iConsole->Printf(_L("     "));                  //clean
    iConsole->SetPos(iDisplaySize.iWidth - 5, 3);
    iConsole->Printf(_L("%d/%d"), iCurrentStatus.iItemIndex, 
                                  iCurrentStatus.iMaxItems);
    
    }
    
// ---------------------------------------------------------------------------
// Redraw progress bar in view
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::RedrawProgress()
    {
    if(iCurrentStatus.iProgressDur <= 0)
        return;
    TInt pos = iCurrentStatus.iProgressPos * 
                    iDisplaySize.iWidth / iCurrentStatus.iProgressDur ;
    iConsole->SetPos(0,  iDisplaySize.iHeight / 2);
    for(TInt x = 1; x <= iDisplaySize.iWidth; x++)
        iConsole->Printf(x<=pos? _L("=") : _L("-"));
    }
    
// ---------------------------------------------------------------------------
// Redraw volume bar in view
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::RedrawVolume()
    {
    if(iCurrentStatus.iMaxVolume <= 0)
        return;
    TInt pos = iCurrentStatus.iVolume * 
                    iCurrentStatus.iMaxVolume / iCurrentStatus.iMaxVolume ;
    iConsole->SetPos(0, 1);
    iConsole->Printf(_L("Volume(8|9):"));
    for(TInt x = 1; x <= iCurrentStatus.iMaxVolume; x++)
        iConsole->Printf(x <= pos? _L("=") : _L("-"));
    iConsole->Printf(_L(" %d/%d"), 
                     iCurrentStatus.iVolume, 
                     iCurrentStatus.iMaxVolume);
    }

// ---------------------------------------------------------------------------
// Redraw status/control part of the view
// ---------------------------------------------------------------------------
//
void CTestPlaybackView::RedrawPlayerStatue()
    {
    TInt linePosY = iDisplaySize.iHeight - KSongInfoYOffset;
    
    //Line 1
    iConsole->SetPos(0, linePosY);
    iConsole->Printf(iCurrentStatus.iPlaying?
                        KStop : KStopSel);
    iConsole->Printf(KSpacer);
    iConsole->Printf(iCurrentStatus.iBackwardSeek?
                        KFastRewindSel : KFastRewind);
    iConsole->Printf(KSpacer);
    iConsole->Printf(iCurrentStatus.iForwardSeek?
                        KFastForwardSel : KFastForward);
    iConsole->Printf(KSpacer);
    iConsole->Printf(iCurrentStatus.iPause?
                        KPauseSel : KPause);
    
    //Line 2
    iConsole->SetPos(0, ++linePosY);
    iConsole->Printf(iCurrentStatus.iPlaying?
                        KPlaySel : KPlay);
    iConsole->Printf(KSpacer);
    iConsole->Printf(KBackward);
    iConsole->Printf(KSpacer);
    iConsole->Printf(KForward);
    
    //Line Exit
    iConsole->SetPos(iDisplaySize.iWidth - KExit().Length(),  
                     iDisplaySize.iHeight - 1);    
    iConsole->Printf(KExit);
    }
    
