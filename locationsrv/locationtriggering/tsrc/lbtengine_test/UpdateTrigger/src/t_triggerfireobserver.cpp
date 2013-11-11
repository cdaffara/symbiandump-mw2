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
* Description:     Test module to implement Update trigger related test cases
*
*/






// INCLUDE FILES
#include "t_triggerfireobserver.h"
//#include <lbtoperation.h>


//#if 0
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTriggerFireObserver::CTriggerFireObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTriggerFireObserver::CTriggerFireObserver( TCoordinate& aCoordinate ):CTimer( CTimer::EPriorityStandard ),iCoordinate(aCoordinate), 
iWait(NULL)
    {
    
        CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTriggerFireObserver::ConstructL( RLbt& aLbt )
    {
     
     	iTriggerFireCount =0;
     	CTimer::ConstructL();
     	iNotifier = CLbtTriggerFiringEventNotifier::NewL( aLbt,*this );

    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTriggerFireObserver* CTriggerFireObserver::NewL( RLbt& aLbt,TCoordinate& aCoordinate )
    {
    
	    CTriggerFireObserver* self = new (ELeave) CTriggerFireObserver( aCoordinate );

	    CleanupStack::PushL( self );
	    self->ConstructL( aLbt );
	    CleanupStack::Pop();

	    return self;

    }

// Destructor
CTriggerFireObserver::~CTriggerFireObserver()
    { 

	    Cancel(); 
	    // Delete notifier object
	    delete iNotifier;
	    
	    iWait=NULL;
	       

    }

void CTriggerFireObserver::TriggerFiredL( const TLbtTriggerFireInfo& aFireInfo )
	{
	    iTriggerFireCount++;
	    iWaitStatus =0;
	    iFireInfo=aFireInfo;
	     if( iWait->IsStarted() )
	    {
	      iWait->AsyncStop( );	
	    }
		CheckPositionInfoL( aFireInfo );
	}
	
void CTriggerFireObserver::StartNotification( CActiveSchedulerWait* aWait )
	{
	
	  	iWait=aWait;
	  	iNotifier->Start( );
	}
	
void CTriggerFireObserver::CancelNotification( )
	{
	
	   	iNotifier->Cancel( );
	}

void CTriggerFireObserver::RunL( )
    {
     
         if( iWait->IsStarted() )
	    {
        iWait->AsyncStop( );
	    }
    }

void CTriggerFireObserver::DoCancel( )
    {
     
     iNotifier->Cancel( );
    }


void CTriggerFireObserver::CheckPositionInfoL( const TLbtTriggerFireInfo& aFireInfo )
    {
     
     
    }
    
    
TInt CTriggerFireObserver::GetFireCount( ) 

{
	return iTriggerFireCount;
}


TLbtTriggerFireInfo CTriggerFireObserver::GetFiredTrigger( ) 

{
	return iFireInfo;
}
    
void CTriggerFireObserver::CreateTriggers( RLbt& aLbt,
                                           CLbtTriggerEntry& aEntry,
                                           TLbtTriggerId& aId,
                                           TBool aFireOnCreation,
                                           CActiveSchedulerWait* aWait 
                                         )   
{
    
    iWait=aWait;	
	aLbt.CreateTrigger( aEntry,aId,aFireOnCreation,iStatus );
	SetActive( ); 
}

void CTriggerFireObserver::GetTriggersList(RLbt& aLbt, RPointerArray < CLbtTriggerInfo >& aTriggerList,
            CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions )   
{
    
    iWait=aWait;	
	aLbt.GetTriggers(iStatus,aTriggerList,aListOptions);
	SetActive( ); 
}

void CTriggerFireObserver::ListTriggerIdsList(RLbt& aLbt, RArray < TLbtTriggerId >& aTriggerIdList,
            CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions)   
{
    
    iWait=aWait;	
	aLbt.ListTriggerIds(iStatus,aTriggerIdList,aListOptions);
	SetActive( ); 
}

void CTriggerFireObserver::UpdateTriggersState(RLbt& aLbt, CLbtTriggerEntry::TLbtTriggerState aState ,
            CActiveSchedulerWait* aWait,TLbtFireOnUpdate aFireOnUpdate, CLbtTriggerFilterBase *  aFilter)   
{
    
    iWait=aWait;	
	aLbt.SetTriggersState(iStatus,aState,aFireOnUpdate,aFilter);
	SetActive( ); 
}

void CTriggerFireObserver::UpdateTrigger(RLbt& aLbt,CLbtTriggerEntry& aTrigger,TLbtTriggerAttributeFieldsMask aFieldMask,CActiveSchedulerWait* aWait,
            TLbtFireOnUpdate aFireOnUpdate) 
{
	iWait=aWait;	
	aLbt.UpdateTrigger(aTrigger,aFieldMask,aFireOnUpdate,iStatus );    
	SetActive( ); 
}

