/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of cell supervisor class
*
*/



#include <lbttriggerconditionarea.h>
#include <lbtgeohybrid.h>

#include "lbtcellsupervisor.h"
#include "lbtlogger.h"
#include "flogger.h"

// CONSTANTS

static const TInt KCellHysterisis = 2;


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CLbtCellSupervisor::NewL
// CLbtCellSupervisor instantiation method
// --------------------------------------------------------------------------
//
CLbtCellSupervisor* CLbtCellSupervisor::NewL( TAny* aConstructionParameters, 
                                              CLbtTriggerView* aView,
                                              MLbtStrategyEngineObsrvr* aObserver,
                                              CLbtCellChangeHandler* aCellChangeHandler)
    {
    CLbtCellSupervisor* self = new ( ELeave ) CLbtCellSupervisor();
    CleanupStack::PushL( self );
    self->ConstructL( aConstructionParameters, aView,aObserver,aCellChangeHandler );
    CleanupStack::Pop();
    return self;
    }


// -----------------------------------------------------------------------------
// CLbtCellSupervisor::~CLbtCellSupervisor
// Default destructor
// -----------------------------------------------------------------------------
//
CLbtCellSupervisor::~CLbtCellSupervisor()
    {
    // This is just done as a precautionary measure!
    StopSupervision();
    
    iRecentlyFiredTriggerArray.Close();
    delete iCellSupervisorTimer;
    }	

// -----------------------------------------------------------------------------
// CLbtCellSupervisor::CLbtCellSupervisor
// Default destructor
// -----------------------------------------------------------------------------
//
CLbtCellSupervisor::CLbtCellSupervisor()
    {
    
    }
    
// -----------------------------------------------------------------------------
// CLbtCellSupervisor::StartSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::StartSupervision()
    {
    FUNC_ENTER("CLbtCellSupervisor::StartSupervision");
    iCellChangeHandler->SetObserver( this );
	}

    
// -----------------------------------------------------------------------------
// CLbtCellSupervisor::StopSupervision
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::StopSupervision()
    {
    FUNC_ENTER("CLbtCellSupervisor::StopSupervision");
    iCellChangeHandler->Remove( this );
    iCellSupervisorTimer->Cancel();
    }
    

// -----------------------------------------------------------------------------
// CLbtCellSupervisor::SettingsChanged
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::SettingsChanged()
    {
    iEngObserver->RetrieveTriggeringSettings( iSettings );
    }


// -----------------------------------------------------------------------------
// CLbtCellSupervisor::Resume
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::Resume()
    {
    iRecentlyFiredTriggerArray.Reset();
    FUNC_ENTER("CLbtCellSupervisor::Resume");
    // Iterate through view and all recently fired triggers triggers
    if( iView->CellTriggerCount() )
        {
        iViewIterator.Reset();
        
        CLbtContainerTriggerEntry* conTrigger;
        
        TBool isPresent = ETrue;
        while( isPresent )
            {
            conTrigger = NULL;
            TRAP_IGNORE( isPresent = iViewIterator.NextL( conTrigger ) );
            if( conTrigger )
                {
                // Fetch strategy data.
                TStrategyData strategyData = conTrigger->StrategyData();
                
                // If the fired time is non-zero, add it to recently fired trigger list
                if( strategyData.iFireTime )
                    {
                    TInt error = iRecentlyFiredTriggerArray.Append( conTrigger );
					if( error != KErrNone )
						{
						LOG1("Failed to append conTrigger into the array:%d",error);
						}
                    }
                }
            }
        }
      
    iCellChangeHandler->SetObserver( this );
    }

// -----------------------------------------------------------------------------
// CLbtCellSupervisor::Reset
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::Reset()
    {
    iRecentlyFiredTriggerArray.Reset();
    }


// -----------------------------------------------------------------------------
// CLbtCellSupervisor::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CLbtCellSupervisor::ConstructL( TAny* /*aConstructionParameters*/,
                                     CLbtTriggerView* aView,
                                     MLbtStrategyEngineObsrvr* aObserver,
                                     CLbtCellChangeHandler* aCellChangeHandler )
    {
    iEngObserver = aObserver;
	//iEngObserver->RetrieveTriggeringSettings( iSettings );
	iView = aView;
	
	iCellChangeHandler = aCellChangeHandler;
	iViewIterator = iView->BeginL( CLbtGeoAreaBase::ECellular,CLbtGeoAreaBase::EHybrid );
	iCellSupervisorTimer = CLbtCellSupervisorTimer::NewL( *this );
	}
	
		
