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
*
*/


// INCLUDES
#include "MCbs.h"
#include "CCbsUiSettingsMonitor.h"
//#include "CbsUiPanic.h"
#include "MCbsUiSettingsObserver.h"




// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::CCbsUiSettingsMonitor
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CCbsUiSettingsMonitor::CCbsUiSettingsMonitor(
    MCbs& aServer, 
    MCbsUiSettingsObserver& aObserver )
    : 
    CActive(EPriorityStandard), 
    iContinue(ETrue), 
    iServer(aServer),
    iObserver(aObserver)
    {
    }

// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::ConstructL
// Symbian OS default constructor can leave.
// ----------------------------------------------------------------------------
//
void CCbsUiSettingsMonitor::ConstructL( )
    {    
    CActiveScheduler::Add( this );
    IssueRequest();
    }

// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::NewL
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
CCbsUiSettingsMonitor* CCbsUiSettingsMonitor::NewL(
    MCbs& aServer, MCbsUiSettingsObserver& aObserver )
    {
    CCbsUiSettingsMonitor* self = new (ELeave) CCbsUiSettingsMonitor( 
        aServer, aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// ----------------------------------------------------------------------------
// CbsUiViewManager::activateMainView
// Destructor
// ----------------------------------------------------------------------------
//
CCbsUiSettingsMonitor::~CCbsUiSettingsMonitor()
    {
    iContinue = EFalse;
    Cancel();
    }


// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::IssueRequest
// 
// This metor issues request
// ----------------------------------------------------------------------------
//
void CCbsUiSettingsMonitor::IssueRequest()
    {
    if ( iContinue )
        {
        SetActive();        
        iServer.NotifySettingsChanged( iStatus, iEvent );
        }
    }

// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::RunL
// 
// calls SettingsChanged method on application class 
// ----------------------------------------------------------------------------
//
void CCbsUiSettingsMonitor::RunL()  
    {
    iObserver.SettingsChangedL(iEvent); // Doesn't inform the reason!!

  /*  __ASSERT_DEBUG( iEvent == ECbsModifiedReceptionStatus ||
                    iEvent == ECbsModifiedTopicDetectionStatus ||
                    iEvent == ECbsModifiedLanguages, 
                    CbsUiPanic(EErrInvalidEvent) );
*/
    if ( iContinue )
        {
        IssueRequest();
        }
    }


// ----------------------------------------------------------------------------
// CCbsUiSettingsMonitor::DoCancel
// 
// ----------------------------------------------------------------------------
//
void CCbsUiSettingsMonitor::DoCancel()
    {
    iContinue = EFalse;    
    iServer.NotifySettingsChangedCancel();
    }

//  End of File  
