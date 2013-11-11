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
* Description:    Test module to implement Test cases for C,T Classes
*
*/





// INCLUDE FILES
#include "t_aooperation.h"

#include <lbt.h> 
#include <lbtmanager.h> 
#include <lbttriggerfilterbase.h>
#include <lbttriggerfilterbyattribute.h>
#include <lbtlisttriggeroptions.h>
#include <lbttriggerinfo.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAoOperation::CAoOperation
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CAoOperation::CAoOperation( ):CActive(EPriorityStandard)
{
    
        CActiveScheduler::Add(this);
        iWait=NULL;
}

// -----------------------------------------------------------------------------
// CAoOperation::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAoOperation::ConstructL( )
{
}

// -----------------------------------------------------------------------------
// CAoOperation::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CAoOperation* CAoOperation::NewL( )
{
    
	    CAoOperation* self = new (ELeave) CAoOperation();

	    CleanupStack::PushL( self );
	    self->ConstructL();
	    CleanupStack::Pop();

	    return self;

}

// Destructor
CAoOperation::~CAoOperation()
{ 

	    Cancel(); 
}


void CAoOperation::RunL( )
{
     
        iCompletionCode= iStatus.Int();
                        
        // Depending on the request type,take the actions.
        
        if( iWait && iWait->IsStarted() )
	    {
	      iWait->AsyncStop( );	
	    }
}

void CAoOperation::DoCancel( )
{
     
     
}

void CAoOperation::SetRequestType(TRequestType aRequestType)
{
	iRequestType=aRequestType;
}

void CAoOperation::SetTriggerState( RLbt& aLbt,
	                                CLbtTriggerEntry::TLbtTriggerState aState,
	                                CLbtTriggerFilterBase* aFilter,
	                                CActiveSchedulerWait* aWait
	                              )
{
	iWait=aWait;
	aLbt.SetTriggersState(iStatus,aState,ELbtTrue,aFilter);
	SetActive();
	
}

void CAoOperation::ListTriggerIds( RLbt& aLbt,
                                   RArray < TLbtTriggerId >& aTriggerIdList, 
                                   CLbtListTriggerOptions* aListOptions,
                                   CActiveSchedulerWait* aWait
                                 )
{
	iWait=aWait;
	aLbt.ListTriggerIds(iStatus,aTriggerIdList,aListOptions);
	SetActive();
	
}

void  CAoOperation::GetTriggers( RLbt& aLbt,
                                 RPointerArray < CLbtTriggerInfo >& aTriggerList,
                                 CLbtListTriggerOptions* aListOptions,
                                 CActiveSchedulerWait* aWait
                               ) 
{
	iWait=aWait;
	aLbt.GetTriggers(iStatus,aTriggerList,aListOptions);
	SetActive();
}



//  End of File