// ----------------------------------------------------------------------------
// CLbtCellSupervisor::HandleCellChangeEvent
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::HandleCellChangeEvent(const TInt aStatus,
                                               const RMobilePhone::TMobilePhoneNetworkInfoV1& aNwInfo,
                                               const RMobilePhone::TMobilePhoneLocationAreaV1& aLocArea)
    {    
    FUNC_ENTER("CLbtCellSupervisor::HandleCellChangeEvent");
    if( KErrNone == aStatus )
	    {
	    // validate network info
		if (aNwInfo.iMode == RMobilePhone::ENetworkModeGsm ||
            aNwInfo.iMode == RMobilePhone::ENetworkModeWcdma)
            {
            TLex lex( aNwInfo.iCountryCode );
            lex.Val( iCurrentCountryCode );
            if( iCurrentCountryCode )
                {
                lex.Assign( aNwInfo.iNetworkId );
                lex.Val( iCurrentNetworkCode );
                if( iCurrentNetworkCode )
                    {
                    if( aLocArea.iLocationAreaCode && aLocArea.iCellId )
                        {
                        iNwInfo = aNwInfo;
		                iLocArea = aLocArea;
		                LOG1("Current cellid:%d", iLocArea.iCellId);
		                TRAP_IGNORE(EvaluateTriggersL());
                        }
                    }
                }
            }
		}
    }
    
    
// ----------------------------------------------------------------------------
// CLbtCellSupervisor::EvaluateTriggersL
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::EvaluateTriggersL()
    {
    FUNC_ENTER("CLbtCellSupervisor::EvaluateTriggersL");
    CLbtContainerTriggerEntry* conTrigger;
    
    iViewIterator.Reset();
    
    while( iViewIterator.NextL( conTrigger ) )
        {
        if( conTrigger )    
            {
            CLbtTriggerEntry* trigger = conTrigger->TriggerEntry();
            if( CLbtTriggerConditionBase::ETriggerConditionArea == 
                                                trigger->GetCondition()->Type() )
                {
                // Area type trigger
                CLbtTriggerConditionArea* triggerConArea = 
                                        static_cast< CLbtTriggerConditionArea* >
                                                ( trigger->GetCondition() );
                   
                if( triggerConArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECellular )
                    {
                    EvalCellularTrigger( conTrigger );
                    }
                else if( triggerConArea->TriggerArea()->Type() == CLbtGeoAreaBase::EHybrid )
                    {
                    EvalHybridTrigger( conTrigger );
                    }
                }
            }
        }
    
    EstimateSleepInterval();    
    }


