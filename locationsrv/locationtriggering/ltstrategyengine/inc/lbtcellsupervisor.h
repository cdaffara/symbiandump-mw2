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
* Description:  Class declaration of Cell supervisor class
*
*/



#ifndef C_LBTCELLSUPERVISOR_H
#define C_LBTCELLSUPERVISOR_H


#include <etelmm.h>
#include "lbtstrategy.h"
#include "lbttriggerview.h"
#include "lbtcellchangehandler.h"
#include "lbtcellchangeobserver.h"
#include "lbtcellsupervisortimer.h"
#include "lbtgeocell.h"

//FORWARD DECLARATIONS
class TLbtWcdmaCellInfo;
class CLbtGeoWcdmaCell;


class MLbtStrategyEngineObsrvr;

/**
 * CLbtCellSupervisor is responsible for supervising triggers in the system. 
 * The instance of CLbtCellSupervisor acquires the terminal's cell and nmr 
 * information using the instance of CLbtCellHandler,CLbtEcidHandler and supervises 
 * triggers. It notifies location triggering server logic component when 
 * triggers are fired. 
 *
 * @since S60 v4.0
 */
class CLbtCellSupervisor : public CBase,
                           public MCellChangeObserver,
                           public MLbtCellTimerObserver
    {

public:
		/**
     * Factory function that instantiates an object of CLbtCoOrdinateSupervisor
     *
     * @param aConstructionParameters -
     * @param aView Pointer to CLbtTriggerView class.
     * @param aObserver Pointer to strategy engine observer.
     */
    static CLbtCellSupervisor* NewL(TAny* aConstructionParameters,
                                    CLbtTriggerView* aView,
                                    MLbtStrategyEngineObsrvr* aObserver,
                                    CLbtCellChangeHandler* aCellChangeHandler);
        

    /**
    * Destructor.
    */
    virtual ~CLbtCellSupervisor();

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
     *  Resumes cell supervision
     */
    void Resume();
     
    /**
     * Resets cell supervision
     */
    void Reset();

public:
    // From MLbtCellChangeObserver
    virtual void HandleCellChangeEvent(
            const TInt aStatus,
            const RMobilePhone::TMobilePhoneNetworkInfoV1& aNwInfo,
            const RMobilePhone::TMobilePhoneLocationAreaV1& aLocArea);
            
public://From MLbtCellTimerObserver
    
    virtual void TimeOut();
            
private:


    CLbtCellSupervisor();

    void ConstructL( TAny* aConstructionParameters,
                     CLbtTriggerView* aView,
                     MLbtStrategyEngineObsrvr* aObserver,
                     CLbtCellChangeHandler* aCellChangeHandler );
    
    
    
    /**
     * Evaluates triggers based on the network and cell information available.
     */
    void EvaluateTriggersL(); 
    
    /**
     * Calculates remaining rearm time 
     */
    TInt CalculateRearmTime( TStrategyData& aStrategyData, TInt aTimeToRearm );
    
    /**
     * 
     */
    TBool CheckForCurrentCGIMatch( CLbtGeoCell* aCellularArea );
    
    /**
     * Evaluates triggers with cellular area definition
     */
    void EvalCellularTrigger( CLbtContainerTriggerEntry* aTrigger );
    
    /**
     * Evaluates triggers with hybrid area definition
     */
    void EvalHybridTrigger( CLbtContainerTriggerEntry* aTrigger );

    
    /**
     * Handles updation of trigger when a trigger is fired
     */
    void HandleTriggerFired( CLbtContainerTriggerEntry* aTrigger, TStrategyData& aStrategyData, 
    						 TLbtTriggerDataMask& aDataMask );
    
    
    /**
     * Estimates sleep interval for cell supervisor
     */
    void EstimateSleepInterval();
    
    /**
     * This method evaluates activated triggers for current cell
     */
    void EvaluateActivatedTriggers( RPointerArray<CLbtContainerTriggerEntry>& 
                                                    aActivatedTriggers ); 
                                                    
    /**
     * This method returns count of visited cell
     */                                                
    TInt VisitedCellCount( TStrategyData& aStrategyData );
    
    /**
     * This method returns ETrue if cellid is present else EFalse
     */
    TBool FindCellId( TStrategyData& aStrategyData,TInt aCellId ); 
    
    /**
     * Adds cellid into the strategy data.
     */
    void AddCellId( TStrategyData& aStrategyData,TInt aCellId ); 
    
    /**
     * Clears visited cell data from strategy data.
     */
    void ClearVisitedCellInfo( TStrategyData& aStrategyData ); 
    
    
private: // data
    
    CLbtTriggerView*                 iView;
    
    MLbtStrategyEngineObsrvr*        iEngObserver;
    
    CLbtCellChangeHandler*           iCellChangeHandler;
    
    RMobilePhone::TMobilePhoneNetworkInfoV1		iNwInfo; ///< network info
    
    RMobilePhone::TMobilePhoneLocationAreaV1	iLocArea; ///< location info
    
    TInt     iCurrentCountryCode;
    
    TInt     iCurrentNetworkCode;
    
    TLbtStrategyTriggeringSupervisionSettings   iSettings;
    
    CLbtTriggerView::TTriggerViewIter  iViewIterator;
    
    CLbtCellSupervisorTimer*         iCellSupervisorTimer;
    
    // Array of recently fired trigger
    RPointerArray<CLbtContainerTriggerEntry>          iRecentlyFiredTriggerArray;
    };


#endif // C_LBTCELLSUPERVISOR_H
