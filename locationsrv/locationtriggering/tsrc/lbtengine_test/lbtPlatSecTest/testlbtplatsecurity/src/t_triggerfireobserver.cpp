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
* Description:    Test module to implement Platform security related test cases
*
*/





// INCLUDE FILES
#include "t_triggerfireobserver.h"
#include <lbtmanager.h>



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTriggerFireObserver::CTriggerFireObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTriggerFireObserver::CTriggerFireObserver( TCoordinate& aCoordinate ):iCoordinate(aCoordinate), 
iWait(NULL),CTimer( CTimer::EPriorityStandard )
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
     
     
    }


void CTriggerFireObserver::CheckPositionInfoL( const TLbtTriggerFireInfo& aFireInfo )
    {
     
     
    }
    
    
TInt CTriggerFireObserver::GetFireCount( ) 

{
	return iTriggerFireCount;
}

void CTriggerFireObserver::Setwait(CActiveSchedulerWait* aWait  ) 

{
    iWait=aWait; 
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


void CTriggerFireObserver::CreateIterator(RLbt& aLbt, 
            CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions)   
{
    
    iWait=aWait;	
	aLbt.CreateGetTriggerIterator(iStatus,NULL);
	SetActive( ); 
}

void CTriggerFireObserver::DeleteTriggers(RLbt& aLbt,CLbtTriggerFilterBase* aFilter,CActiveSchedulerWait* aWait)
{
	 iWait=aWait;	
	aLbt.DeleteTriggers(iStatus,aFilter);
	SetActive( ); 
}

void CTriggerFireObserver::DeleteTriggers(RLbt& aLbt,RArray<TLbtTriggerId>& aTriggerArray,CActiveSchedulerWait* aWait)
{
	 iWait=aWait;	
	aLbt.DeleteTriggers(aTriggerArray,iStatus);
	SetActive( ); 
}

void CTriggerFireObserver::SetTriggersState(RLbt& aLbt,CLbtTriggerFilterBase* aFilter,CLbtTriggerEntry::TLbtTriggerState aState,CActiveSchedulerWait* aWait)
{
	 iWait=aWait;	
	aLbt.SetTriggersState(iStatus,aState,ELbtTrue,aFilter);
	SetActive( ); 
}

void CTriggerFireObserver::ListTriggerIds(RLbt& aLbt,RArray < TLbtTriggerId >& aTriggerIdArray,CActiveSchedulerWait* aWait)
{
	 iWait=aWait;	
	aLbt.ListTriggerIds(iStatus,aTriggerIdArray,NULL);
	SetActive( ); 
}

void CTriggerFireObserver::GetTriggers( RLbt& aLbt,
                                 RPointerArray < CLbtTriggerInfo >& aTriggerList,
                                 CLbtListTriggerOptions* aListOptions,
                                 CActiveSchedulerWait* aWait
                               ) 
{
	iWait=aWait;
	aLbt.GetTriggers(iStatus,aTriggerList,aListOptions);
	SetActive();
}

void CTriggerFireObserver::CreateGetTriggerIterator(RLbt& aLbt,CActiveSchedulerWait* aWait)
{
	iWait=aWait;	
	aLbt.CreateGetTriggerIterator(iStatus,NULL);
	SetActive( ); 
}

void CTriggerFireObserver::NotifyTriggerChangeEvent(RLbt& aLbt,TLbtTriggerChangeEvent& aEvent,CActiveSchedulerWait* aWait)
{
	iWait=aWait;	
	aLbt.NotifyTriggerChangeEvent(aEvent,iStatus);
	SetActive( ); 
}

void CTriggerFireObserver::NotifyTriggerFired(RLbt& aLbt, TLbtTriggerFireInfo& aFireInfo,CActiveSchedulerWait* aWait)
{
	iWait=aWait;	
	aLbt.NotifyTriggerFired(aFireInfo,iStatus);
	SetActive( ); 
}

void CTriggerFireObserver::NotifyTriggeringSystemSettingChange(RLbt& aLbt,TLbtTriggeringSystemSettings& aSettings,CActiveSchedulerWait* aWait)
{
	iWait=aWait;	
	aLbt.NotifyTriggeringSystemSettingChange(aSettings,iStatus);
	SetActive( ); 
}

void CTriggerFireObserver::NotifyTriggeringSystemStatusChange(RLbt& aLbt,CActiveSchedulerWait* aWait)
 {
 	iWait=aWait;
	RLbtManager* lbtManager=static_cast<RLbtManager*>(&aLbt);	
	lbtManager->NotifyTriggeringSystemStatusChange(iStatus);
	SetActive( ); 
 }
//  End of File
