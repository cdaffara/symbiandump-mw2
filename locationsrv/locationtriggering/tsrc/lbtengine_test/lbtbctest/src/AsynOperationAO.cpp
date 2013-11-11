/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    Test module to implement Test cases for BC
*
*/





// INCLUDE FILES
#include "AsynOperationAO.h"
//#include <lbtoperation.h>
#include <flogger.h>
_LIT(KLbtTraceDir,"epos");
 _LIT(KLbtTraceFile,"TriggerFireInfo.log");



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTriggerFireObserver::CTriggerFireObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTriggerFireObserver::CTriggerFireObserver(  ): 
CTimer( CTimer::EPriorityStandard ),iWait(NULL)
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
CTriggerFireObserver* CTriggerFireObserver::NewL( RLbt& aLbt)
    {
    
	    CTriggerFireObserver* self = new (ELeave) CTriggerFireObserver(  );

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
	    delete iWait;
	   // iWait=NULL;
	       

    }

void CTriggerFireObserver::TriggerFiredL( const TLbtTriggerFireInfo& aFireInfo )
	{
	   
	    RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Trigger FiredL called "));
	    iTriggerFireCount++;
	    iWaitStatus =0;
	    iFireInfo=aFireInfo;
	     if( iWait->IsStarted() )
	    {
	      RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "TriggerFiredL wait stopped "));
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
             
        RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "CTriggerFireObserver::RunL called "));
        error = iStatus.Int();
         if( iWait->IsStarted() )
	    {
	    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "RunL wait stopped "));
        iWait->AsyncStop( );
	    }
    }

void CTriggerFireObserver::DoCancel( )
    {
     
     
    }


void CTriggerFireObserver::CheckPositionInfoL( const TLbtTriggerFireInfo& /*aFireInfo*/ )
    {
     
     
    }
    
    
TInt CTriggerFireObserver::GetFireCount( ) 

{
	return iTriggerFireCount;
}

TInt CTriggerFireObserver::GetError( ) 

{
	return error;
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
	aLbt.CreateGetTriggerIterator(iStatus,aListOptions);
	SetActive( ); 
}
//  End of File


void CTriggerFireObserver::DeleteFilteredTriggers( RLbt& aLbt,
                                                     CLbtTriggerFilterBase* aFilter,
                                                     CActiveSchedulerWait* aWait 
                                                   )
	{
		
	  iWait=aWait;
	  aLbt.DeleteTriggers(iStatus,aFilter);
	  SetActive();
	
	}
	
void CTriggerFireObserver::PopulateFromCurrentCell(CLbtGeoCell& /*cell*/,CActiveSchedulerWait* aWait )	
{
	iWait=aWait;
	//cell.PopulateFromCurrentCellL(iStatus);
	SetActive();
}

void CTriggerFireObserver::UpdateTriggersState(RLbt& aLbt, CLbtTriggerEntry::TLbtTriggerState aState ,
            CActiveSchedulerWait* aWait, CLbtTriggerFilterBase *  aFilter)   
{
    
    iWait=aWait;	
	aLbt.SetTriggersState(iStatus,aState,ELbtTrue,aFilter);
	SetActive( ); 
}


/*CLBTOperation * CTriggerFireObserver::CreateGeoAreaForCurrentLocationL( CLbtGeoAreaBase::TGeoAreaType aTechType,
																        CLbtGeoAreaBase ** aArea,
																        CActiveSchedulerWait* aWait )
{
	iWait=aWait;
	CLBTOperation* operation;
	operation = CLBTOperation::CreateGeoAreaForCurrentLocationL(iStatus,aTechType,aArea);
	if(operation != NULL)
		SetActive( ); 
	return operation;
}*/


void CTriggerFireObserver::DeleteTriggersById( RLbt& aLbt,
                                                 const RArray< TLbtTriggerId >& aTriggerIdList,
                                                 CActiveSchedulerWait* aWait 
                                               )
	{
	  iWait=aWait;
	  aLbt.DeleteTriggers( aTriggerIdList,iStatus );
	  SetActive(); 	
	}
	
void CTriggerFireObserver::NotifyTriggerFired(RLbt& aLbt, TLbtTriggerFireInfo& aFireInfo,CActiveSchedulerWait* aWait)
{
	iWait=aWait;	
	aLbt.NotifyTriggerFired(aFireInfo,iStatus);
	SetActive( ); 
}
void CTriggerFireObserver::NotifyTriggeringSettingsChange( RLbt& aLbt,TLbtTriggeringSystemSettings &  aSettings,TLbtTriggeringSystemSettings /*dummySettings*/,CActiveSchedulerWait* aWait
                                                         )   
{
	iWait = aWait;
    //iSettingsChangeReqIssued=ETrue;
   // iSettings=dummySettings;
    aLbt.NotifyTriggeringSystemSettingChange( aSettings,iStatus );
	SetActive( ); 
}

