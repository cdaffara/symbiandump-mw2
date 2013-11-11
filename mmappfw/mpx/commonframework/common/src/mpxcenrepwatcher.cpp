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
* Description:  Class for observing changes in central repository values
*
*/



// INCLUDE FILES
#include <mpxcenrepobserver.h>
#include <mpxcenrepwatcher.h>

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXCenRepWatcher* CMPXCenRepWatcher::NewL(const TUid& aRepositoryUid,
                                          TUint32 aId,
                                       MMPXCenRepObserver* aObserver)
    {
    CMPXCenRepWatcher* self = new(ELeave) CMPXCenRepWatcher(
        aRepositoryUid, aId, aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMPXCenRepWatcher::CMPXCenRepWatcher(const TUid& aRepositoryUid,
                                     TUint32 aId,
                                     MMPXCenRepObserver* aObserver)
:   CActive(EPriorityStandard), 
    iRepository(NULL), 
    iRepositoryUid(aRepositoryUid), 
    iId(aId), 
    iObserver(aObserver)
    {
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMPXCenRepWatcher::ConstructL()
    {
    iRepository = CRepository::NewL(iRepositoryUid);
    CActiveScheduler::Add( this );
    User::LeaveIfError(iRepository->NotifyRequest(iId, iStatus));
    SetActive();
    }

// Destructor
EXPORT_C CMPXCenRepWatcher::~CMPXCenRepWatcher()
    {
    Cancel();
    delete iRepository;
    iObserver = NULL;
    }
    
// -----------------------------------------------------------------------------
// CMPXCenRepWatcher::CurrentValueL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXCenRepWatcher::CurrentValueL()
    {
    TInt res = 0;
    User::LeaveIfError(iRepository->Get(iId, res));
    return res;
    }    

// -----------------------------------------------------------------------------
// CMPXCenRepWatcher::SetValueL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXCenRepWatcher::SetValueL(TInt aNewValue)
    {
    User::LeaveIfError(iRepository->Set(iId, aNewValue));
    }    
    
// -----------------------------------------------------------------------------
// CMPXCenRepWatcher::RunL
// -----------------------------------------------------------------------------
//
void CMPXCenRepWatcher::RunL()
    {
    User::LeaveIfError(iRepository->NotifyRequest(iId, iStatus));
    SetActive();

    iObserver->HandleSettingChange(iRepositoryUid, iId);
    }

// -----------------------------------------------------------------------------
// CMPXCenRepWatcher::DoCancel
// -----------------------------------------------------------------------------
//
void CMPXCenRepWatcher::DoCancel()
    {
    iRepository->NotifyCancel(iId);
    }


// End of File
