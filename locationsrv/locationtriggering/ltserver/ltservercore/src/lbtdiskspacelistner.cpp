/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of CLbtDiskSpaceListner class
*
*/


#include "lbtdiskspacelistner.h"
#include "lbtdiskspaceobserver.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::NewL
// CLbtPsyChangeListner instantiation method
// -----------------------------------------------------------------------------
//
CLbtDiskSpaceListner* CLbtDiskSpaceListner::NewL( MLbtDiskSpaceObserver& aObserver )
    {
    CLbtDiskSpaceListner* self = new( ELeave ) CLbtDiskSpaceListner( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// -----------------------------------------------------------------------------
// CLbtDiskSpaceListner::CLbtDiskSpaceListner
// Default constructor
// -----------------------------------------------------------------------------
//
CLbtDiskSpaceListner::CLbtDiskSpaceListner( MLbtDiskSpaceObserver& aObserver ):
                                        CActive( EPriorityNormal ),iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CLbtPsyChangeListner::ConstructL
// 
// -----------------------------------------------------------------------------
//
void CLbtDiskSpaceListner::ConstructL()
    {
    User::LeaveIfError( iFs.Connect() );
    }

    
// -----------------------------------------------------------------------------
// CLbtDiskSpaceListner::~CLbtDiskSpaceListner
// Destructor
// -----------------------------------------------------------------------------
//
CLbtDiskSpaceListner::~CLbtDiskSpaceListner()
    {
    Cancel();
    iFs.Close();
    }

// -----------------------------------------------------------------------------
// CLbtDiskSpaceListner::StartToListen
// 
// -----------------------------------------------------------------------------
//
void CLbtDiskSpaceListner::StartToListen()
    {
    LOG("CLbtDiskSpaceListner::StartToListenL");
    iFs.NotifyChange( ENotifyAll ,iStatus );
    SetActive();
    }

// -----------------------------------------------------------------------------
// CLbtDiskSpaceListner::RunL
// 
// -----------------------------------------------------------------------------
//
void CLbtDiskSpaceListner::RunL()
    {
    LOG("CLbtDiskSpaceListner::RunL");
    LOG1("Status:%d",iStatus.Int());
    if( iStatus.Int() == KErrNone )
        {
        iObserver.HandleDiskSpaceChange();  
        }
    }
    
// -----------------------------------------------------------------------------
// CLbtDiskSpaceListner::DoCancel
// 
// -----------------------------------------------------------------------------
//    
void CLbtDiskSpaceListner::DoCancel()
    {
    iFs.NotifyChangeCancel();
    }
    
// End of file
