/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Location Acquisition Strategy Engine
*
*/



#ifndef C_LBTCOORDSUPERVISOR_H
#define C_LBTCOORDSUPERVISOR_H

#include "lbtstrategy.h"
#include "lbtlocationrequestor.h"
#include "lbttriggerview.h"	
#include "lbttriggerchangeobserver.h"
#include "lbtstatusmanager.h"
#include "lbtstrategytimer.h"
#include "lbtcellchangeobserver.h"
#include "lbtpsychangeobserver.h"

class CLbtCellChangeHandler;
class CLbtCellIdDatabase;
class CLbtPsyChangeListner;

/**
 * CLbtCoOrdinateSupervisor is responsible for supervising triggers in the system. 
 * The instance of CLbtCoOrdinateSupervisor acquires the terminal's location 
 * information using the instance of CLbtLocationRequestor and supervises 
 * triggers. It notifies location triggering server logic component when 
 * triggers are fired. 
 *
 * @since S60 v4.0
 */
class CLbtCoOrdinateSupervisor : public CActive,
                                 public MCellChangeObserver,
                                 public MLbtPsyChangeObserver
    {
    struct TTriggerArea;
    /**
     * Enum that specifies whether we need proper fix using any of the psy
     * or fix from the cell-id database    
     */
    enum TLocationFixType 
    		{
    		EProperFix,
    		
    		EFixFromNpp,
    		
    		EFixFromDb,
    		
    		EFixUnknown
    		};
    
public:     // Methods

    /**
     * Factory function that instantiates an object of CLbtCoOrdinateSupervisor
     *
     * @param aConstructionParameters - 
     */
    static CLbtCoOrdinateSupervisor* NewL( TAny* aConstructionParameters,
                                        	 CLbtTriggerView* aView,
                                        	 MLbtStrategyEngineObsrvr* aObserver,
                                        	 CLbtCellChangeHandler* aCellChangeHandler );

    /**
     * Destructor
     */
    ~CLbtCoOrdinateSupervisor();

public:

    /**
     * Called by CLbtStrategyEngine to issue request to start supervision
     */
    void StartSupervision();

    /**
     * Called by CLbtStrategyEngine to issue request to stop the supervision process
     */
    void StopSupervision();

    /**
     * Called by CLbtStrategyEngine when settings related to trigger supervision 
     * changes
     */
    void SettingsChanged();
    
    /**
     *
     */
    void Resume();
    
    /**
     *
     */
    void Suspend();
    
    /**
     *
     */
    void Reset();
    
public:// from cell change observer
    void HandleCellChangeEvent(
            const TInt aStatus,
            const RMobilePhone::TMobilePhoneNetworkInfoV1& aNwInfo,
            const RMobilePhone::TMobilePhoneLocationAreaV1& aLocArea);   

public: // from psy change observer             
    void HandlePsyChangeEvent( TPositionModuleId aModuleId  ); 
    
private:    // from base class CActive

    /**
     * From CActive
     * Request completion event handler
     */
    void RunL();

    /**
     * From CActive
     * Called to cancel outanding requests
     */
    void DoCancel();

    /**
     * From CActive
     * Handles the event of leave from the request completion 
     * event handler RunL()
     *
     * @param aError - Argument specifying error code
     */
    TInt RunError( TInt aError );

private:	// Methods

    /**
     * Handles response from Location Requestor 
     */
    void GetPositionInfoL();
    
    /**
     * Evaluates triggers based on the location information obtained
     */
    void EvaluateTriggersL();
    
    /**
     * Evaluates a specific set of triggers based on the location information
     * obtained
     *
     * @return ETrue if a location request is need EFalse otherwise
     */
    TBool EvaluateModifiedTriggersL();
     
    /**
     * Evaluates triggers with circular area definition
     */
    void EvalCircularTriggerL( CLbtContainerTriggerEntry* aTrigger );
    
    /**
     * Evaluates trigger based on cell location information
     *
     * @retrun ETrue If proper fix is required.
     */
    TBool EvalCircularTriggerBasedOnCellPositionL( 
                                          CLbtContainerTriggerEntry* aTrigger );
    
    /**
     * Calculates the sleep interval
     */    
    void EstimateSleepInterval();
    
    /**
     * Evaluates the cause of a timer interrupt and determines 
     * the necessary actions to be performed
     */
    void ProcessInterrupt();
    
    /**
     * Issues an asynchronous request to determine terminal's position
     */    
    void RequestTerminalPosition( TInt aLocationRequestTimeout,
                                  TLocationFixType aFixLocationFixType = EFixFromDb );
    
    /**
     * Get the position corresponding to current cell id
     */
    TInt GetCellIdPosition( TPositionInfo& aPositionInfo ); 
    
    /**
     * Maintains a record of the nearest triggers
     */
    void UpdateNearestTriggers( TLbtTriggerId aID, TReal aDistance ,TInt aRemRearmTime );
    
    /**
     * Calculcates the trigger radius and the radius of the hysteresis circle
     */
    TTriggerArea EstimateTriggerRadii( TReal aTriggerRadius, TReal aHysteresisRadius,
                                       TReal aPositionAccuracy );
   
   	
   	/**
   	 * Fires all the triggers which are very close. This is done because of unavailability of fix with in
   	 * the specified time. 
   	 */
   	void FireClosestTriggers();        
   	
   	/**
   	 *  Calculates the tolerable radius for with in which trigger can be fired
   	 */
   	TReal CalculateTolerableRadius( TReal aRadius );
    /**
     * Determines the hysteresis radius
     */
    TReal Hysteresis( TReal aTriggerRadius );
    
    /**
     * Algorithm to determine the shortest distance
     */
    static TInt CompareByDistance( const TNearestTrigger& src, 
                                        const TNearestTrigger& dst );
    
    /**
     * Determine equality of nearest triggers by id
     */
    static TBool CompareByID( const TNearestTrigger& src,
                                    const TNearestTrigger& dst );
    /**
     * Resets the trigger view iterator
     */
    TInt ResetViewIterator();    
	
    /**
     * Default constructor
     */
    CLbtCoOrdinateSupervisor();

    /**
     * 2nd phase constuctor for instantiating member variables
     *
     * @param aConstructionParameters - 
     */
    void ConstructL( TAny* aConstructionParameters, 
                        CLbtTriggerView* aView,
                        MLbtStrategyEngineObsrvr* aObserver,
                        CLbtCellChangeHandler* aCellChangeHandler );

    /**
     * Completes the iRequestStatus with KErrNone 
     */
    void SetEvent();
    
    /**
     * Cancels all outstanding requests and initiates termination
     */
    void Term();

private:	// Attributes

    /**
     * Traces the number of location requests made by Strategy Engine
     */
    TInt                                        iLocReqCount;
    
    /**
     * Sleep time interval
     */
    TTimeIntervalSeconds		                iSleepInterval;
    
    /**
     * Estimated speed
     */
    TReal                                       iEstimatedUserSpeed; 
    
    /**
     * Triggering settings
     */
    TLbtStrategyTriggeringSupervisionSettings   iSettings;

    /**
     * Enumeration to define states of the Strategy Engine
     */
    typedef enum TState 
    
    	{  
    	
    	EIdle = 1, 
    	
    	ELocationAcquisition, 
    	
    	ETriggerEvaluation,
    	
    	EUpdateIntervalEstimation,
    	
    	ESleep,
    	
    	ETerm,
    	
    	};
    	
    /**
     * Boundary values of the trigger
     */ 
    typedef enum TTriggerBoundary
    
        {
        
        EUndefined = 0,
        
        EInside,
        
        EOutside,
        
        };
        
    /**
     * Structure that contains the observed speed and time values
     */
    struct TObservedValues
        
        {

        TTime    time;

        TReal    speed;

        };
        
    /**
     * Structure that contains the trigger and hysteresis radius
     */
    struct TTriggerArea
        
        {
        
        TReal triggerRadius;
        
        TReal hysteresisRadius;
        
        };    
        
    RArray< TObservedValues >                   iNObservedValues;
    
    RArray< TNearestTrigger >                   iNearestTriggers;
    
    TPositionModuleId                           iModuleId;
    
    TInt                                        iProcessingInterval;
    
    TBool                                       iSupervisionInitiated;

    TState 							            iState, iProjectedState;
    
	TPositionInfo					            iPosInfo, iPrevPosInfo;
    
	CLbtStatusManager*				            iStatusManager;
	
	CLbtTriggerView*				            iView;
	
	CLbtTriggerView::TTriggerViewIter           iViewIterator;
	
	CLbtLocationRequestor*			            iLocationRequestor;
	
	CLbtStrategyTimer*                          iTimer;
	
	MLbtStrategyEngineObsrvr*                   iEngObserver;
	
	TLbtLocationAcquisitionStatus               iAcquisitionStatus;
	
	CLbtCellChangeHandler*                      iCellChangeHandler;
	
	CLbtCellIdDatabase*                    		iCellIdDatabase;
	
	CLbtPsyChangeListner*                       iPsyChangeListner;
	
	RMobilePhone::TMobilePhoneNetworkInfoV1		iNwInfo; ///< network info
    
    RMobilePhone::TMobilePhoneLocationAreaV1	iLocArea; ///< location info
    
    TLocationFixType                            iLastLocationFixType;
	
	TInt                                        iLastCellRequestStatus;
	
	TTime 										iLastFixTime;
	
	TBool                                       iFixForFiringCloseTriggers;
    };

#endif	// C_LBTCOORDSUPERVISOR_H
