/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Radio Monitor body implementation
*
*/


#include <RadioMonitor.h>
#include "RadioMonitorBody.h"
#include "RadioServerData.h"
#include "trace.h"

// This has to be the last include. 
#ifdef STUB_CONSTELLATION
#   include "RadioStubManager.h"
#   define KRadioServerPropertyCategory KStub_KRadioServerPropertyCategory
#endif //STUB_CONSTELLATION

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRadioMonitor::CBody::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioMonitor::CBody* CRadioMonitor::CBody::NewL(
    MRadioMonitorObserver& aObserver )
    {
    FUNC_LOG;
    CRadioMonitor::CBody* s = new(ELeave) CRadioMonitor::CBody( aObserver );
    CleanupStack::PushL(s);
    s->ConstructL();
    CleanupStack::Pop();
    return s;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CRadioMonitor::CBody::~CBody()
    {
    FUNC_LOG;
    Cancel();
    iProperty.Close();
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::CBody::CBody
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CRadioMonitor::CBody::CBody( MRadioMonitorObserver& aObserver ) :
    CActive(EPriorityStandard),
    iRadioMonitorClient(aObserver)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CRadioMonitor::CBody::ConstructL()
    {
    FUNC_LOG;
    CActiveScheduler::Add(this);

    // Attach property
    User::LeaveIfError(iProperty.Attach(KRadioServerPropertyCategory,
                                        KRadioServPsMonitorState));

    iProperty.Subscribe(iStatus);
    SetActive();
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::IsRadioOn
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CRadioMonitor::CBody::IsRadioOn() const
    {
    FUNC_LOG;
    TBool radioState = ERadioStateOff;
    RProperty::Get(KRadioServerPropertyCategory,
                   KRadioServPsMonitorState,
                   radioState);

    return radioState;
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::RunL
// Called when property value changes
// -----------------------------------------------------------------------------
//
void CRadioMonitor::CBody::RunL()
    {
    FUNC_LOG;
    TBool radioState = ERadioStateOff;

    // Resubscribe before processing new value to prevent missing updates
    iProperty.Subscribe(iStatus);
    SetActive();

    TInt error = iProperty.Get(radioState);
    if ( error == KErrNone )
        {
        iRadioMonitorClient.MrmEvent((TRadioMonitorEvent)radioState);
        }
    else if ( error == KErrNotFound )
        {
        iRadioMonitorClient.MrmEvent(ERadioStateOff);
        }
    else
        {
        // pass
        }
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::DoCancel
// Cancels event listening
// -----------------------------------------------------------------------------
//
void CRadioMonitor::CBody::DoCancel()
    {
    FUNC_LOG;
    iProperty.Cancel();
    }

// End of File
