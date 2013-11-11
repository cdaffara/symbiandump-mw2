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
* Description:   Server application for launching SendUi with low 
*                capability requirements.
*
*/



#include <avkon.hrh>
#include <akntitle.h>                   // CAknTitlePane
#include <aknnotewrappers.h> 
#include <sendui.h>

#include "senduilauncherappui.h"
#include "senduilauncher.hrh"
#include "senduiserviceslog.h"

// ======== LOCAL FUNCTIONS ========

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CSendUiLauncherAppUi::~CSendUiLauncherAppUi()
    {
    delete iIdle;
    }

// ---------------------------------------------------------------------------
// Use SendUiImpl class to launch SendUi. Called from CSendUiService.
// ---------------------------------------------------------------------------
//
void CSendUiLauncherAppUi::CreateAndSendMessageL( 
        TUid aServiceUid, 
        CMessageData* aMessageData, 
        TBool aLaunchEmbedded )
    {
    LOGTEXT(_L("LauncherAppUi::CreateAndSendMessageL >>"));
    
    CSendUi* sendUi = CSendUi::NewL();
    CleanupStack::PushL( sendUi );
    sendUi->CreateAndSendMessageL(
        aServiceUid,
        aMessageData,
        KNullUid,
        aLaunchEmbedded );
     CleanupStack::PopAndDestroy( sendUi );
     LOGTEXT(_L("LauncherAppUi::CreateAndSendMessageL <<"));
     }

// ---------------------------------------------------------------------------
// Delayed exit is used for server app to exit automatically and properly.
// ---------------------------------------------------------------------------
//
void CSendUiLauncherAppUi::DoDelayedExitL( TInt aDelayTime )
    {
    LOGTEXT(_L("LauncherAppUi::DoDelayedExit >>"));
    delete iIdle;
    iIdle = 0;
    // Priority must be lower than the priority of iWrappedWaitDialog
    iIdle = CPeriodic::NewL( EPriorityNormal - 1);
    iIdle->Start(   aDelayTime,
                    aDelayTime, 
                    TCallBack( DelayedExit, this ));
    }

// ---------------------------------------------------------------------------
// Delayed exit is used for server app to exit properly.
// ---------------------------------------------------------------------------
//
TInt CSendUiLauncherAppUi::DelayedExit( TAny* aThis )
    {
    LOGTEXT(_L("CSendUiLauncherAppUi: DelayedExit >>"));
                
    CSendUiLauncherAppUi* editor = static_cast<CSendUiLauncherAppUi*>( aThis );
    editor->Exit( );
    LOGTEXT(_L("LauncherAppUi::DelayedExit <<"));
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// HandleCommandL
// ---------------------------------------------------------------------------
//   
void CSendUiLauncherAppUi::HandleCommandL(TInt aCommand)
    {
    switch(aCommand)
       {
        case EEikCmdExit:  // fall through
        case EAknSoftkeyExit:
            Exit();
            break;

        default:
            break;
        }
    } 

// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//
void CSendUiLauncherAppUi::ConstructL()
    {
    LOGTEXT(_L("LauncherAppUi::ConstructL >>"));
                
    BaseConstructL(ENoAppResourceFile);
    HideApplicationFromFSW( ETrue ); 
    
    // application name is shown, if not set otherwise
    _LIT( KEmptyText, "" );
    CAknTitlePane* title = static_cast<CAknTitlePane*>
    (StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle )));
    title->SetTextL( KEmptyText );

    LOGTEXT(_L("LauncherAppUi::ConstructL <<"));
    }


// ---------------------------------------------------------------------------
// Empty constructor
// ---------------------------------------------------------------------------
//
CSendUiLauncherAppUi::CSendUiLauncherAppUi()
    {
    }