// ----------------------------------------------------------------------------
// CLbtCellSupervisor::EvalCellularTrigger
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::EvalCellularTrigger( CLbtContainerTriggerEntry* aTrigger )
    {
    FUNC_ENTER("CLbtCellSupervisor::EvalCellularTrigger");    
    
    CLbtTriggerConditionArea* trgCondition = 
                              static_cast< CLbtTriggerConditionArea* >
                              ( ( aTrigger->TriggerEntry() )->GetCondition() );
                
    CLbtGeoCell* cellularArea = static_cast< CLbtGeoCell* >
                                               ( trgCondition->TriggerArea() );


    // Fetch strategy data.
    TStrategyData strategyData = aTrigger->StrategyData();
    
    // Calculate remaining rearm time
    TInt remRearmTime = CalculateRearmTime( strategyData,aTrigger->TriggerEntry()->TimeToRearm());
    
    // If rearm time is not expired, trigger is not supervised.
    if( remRearmTime > 0 )
        {
        return;        
        }
    	
    LOGSTRING("Trigger:\"%S\" ", &(aTrigger->TriggerEntry()->Name()));
    LOG1("CID:%d", cellularArea->CellId());

    // ENTRY type trigger
    if( CLbtTriggerConditionArea::EFireOnEnter == trgCondition->Direction() )
        {
        TLbtTriggerDataMask dataMask = 0;
        if( CheckForCurrentCGIMatch( cellularArea ) )
            {
            if( EActive == strategyData.iActivityStatus )
                {
                if( aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
                    {
                    LOG("TriggerFireOnCreation");
                    HandleTriggerFired(aTrigger,strategyData,dataMask);                         
                    }
                else
                    {
                    strategyData.iActivityStatus = EInactive;
                    aTrigger->ExtendedTriggerInfo()->SetTriggerFireOnCreation( ETrue );
                    dataMask |= CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
                    }
                }
            }
        else
        	{
            LOG("Check Visited Cell Count");
            
            if( VisitedCellCount( strategyData) <KCellHysterisis )
              	{
                LOG("visit count < Hysteresis");
                
              	if( iLocArea.iCellId && !FindCellId( strategyData,iLocArea.iCellId )  )
               		{
               		AddCellId( strategyData,iLocArea.iCellId );
               		
               	    if( VisitedCellCount( strategyData) >= KCellHysterisis )
                        {
			            LOG("ACTIVATING TRIGGER !!!!!");		            
                        strategyData.iActivityStatus = EActive;
                        }
                    }
                }
             }
        
        if(dataMask)
        	{
            // This is dummy position info. It has no significance.
        	TPositionInfo dummy;  
	        iView->UpdateTriggerInfo( CLbtGeoAreaBase::ECellular,dummy, aTrigger, dataMask );
        	}        
        }
    
    // EXIT Trigger    
    else
        {
        //This is not currently supported.
        }
    aTrigger->SetStrategyData( strategyData );    
    }


// ----------------------------------------------------------------------------
// CLbtCellSupervisor::EvalHybridTrigger
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::EvalHybridTrigger( CLbtContainerTriggerEntry* aTrigger )
    {
    FUNC_ENTER("CLbtCellSupervisor::EvalHybridTrigger");    
        
    CLbtTriggerConditionArea* trgCondition = 
                              static_cast< CLbtTriggerConditionArea* >
                              ( ( aTrigger->TriggerEntry() )->GetCondition() );
                
    CLbtGeoHybrid* hybridArea = static_cast< CLbtGeoHybrid* >
                                               ( trgCondition->TriggerArea() );
                                               
    // Fetch strategy data.
    TStrategyData strategyData = aTrigger->StrategyData();
   
    // Calculate remaining rearm time
    TInt remRearmTime = CalculateRearmTime( strategyData,aTrigger->TriggerEntry()->TimeToRearm());
   
    // If rearm time is not expired, skip trigger supervision.
    if( remRearmTime > 0 )
       {
       return;        
       }
    
    RPointerArray<CLbtGeoAreaBase> hybridArray = hybridArea->HybridArea();
    TBool cellMatch = EFalse;
    for( TInt i=0;i<hybridArray.Count();i++ )
        {
        if( hybridArray[i]->Type() == CLbtGeoAreaBase::ECellular )
            {
            CLbtGeoCell* cellularArea = static_cast<CLbtGeoCell*> ( hybridArray[i] );
            if( CheckForCurrentCGIMatch( cellularArea ) )
                {
                cellMatch = ETrue;
                break;
                }
            }
        }
    
    // ENTRY type trigger
    if( CLbtTriggerConditionArea::EFireOnEnter == trgCondition->Direction() )
        {
        TLbtTriggerDataMask dataMask = 0;
        if( cellMatch )
            {
            if( EActive == strategyData.iActivityStatus )
                {
                if( aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
                    {
                    LOG("TriggerFireOnCreation");
                    HandleTriggerFired(aTrigger,strategyData,dataMask);                         
                    }
                else
                    {
                    strategyData.iActivityStatus = EInactive;
                    aTrigger->ExtendedTriggerInfo()->SetTriggerFireOnCreation( ETrue );
                    dataMask |= CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
                    }
                }
            }
        else
            {
            LOG("Check Visited Cell Count");
            
            if( VisitedCellCount( strategyData) <KCellHysterisis )
                {
                LOG("visit count < Hysteresis");
                
                if( iLocArea.iCellId && !FindCellId( strategyData,iLocArea.iCellId )  )
                    {
                    AddCellId( strategyData,iLocArea.iCellId );
                    
                    if( VisitedCellCount( strategyData) >= KCellHysterisis )
                        {
                        LOG("ACTIVATING TRIGGER !!!!!");                    
                        strategyData.iActivityStatus = EActive;
                        }
                    }
                }
             }
        
        if(dataMask)
            {
            // This is dummy position info. It has no significance.
            TPositionInfo dummy;  
            iView->UpdateTriggerInfo( CLbtGeoAreaBase::ECellular,dummy, aTrigger, dataMask );
            }        
        }
    
    // EXIT Trigger    
    else
        {
        
        }
    aTrigger->SetStrategyData( strategyData );
    }


// ----------------------------------------------------------------------------
// CLbtCellSupervisor::CheckForCurrentCGIMatch
// ----------------------------------------------------------------------------
//
TBool CLbtCellSupervisor::CheckForCurrentCGIMatch( CLbtGeoCell* aCellularArea )
    {
    TInt trCountryCode,trNetworkCode;
    
    TLex lex;
    lex.Assign( aCellularArea->NetworkCountryCode() );
    lex.Val( trCountryCode );
    lex.Assign( aCellularArea->NetworkIdentityCode() );
    lex.Val( trNetworkCode );
    
    if( aCellularArea->NetworkType() == iNwInfo.iMode &&
        iCurrentCountryCode == trCountryCode  && 
        iCurrentNetworkCode == trNetworkCode &&
        aCellularArea->LocationAreaCode() == iLocArea.iLocationAreaCode &&
        aCellularArea->CellId() == iLocArea.iCellId )
        {
        return ETrue;
        }
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CLbtCellSupervisor::CalculateRearmTime
// ----------------------------------------------------------------------------
//
TInt CLbtCellSupervisor::CalculateRearmTime( TStrategyData& aStrategyData, TInt aTimeToRearm )
    {
    TInt remRearmTime = 0;
    if( aStrategyData.iFireTime )
       {
       TTime triggerFiredTime( aStrategyData.iFireTime );

       TTime currentTime;
       currentTime.UniversalTime();
       
       TTimeIntervalSeconds interval;
       currentTime.SecondsFrom( triggerFiredTime, interval );    
           
       remRearmTime = aTimeToRearm - interval.Int();
       if( remRearmTime <= 0 )
           {
           aStrategyData.iFireTime = 0;        
           }
       }
    return remRearmTime;
    }


// ----------------------------------------------------------------------------
// CLbtCellSupervisor::HandleTriggerFired
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::HandleTriggerFired( CLbtContainerTriggerEntry* aTrigger, TStrategyData& aStrategyData, 
                                                        TLbtTriggerDataMask& aDataMask )
	{
	FUNC_ENTER("CLbtCellSupervisor::HandleTriggerFired");
	aTrigger->ExtendedTriggerInfo()->SetTriggerFiredState( ETrue );
                            
    TLbtTriggerFireInfo triggerFireInfo;
    triggerFireInfo.iAreaType = CLbtGeoAreaBase::ECellular;
    triggerFireInfo.iTriggerId = aTrigger->TriggerEntry()->Id();
    LOG("Trigger fired:");
    LOG1("%d",aTrigger->TriggerEntry()->Id());                        
    aTrigger->ExtendedTriggerInfo()->SetFiredInfo( triggerFireInfo );
     
    // Time at which trigger fired
    TTime triggerFiredTime;
    triggerFiredTime.UniversalTime();
    
    aDataMask |= CLbtContainerTriggerEntry::EContainerAttributeIsFired | 
	    		 CLbtContainerTriggerEntry::EContainerAttributeFireInfo;
    
    aStrategyData.iFireTime =  triggerFiredTime.Int64();
        
    // Set trigger to inactive state
    aStrategyData.iActivityStatus = EInactive;
    ClearVisitedCellInfo( aStrategyData );
    
    // Append this trigger to recently fired trigger
    TInt error = iRecentlyFiredTriggerArray.Append( aTrigger );
	if( error != KErrNone )
		{
	    LOG1("Failed to append trigger to the array:%d",error);		
		}
	}



// ----------------------------------------------------------------------------
// CLbtCellSupervisor::EstimateSleepInterval
// ----------------------------------------------------------------------------
//    
void CLbtCellSupervisor::EstimateSleepInterval()    
    {
    FUNC_ENTER("CLbtCellSupervisor::EstimateSleepInterval");
    TInt minimumSleep = 0;
    
    TTime currentTime;
    currentTime.UniversalTime();
    
    for( TInt i=0;i<iRecentlyFiredTriggerArray.Count();i++ )
        {
        TInt remRearmTime =0;
        
        TStrategyData strategyData = iRecentlyFiredTriggerArray[i]->StrategyData();
        
        if( strategyData.iFireTime )
            {
            TTime firedTime( strategyData.iFireTime );
            TTimeIntervalSeconds interval;
            currentTime.SecondsFrom( firedTime,interval );
             
            remRearmTime = iRecentlyFiredTriggerArray[i]->TriggerEntry()->TimeToRearm() - interval.Int();
            }
        
        if( !minimumSleep || minimumSleep > remRearmTime )
            {
            minimumSleep = remRearmTime;
            }
        }
    
    LOG1("Sleep interval:%d",minimumSleep);    
    if( minimumSleep > 0 )
        {
        if( iCellSupervisorTimer->IsActive() )    
            {
            if( minimumSleep < iCellSupervisorTimer->RemainingTime() )
                {
                iCellSupervisorTimer->InterruptSleep( minimumSleep );
                }
            }
        else
            {
            iCellSupervisorTimer->SetTimer( minimumSleep );
            }
        }
    }

// ----------------------------------------------------------------------------
// CLbtCellSupervisor::TimeOut
// ----------------------------------------------------------------------------
//    
void CLbtCellSupervisor::TimeOut()
    {
    FUNC_ENTER("CLbtCellSupervisor::TimeOut");
    RPointerArray<CLbtContainerTriggerEntry> activatedTriggers;
    
    TTime currentTime;
    currentTime.UniversalTime();
    
    for( TInt i=0;i<iRecentlyFiredTriggerArray.Count();i++ )
        {
        TInt remRearmTime = 0;
        
        TStrategyData strategyData = iRecentlyFiredTriggerArray[i]->StrategyData();
        
        if( strategyData.iFireTime )
            {
            TTime firedTime( strategyData.iFireTime );
            TTimeIntervalSeconds interval;
            currentTime.SecondsFrom( firedTime,interval );
             
            remRearmTime = iRecentlyFiredTriggerArray[i]->TriggerEntry()->TimeToRearm() - interval.Int();
            
            if( remRearmTime <= 0 )
                {
                strategyData.iFireTime = 0;
                iRecentlyFiredTriggerArray[i]->SetStrategyData( strategyData );
                TInt error = activatedTriggers.Append( iRecentlyFiredTriggerArray[i] );
                if( error != KErrNone )
                    {
                    LOG1("Failed to append to fired triggers to the array:%d",error);
                    }
                iRecentlyFiredTriggerArray.Remove(i);
                }
            }
            
        }
    // Supervise recently activated for current cell. 
    if( activatedTriggers.Count() )   
        {
        EvaluateActivatedTriggers( activatedTriggers ); 
        }
    activatedTriggers.Close();   
    
    EstimateSleepInterval();
    }

// ----------------------------------------------------------------------------
// CLbtCellSupervisor::EvaluateActivatedTriggers
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::EvaluateActivatedTriggers( RPointerArray<CLbtContainerTriggerEntry>& 
                                                    aActivatedTriggers )
    {
    for( TInt i=0;i<aActivatedTriggers.Count();i++ )
        {
        CLbtContainerTriggerEntry* conTrigger = aActivatedTriggers[i];
        
        CLbtTriggerEntry* trigger = conTrigger->TriggerEntry();
        if( CLbtTriggerConditionBase::ETriggerConditionArea == 
                                            trigger->GetCondition()->Type() )
            {
            // Area type trigger
            CLbtTriggerConditionArea* triggerConArea = 
                                    static_cast< CLbtTriggerConditionArea* >
                                            ( trigger->GetCondition() );
               
            if( triggerConArea->TriggerArea()->Type() == CLbtGeoAreaBase::ECellular )
                {
                EvalCellularTrigger( conTrigger );
                }
            else if( triggerConArea->TriggerArea()->Type() == CLbtGeoAreaBase::EHybrid )
                {
                EvalHybridTrigger( conTrigger );
                }
            }
        }
    // These are recently fired triggers, hysterisis condition will surely fail for these triggers.
    // This evaluation is just to keep track of visited cell.
    }    

// ----------------------------------------------------------------------------
// CLbtCellSupervisor::VisitedCellCount
// ----------------------------------------------------------------------------
//
TInt CLbtCellSupervisor::VisitedCellCount( TStrategyData& aStrategyData )
    {
    TInt count = 0;
    
    for( TInt i=0;i<KVisitedCellSize;i++ )
        {
        if( aStrategyData.iVisitedCell[i] )
            {
            count++;
            }
        }
    return count;    
    }
    
// ----------------------------------------------------------------------------
// CLbtCellSupervisor::FindCellId
// ----------------------------------------------------------------------------
//
TBool CLbtCellSupervisor::FindCellId( TStrategyData& aStrategyData,TInt aCellId )
    {
    for( TInt i=0;i<KVisitedCellSize;i++ )
        {
        if( aStrategyData.iVisitedCell[i] == aCellId )
            {
            return ETrue;
            }
        }
    return EFalse;    
    }

// ----------------------------------------------------------------------------
// CLbtCellSupervisor::AddCellId
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::AddCellId( TStrategyData& aStrategyData,TInt aCellId )
    {
    for( TInt i=0;i<KVisitedCellSize;i++ )
        {
        if( aStrategyData.iVisitedCell[i] == 0 )
            {
            aStrategyData.iVisitedCell[i] = aCellId;
            return;
            }
        }
    }
// ----------------------------------------------------------------------------
// CLbtCellSupervisor::ClearVisitedCellInfo
// ----------------------------------------------------------------------------
//
void CLbtCellSupervisor::ClearVisitedCellInfo( TStrategyData& aStrategyData )
	{ 
	for( TInt i=0;i<KVisitedCellSize;i++ )
        {
        aStrategyData.iVisitedCell[i] = 0;
        }	
    }       
// end of file

