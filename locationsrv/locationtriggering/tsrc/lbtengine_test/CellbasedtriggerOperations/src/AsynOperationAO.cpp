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
* Description:     Test module to implement Cell based triggering test cases
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
             
     //   RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "CTriggerFireObserver::RunL called "));
        error = iStatus.Int();
         if( iWait->IsStarted() )
	    {
	  //  	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "RunL wait stopped "));
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
	  aLbt.DeleteTriggers( iStatus,aFilter);
	  SetActive();
	
	}
	
void CTriggerFireObserver::PopulateFromCurrentCell(CLbtGeoCell& cell,CActiveSchedulerWait* aWait )	
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


void CLbtEventObserver::NotifyTriggerChangeEvent( RLbt& aLbt,TLbtTriggerChangeEvent& aEvent ,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
		aLbt.NotifyTriggerChangeEvent(aEvent,iStatus );
		SetActive( ); 
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
void CLbtEventObserver::ConstructL( RLbt& aLbt )
    {
     //	CTimer::ConstructL();
     //	iNotifier = CLbtTriggerFiringEventNotifier::NewL( aLbt,*this );
     RFileLogger aLog;
     iCidchangenotifier=CLbtCidChangeNotifier::NewL(aLog,this);

    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CLbtEventObserver::ConstructL()
    {
     RFileLogger aLog;
     iCidchangenotifier=CLbtCidChangeNotifier::NewL(aLog,this);
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


// -----------------------------------------------------------------------------
// CTriggerFireObserver::NewL
// -----------------------------------------------------------------------------
CLbtEventObserver* CLbtEventObserver::NewL()
    {
    CLbtEventObserver* self = new (ELeave) CLbtEventObserver();
    CleanupStack::PushL( self );
    self->ConstructL();
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
    	delete iCidchangenotifier;
   // 	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtEventObserver iCidchangenotifier deleted "));
    	
    	delete iWait;
  //  		RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "iWait deleted "));
	   // iWait=NULL;
	       

    }

void CLbtEventObserver::RunL( )
    {
     
   //    RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "CLbtEventObserver::RunL called "));
        
        if( iWait && iWait->IsStarted() )
	    {
//	      RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "RunL wait stopped "));
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

void CLbtEventObserver::GetCurrentCGIInfo( CTelephony::TNetworkInfoV1& aNwInfo,
                                           CActiveSchedulerWait* aWait )
    {
    iWait=aWait;
    iNetWorkInfo = &aNwInfo;
    iCidchangenotifier->GetCurrentCGIInfo();
    }
    
void CLbtEventObserver::HandleCIDChangeL(const CTelephony::TNetworkInfoV1 &aNwInfo)
    { 

//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Network mode = %d"),aNwInfo.iMode);
//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Country code = %s"),aNwInfo.iCountryCode);
//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Network id = %s"),aNwInfo.iNetworkId);
//	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Location Area code= %d"),aNwInfo.iLocationAreaCode);
	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "Cell Id= %d"),aNwInfo.iCellId);
	if( iNetWorkInfo )
	    {
        iNetWorkInfo->iMode = aNwInfo.iMode;
        iNetWorkInfo->iCountryCode = aNwInfo.iCountryCode;
        iNetWorkInfo->iNetworkId = aNwInfo.iNetworkId;
        iNetWorkInfo->iLocationAreaCode = aNwInfo.iLocationAreaCode;
        iNetWorkInfo->iCellId  = aNwInfo.iCellId;
        iWait->AsyncStop( );
	    }
    }
    
    void CLbtEventObserver::StartCidNotification( CActiveSchedulerWait* aWait )
	{
        iNetWorkInfo = NULL;
	  	iWait=aWait;
	  	iCidchangenotifier->Start();
	}
	


