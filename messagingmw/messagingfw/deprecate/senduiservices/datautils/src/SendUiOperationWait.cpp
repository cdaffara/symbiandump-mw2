/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Active object that is used for waiting the asyncronous
*                request to complete, such as attachment insertion and 
*                launching editor operations.
*
*/



// INCLUDE FILES

#include <mtclbase.h>
#include <akninputblock.h>

#include "SendUiOperationWait.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CSendUiOperationWait::CSendUiOperationWait
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUiOperationWait::CSendUiOperationWait( TInt aPriority )
    : CActive( aPriority )
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CSendUiOperationWait::NewLC
// -----------------------------------------------------------------------------
//
EXPORT_C CSendUiOperationWait* CSendUiOperationWait::NewLC( TInt aPriority )
    {
    CSendUiOperationWait* self =
        new ( ELeave ) CSendUiOperationWait( aPriority );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( );
    return self;
    }

// Destructor
CSendUiOperationWait::~CSendUiOperationWait()
    {
    Cancel();
    if ( iInputBlock )
        {
        delete iInputBlock;
        }
    }


// -----------------------------------------------------------------------------
// CSendUiOperationWait::Start
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUiOperationWait::Start()
    {
    iSuccess = ETrue;
    SetActive();
    // Start the scheduler loop.
    iWait.Start();
    return iSuccess;
    }
// -----------------------------------------------------------------------------
// CSendUiOperationWait::Start
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUiOperationWait::Start(CActive* aOperation)
    {
    iBaseMtmOperation = NULL;
    iOperation = aOperation;
    return Start();
    }
// -----------------------------------------------------------------------------
// CSendUiOperationWait::Start
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSendUiOperationWait::Start(CBaseMtm* aBaseMtmOperation)
    {
    iOperation = NULL;
    iBaseMtmOperation = aBaseMtmOperation;
    return Start();
    }


// -----------------------------------------------------------------------------
// CSendUiOperationWait::RunL
// -----------------------------------------------------------------------------
//
void CSendUiOperationWait::RunL()
    {
    if ( iWait.IsStarted() )
        {
        // Stops the scheduler loop, when all nested scheduler loops have stopped.
        iWait.AsyncStop();
        }
    iOperation = NULL;
    }

// -----------------------------------------------------------------------------
// CSendUiOperationWait::DoCancel
// -----------------------------------------------------------------------------
//
void CSendUiOperationWait::DoCancel()
    {
    iSuccess = EFalse;    
    if ( iOperation )
        {
        iOperation->Cancel();
        iOperation = NULL;
        }
    else if(iBaseMtmOperation)
        {
        iBaseMtmOperation->CancelAttachmentOperation();
        iBaseMtmOperation = NULL;
        }
    
    if ( iWait.IsStarted() )
        {
        // Stops the scheduler loop, when all nested scheduler loops have stopped.
        iWait.AsyncStop();
        }
    iInputBlock->SetCancelActive( NULL ); // inputblock cancel was this DoCancel function. Deletion causes call to Cancel.
	delete iInputBlock;
	iInputBlock = NULL;
    }

// -----------------------------------------------------------------------------
// CSendUiOperationWait::ConstructL
// -----------------------------------------------------------------------------
//
void CSendUiOperationWait::ConstructL()
	{
    this->iInputBlock = CAknInputBlock::NewCancelActiveLC( this );  // notify "this" on cancel
	}

// End of file
