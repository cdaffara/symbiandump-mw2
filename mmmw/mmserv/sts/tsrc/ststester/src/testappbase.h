/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 * Header specifying the common test functionality.
 */

#ifndef TESTAPPBASE_H_
#define TESTAPPBASE_H_

#include <e32base.h>
#include <w32std.h>
#include <remconcoreapitargetobserver.h>  // for volume key handling
class CRemConCoreApiTarget;
class CRemConInterfaceSelector;

#define STR(a) (TText*)L##a

// 15 keys are supported.  The keys are in this order:
//    Enter, Up, Down, Left, Right, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
const TInt KSupportedKeysCount = 15;

struct TOperationEntry
    {
    const TText* text;
    TInt operation;
    };

struct TOperationsPage
    {
    const TText* pageName;
    TInt defaultSoftkeyIndex;
    TOperationEntry mapping[KSupportedKeysCount];
    };

// Predefined standard operations
const TInt KOperation_None = 0;
const TInt KOperation_Exit = 1;
const TInt KOperation_PreviousOptionPage = 2;
const TInt KOperation_NextOptionPage = 3;
const TInt KOperation_NextOption = 4;
const TInt KOperation_PreviousOption = 5;
const TInt KOperation_ExecuteOption = 6;
const TInt KOperation_ToggleHelpVisibility = 7;
const TInt KOperation_ToggleHelpTransparency = 8;
const TInt KOperation_FirstCustomIndex = 10; // app-specific operations can start here

const TUint32 KNullWsHandle = 0xFFFFFFFF;

class CTestAppBase : public CActive, private MRemConCoreApiTargetObserver
    {
public:

    CTestAppBase(TInt aFontSize);

    ~CTestAppBase();

    // inherited from CActive
    void RunL();
    void DoCancel();

protected:

    virtual void ExecuteOperation(TInt aOperation,
            const TDesC& aOperationText) = 0;

    // Subclasses can override this function to take action when the current softkey function has been changed.
    virtual void SoftkeyFunctionUpdated()
        {
        }
    ;

    // Subclasses can override this function to override the default key event handling.
    virtual bool ConsumeKeyEvent(TInt /*aScanCode*/)
        {
        return false;
        }
    ;

    void BaseConstructL(const TOperationsPage* aKeyMap, TInt aPageCount);

    void SetupVolumeKeysL();

    void StartMonitoringWindowEvents();

    TInt CurrentPageNumber();

    TPtrC CurrentPageName();

    TPtrC CurrentSoftkeyName();

    // Presents a selection list to the user and returns the index of the selected entry.
    // Synchronous call.
    // returns -1 if the selection was backed out without making a selection
    TInt SelectFromListL(TPoint aTopLeft, TSize aSize,
            const TDesC& aHeaderText, RPointerArray<TDesC>& aSelectionList,
            TInt aInitialSelectionIndex = 0);

    // Synchronous call.
    // returns false if the selection was backed out without making a selection
    bool SelectDriveL(TPoint aTopLeft, TSize aSize, const TDesC& aHeaderText,
            TDes& aDrive);

    // Synchronous call.
    // returns false if the selection was backed out without making a selection
    bool SelectFileL(TPoint aTopLeft, TSize aSize, const TDesC& aHeaderText,
            const TDesC& aDrive, TDes& aFullFilename);

    // Synchronous call.
    // returns false if the selection was backed out without making a selection
    bool SelectFileWithHistoryL(TPoint aTopLeft, TSize aSize,
            TDes& aFullFilename, const TDesC& aHistoryFilename,
            TInt aMaxHistoryEntries);

    // Synchronous call.
    // returns false if the selection was backed out without making a selection
    bool SelectIntegerL(TPoint aTopLeft, TSize aSize,
            const TDesC& aHeaderText, TInt aMin, TInt aMax, TInt& aSelection); // set aSelection to default value

    // Synchronous call.  Returns the scan code of the pressed key.
    TInt WaitForAnyKey();

    const TInt iFontSize;

    RFs iFs;
    RWsSession iWs;
    CWsScreenDevice* iScreenDevice;
    RWindowGroup* iWindowGroup;
    CWindowGc* iGc;
    CFont* iFont;
    CFbsTypefaceStore* iTypefaceStore;
    TSize iDisplaySize;
    RWindow* iSelectionWindow;
    RWindow* iHelpWindow;

private:

    enum TTestAppPointerEvent
        {
        EPointerEvent_None,
        EPointerEvent_Up,
        EPointerEvent_Down,
        EPointerEvent_Left,
        EPointerEvent_Right,
        EPointerEvent_Select
        };

    // inherited from MRemConCoreApiTargetObserver
    void MrccatoCommand(TRemConCoreApiOperationId aOperationId,
            TRemConCoreApiButtonAction aButtonAct);

    TInt KeyMapOperation(TInt aIndex, TInt aPage);

    TPtrC KeyMapText(TInt aIndex, TInt aPage);

    void IncrementKeymapIndex(TInt& aIndex, TInt aPage);

    void DecrementKeymapIndex(TInt& aIndex, TInt aPage);

    void CalculateHelpWindowSize();

    void DrawHelpText();

    void DoSelectFileL(TPoint aTopRight, TSize aWindowSize,
            const TDesC& aHeaderText, const TFileName& aDirectory,
            TInt aDirectoryLevel, TDes& aSelectedDirectory,
            TDes& aSelectedFilename);

    void ReadDirectoryEntriesL(const TFileName& aDirectoryName,
            RPointerArray<TDesC>& aFileNames);

    void ReadFileHistory(const TDesC& aHistoryFilename);

    void AddToFileHistory(const TDesC& aFilename,
            const TDesC& aHistoryFilename, TInt aMaxHistoryEntries);

    TTestAppPointerEvent CharacterizePointerEvent(
            TAdvancedPointerEvent& event);

    const TOperationsPage* iKeyMap;
    TInt iPageCount;
    TInt iCurrentPage;
    TInt iSoftkeyIndex;
    RPointerArray<HBufC> iFileHistory;

    CActiveSchedulerWait iWait;

    TPoint iHelpWindowTopRight;
    TSize iHelpWindowSize;
    TInt iHelpWindowColumn1Width;
    TInt iHelpWindowColumn2Width;
    bool iHelpActive;
    bool iHelpSemitransparentBackgroundActive;

    TPoint iPointerDownPosition;

    // For volume key support
    CRemConCoreApiTarget* iCoreTarget;
    CRemConInterfaceSelector* iInterfaceSelector;
    };

#endif // TESTAPPBASE_H_
