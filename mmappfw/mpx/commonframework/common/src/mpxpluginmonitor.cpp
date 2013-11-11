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
* Description:  Plugin monitor
*
*  An instance of CMPXPluginMonitor monitors plugins for the designated
*  plugin interface. When a plugin for that plugin interface is added or
*  removed from the system, CMPXPluginMonitor is notified. CMPXPluginMonitor
   notifies its client through MMPXPluginMonitorObserver if any changes of plugin.
*
*
*/



#include <bamdesca.h>
#include <badesca.h>
#include <uri16.h>
#include <apgcli.h>

#include <mpxlog.h>
#include <mpxuser.h>
#include "mpxpluginmonitor.h"


// ============================ MEMBER FUNCTIONS ==============================
// ----------------------------------------------------------------------------
// Two-phased constructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPluginMonitor* CMPXPluginMonitor::NewL(
    const TUid& aInterfaceUid)
    {
    CMPXPluginMonitor* self = new(ELeave)CMPXPluginMonitor(aInterfaceUid);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
//
CMPXPluginMonitor::CMPXPluginMonitor(
    const TUid& aInterfaceUid)
:   CActive(EPriorityStandard),
    iInterfaceUid(aInterfaceUid)
    {
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// 2nd phase constructor.
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::ConstructL()
    {
    iECs = REComSession::OpenL();
    iECs.NotifyOnChange(iStatus);
    SetActive();
    }

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXPluginMonitor::~CMPXPluginMonitor()
    {
    Cancel();
    iObservers.Reset();
    iObservers.Close();
    iECs.Close();
    REComSession::FinalClose();
    }

// ----------------------------------------------------------------------------
// CMPXPluginMonitor::AddObserverL
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::AddObserverL(MMPXPluginMonitorObserver& aObs)
    {
    iObservers.AppendL(&aObs);
    }

// ----------------------------------------------------------------------------
// CMPXPluginMonitor::RemoveObserverL
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::RemoveObserverL(MMPXPluginMonitorObserver& aObs)
    {
    TInt i=iObservers.FindL(&aObs);
    iObservers.Remove(i);
    }

// ----------------------------------------------------------------------------
// CMPXPluginMonitor::NotifyUnload
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::NotifyUnload(
    const TUid& aPluginUid)
    {
    // notify client
    for (TInt i = iObservers.Count(); --i >= 0;)
        {
        iObservers[i]->HandlePluginUnload(aPluginUid);
        }
    }

// ----------------------------------------------------------------------------
// Handles request completion event
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::RunL()
    {
    MPX_DEBUG_THREAD("CMPXPluginMonitor::RunL");
    MPX_DEBUG2("CMPXPluginMonitor::RunL interface id 0x%08x", iInterfaceUid.iUid);
    iECs.NotifyOnChange(iStatus);
    SetActive();
    // notify client
    for (TInt i = iObservers.Count(); --i >= 0;)
        {
        iObservers[i]->PluginsChangedL();
        }
    }

// ----------------------------------------------------------------------------
// Handles a leave occurring in the request completion event handler RunL()
// ----------------------------------------------------------------------------
//
TInt CMPXPluginMonitor::RunError(TInt aError)
    {
    (void)aError;
    MPX_DEBUG_THREAD("CMPXPluginMonitor::RunError");
    MPX_DEBUG3("CMPXPluginMonitor::RunError interface id 0x%08x, error",
               iInterfaceUid.iUid, aError);
    // Restart observer
    if ( !IsActive() )
        {
        iECs.NotifyOnChange(iStatus);
        SetActive();
        }
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Implements cancellation of an outstanding request.
// ----------------------------------------------------------------------------
//
void CMPXPluginMonitor::DoCancel()
    {
    MPX_DEBUG_THREAD("CMPXPluginMonitor::DoCancel");
    MPX_DEBUG2("CMPXPluginMonitor::DoCancel interface id 0x%08x", iInterfaceUid.iUid);
    iECs.CancelNotifyOnChange(iStatus);
    }

// End of file
