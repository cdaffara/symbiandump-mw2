/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Delete trigger related test cases
*
*/





// INCLUDE FILES
#include "t_deletetriggerobserver.h"

#include <lbt.h>
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyarea.h>
#include <lbttriggerfilterbyattribute.h> 

#include <lbtmanager.h>





// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDeleteTriggerObserver::CDeleteTriggerObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CDeleteTriggerObserver::CDeleteTriggerObserver():CActive( CActive::EPriorityStandard ),iWait(NULL)
    {
    
        CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CDeleteTriggerObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDeleteTriggerObserver::ConstructL( )
    {
     	
    }

// -----------------------------------------------------------------------------
// CDeleteTriggerObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CDeleteTriggerObserver* CDeleteTriggerObserver::NewL()
    {
    
	    CDeleteTriggerObserver* self = new (ELeave) CDeleteTriggerObserver( );

	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop();

	    return self;

    }

// Destructor
CDeleteTriggerObserver::~CDeleteTriggerObserver()
    { 

	    Cancel(); 
	    
    }

void CDeleteTriggerObserver::RunL( )
    {
        
        iCompletionCode=iStatus.Int();
        
        if( iWait && iWait->IsStarted() )
	    {
	      iWait->AsyncStop( );	
	    }
    }

void CDeleteTriggerObserver::DoCancel( )
    {
     
     
    }

void CDeleteTriggerObserver::DeleteFilteredTriggers( RLbt& aLbt,
                                                     CLbtTriggerFilterBase* aFilter,
                                                     CActiveSchedulerWait* aWait 
                                                   )
	{
		
	  iWait=aWait;
	  aLbt.DeleteTriggers( iStatus,aFilter);
	  SetActive();
	
	}

void CDeleteTriggerObserver::DeleteTriggersById( RLbt& aLbt,
                                                 const RArray< TLbtTriggerId >& aTriggerIdList,
                                                 CActiveSchedulerWait* aWait 
                                               )
	{
	  iWait=aWait;
	  aLbt.DeleteTriggers( aTriggerIdList,iStatus );
	  SetActive(); 	
	}
	
void CDeleteTriggerObserver::DeleteAllTriggers( RLbt& aLbt,
                                             CActiveSchedulerWait* aWait
                                           )
	{
	  iWait=aWait;
	  aLbt.DeleteTriggers(iStatus,NULL);
	  SetActive();
	  	
	}

TInt CDeleteTriggerObserver::CompletionCode()
	{
	
	  return(iCompletionCode);	
	}
//  End of File
