/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Location Trigger Creation and monitoring
*
*/


#include "lbttriggeradapter.h"
#include "lbtcontextsourceplugindebug.h"
#include "lbtcontextsourcepluginconst.hrh"


// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::CLocationTriggerAdapter
// ---------------------------------------------------------------------------
//
CLocationTriggerAdapter::CLocationTriggerAdapter( MLBTContextTriggerObserver& aObserver ) :
CActive( EPriorityStandard ),iNotifier(NULL),
iObserver( aObserver )
    {
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter()");
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::~CLocationTriggerAdapter
// ---------------------------------------------------------------------------
//
CLocationTriggerAdapter::~CLocationTriggerAdapter()
    {
    CActive::Cancel();
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "~CLocationTriggerAdapter()");
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "Closing Lbt");
    iLbt.Close();
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "Closing Lbt Notifier");
    iLbtNotifier.Close();
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "Closing Lbt Server");
    iLbtServ.Close();
    
    if( iNotifier )
        {
        delete iNotifier;
        iNotifier = NULL;
        }
    
    LBTCONTEXTSOURCEPLUGINDEBUG( "~CLocationTriggerAdapter : Out");
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::NewLC
// ---------------------------------------------------------------------------
//
CLocationTriggerAdapter* CLocationTriggerAdapter::NewLC( MLBTContextTriggerObserver& aObserver )
    {
    CLocationTriggerAdapter* self = new (ELeave) CLocationTriggerAdapter( aObserver );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::NewL
// ---------------------------------------------------------------------------
//
CLocationTriggerAdapter* CLocationTriggerAdapter::NewL( MLBTContextTriggerObserver& aObserver )
    {
    CLocationTriggerAdapter* self = CLocationTriggerAdapter::NewLC( aObserver );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::ConstructL
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::ConstructL()
    {
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::ConstructL() : In");
    TInt ret = iLbtServ.Connect();

    if( ret == KErrNone ) 
        {
        LBTCONTEXTSOURCEPLUGINDEBUG( "Opening LBT connection");
        User::LeaveIfError(iLbt.Open(iLbtServ));
        User::LeaveIfError(iLbtNotifier.Open(iLbtServ));
        
        LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::ConstructL() : Out");
        }
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::CreateContextTriggerL
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::CreateContextTriggerL( TTriggerNode& aTriggerNode )
    {
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::CreateContextTriggerL - IN" );
    iTriggerList.iTriggerId = KLbtNullTriggerId;
    iTriggerList.contextName.Set( aTriggerNode.GetContextSource() );
    TPtrC contextSource = aTriggerNode.GetContextSource();
    LBTCONTEXTSOURCEPLUGINDEBUG1( "GetContextSource = %S", &contextSource );
    // Get the trigger name
    TPtrC triggerName;
    if( !( aTriggerNode.GetTriggerName( triggerName ) ) )
        {
        iTriggerList.triggerName.Set( triggerName );
        LBTCONTEXTSOURCEPLUGINDEBUG1( "GetTriggerName = %S", &triggerName );
        if ( IsActive() )
            {
            User::Leave( KErrInUse );
            }
        // Get the latitude & longitude
        TReal latitude;
        TReal longitude;
        if( !( aTriggerNode.GetLatitude( latitude ) ) && 
                !( aTriggerNode.GetLongitude( longitude ) ) )
            {
            LBTCONTEXTSOURCEPLUGINDEBUG1( "GetLatitude = %f", latitude );
            LBTCONTEXTSOURCEPLUGINDEBUG1( "GetLongitude = %f", longitude );
            TCoordinate centre( latitude,longitude ); 
            // Get the region
            TPtrC region;
            if( !( aTriggerNode.GetRegion( region ) ) )
                {
                LBTCONTEXTSOURCEPLUGINDEBUG1( "GetRegion = %S", &region );
                // Get the radius
                TReal radius;
                if( !( aTriggerNode.GetRadius( radius ) ) )
                    {
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "GetRadius = %f", radius );
                    CLbtGeoCircle* trigArea = CLbtGeoCircle::NewL( 
                                        centre,radius );
                                            
                    CleanupStack::PushL( trigArea );

                    CLbtTriggerConditionArea* cond = 
                        CLbtTriggerConditionArea::NewL( 
                        trigArea,
                        aTriggerNode.GetDirection() );
                    
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "GetDirection = %d", aTriggerNode.GetDirection() );
                        
                    
                    CleanupStack::PushL( cond );
                    
                    TUid managerUi;
                    managerUi = TUid::Uid( KLBTContextSourcePluginDllUid );
                    
                    CLbtSessionTrigger* trig = CLbtSessionTrigger::NewL( 
                            triggerName,               
                            aTriggerNode.GetCurrentState(),
                            CRequestorBase::ERequestorService,         
                            CRequestorBase::EFormatApplication,         
                            KMyRequestor,                               
                            managerUi,                                   
                            cond                                       
                        );  
                    LBTCONTEXTSOURCEPLUGINDEBUG1( "GetCurrentState = %d", aTriggerNode.GetCurrentState() );
                        
                    CleanupStack::PushL( trig );
                    
                    iLbt.CreateTrigger( *trig, iTriggerList.iTriggerId, ETrue, iStatus );
                    
                    SetActive();
                    
                    CleanupStack::PopAndDestroy( trig );
                    CleanupStack::Pop( cond );
                    CleanupStack::Pop( trigArea ); 
                    }
                else
                    {
                    User::Leave(KErrGeneral);
                    }
                }
            else
                {
                User::Leave(KErrGeneral);
                }
            }
        else
            {
            User::Leave(KErrGeneral);
            }
        }
    else  
	    {      
        User::Leave(KErrGeneral);
		}
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::CreateContextTriggerL - OUT" ); 
   }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::TriggerFiredL
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::TriggerFiredL( const TLbtTriggerFireInfo& /*aFireInfo*/ )
  {
  LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::TriggerFiredL() : In");
  
  RArray < TLbtTriggerFireInfo > triggerInfoList;
  iLbt.GetFiredTriggersL(triggerInfoList);
  CleanupClosePushL( triggerInfoList );
  
  for( int i=0; i<triggerInfoList.Count(); i++)
      {
      LBTCONTEXTSOURCEPLUGINDEBUG1( "Trigger Fired, Trigger id : %d", triggerInfoList[i].iTriggerId );
      TLbtTriggerId firedTrigger = triggerInfoList[i].iTriggerId;
      iObserver.ContextTriggerFiredL( firedTrigger );
      }

  CleanupStack::PopAndDestroy( &triggerInfoList );
  LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::TriggerFiredL() : Out");
  }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::DeleteAllTriggersL
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::DeleteAllTriggersL()
    {
    iLbt.DeleteTriggersL( NULL );
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::RunL
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::RunL()
    {
    LBTCONTEXTSOURCEPLUGINDEBUG( "CLocationTriggerAdapter::RunL() : In");
    if(iStatus.Int()!=KErrNone)
        {       
        LBTCONTEXTSOURCEPLUGINDEBUG( "Trigger Create FAILED");
        // Trigger not created properly
        iTriggerList.iTriggerId = KLbtNullTriggerId;                
        User::Leave( iStatus.Int() );
        return;
        }
             
    if( !iNotifier )
        {
        LBTCONTEXTSOURCEPLUGINDEBUG( "Starting LBT Notifier");
        iNotifier = CLbtTriggerFiringEventNotifier::NewL( iLbtNotifier,*this );
        iNotifier->Start();
        }
    
    // If trigger created successfully, call define contexts
    LBTCONTEXTSOURCEPLUGINDEBUG1( "Trigger Create Successful, Trigger ID : %d",iTriggerList.iTriggerId);
    iObserver.TriggerCreatedL( iTriggerList.iTriggerId , 
                               iTriggerList.contextName , 
                               iTriggerList.triggerName );
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::RunError
// ---------------------------------------------------------------------------
//
TInt CLocationTriggerAdapter::RunError( TInt aError )
    {
    if( aError != KErrNone )
        {
        LBTCONTEXTSOURCEPLUGINDEBUG1( "Trigger Create FAILED with error : %d",aError);
        //if error, notify it to the engine.      
        iObserver.NotifyError( aError );
        return aError;
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerAdapter::DoCancel
// ---------------------------------------------------------------------------
//
void CLocationTriggerAdapter::DoCancel()
    {    
    iLbt.CancelAll();

    }
