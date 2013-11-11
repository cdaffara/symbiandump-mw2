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
* Description:  Implementation of coordinate supervisor class
*
*/



#include <e32math.h>
#include <e32const.h>
#include <lbttriggerconditionarea.h>
#include <lbtgeocircle.h>
#include "lbtcelliddatabase.h"
#include "lbtcellchangehandler.h"
#include "lbtlogger.h"
#include "lbtcoordinatesupervisor.h"
#include "lbtstrategyengineobserver.h"
#include "lbtpsychangelistner.h"

// ==================== CONSTANTS ===================================

/**
 * Constant used to adjust trigger radius to accomodate inaccuracy 
 * of position fixes
 */
static const TReal32 KAccuracyMultiplier = 0.666667;

/**
 * The cut off value of the ratio of radius to position accuracy. Over this 
 * value the radius of a trigger will not be expanded.
 */
static const TReal32 KCofftOfHighEdgeLimit = 1.5;

/**
 * Constant used in calculation of trigger hysteresis
 */
static const TUint16 KTriggerRadiusCutoff = 500;

/**
 *  Contant time use in calculation of speed
 */
static const TReal32 constantTime = 20;

/**
 * Defines the number of historic speed values that will be stored
 * and used for calculation of current speed
 */
static const TUint16 maxObsValues = 10;

/**
 * Minimum calculated speed. If this calculated speed drops below this value
 * then the below value is speed as the speed value 
 */
static const TReal KMinProjSpeed = 20.0;

static const TReal KMaxSpeedToUseMvmtDet = 5.0;
/**
 * If the estimated sleep interval goes below this value then location request
 * will be issued immediately with the estimated interval as the time out value
 * for the location rquest
 */
static const TInt KMinSleepInterval = 2;

/**
 * Multiplier used for 10 power 3. Used for conversion of seconds to micro
 * seconds
 */
static const TUint KMicroMultiplier = 1000000;

/**
 * Number of seconds to sleep before making location request in cases where 
 * a location acquisition attempt fails to obtain a fix
 */
static const TUint KSleepTimeOnLocAcqFailure = 10; // 10 seconds

/**
 * Safe range of HA obtained from NPP
 */
//static const TUint KNppSafeHA = 1500; // 1500 meters

// ============================== MEMBER FUNCTIONS ============================================

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::NewL
// CLbtCoOrdinateSupervisor instantiation method
// -----------------------------------------------------------------------------
//
CLbtCoOrdinateSupervisor* CLbtCoOrdinateSupervisor::NewL( TAny* aConstructionParameters, 
                                                          CLbtTriggerView* aView,
                                                          MLbtStrategyEngineObsrvr* aObserver,
                                                          CLbtCellChangeHandler* aCellChangeHandler )
    {
    CLbtCoOrdinateSupervisor* self = new ( ELeave ) CLbtCoOrdinateSupervisor();
    CleanupStack::PushL( self );
    self->ConstructL( aConstructionParameters, aView,aObserver,aCellChangeHandler );
    CleanupStack::Pop();
    return self;    
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::~CLbtCoOrdinateSupervisor
// Default destructor
// -----------------------------------------------------------------------------
//
CLbtCoOrdinateSupervisor::~CLbtCoOrdinateSupervisor()
    {
    // This is just done as a precautionary measure!
    StopSupervision();
    delete iLocationRequestor;
    delete iStatusManager;
    delete iTimer;
    delete iPsyChangeListner;
    delete iCellIdDatabase;
    iNObservedValues.Close();
    iNearestTriggers.Close();
    }	


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::StartSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::StartSupervision()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::StartSupervision");
    iCellChangeHandler->SetObserver( this );    
    RequestTerminalPosition( iSettings.iLocationRequestTimeout );    
    }

    
// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::StopSupervision
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::StopSupervision()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::StopSupervision");
    iCellChangeHandler->Remove( this );
    Term(); 
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::SettingsChanged
// Handles notification of triggering settings change provided by server
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::SettingsChanged()
    {    
    iEngObserver->RetrieveTriggeringSettings( iSettings );    
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::Resume

// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::Resume()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::Resume");
    TInt err = KErrNone;
    iCellChangeHandler->SetObserver( this );    

    switch( iProjectedState )
            {
        case ELocationAcquisition:
            iProjectedState = EIdle;
            RequestTerminalPosition( iSettings.iLocationRequestTimeout );
            break;

        case ETriggerEvaluation:
            // Reset trigger iterator and start evaluation of all triggers in
            // view            
            iProjectedState = EIdle;
            err = ResetViewIterator();
            if( KErrNone != err )
                {
                return;
                }
            iState = ETriggerEvaluation;
            SetEvent();
            break;
            
        case EUpdateIntervalEstimation:

            // Evaluate triggers that have undergone changes during the state
            // change. This is to determine any new nearest trigger
            iProjectedState = EIdle;


            iView->DetermineNearestTriggerChanges( &iNearestTriggers );
            if( iNearestTriggers.Count() == 0 )
                {
                // This means all known nearest triggers have been modified. 
                // Reset iterator re-evaluate all triggers in the system. 
                TInt err = ResetViewIterator();
                if( KErrNone != err )
                    {
                    return;
                    }
                
                iState = ETriggerEvaluation;
                SetEvent();
                }
            else
                {
                // Evaluate only the modified triggers
                TBool makeLocReq = EFalse;
                TRAP( err, makeLocReq = EvaluateModifiedTriggersL() );
                if( KErrNone == err )
                    {
                    if( makeLocReq )
                    	{
                    	// Location request is needed based on evaluating
            			// modified triggers
                    	RequestTerminalPosition( iSettings.iLocationRequestTimeout );
                    	return;
                    	}
                    // Switch to estimation of update interval
                    iState = EUpdateIntervalEstimation;
                    SetEvent();
                    }
                else
                    {
                    Term();
                    }
                }
            break;

        case ESleep:
            iProjectedState = EIdle;
            iState = ESleep;
            SetEvent();
            break;
            
        case EIdle:
            if( EIdle == iState && EIdle == iProjectedState
                           && iView->CoordTriggerCount() )
                {
                RequestTerminalPosition( iSettings.iLocationRequestTimeout );
                }
            break;
            
        default:
            break;
            }
     }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::Suspend
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::Suspend()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::Suspend");
    iCellChangeHandler->Remove( this );
    switch( iState )
        {
    case ELocationAcquisition:
    	if( iLastLocationFixType == EProperFix )
    		{
    		iLocationRequestor->Cancel();
    		}
    	else
    		{
    		iLocationRequestor->CancelFixFromNpp();
    		}
    	
        iProjectedState = iState;
        iState = EIdle;
        break;
        
    case ETriggerEvaluation:
        iProjectedState = ETriggerEvaluation;
        iState = EIdle;
        break;
        
    case EUpdateIntervalEstimation:
        iProjectedState = EUpdateIntervalEstimation;
        iState = EIdle;
        break;
        
    case ESleep:
        iTimer->Cancel();
        iProjectedState = iState;
        iState = EIdle;
                
    case EIdle:
        break;
        
    default:
        break;
        }
    
    Cancel();
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::Reset
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::Reset()
    {
    iState = iProjectedState = EIdle;
    iNearestTriggers.Reset();
    iLocReqCount = 0;
    TPositionInfo posInfo;
    iPosInfo = posInfo;
    iFixForFiringCloseTriggers = EFalse;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::GetPositionInfo
// Processes the location information obtained from location requestor
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::GetPositionInfoL()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::GetPositionInfo");
    
    // Update location acquisition status information
    
    if( !( (KErrNone == iStatus.Int()) ||
    	   (KPositionQualityLoss == iStatus.Int()) ) )
    	{
    	ERROR("Position Information with : %d",iStatus.Int());
    	iStatusManager->ReportLocationAcquisitionStatus( iStatus.Int() );
    	    	
    	// The following check determines if there was a problem using the 
    	// specified positioning technology to retrieve position information
    	if( KErrNotFound == iStatus.Int() &&
    	    iLastLocationFixType == EProperFix )
    	    {
    	    LOG("Specified PSY not found");
    	    iAcquisitionStatus = ELocationAcquisitionNoMethod;
    	    
    	    if( iPsyChangeListner == NULL )
    	        {
    	        iPsyChangeListner = CLbtPsyChangeListner::NewL( *this );
    	        }
    	    iPsyChangeListner->StartToListen( iModuleId );
    	    LOG1("Listen to PSY change:%x",iModuleId);
    	    TUid defaultID = { 0XFFFFFFFF };
    	    if( iLocationRequestor->GetPositionModuleId() != defaultID )
    	        {
    	        LOG("Use default proxy");
    	        // switch to using default positioning technology
    	        iModuleId = defaultID;
    	        RequestTerminalPosition( iSettings.iLocationRequestTimeout,EProperFix );   
    	        }
    	    }
    	else
    	    {
    	    LOG("Location acquisition failed");
    	    if( iFixForFiringCloseTriggers && 
    	        (KErrTimedOut == iStatus.Int())&&
    	        iLastLocationFixType == EProperFix )
    	        {
    	        LOG("Firing Close triggers");
                FireClosestTriggers();                
                RequestTerminalPosition(iSettings.iLocationRequestTimeout, EProperFix);
                }
            else
                {
                if( iLastLocationFixType == EProperFix )
                    {
                    // Get fix from NPP PSY
                    LOG("Timed Out. Taking fix from NPP");
                    RequestTerminalPosition( iSettings.iLocationRequestTimeout, EFixFromNpp );
                    return;
                    
                    /*
                    iState = ESleep;
                    iTimer->Set( iStatus, KSleepTimeOnLocAcqFailure, ETrue );               
                    SetActive();

                    if( KErrAccessDenied == iStatus.Int() )
                        {
                        iAcquisitionStatus = ELocationAcquisitionPrivacyReject;
                        }
                    else 
                        {
                        iAcquisitionStatus = ELocationAcquisitionFailure;
                        }*/
                    }
                else
                    {
                    LOG("Npp psy failed to provide fix. Sleep now");
                    iState = ESleep;
                    iTimer->Set( iStatus, KSleepTimeOnLocAcqFailure, ETrue );               
                    SetActive();

                    if( KErrAccessDenied == iStatus.Int() )
                        {
                        iAcquisitionStatus = ELocationAcquisitionPrivacyReject;
                        }
                    else 
                        {
                        iAcquisitionStatus = ELocationAcquisitionFailure;
                        }
                    }
                }
    	    }

    	if( iFixForFiringCloseTriggers )
    	    {
    	    iFixForFiringCloseTriggers = EFalse;
    	    }

    	// Update supervision dynamic info to the strategy class
        TLbtStrategySupervisionDynamicInfo supervisionDynamicInfo;
        supervisionDynamicInfo.iDistanceToNearestTrigger = 0.0;
        supervisionDynamicInfo.iLatestAcquiredPosInfo = iPosInfo;
        supervisionDynamicInfo.iLocationAcquisitionStatus = iAcquisitionStatus;
        supervisionDynamicInfo.iNearestTriggerId = 0;
    
        iEngObserver->ReportTriggeringSupervisionStatus( supervisionDynamicInfo );    
    	return;
    	}

	if( iStatus.Int() == KPositionQualityLoss )
		{
		TPosition currentPosition;
		iPosInfo.GetPosition( currentPosition );
		LOG("Received KPositionQualityLoss");
		LOG1("Lat:%f",currentPosition.Latitude());
		LOG1("Long:%f",currentPosition.Longitude());
		LOG1("HA:%f",currentPosition.HorizontalAccuracy());
		}
    		
    iStatusManager->ReportLocationAcquisitionStatus( KErrNone );
    // The fix was taken to fire nearest triggers then reset the flag
    // to EFalse since the fix was successful and the nearest triggers will be
    // evaluated and will be fired
    if(iFixForFiringCloseTriggers)
        {
        iFixForFiringCloseTriggers = EFalse;
        }
    
    TPosition currentPosition, prevPosition;
    iPosInfo.GetPosition( currentPosition );
    iPrevPosInfo.GetPosition( prevPosition );
    
    if( Math::IsNaN( currentPosition.Latitude() ) || 
        Math::IsNaN( currentPosition.Longitude() ) )
    	{
    	LOG("ERROR:Lat/Long is Nan. Sleep now");
    	iState = ESleep;
        iTimer->Set( iStatus, KSleepTimeOnLocAcqFailure, ETrue );               
        SetActive();

        if( KErrAccessDenied == iStatus.Int() )
            {
            iAcquisitionStatus = ELocationAcquisitionPrivacyReject;
            }
        else 
            {
            iAcquisitionStatus = ELocationAcquisitionFailure;
            }
    	return;
    	}
    
    // Record the time at which we got the fix
    iLastFixTime.UniversalTime();
    
    // Take note of reference time that would be used during estimation of 
    // sleep interval
    iAcquisitionStatus = ELocationAcquisitionActive;
    
    iLocReqCount++;
    iLocationRequestor->GetModuleInfo( iPosInfo.ModuleId() );    
        
    if( iLocArea.iCellId && iLastCellRequestStatus == KErrNone )
        {
        LOG("Writing position to DB");
        LOG1("Cell Id = %d", iLocArea.iCellId );
        LOG1("LAC = %d", iLocArea.iLocationAreaCode );
        // Add current cell id position to cell-id database
        TInt countryCode,networkCode;
        
        TLex lex( iNwInfo.iCountryCode );
        lex.Val( countryCode );
        
        lex.Assign( iNwInfo.iNetworkId );
        lex.Val( networkCode );
        
        TPosition cellPosition;
        iPosInfo.GetPosition( cellPosition );
        if( iLastLocationFixType == EProperFix )
            {
            cellPosition.SetHorizontalAccuracy( 3000 );
            }
        
        iCellIdDatabase->InsertCidLocation( countryCode,
                                            networkCode,
                                            iLocArea.iLocationAreaCode,
                                            iLocArea.iCellId,
                                            cellPosition );
        }
    
    // Check if there is no movement from last location fix. If true then 
    // re-evaluate sleep interval
    TReal32 dist;
    currentPosition.Distance( prevPosition, dist );
    if( dist == 0 )
        {
        iState = EUpdateIntervalEstimation;
        SetEvent();
        }
    
    LOG1("Lat:%f",currentPosition.Latitude());
	LOG1("Long:%f",currentPosition.Longitude());
	LOG1("HA:%f",currentPosition.HorizontalAccuracy());
    
    // Switch to idle state as refresh is an async operation. Set projected 
    // state to trigger evaluation which will commence once the refresh 
    // operation is complete
    iState = ETriggerEvaluation;
    SetEvent();

    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EvaluateTriggersL
// Evaluates triggers based on the location information obtained 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::EvaluateTriggersL()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::EvaluateTriggersL");
    
    // Reset nearest trigger array
    iNearestTriggers.Reset();
    
    // TODO: In case of updates to trigger information do not change state
    //       else change state to EUpdateIntervalEstimation
    CLbtContainerTriggerEntry* conTrigger;
    
    // ResetViewIterator will switch state to Idle if there are no triggers in 
    // the system
    if( KErrNone != ResetViewIterator() )
        {
        return;
        }

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
                   
                switch( triggerConArea->TriggerArea()->Type() )
                    {
                	// Circular area
                	case CLbtGeoAreaBase::ECircle:
                		{
                		if( iLastLocationFixType == EProperFix )
                            {
                            EvalCircularTriggerL( conTrigger );
                            }
                        else if( iLastLocationFixType == EFixFromDb || iLastLocationFixType == EFixFromNpp )
                            {
                            if( EvalCircularTriggerBasedOnCellPositionL( conTrigger ) )
                                {
                                if( iLastLocationFixType == EFixFromNpp )
                                	{
                                	// Fix is taken from NPP only when Proper Fix fails. Now we should
                                	// sleep for location acquisition failure time our period since there is no point
                                	// in attempting another proper fix
                                	iState = ESleep;
				                    iTimer->Set( iStatus, KSleepTimeOnLocAcqFailure, ETrue );               
				                    SetActive();

				                    if( KErrAccessDenied == iStatus.Int() )
				                        {
				                        iAcquisitionStatus = ELocationAcquisitionPrivacyReject;
				                        }
				                    else 
				                        {
				                        iAcquisitionStatus = ELocationAcquisitionFailure;
				                        }
                                	}
                                else
                                	{
                                	RequestTerminalPosition( iSettings.iLocationRequestTimeout,EProperFix );
                                	}                                
                                return;
                                }
                            }                            
                    	break;
                		}                    
                	default:
                    	break;
                    }
                }
            }
        }
        
    // Update supervision dynamic info to the strategy class
    TLbtStrategySupervisionDynamicInfo supervisionDynamicInfo;
    if( iNearestTriggers.Count() )
        {
        supervisionDynamicInfo.iDistanceToNearestTrigger = iNearestTriggers[0].distance;
        supervisionDynamicInfo.iNearestTriggerId = iNearestTriggers[0].id;
        }
    supervisionDynamicInfo.iLatestAcquiredPosInfo = iPosInfo;
    supervisionDynamicInfo.iLocationAcquisitionStatus = iAcquisitionStatus;
    
    
    iEngObserver->ReportTriggeringSupervisionStatus( supervisionDynamicInfo );
    
    iState = EUpdateIntervalEstimation;
    SetEvent();
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EvaluateTriggers
// Evaluates triggers based on the location information obtained 
// -----------------------------------------------------------------------------
//
TBool CLbtCoOrdinateSupervisor::EvaluateModifiedTriggersL()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::EvaluateModifiedTriggersL");
    
    // Retrieve modified trigger list from trigger view to be supervised
    RArray< TLbtTriggerId > modifiedTriggers;
    iView->GetModifiedTriggers( modifiedTriggers );
    CleanupClosePushL( modifiedTriggers );
    if( modifiedTriggers.Count() )
        {
        // TODO: Need to check for the deletion of
        // Create an iterator to parse through the modified trigger list
        CLbtTriggerView::TTriggerViewIter iter = iView->Begin( &modifiedTriggers, CLbtGeoAreaBase::ECircle );        
        
        CLbtContainerTriggerEntry* conTrigger;
        while( iter.NextL( conTrigger ) )
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
                       
                    switch( triggerConArea->TriggerArea()->Type() )
                        {
	                    // Circular area
	                    case CLbtGeoAreaBase::ECircle:
	                        if( iLastLocationFixType == EProperFix )
	                            {
	                            EvalCircularTriggerL( conTrigger );
	                            }
	                        else if( iLastLocationFixType == EFixFromDb || iLastLocationFixType == EFixFromNpp )
	                            {
	                            if( EvalCircularTriggerBasedOnCellPositionL( conTrigger ) )
	                                {
	                                CleanupStack::PopAndDestroy();  // modifiedTriggers
	                                return ETrue;
	                                }
	                            }
	                        break; 
	                        
	                    default:
	                        break;
                        }
                    }
                }
            }        
        }
        
    CleanupStack::PopAndDestroy();  // modifiedTriggers
    return EFalse;
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EvalCircularTrigger
// Evaluates triggers with circular area definition
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::EvalCircularTriggerL( 
                                          CLbtContainerTriggerEntry* aTrigger )
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::EvalCircularTrigger");
    
    
    // Extract strategy data
    TStrategyData strategyData = aTrigger->StrategyData();                                          
    
    TInt remRearmTime = 0;
    TReal distanceToLatestLocation = 0.0;
    TBool hasRearmTimeExpired = ETrue;
    
    LOGSTRING("Trigger: \"%S\" ", &(aTrigger->TriggerEntry()->Name()));
        
    if( strategyData.iFireTime )
        {
        TTime triggerFiredTime( strategyData.iFireTime );

        TTime currentTime;
        currentTime.UniversalTime();
        
        TTimeIntervalSeconds interval;
        currentTime.SecondsFrom( triggerFiredTime, interval );
        
        remRearmTime = aTrigger->TriggerEntry()->TimeToRearm() - interval.Int();
        
        
        if( remRearmTime <= 0 )
            {
            remRearmTime = 0;
            strategyData.iFireTime = 0;
            }
        else
        	{
        	LOG("Rearm Time has not expired");
        	hasRearmTimeExpired = EFalse;
        	}
        }
        
    CLbtTriggerConditionArea* trgCondition = 
                              static_cast< CLbtTriggerConditionArea* >
                              ( ( aTrigger->TriggerEntry() )->GetCondition() );
    
    CLbtGeoCircle* circularArea = static_cast< CLbtGeoCircle* >
                                               ( trgCondition->TriggerArea() );

    // Calculate distance of trigger from current location
    TReal32 distanceToCenter;
    TPosition currentPosition;
    iPosInfo.GetPosition( currentPosition );
    TInt err = currentPosition.Distance( circularArea->Center(), 
                                         distanceToCenter );
    if( KErrNone != err )
        {
        ERROR("Error in calculating distance:%d",err);
        LOG1("Current Lat:%f",currentPosition.Latitude());
	    LOG1("Current Long:%f",currentPosition.Longitude());   
	    LOG1("Trigger Lat:%f",circularArea->Center().Latitude());   
	    LOG1("Trigger Long:%f",circularArea->Center().Longitude());
        return;
        }

    // Determine trigger radius and hysteresis radius to be used for supervision
    // based on the accuracy of location fix
    TTriggerArea triggerArea;
    TReal positionAccuracy = currentPosition.HorizontalAccuracy();    
    triggerArea = EstimateTriggerRadii( circularArea->Radius(), 
                    aTrigger->ExtendedTriggerInfo()->HysteresisRadius(), 
                        positionAccuracy );
    
    // ENTRY Trigger
    // Determine current position of terminal relative to the trigger area and 
    // set trigger fire status accordingly
    if( CLbtTriggerConditionArea::EFireOnEnter == trgCondition->Direction() )
        {
        TLbtTriggerDataMask dataMask = 0;
        
        TReal tolerableRadius = CalculateTolerableRadius( triggerArea.triggerRadius );
        // Terminal is inside trigger area
        if( ( distanceToCenter < tolerableRadius ) && 
            ( ( distanceToCenter + positionAccuracy ) <= triggerArea.hysteresisRadius ) )
            {
            LOG("Inside triggerin area");
            // If trigger in active state then fire trigger and set state to
            // inactive. If trigger is inactive nothing is to be done other
            // than calculating its distance from the hysteresis boundary
            if( EActive == strategyData.iActivityStatus )
                {
				LOG("Active trigger");
                // Check FireOnCreation flag here. If EFalse then dont 
                // fire the trigger. Instead only change the activity
                // status.
                if( aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() &&
                    hasRearmTimeExpired )
                    {
                    // Set trigger state to fired
                    aTrigger->ExtendedTriggerInfo()->SetTriggerFiredState( ETrue );
                    TLbtTriggerFireInfo triggerFireInfo;
                    TPositionInfo positionInfo;
                    triggerFireInfo.iTriggerId = aTrigger->TriggerEntry()->Id();
                    triggerFireInfo.iFiredPositionInfo = iPosInfo;
                    
                    aTrigger->ExtendedTriggerInfo()->SetFiredInfo( triggerFireInfo );
                    
                    // Time at which trigger fired
                    TTime triggerFiredTime;
                    triggerFiredTime.UniversalTime();
                    LOG("TRIGGER FIRED");
                    
                    strategyData.iFireTime = triggerFiredTime.Int64();
                    
                    remRearmTime = aTrigger->TriggerEntry()->TimeToRearm();
                    dataMask |= 
                    CLbtContainerTriggerEntry::EContainerAttributeIsFired | 
                    CLbtContainerTriggerEntry::EContainerAttributeFireInfo ;
                    }
                
                // Set trigger to inactive state                
                strategyData.iActivityStatus = EInactive;
                }
            
            // Calculate distance to current location and update trigger's 
            // dynamic info. This distance is (rH - D) where D is the distance 
            // between the two centers and rH is the radius of the hysteresis 
            // circle. Here the radius of hysteresis boundary is considered for 
            // distance calculation as this trigger would undergo a state 
            // change only when it crosses the hysteresis area
            distanceToLatestLocation = Abs( triggerArea.hysteresisRadius - distanceToCenter );
            }
            
        // Terminal is outside hysteresis area
        else if( distanceToCenter > ( triggerArea.hysteresisRadius + positionAccuracy ) )
            {
            if( EInactive == strategyData.iActivityStatus )
                {
                // Reset trigger state to valid
                strategyData.iActivityStatus = EActive;     
                LOG("Trigger activated");
                }
                
            // Calculate distance of trigger to current location and update
            // trigger's dynamic info. This distance is (D - rT) where D is 
            // the distance between two centers and rT is the radius of trigger. 
            // State change will occur only when the terminal moves beyond the
            // boundary of the trigger
            distanceToLatestLocation = Abs( distanceToCenter - triggerArea.triggerRadius );
            }

        // Terminal is inside hysteresis area. 
        else
            {
            // Trigger does not undergo any state change in this region. Theres 
            // only distance to be determined here
            if( EActive == strategyData.iActivityStatus )
                {
                // Terminal was outside hysteresis area earlier. Distance is
                // therefore (D - rT)
                distanceToLatestLocation = Abs( distanceToCenter - triggerArea.triggerRadius );
                }
            else
                {
                // Terminal was inside trigger area earlier. Distance is
                // therefore (rH - D)
                distanceToLatestLocation = Abs( triggerArea.hysteresisRadius - distanceToCenter );
                }
            }
        
        // Set FireOnCreation flag to ETrue here if its not already
        // ETrue. This is done to ensure that the trigger is fired 
        // in the subsequent iterations if the triggering conditions
        // are satisfied.
        if( !aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
            {
            aTrigger->ExtendedTriggerInfo()->SetTriggerFireOnCreation( ETrue );
            dataMask |= CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
            }
        
        if( dataMask )
            {
            iView->UpdateTriggerInfo( CLbtGeoAreaBase::ECircle,iPosInfo, aTrigger, dataMask );            
            }
        }
        
    // EXIT Trigger
    else
        {
        TLbtTriggerDataMask dataMask = 0;
        // Retrieve boundary information of trigger
        TTriggerBoundary boundary = EUndefined;
        
        // Retrieve boundary information of trigger
        ( strategyData.iBoundaryCondition >= 0 && strategyData.iBoundaryCondition <= 2 )?boundary =
                                     TTriggerBoundary( strategyData.iBoundaryCondition ):boundary;
        
        TReal tolerableRadius = CalculateTolerableRadius( triggerArea.triggerRadius );                             
        
        // Terminal is inside trigger area
        if( ( distanceToCenter < tolerableRadius ) && 
            ( ( distanceToCenter + positionAccuracy ) <= triggerArea.hysteresisRadius ) )
            {
            // Check boundary condition to determine if the terminal has moved
            // inside the trigger area without having gone beyond the area of
            // hysteresis. In this case the trigger shall not be set as active
            if( EOutside != boundary && 
                    EInactive == strategyData.iActivityStatus )
                {
                // Set trigger state to active
                strategyData.iActivityStatus = EActive;
                }
            
            // Set boundary as Outside
            strategyData.iBoundaryCondition =  TInt( EOutside );
            
            if( EActive == strategyData.iActivityStatus )
                {
                // Set Distance to the border of trigger area. This distance is
                // (rT - D).
                distanceToLatestLocation = Abs( triggerArea.triggerRadius - distanceToCenter );
                }
            else
                {
                // Set distance to border of hysteresis area. This distance is
                // (rH - D) where rH is the radius of hysteresis and D is the 
                // distance between the center of trigger and the center of 
                // location
                distanceToLatestLocation = Abs( triggerArea.hysteresisRadius - distanceToCenter );
                }
            }
            
        // Terminal is outside hysteresis area  
        else if( ( distanceToCenter > triggerArea.hysteresisRadius )  &&
            ( distanceToCenter > ( triggerArea.triggerRadius + positionAccuracy ) ) )
            {
            if( EActive == strategyData.iActivityStatus  )
                {
                // Check FireOnCreation flag here. If EFalse then dont 
                // fire the trigger. Instead only change the activity
                // status.
                if( aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() &&
                    hasRearmTimeExpired )
                    {
                    // Set trigger state to fired
                    aTrigger->ExtendedTriggerInfo()->SetTriggerFiredState( ETrue );
                    
                    TLbtTriggerFireInfo triggerFireInfo;
                    TPositionInfo positionInfo;
                    triggerFireInfo.iTriggerId = aTrigger->TriggerEntry()->Id();
                    triggerFireInfo.iFiredPositionInfo = iPosInfo;
                    
                    aTrigger->ExtendedTriggerInfo()->SetFiredInfo( triggerFireInfo );
                    
                     // Time at which trigger fired
                    TTime triggerFiredTime;
                    triggerFiredTime.UniversalTime();
                    strategyData.iFireTime = triggerFiredTime.Int64();
                    LOG("TRIGGER FIRED");
                    remRearmTime = aTrigger->TriggerEntry()->TimeToRearm();             
                    dataMask |= 
                    CLbtContainerTriggerEntry::EContainerAttributeIsFired | 
                    CLbtContainerTriggerEntry::EContainerAttributeFireInfo;
                    }
                
                // Set the trigger state to inactive
                strategyData.iActivityStatus = EInactive;
                
                }

            // Set boundary as Inside
            strategyData.iBoundaryCondition = TInt( EInside );
            
            // Calculate distance to border of trigger area. This distance
            // is (D - rT)
            distanceToLatestLocation = Abs( distanceToCenter - triggerArea.triggerRadius );
            }
        
        // Terminal is inside hysteresis area
        else
            {
            if( EOutside == boundary && EActive == strategyData.iActivityStatus )
                {
                // Check FireOnCreation flag here. If EFalse then dont 
                // fire the trigger. Instead only change the activity
                // status.
                if( aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
                    {
                    // Set trigger state to fired
                    aTrigger->ExtendedTriggerInfo()->SetTriggerFiredState( ETrue );
                    TLbtTriggerFireInfo triggerFireInfo;
                    TPositionInfo positionInfo;
                    triggerFireInfo.iTriggerId = aTrigger->TriggerEntry()->Id();
                    triggerFireInfo.iFiredPositionInfo = iPosInfo;
                    
                    aTrigger->ExtendedTriggerInfo()->SetFiredInfo( triggerFireInfo );
                    
                     // Time at which trigger fired
                    TTime triggerFiredTime;
                    triggerFiredTime.UniversalTime();
                    
                    strategyData.iFireTime = triggerFiredTime.Int64();
                    strategyData.iBoundaryCondition =  TInt( EOutside );
                    
                    remRearmTime = aTrigger->TriggerEntry()->TimeToRearm();             
                    dataMask |= 
                    CLbtContainerTriggerEntry::EContainerAttributeIsFired | 
                    CLbtContainerTriggerEntry::EContainerAttributeFireInfo;
                    }
                
                // Set the trigger state to inactive
                strategyData.iActivityStatus = EInactive;
                }

            if( EOutside != boundary )
                {
                // Set the trigger state to inactive. This is done here because
                // if a trigger was newly created (state is active) and it is
                // found to be within hysteresis area, then the activity status 
                // needs to be set to inactive to enable the trigger to be
                // handled appropriately when terminal moves inside trigger
                // area.
                strategyData.iActivityStatus = EInactive;
                // Calclulate distance to border of trigger area. This 
                // distance is (D - rT)
                distanceToLatestLocation = Abs( distanceToCenter - triggerArea.triggerRadius );
                }
            else 
                {
                // Calculate distance to the boundary of hysteresis. This 
                // distance is (rH - D)
                distanceToLatestLocation = 
                               Abs( triggerArea.hysteresisRadius - distanceToCenter );
                }
            }
        
        // Set FireOnCreation flag to ETrue here if its not already
        // ETrue. This is done to ensure that the trigger is fired 
        // in the subsequent iterations if the triggering conditions
        // are satisfied.
        if( !aTrigger->ExtendedTriggerInfo()->IsTriggerFireOnCreation() )
            {
            aTrigger->ExtendedTriggerInfo()->SetTriggerFireOnCreation( ETrue );
            dataMask |= CLbtContainerTriggerEntry::EContainerAttributeIsTriggerFireOnCreation;
            }
        
        if( dataMask )    
            {
            iView->UpdateTriggerInfo( CLbtGeoAreaBase::ECircle,iPosInfo, aTrigger, dataMask );
            }
        }
    aTrigger->SetStrategyData( strategyData );    
    UpdateNearestTriggers( aTrigger->TriggerEntry()->Id(), distanceToLatestLocation , remRearmTime );
    LOG1("DistToTrig:%f",distanceToLatestLocation );
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EvalCircularTriggerBasedOnCellPosition
// Evaluates triggers with circular area definition
// -----------------------------------------------------------------------------
//
TBool CLbtCoOrdinateSupervisor::EvalCircularTriggerBasedOnCellPositionL( 
                                          CLbtContainerTriggerEntry* aTrigger )
    {
    TTime currentTime;
    currentTime.UniversalTime();
    
    CLbtTriggerConditionArea* trgCondition = 
                              static_cast< CLbtTriggerConditionArea* >
                              ( ( aTrigger->TriggerEntry() )->GetCondition() );
        
    // Extract strategy data
    TStrategyData strategyData = aTrigger->StrategyData();
    
    TInt remRearmTime = 0;
    TReal distanceToLatestLocation = 0.0;
       
    if( strategyData.iFireTime )
        {
        TTime triggerFiredTime( strategyData.iFireTime );

        TTimeIntervalSeconds interval;
        currentTime.SecondsFrom( triggerFiredTime, interval );
    
        remRearmTime = aTrigger->TriggerEntry()->TimeToRearm() - interval.Int();
        
        if( remRearmTime <= 0 )            
            {
            remRearmTime = 0;
            strategyData.iFireTime = 0;
            aTrigger->SetStrategyData( strategyData );
            }
        }
            
    
    TPosition currentPosition;
    iPosInfo.GetPosition( currentPosition );
    TReal positionAccuracy = currentPosition.HorizontalAccuracy();    
    
    CLbtGeoCircle* circularArea = static_cast< CLbtGeoCircle* >
                                           ( trgCondition->TriggerArea() );
                                           
    TReal32 distanceToCenter;
    TInt err = currentPosition.Distance( circularArea->Center(), 
                                                        distanceToCenter );
    if( err!=KErrNone )
        {
        return ETrue;
        }
    
    // If the trigger is entry type,we calculate distance to the hysterisis radius
    if( CLbtTriggerConditionArea::EFireOnEnter == trgCondition->Direction() )
        {
        distanceToLatestLocation = distanceToCenter -
                                         ( positionAccuracy+aTrigger->ExtendedTriggerInfo()->HysteresisRadius());
                                         
        if( distanceToLatestLocation < 0 && (remRearmTime == 0) )
        	{
        	// Request fix using any of the available psy
        	LOG("Request Proper Fix");
        	return ETrue;
        	}

        // Activate the trigger if the trigger is outside Hysteresis area
        if( EInactive == strategyData.iActivityStatus && distanceToLatestLocation > 0 )
        	{
	   		// Activate the trigger
    		strategyData.iActivityStatus = EActive;
    		aTrigger->SetStrategyData( strategyData );
    		LOG("Trigger activated based on NPP fix");
        	}
        UpdateNearestTriggers( aTrigger->TriggerEntry()->Id(), distanceToLatestLocation, remRearmTime );
        }
    // If the trigger is exit type,we calculate distance to the radius    
    else if( CLbtTriggerConditionArea::EFireOnExit == trgCondition->Direction() )  
        {
        distanceToLatestLocation = distanceToCenter -
                                     ( positionAccuracy+circularArea->Radius() );

        if( distanceToLatestLocation < 0 && (remRearmTime == 0) )
        	{
        	// Request fix using any of the available psy
        	LOG("Request Proper Fix");
        	return ETrue;
        	}	                             
        UpdateNearestTriggers( aTrigger->TriggerEntry()->Id(), distanceToLatestLocation,remRearmTime );                                                          
        }
    return EFalse;     
    }

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EstimateSleepInterval
// Calculates the sleep interval
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::EstimateSleepInterval()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::EstimateSleepInterval");
    
    TReal maxUserSpeed = iSettings.iMaximumUserSpeed;
        
    TReal actualEstimatedSpeed = KMinProjSpeed;
    // Flag to determine if the nearest trigger is an inactive trigger
    TBool nearestTriggerInactive = EFalse;
    
    // This is first time when there are no previously observed values
    if( iLocReqCount == 1 )
        {
        // First iteration use the minimum update interval
        if( iNearestTriggers.Count() )
            {
            iSleepInterval = iNearestTriggers[0].distance / maxUserSpeed;
            }
        else
            {
            iSleepInterval = iSettings.iMinimumLocationUpdateInterval;
            }
        }
    else
        {
        iEstimatedUserSpeed = 0.0;
        if( (iLastLocationFixType == EFixFromDb) || 
            (iLastLocationFixType == EFixFromNpp) )
            {
            iEstimatedUserSpeed = (maxUserSpeed + KMinProjSpeed)/2;
            }
        else if( iLastLocationFixType == EProperFix )
            {
            // determine average speed from previous location request which is the
            // observed speed
            TPosition currentPosition, prevPosition;
            iPosInfo.GetPosition( currentPosition );
            iPrevPosInfo.GetPosition( prevPosition );
            
            TReal32 avgSpeed = 0.0;
            
            currentPosition.Speed( prevPosition, avgSpeed );
            TObservedValues obsValue;
            obsValue.speed = avgSpeed;
            obsValue.time = currentPosition.Time();

            if( iNObservedValues.Count() == maxObsValues )
                {
                iNObservedValues.Remove( 0 );
                }
            TInt error = iNObservedValues.Append( obsValue );
            if( error != KErrNone )
                {
                LOG1("Failed to append obsValue:%d",error);
                }
            TReal num = 0.0;
            TReal den = 0.0;
            TReal obsSpeed = 0.0;
            TReal obsTime = 0.0;
            TTime currentTime;

            // Determine average of previously obtained observed speed, time values.
            // Use the current time value obtained from the approximation of speed.
            currentTime.UniversalTime();
            
            for( TInt i = 0; i < iNObservedValues.Count(); i++ )
                {
                obsSpeed = maxUserSpeed - ( iNObservedValues[i] ).speed;
                TTimeIntervalSeconds timediff; 
                currentTime.SecondsFrom( ( iNObservedValues[i] ).time, timediff );  
                obsTime = timediff.Int() / constantTime;
                TReal timeExp = 0.0;
                Math::Exp( timeExp, (-1 * obsTime ) );
                
                num += obsSpeed * timeExp * timeExp;
                den += timeExp;
                }
        
            iEstimatedUserSpeed = maxUserSpeed - ( num / den );
            
            if( Math::IsNaN(iEstimatedUserSpeed) )
            	{
            	iEstimatedUserSpeed = maxUserSpeed;
            	}

            actualEstimatedSpeed = iEstimatedUserSpeed;
            LOG1("Actual estimated speed:%f",actualEstimatedSpeed);
            
            if(iEstimatedUserSpeed < KMinProjSpeed)
                {
                iEstimatedUserSpeed = KMinProjSpeed;
                }
            // There are some cases estimate user speed goes to infinty.    
            else if( iEstimatedUserSpeed > maxUserSpeed )    
                {
                iEstimatedUserSpeed = maxUserSpeed;
                }
            }
        
        /**
         * Find the position of the nearest trigger which: -
         *
         * a) Is Active
         * b) Its rearm time has expired
         */
        TInt position = -1;
        for( TInt i=0;i<iNearestTriggers.Count();i++ )
            {
            iNearestTriggers[i].minTimeRequired = iNearestTriggers[i].distance / iEstimatedUserSpeed;
                
            if( iNearestTriggers[i].remRearmTime == 0 )
                {
                // It is enough for us to know the approx time required to reach nearest trigger whose 
                // rearm time is not zero( nearest which is not fired )  
                position = i;
                break;
                }
            }
        
        /**
         * Determine the sleep interval considering the rearm time of all the fired triggers.
         * Sleep interval should be min[max[rearm time,minimum time to reach trigger]]. Flag needed
         * for determining if movement detection is needed. Movement detector will be used if
         * position acquisition should be made to re-activate the trigger.
         */        
        iSleepInterval = 0;
        TLbtTriggerId nearestTriggerId = 0;
        
        // Sleep interval should be min[max[rearm time,minimum time to reach trigger]]
        if( position >= 0 )
            {
            for( TInt i=0;i<=position;i++ )
                {
                // Sleep interval for a trigger is the maximum of time to trigger and rearm time
                TInt maxSleepTimeForTrigger;
                if( iNearestTriggers[i].remRearmTime < iNearestTriggers[i].minTimeRequired )
                    {
                    maxSleepTimeForTrigger = iNearestTriggers[i].minTimeRequired;
                    }
                else
                    {
                    maxSleepTimeForTrigger = iNearestTriggers[i].remRearmTime;
                    }
                
                // Sleep interval for system is minimum sleep interval of all triggers    
                if( ( !iSleepInterval.Int() && maxSleepTimeForTrigger ) ||
                      iSleepInterval.Int() > maxSleepTimeForTrigger )  
                    {
                    iSleepInterval = maxSleepTimeForTrigger;
                    nearestTriggerId = iNearestTriggers[i].id;
                    }
                }
            }
        // Case where all the triggers are waiting for rearm time to elapse
        else
            {
            for( TInt i=0;i<iNearestTriggers.Count();i++ )
                {
                if( !iSleepInterval.Int() || 
                    ( iSleepInterval.Int() > iNearestTriggers[i].remRearmTime ))
                    {
                    iSleepInterval = iNearestTriggers[i].remRearmTime;
                    }
                }
            }  
            
        LOG1("Estimated speed:%f",iEstimatedUserSpeed);
        
        
        if( nearestTriggerId )
            {
            // Update the nearestTriggerInactive flag                        
            CLbtContainerTriggerEntry* trigEntry = NULL;
            TBool found = iView->GetTriggerByID( nearestTriggerId, trigEntry );
            
            if( found && (trigEntry != NULL) )
                {
                TStrategyData strategyData = trigEntry->StrategyData();
                
                if( strategyData.iActivityStatus == EInactive )
                    {
                    // Inside triggering area
                    nearestTriggerInactive = ETrue;
                    }
                else
                    {
                    // Outside triggering area
                    nearestTriggerInactive = EFalse;
                    }
                }
            }
        
        // Reduce the time taken from last position fix
        if( iLastLocationFixType == EProperFix )
        	{
        	TTime currentTime;
    	    currentTime.UniversalTime();
    	    TTimeIntervalSeconds seconds;
    	    currentTime.SecondsFrom(iLastFixTime, seconds);
    	    iProcessingInterval = seconds.Int();
    	    iSleepInterval = iSleepInterval.Int() - iProcessingInterval;
        	}
        }   
    
    // If sleep interval is less than KMinSleepInterval and the UE is travelling in
    // high speed then request terminal position immediately
    if( !nearestTriggerInactive &&
        ( iSleepInterval.Int() <= KMinSleepInterval ) &&
        ( iLastLocationFixType == EProperFix ) )
        {
        LOG1("Requesting time out %d", iSleepInterval.Int());
        iFixForFiringCloseTriggers = ETrue;
        // Here we need fix with in the estimated time to reach trigger.Hence setting timeout value to 
        // estimated sleep interval	
        RequestTerminalPosition( (KMinSleepInterval * KMicroMultiplier),EProperFix );
        return;
        }
    
    iState = ESleep;
    TBool useMvmtDet = ETrue;
    if( iLastLocationFixType != EProperFix )
    	{
    	useMvmtDet = ETrue;
    	}
    else if( nearestTriggerInactive )
    	{
    	useMvmtDet = ETrue;
    	}    
    else if( iSleepInterval.Int() < 20 )
    	{
    	useMvmtDet = EFalse;
    	}
	else if( actualEstimatedSpeed > KMaxSpeedToUseMvmtDet )
    	{
    	useMvmtDet = EFalse;
    	}
    LOG1("useMvmtDet = %d", useMvmtDet);
    iTimer->Set( iStatus, iSleepInterval, useMvmtDet );
    SetActive();
    }

    
// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::ProcessInterrupt
// Evaluates the cause of a timer interrupt and determines the necessary 
// actions to be performed
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::ProcessInterrupt()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::ProcessInterrupt");
    
    // This is when errors occur while acquiring terminal position
    if( !iStatusManager->LocationAcqSuccess() )
        {
        // Check sleep interval and determine if sleep mode can continue else 
        // retry acquiring terminal position. This happens when there are 
        // changes to triggers in the sleep mode during which the sleep is 
        // cancelled and trigger view is refreshed. Once the refresh operation
        // is completed, this method is invoked.
        TTimeIntervalSeconds interval;
        if( iTimer->RemainingSleepInterval( interval ) )
            {
            iState = ESleep;
            if( interval.Int() < 5 )
                {
                iTimer->ResetInterval( iStatus, interval,EFalse );
                }
            else
                {
                iTimer->ResetInterval( iStatus, interval,ETrue );
                }
            SetActive();
            }
        else
            {
            // If the last location acqusition has failed and the location information for the current cell
            // is unavailable then request location from NPP PSY else request from DB itself
            if( (iLastCellRequestStatus == KErrNone) && 
                iLocArea.iCellId )
		        {
		        // Cell information is available
		        if( !(KErrNone == GetCellIdPosition( iPosInfo ) ) )
		        	{
		        	RequestTerminalPosition( iSettings.iLocationRequestTimeout, EProperFix );
		        	}
		        else
		            {
		            RequestTerminalPosition( iSettings.iLocationRequestTimeout, EFixFromDb );
		            }
            	}
            else
            	{
            	// Current cell information un-available. Make proper fix
            	RequestTerminalPosition( iSettings.iLocationRequestTimeout, EProperFix );
            	}
        	}
        }

    
    // This is when sleep mode is interrupted due to notifications from 
    // container for addition/modification/deletion of triggers in the system
    else if( iTimer->SleepIntervalInterrupted() )
        {
        TLbtTriggerId nearestTrigger = 0;
        if( iNearestTriggers.Count() )
            {
            for( TInt i=0;i<iNearestTriggers.Count();i++ )
                {
                if( iNearestTriggers[i].remRearmTime == 0 )
                    {
                    nearestTrigger = iNearestTriggers[i].id;
                    break;
                    }
                }
            }
        iView->DetermineNearestTriggerChanges( &iNearestTriggers );
        
        if( iNearestTriggers.Count() == 0 )
            {
            // This means the nearest 5 triggers have been changed. Reset
            // iterator re-evaluate all triggers in the system. 
            if( KErrNone != ResetViewIterator() )
                {
                return;
                }
            iState = ETriggerEvaluation;
            SetEvent();
            }
        else
            {
            TBool makeLocReq = EFalse;
            TRAPD( err, makeLocReq = EvaluateModifiedTriggersL( ) );
            
            if( KErrNone != err )
            	{
            	ERROR("EvaluateTriggersL leaving with %d", err);
	            Term();
	            return;            	
            	}
            
            if( makeLocReq )
            	{
            	// Location request is needed based on evaluating
            	// modified triggers
            	RequestTerminalPosition( iSettings.iLocationRequestTimeout );
            	return;
            	}
            
            // Check if nearest trigger has changed
            TLbtTriggerId nearestTriggerAfterEvaluation = 0;
            if( iNearestTriggers.Count() )
                {
                for( TInt i=0;i<iNearestTriggers.Count();i++ )
                    {
                    if( iNearestTriggers[i].remRearmTime == 0 )
                        {
                        nearestTriggerAfterEvaluation = iNearestTriggers[i].id;
                        break;
                        }
                    }
                }
            // This is check is required when all triggers in the system has fired
            // and new trigger has been added.
            if( (nearestTriggerAfterEvaluation) && 
                (nearestTrigger != nearestTriggerAfterEvaluation) )                 
                {
                RequestTerminalPosition( iSettings.iLocationRequestTimeout );
                }            
            else
                {
                // Check if there be any remaining sleep interval. If true, 
                // then switch to sleep mode for the remaining time period. 
                // Else switch to acquiring location information.
                TTimeIntervalSeconds remainingInterval;
                if( iTimer->RemainingSleepInterval( remainingInterval ) )
                    {
                    iState = ESleep;
                    if( remainingInterval.Int() < 5 )
                        {
                        iTimer->ResetInterval( iStatus, remainingInterval,EFalse ); 
                        }
                    else
                        {
                        iTimer->ResetInterval( iStatus, remainingInterval,ETrue ); 
                        }
                    SetActive();
                    }
                else
                    {
                    RequestTerminalPosition( iSettings.iLocationRequestTimeout );
                    }
                }
            }
        }        
    else if( KErrNone == iStatus.Int() || KErrAbort == iStatus.Int() )
        {
        RequestTerminalPosition( iSettings.iLocationRequestTimeout );
        }
    else
        {
        LOG1("Error in timer:%d",iStatus.Int());
        }
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::RequestTerminalPosition
// Issues an asynchronous request to determine terminal's position
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::RequestTerminalPosition( TInt aLocationRequestTimeout,
                                                        TLocationFixType aFixLocationFixType )
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::RequestTerminalPosition");
    iPrevPosInfo = iPosInfo;
    LOG1("aFixLocationFixType = %d", aFixLocationFixType);
    LOG1("Cell Id = %d", iLocArea.iCellId);

    if( (aFixLocationFixType == EFixFromDb) && 
        (iLastCellRequestStatus == KErrNone) && 
        iLocArea.iCellId )
        {
        LOG("Taking fix from DB");
        TInt error = GetCellIdPosition( iPosInfo );
        if( KErrNone ==  error)
            {
            LOG("Fix From DB");
            TPosition position;
            iPosInfo.GetPosition(position);
            LOG1("Lat:%f", position.Latitude());
			LOG1("Long:%f", position.Longitude());
			LOG1("HA:%f", position.HorizontalAccuracy());
            iLastLocationFixType = EFixFromDb;
            iState = ETriggerEvaluation;
            SetEvent();
            return;
            }
        else
        	{
        	LOG1("Failed to get Fix From DB with %d", error);
        	}
        }
    else if( aFixLocationFixType == EFixFromNpp )
        {
        iLastLocationFixType = EFixFromNpp;
        iLocationRequestor->CurrentLocationFromNpp( iPosInfo, iStatus, 
                                                    TTimeIntervalMicroSeconds( aLocationRequestTimeout ) );
        iState = ELocationAcquisition;
        SetActive();
        return;
        }

    iLastLocationFixType = EProperFix;
    iLocationRequestor->CurrentLocation( iPosInfo, iStatus, 
                                            iModuleId,
                                            TTimeIntervalMicroSeconds( aLocationRequestTimeout ) );
    iState = ELocationAcquisition;
    SetActive();        
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::GetCellIdPosition
// Get the position corresponding to current cell id
// -----------------------------------------------------------------------------
//
TInt CLbtCoOrdinateSupervisor::GetCellIdPosition( TPositionInfo& aPositionInfo )
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::GetCellIdPosition");
    TInt countryCode,networkCode;
    
    TLex lex( iNwInfo.iCountryCode );
    lex.Val( countryCode );
    
    lex.Assign( iNwInfo.iNetworkId );
    lex.Val( networkCode );
    
    TPosition position;
    TInt error = iCellIdDatabase->GetLocationForCid( countryCode,networkCode,iLocArea.iLocationAreaCode,
                                                     iLocArea.iCellId,position );
    if( error == KErrNone )
        {
        aPositionInfo.SetPosition( position );
        }
    return error;    
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::UpdateNearestTriggers
// Updates the record of nearest triggers
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::UpdateNearestTriggers( TLbtTriggerId aID, TReal aDistance , TInt aRemRearmTime )
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::UpdateNearestTriggers");
    TNearestTrigger nTrigger;
    nTrigger.distance = aDistance;
    nTrigger.id = aID;
    nTrigger.remRearmTime = aRemRearmTime;
    
    TLinearOrder< TNearestTrigger > byDistance( CompareByDistance );
    iNearestTriggers.InsertInOrderAllowRepeats( nTrigger, byDistance );
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::EstimateTriggerRadii
// Calculcates the trigger radius and the radius of the hysteresis circle
// -----------------------------------------------------------------------------
//
CLbtCoOrdinateSupervisor::TTriggerArea CLbtCoOrdinateSupervisor::EstimateTriggerRadii( 
                         TReal aTriggerRadius, TReal aHysteresisRadius,
                         TReal aPositionAccuracy )
    {
    TTriggerArea trgArea;
    TReal ratioOfCircularAreas =  aPositionAccuracy / aTriggerRadius;
    
    if(  ratioOfCircularAreas >= KCofftOfHighEdgeLimit )
        {
        trgArea.triggerRadius = aTriggerRadius;
        trgArea.hysteresisRadius = aHysteresisRadius;
        }
    else
        {
        // Update radiusOfTrigger and radiusOfHysteresis accordingly
        trgArea.triggerRadius = aTriggerRadius +( KAccuracyMultiplier * aPositionAccuracy );
        trgArea.hysteresisRadius = Hysteresis( trgArea.triggerRadius );        
        }
        
    return trgArea;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::Hysteresis
// Calculates the hysteresis radius 
// -----------------------------------------------------------------------------
//
TReal CLbtCoOrdinateSupervisor::Hysteresis( TReal aTriggerRadius )
    {
    TReal hysteresis;
    if( aTriggerRadius > KTriggerRadiusCutoff )
        {
        TReal logRadius;				
        Math::Ln( logRadius, aTriggerRadius / 10 );
        hysteresis = aTriggerRadius / logRadius;        
        }
    else
        {
        hysteresis = aTriggerRadius / 4;
        }
    
    return hysteresis + aTriggerRadius;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::DetermineNearestDistances
// Algorithm to determine the shortest distance
// -----------------------------------------------------------------------------
//
TInt CLbtCoOrdinateSupervisor::CompareByDistance( const TNearestTrigger& src,
        const TNearestTrigger& dst )
    {
    if( src.distance > dst.distance )
        {
        return 1;
        }
    else if( src.distance < dst.distance )
        {
        return -1;
        }
    return 0;
    }
   

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::CompareByID
// Determine equality of nearest triggers by id
// -----------------------------------------------------------------------------
//
TBool CLbtCoOrdinateSupervisor::CompareByID( const TNearestTrigger& src,
        const TNearestTrigger& dst )
    {
    if( src.id > dst.id )
        {
        return ETrue;
        }
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::ResetViewIterator
// Resets trigger view iterator.
// -----------------------------------------------------------------------------
//
TInt CLbtCoOrdinateSupervisor::ResetViewIterator()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::ResetViewIterator");
    TInt err = iViewIterator.Reset();
    if( KErrNone != err )
        {
        if( KErrNotFound == err )
            {
            // determine update interval
            iState = iProjectedState = EIdle;
            }
        else
            {
            // TODO: Report error to server. This can be done only after 
            // introducing new state to strategy dynamic info.
            Term();
            }
        return err;
        }
        
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::RunL
// Inherited from CActive - called when the request issued is completed.
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::RunL()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::RunL");
    switch ( iState )
        {
        case ELocationAcquisition:
            GetPositionInfoL();
            break;

        case ETriggerEvaluation:
            EvaluateTriggersL();
            break;
            
        case EUpdateIntervalEstimation:
            EstimateSleepInterval();
            break;

        case ESleep:
        	LOG("Wakeup");
            ProcessInterrupt();
            break;
        
        default:
            break;
        }
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::DoCancel
// Inherited from CActive - called to cancel outanding events.
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::DoCancel()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::DoCancel");
    iFixForFiringCloseTriggers = EFalse;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::RunError
// Handles the event of leave from the request completion event handler RunL()
// -----------------------------------------------------------------------------
//
TInt CLbtCoOrdinateSupervisor::RunError( TInt /*aError*/ ) 
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::RunError");
    Term();
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::CLbtCoOrdinateSupervisor
// C++ default constructor.
// -----------------------------------------------------------------------------
//
CLbtCoOrdinateSupervisor::CLbtCoOrdinateSupervisor() : 
                CActive( EPriorityHigh )
    {
    CActiveScheduler::Add( this );
    }

    
// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::ConstructL
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::ConstructL( TAny* /*aConstructionParameters*/,
                                           CLbtTriggerView* aView,
                                           MLbtStrategyEngineObsrvr* aObserver,
                                           CLbtCellChangeHandler* aCellChangeHandler )
    {
	iEngObserver = aObserver;
	iState = iProjectedState = EIdle;
	iEngObserver->RetrieveTriggeringSettings( iSettings );
	iCellChangeHandler = aCellChangeHandler;
  
    iModuleId = iSettings.iPositioningTechnology;
    
	iLocationRequestor = CLbtLocationRequestor::NewL();
	
	iStatusManager = CLbtStatusManager::NewL();
	iTimer = CLbtStrategyTimer::NewL();
	iView = aView;	
	
	iViewIterator = iView->BeginL( CLbtGeoAreaBase::ECircle );
	iCellIdDatabase = CLbtCellIdDatabase::NewL();
	iLastCellRequestStatus = KErrUnknown;
	iLastLocationFixType = EFixUnknown;
	iPsyChangeListner = NULL;
	iFixForFiringCloseTriggers = EFalse;
    }
    

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::SetEvent
// Completes the iRequestStatus with KErrNone.
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::SetEvent()
    {
    if ( !IsActive() )
    	{
    	TRequestStatus* lStatus = &iStatus;
    	User::RequestComplete( lStatus, KErrNone );
        SetActive();
    	}    
    }


// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::Term
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::Term()
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::Term");
    
    iSupervisionInitiated = EFalse;
            
    iView->Cancel();
    switch( iState )
        {
    case ELocationAcquisition:
        if( iLastLocationFixType == EProperFix )
    		{
    		iLocationRequestor->Cancel();
    		}
    	else
    		{
    		iLocationRequestor->CancelFixFromNpp();
    		}
        break;
        
    case ESleep:
        iTimer->Cancel();
        break;
    
    default:
        Cancel();
        break;
        }
        
    iState = iProjectedState = EIdle;
    iLastLocationFixType = EFixUnknown;
    iFixForFiringCloseTriggers = EFalse;
    }

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::FireClosestTriggers
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::FireClosestTriggers()
	{
	FUNC_ENTER("CLbtCoOrdinateSupervisor::FireClosestTriggers");
	LOG1("NT Count = %d", iNearestTriggers.Count());
	for( TInt i=0;i<iNearestTriggers.Count();i++ )
		{
		if(iNearestTriggers[i].remRearmTime != 0)
		    {
		    continue;
		    }

		TInt timeToTrigger = (iNearestTriggers[i].distance / iEstimatedUserSpeed);		

        TInt estimatedInterval = timeToTrigger - iProcessingInterval;

		LOG1("estimatedInterval = %d", estimatedInterval);

		if( estimatedInterval <= KMinSleepInterval )
			{
			LOG("NEED TO FIRE");
			CLbtContainerTriggerEntry* trigger;
			if( iView->GetTriggerByID( iNearestTriggers[i].id,trigger ) )
				{
				LOGSTRING("Trigger: \"%S\" ", &(trigger->TriggerEntry()->Name()));
				// check the activity status
				TStrategyData strategyData = trigger->StrategyData();   
				if( strategyData.iActivityStatus == EActive )
				    {
				    // Fire the trigger
                    TLbtTriggerDataMask dataMask = 0;
                    
                    // Set trigger state to fired
                    trigger->ExtendedTriggerInfo()->SetTriggerFiredState( ETrue );
                    TLbtTriggerFireInfo triggerFireInfo;
                    TPositionInfo positionInfo;
                    triggerFireInfo.iTriggerId = trigger->TriggerEntry()->Id();
                    triggerFireInfo.iFiredPositionInfo = iPrevPosInfo;
                    
                    trigger->ExtendedTriggerInfo()->SetFiredInfo( triggerFireInfo );
                    
                    // Time at which trigger fired
                    TTime triggerFiredTime;
                    triggerFiredTime.UniversalTime();
                    LOG("NEAREST TRIGGER FIRED");
                    
                    strategyData.iFireTime = triggerFiredTime.Int64();
                    // Set trigger to inactive state
                    strategyData.iActivityStatus = EInactive;
                    
                    dataMask |= 
                    CLbtContainerTriggerEntry::EContainerAttributeIsFired | 
                    CLbtContainerTriggerEntry::EContainerAttributeFireInfo ;
                    
                    iView->UpdateTriggerInfo( CLbtGeoAreaBase::ECircle,iPosInfo, trigger, dataMask );
                    trigger->SetStrategyData( strategyData );
				    }
                }
            else
                {
                LOG("TRIGGER NOT FOUND ??");
                }
			}
		else
			{
			LOG("ELSE");
			// Since the array is in the ascending order of distance( which is directly proportional to time ),
			// we can need not search rest of the array.	
			break;
			}					
		}
	}

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::CalculateTolerableRadius
// 
// -----------------------------------------------------------------------------
//
TReal CLbtCoOrdinateSupervisor::CalculateTolerableRadius( TReal aRadius )
    {
    // Tolerance can be calculated using the straight line equation. Slope of straight line is 0.07 and 
    // intercept is 22.2
    TReal tolerableInaccuracy = ( 0.07 * aRadius ) + 22.2;
    return ( tolerableInaccuracy + aRadius );
    }

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::HandleCellChangeEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::HandleCellChangeEvent(
            const TInt aStatus,
            const RMobilePhone::TMobilePhoneNetworkInfoV1& aNwInfo,
            const RMobilePhone::TMobilePhoneLocationAreaV1& aLocArea)
    {
    FUNC_ENTER("CLbtCoOrdinateSupervisor::HandleCellChangeEvent");
    if( KErrNone == aStatus )
	    {
	    // validate network info
		if (aNwInfo.iMode == RMobilePhone::ENetworkModeGsm ||
            aNwInfo.iMode == RMobilePhone::ENetworkModeWcdma)
            {
            TLex lex( aNwInfo.iCountryCode );
            TInt countryCode,networkCode;
            lex.Val( countryCode );
            if( countryCode )
                {
                lex.Assign( aNwInfo.iNetworkId );
                lex.Val( networkCode );
                if( networkCode )
                    {
                    if( aLocArea.iLocationAreaCode && aLocArea.iCellId )
                        {
                        iLastCellRequestStatus = KErrNone;
                        iNwInfo = aNwInfo;
		                iLocArea = aLocArea;
		                }
                    }
                }
            }
		}
	else 
	    {
	    iLastCellRequestStatus = aStatus;
	    }	
    }

// -----------------------------------------------------------------------------
// CLbtCoOrdinateSupervisor::HandlePsyChangeEvent
// 
// -----------------------------------------------------------------------------
//
void CLbtCoOrdinateSupervisor::HandlePsyChangeEvent( TPositionModuleId aModuleId  )
    {
    LOG("CLbtCoOrdinateSupervisor::HandlePsyChangeEvent");
    if( aModuleId == iSettings.iPositioningTechnology )
        {
        LOG("aModuleId == iSettings.iPositioningTechnology");
        iModuleId = aModuleId;
        delete iPsyChangeListner;
        iPsyChangeListner = NULL;        
        }
    else
        {
        LOG("aModuleId != iSettings.iPositioningTechnology");
        // We will use defualt proxy
        iModuleId = TUid::Uid(0xFFFFFFFF);
        iPsyChangeListner->StartToListen( iSettings.iPositioningTechnology );
        }
    
    // If coordinate supervisor AO is not active,request terminal position    
    if( !IsActive() )
        {
        RequestTerminalPosition( iSettings.iLocationRequestTimeout,EProperFix );
        }    
    }
// end of file
