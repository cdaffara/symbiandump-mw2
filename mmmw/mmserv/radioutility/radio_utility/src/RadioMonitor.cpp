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
* Description:  Radio Monitor interface implementation
*
*/


#include "RadioMonitorBody.h"
#include "trace.h"


// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CRadioMonitor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioMonitor* CRadioMonitor::NewL(
    MRadioMonitorObserver& aObserver )
    {
    FUNC_LOG;
    CRadioMonitor* self = new(ELeave) CRadioMonitor( aObserver );
    CleanupStack::PushL(self);
    self->iBody = CRadioMonitor::CBody::NewL( aObserver );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CRadioMonitor::~CRadioMonitor()
    {
    FUNC_LOG;
    delete iBody;
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::CRadioMonitor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRadioMonitor::CRadioMonitor(
    MRadioMonitorObserver& /*aObserver*/ )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CRadioMonitor::IsRadioOn
// Abstraction of PlayerState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CRadioMonitor::IsRadioOn() const
    {
    FUNC_LOG;
    ASSERT(iBody);
    return iBody->IsRadioOn();
    }



// End of File