void CTriggerFireObserver::UpdateTrigger(RLbt& aLbt,CLbtTriggerEntry& aTrigger,TLbtTriggerAttributeFieldsMask aFieldMask,CActiveSchedulerWait* aWait,
            TLbtFireOnUpdate aFireOnUpdate) 
{
	iWait=aWait;	
	aLbt.UpdateTrigger(aTrigger,aFieldMask,aFireOnUpdate,iStatus );    
	SetActive( ); 
}
void CLbtEventObserver::NotifyTriggerChangeEvent( RLbt& aLbt,TLbtTriggerChangeEvent& aEvent ,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
		aLbt.NotifyTriggerChangeEvent(aEvent,iStatus );
		SetActive( ); 
	}
	
void CLbtEventObserver::NotifyTriggerChangeEvent2L( RLbt& aLbt,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
	    itrigchangenotifier= CLbtTriggerChangeEventNotifier ::NewL( aLbt,*this );
		itrigchangenotifier->Start();
	}
	
void CLbtEventObserver::NotifyTriggeringsettingsChangeEventL( RLbt& aLbt,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
	    isettingschangenotifier= CLbtTriggeringSystemSettingsChangeEventNotifier::NewL( aLbt,*this);
		isettingschangenotifier->Start();
	}		

void CLbtEventObserver::TriggeringsettingsChangeEventDestructor()   
	{
	isettingschangenotifier->~CLbtTriggeringSystemSettingsChangeEventNotifier();
	}
	
void CLbtEventObserver::TriggerChangeEventDestructor()   
	{
	itrigchangenotifier->~CLbtTriggerChangeEventNotifier();
	}		

/*void CLbtEventObserver::NotifyTriggerChangeEvent( RLbtManager& aLbtmanager,TLbtTriggerChangeEvent& aEvent ,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
		aLbtmanager.NotifyTriggerChangeEvent(aEvent,iStatus );
		SetActive( ); 
	}
*/

CLbtEventObserver::CLbtEventObserver(  ) :
CTimer( CTimer::EPriorityStandard ),iWait(NULL)
    {
    
        CActiveScheduler::Add(this);
    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLbtEventObserver::ConstructL( RLbt& /*aLbt*/ )
    {
     //	CTimer::ConstructL();
     //	iNotifier = CLbtTriggerFiringEventNotifier::NewL( aLbt,*this );
    // RFileLogger aLog;
    // iCidchangenotifier=CLbtCidChangeNotifier::NewL(aLog,this);

    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CLbtEventObserver* CLbtEventObserver::NewL( RLbt& aLbt)
    {
    
    CLbtEventObserver* self = new (ELeave) CLbtEventObserver(  );

	    CleanupStack::PushL( self );
	    self->ConstructL( aLbt );
	    CleanupStack::Pop();

	    return self;

    }

// Destructor
CLbtEventObserver::~CLbtEventObserver()
    { 
	//		RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtEventObserver before Cancel"));
	    Cancel(); 
	//    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtEventObserver After Cancel "));
	    // Delete notifier object
    //	delete iCidchangenotifier;
    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtEventObserver iCidchangenotifier deleted "));
    	
    	delete iWait;
    	delete itrigchangenotifier;
    	delete isettingschangenotifier;
    //		RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "iWait deleted "));
	   // iWait=NULL;
	       

    }

void CLbtEventObserver::RunL( )
    {
     
       RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "CLbtEventObserver::RunL called "));
        
        if( iWait && iWait->IsStarted() )
	    {
	      RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "RunL wait stopped "));
	      iWait->AsyncStop( );	
	    }
    }

void CLbtEventObserver::DoCancel( )
    {
     
     
    }
    
    void CLbtEventObserver::CreateTriggers( RLbt& aLbt,
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

void CLbtEventObserver::TriggerChangedL( const TLbtTriggerChangeEvent& aEvent )
	{
	   iEvent = aEvent;
	    if(aEvent.iEventType == ELbtTriggerChangeEventCreated)
	    {
	    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Trigger Created "));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventDeleted)
	    {
	    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Trigger Deleted "));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventUpdated)
	    {
	    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Trigger Deleted "));
	    }
	    else if(aEvent.iEventType == ELbtTriggerChangeEventMultiple)
	    {
	    	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Multiple Events "));
	    }
	     if( iWait->IsStarted() )
	    {
	      RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "TriggerFiredL wait stopped "));
	      iWait->AsyncStop( );	
	    }
		
	}
	
void CLbtEventObserver::TriggeringSystemSettingsChangedL( const TLbtTriggeringSystemSettings& aSettings )
	{
	   iSettings = aSettings;
	    
	     if( iWait->IsStarted() )
	    {
	      RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "TriggerFiredL wait stopped "));
	      iWait->AsyncStop( );	
	    }
		
	}	

/*void CLbtEventObserver::HandleCIDChangeL(const CTelephony::TNetworkInfoV1 &aNwInfo)
    { 

//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Network mode = %d"),aNwInfo.iMode);
//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Country code = %s"),aNwInfo.iCountryCode);
//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Network id = %s"),aNwInfo.iNetworkId);
	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Location Area code= %d"),aNwInfo.iLocationAreaCode);
	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Cell Id= %d"),aNwInfo.iCellId);

    }
    
    void CLbtEventObserver::StartCidNotification( CActiveSchedulerWait* aWait )
	{
	
	  	iWait=aWait;
	  	iCidchangenotifier->Start();
	}*/
