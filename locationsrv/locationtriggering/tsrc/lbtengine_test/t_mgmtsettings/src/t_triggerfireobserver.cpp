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
* Description:    Test module to implement Test cases for LBT Management settings
*
*/




// INCLUDE FILES
#include "t_triggerfireobserver.h"


#include <lbttriggerfiringeventnotifier.h>
#include <lbtsessiontrigger.h>
#include <lbtstartuptrigger.h>
#include <lbttriggerentry.h>




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTriggerFireObserver::CTriggerFireObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CTriggerFireObserver::CTriggerFireObserver( TCoordinate aCoordinate,TInt aRadiusInMetres ):CTimer( CTimer::EPriorityStandard ),iTriggerFired(EFalse),iCompletionCode(0),iWait(NULL),iCoordinate(aCoordinate),iRadiusInMetres(aRadiusInMetres),
iSettingsChangeReqIssued(EFalse)
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
     	CTimer::ConstructL();
     	iNotifier = CLbtTriggerFiringEventNotifier::NewL( aLbt,*this );

    }

// -----------------------------------------------------------------------------
// CTriggerFireObserver::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//

CTriggerFireObserver* CTriggerFireObserver::NewL( RLbt& aLbt,TCoordinate aCoordinate,TInt aRadiusInMetres )
    {
    
	    CTriggerFireObserver* self = new (ELeave) CTriggerFireObserver( aCoordinate,aRadiusInMetres );

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
	    if( iWait && iWait->IsStarted() )
	    {
	      iWait->AsyncStop( );	
	    }
	    
		iTriggerFired=ETrue;
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
     
        iCompletionCode= iStatus.Int();
        
        if( iSettingsChangeReqIssued )
        {
          TReal32 minArea=iSettings.MinimumTriggerAreaSize();
          TLbtTriggeringMechanismState state=iSettings.TriggeringMechanismState();	
        
        
        }
        
        if( iWait && iWait->IsStarted() )
	    {
	      iWait->AsyncStop( );	
	    }
    }

void CTriggerFireObserver::DoCancel( )
    {
     
     
    }


void CTriggerFireObserver::CheckPositionInfoL( const TLbtTriggerFireInfo& aFireInfo )
    {
     
     TPositionInfo posInfo= aFireInfo.iFiredPositionInfo;
     TPosition pos;
     posInfo.GetPosition(pos);
     TCoordinate firedinfo( pos.Latitude(),pos.Longitude());
     TReal32 distance;
     firedinfo.Distance( iCoordinate,distance);
     
	     if( distance<=iRadiusInMetres )
	     {
	       	User::Leave(KErrGeneral);
	     	
	     }
     
     
     
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


void CTriggerFireObserver::NotifyTriggeringSettingsChange( RLbt& aLbt,
                                                          TLbtTriggeringSystemSettings &  aSettings,
                                                          TLbtTriggeringSystemSettings dummySettings,
                                                          CActiveSchedulerWait* aWait
                                                         )   
{
	iWait = aWait;
    iSettingsChangeReqIssued=ETrue;
    iSettings=dummySettings;
    aLbt.NotifyTriggeringSystemSettingChange( aSettings,iStatus );
	SetActive( ); 
}

void CTriggerFireObserver::NotifyTriggeringSettingsChange( RLbtManager& aLbt,
                                                          TLbtTriggeringSystemManagementSettings &  aSettings,
                                                          TLbtTriggeringSystemManagementSettings dummySettings,
                                                          CActiveSchedulerWait* aWait
                                                         )   
{
	iWait = aWait;
    iSettingsChangeReqIssued=ETrue;
    iSettings=dummySettings;
    aLbt.NotifyTriggeringSystemSettingChange( aSettings,iStatus );
	SetActive( ); 
}
void CTriggerFireObserver::CancelNotifyTriggeringSettingsChange( RLbt& aLbt )   
{
    aLbt.CancelNotifyTriggeringSystemSettingChange();
}

void CTriggerFireObserver::DeleteAllTriggers( RLbtManager& aLbtmanager,CActiveSchedulerWait* aWait )
{
iWait=aWait;
aLbtmanager.DeleteTriggers(iStatus,NULL);
SetActive();

}

void CTriggerFireObserver::CreateIterator(RLbtManager& aLbtmanager, 
        CActiveSchedulerWait* aWait,CLbtListTriggerOptions* aListOptions)   
{

iWait=aWait;	
aLbtmanager.CreateGetTriggerIterator(iStatus,aListOptions);
SetActive( ); 
}
//  End of File
void CLbtEventObserver::NotifyTriggerChangeEvent( RLbt& aLbt,TLbtTriggerChangeEvent& aEvent ,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
		aLbt.NotifyTriggerChangeEvent(aEvent,iStatus );
		SetActive( ); 
	}
	
void CLbtEventObserver::NotifyTriggerChangeEvent2L( RLbt& aLbt,CActiveSchedulerWait* aWait )   
	{
		iWait = aWait;
		CLbtTriggerChangeEventNotifier * trigchangenotifier= CLbtTriggerChangeEventNotifier ::NewL( aLbt,*this );
		trigchangenotifier->Start();
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
    //	RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "~CLbtEventObserver iCidchangenotifier deleted "));
    	
    	delete iWait;
    //		RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "iWait deleted "));
	   // iWait=NULL;
	       

    }

void CLbtEventObserver::RunL( )
    {
     
     //  RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "CLbtEventObserver::RunL called "));
        
        if( iWait && iWait->IsStarted() )
	    {
	  //    RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "RunL wait stopped "));
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
	    
	     if( iWait->IsStarted() )
	    {
	   //   RFileLogger::WriteFormat(KLbtTraceDir, KLbtTraceFile, EFileLoggingModeAppend,_L( "TriggerFiredL wait stopped "));
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
	


