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
* Description:  File System format monitor
*
*/


#include <e32base.h>
#include <f32file.h>
#include <driveinfo.h>
#include <mpxlog.h>
#include "mpxfsformatmonitor.h"

static const TInt KDriveCount = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Default Constructor
// ---------------------------------------------------------------------------
//
CMPXFsFormatMonitor::CMPXFsFormatMonitor( MMPXSystemEventObserver& aObserver ) :
                                                        iObserver( aObserver )
    {

    }


// ---------------------------------------------------------------------------
// Second Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXFsFormatMonitor::ConstructL()
    {
    MPX_DEBUG1(_L("CMPXFsFormatMonitor::ConstructL <---"));

    iBackupSession = CBaBackupSessionWrapper::NewL();
    iBackupSession->RegisterBackupOperationObserverL( *this );

    TInt drive = EDriveE;
    for(TInt i=0; i<KDriveCount; ++i)
        {
        iBackupDrives.Append(drive);
        ++drive;
        }

    MPX_DEBUG1(_L("CMPXFsFormatMonitor::ConstructL --->"));
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXFsFormatMonitor* CMPXFsFormatMonitor::NewL
                                        ( MMPXSystemEventObserver& aObserver )
    {
    CMPXFsFormatMonitor* self = CMPXFsFormatMonitor::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXFsFormatMonitor* CMPXFsFormatMonitor::NewLC
                                        ( MMPXSystemEventObserver& aObserver )
    {
    CMPXFsFormatMonitor* self = new( ELeave ) CMPXFsFormatMonitor( aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CMPXFsFormatMonitor::~CMPXFsFormatMonitor()
    {
    iBackupDrives.Close();

    if( iBackupSession )
        {
        iBackupSession->DeRegisterBackupOperationObserver( *this );
        }
    delete iBackupSession;
    }

// ---------------------------------------------------------------------------
// Checks the current status
// ---------------------------------------------------------------------------
//
void CMPXFsFormatMonitor::PollStatus()
    {
    TBool aFormatting = iBackupSession->IsBackupOperationRunning();
    if( aFormatting )
        {
        for(TInt i=0; i<KDriveCount; ++i)
            {
            TRAP_IGNORE(iObserver.HandleSystemEventL(EFormatStartEvent, iBackupDrives[i]));
            }
        }
    }

// ---------------------------------------------------------------------------
// CMPXFsFormatMonitor::HandleBackupOperationEventL
// Handles a format operation
// ---------------------------------------------------------------------------
//
void CMPXFsFormatMonitor::HandleBackupOperationEventL(
                  const TBackupOperationAttributes& aBackupOperationAttributes)
    {
    MPX_DEBUG1(_L("CMPXFsFormatMonitor::HandleBackupOperationEventL <---"));

    if( aBackupOperationAttributes.iOperation == EStart )
        {
        for(TInt i=0; i<KDriveCount; ++i)
            {
            iObserver.HandleSystemEventL(EFormatStartEvent, iBackupDrives[i]);
            }
        }
    else  // TOperationType::EEnd or TOperationType::EAbort
        {
        //Ignore if formatting is ongoing
        TBool formatting = iBackupSession->IsBackupOperationRunning();
        if ( formatting )
        	{
            return;
        	}
        for(TInt i=0; i<KDriveCount; ++i)
            {
            iObserver.HandleSystemEventL(EFormatEndEvent, iBackupDrives[i]);
            }
        }

    MPX_DEBUG1(_L("CMPXFsFormatMonitor::HandleBackupOperationEventL --->"));
    }
