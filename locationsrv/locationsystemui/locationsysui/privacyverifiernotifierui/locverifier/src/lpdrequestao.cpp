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
* Description:  Active object which ensures that current call stack is run to
*                completion before a new request is handled
*
*/


// INCLUDE FILES
#include "lpdrequestao.h"
#include "lpdverifierplugin.h"
#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_rposrequestorstack.h>

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLpdRequestAO::CLpdRequestAO
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdRequestAO::CLpdRequestAO( CLpdVerifierPlugin& aPlugin )
    : CActive( CActive::EPriorityHigh ), iPlugin( aPlugin )
    {
	}

// -----------------------------------------------------------------------------
// CLpdRequestAO::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdRequestAO* CLpdRequestAO::NewL( CLpdVerifierPlugin& aPlugin )
	{
    CLpdRequestAO* self = new( ELeave ) CLpdRequestAO( aPlugin );
    CActiveScheduler::Add(self);
    // Nothing to do in the Second Phase Constructor
    return self;
	}

// Destructor
CLpdRequestAO::~CLpdRequestAO()
	{
    Cancel();
    
	}

// -----------------------------------------------------------------------------
// CLpdRequestAO::ScheduleRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::ScheduleRequest()
    {	     
    Cancel();
    SetActive();
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrNone );            
    }

// -----------------------------------------------------------------------------
// CLpdRequestAO::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::RunL()
    {	    
   	iPlugin.HandleNextRequest();
    }

// -----------------------------------------------------------------------------
// CLpdRequestAO::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdRequestAO::DoCancel()
    {
    // Not important with this AO
	}



	
//  End of File
