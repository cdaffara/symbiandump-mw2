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
* Description:  Sim change handler class definition
*
*/


#ifndef C_LBTSIMCHANGEHANDLER_H
#define C_LBTSIMCHANGEHANDLER_H

#include <e32base.h>
#include <etelmm.h>
#include "lbttriggerchangeobserver.h"

class RProperty;
class CLbtContainer;
class CLbtCleanupHandler;
class CLbtTriggerFilterByArea;
class MLbtServerLogicTriggerChangeObserver;

/**
 * Class definition for sim change handler.
 *
 * @since S60 v4.0
 * This class listens to sim change event and takes action based on sim change
 */
class CLbtSimChangeHandler : public CActive,
                             public MLbtContainerChangeEventObserver
    {
    enum TState
        {
        EStateSimChangeEvent,
        
        EStateGetNetworkInfo,
        
        EStateListTriggers,
        
        EStateEvaluateTriggers,
        
        EStateInvalidateTriggers,
        
        EStateValidateTriggers,
        
        EStateIdle
        };
public:
	/**
     * Instantiates a new object of 
     * CLbtSimChangeHandler
     */
	static CLbtSimChangeHandler* NewL( CLbtContainer& aContainer,CLbtCleanupHandler& aCleanupHandler,
	                                   MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver );

   /**
    * Destructor.
    */
    virtual ~CLbtSimChangeHandler();
    
    /**
     * Starts listening to sim change event events
     *
     */
    void StartToListenL();
    
public: // from MLbtContainerChangeEventObserver
    void TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                              TLbtTriggerEventMask aEventMask,
                              TLbtContainerChangedAreaType aAreaType,
                              RArray<TUid>& aManagerUids );   

protected:// From CActive
    void RunL();
    void DoCancel();
    TInt RunError( TInt aError );
    
private:
    /**
     * Default constructor
     */
    CLbtSimChangeHandler( CLbtContainer& aContainer,CLbtCleanupHandler& aCleanupHandler,
                          MLbtServerLogicTriggerChangeObserver& aTriggerChangeObserver );
    
    void ConstructL();
    
    /** 
     * Get the current network information
     */
    void GetCurrentNetworkInfo(); 
    
    /**
     * Lists all the cell based trigger in the system
     */
    void ListCellTriggersL();
    
    /**
     * Invalidates all the cell trigger that doesnt fall in the 
     * current network 
     */
    void EvaluateTriggers();
    
    void InvalidateDifferentNwTriggersL();
    
    void ValidateCurrentNwTriggersL();
    
    /**
     * Subscribe for sim change
     */
    void SubscribeForSimChange(); 
      
    /**
     * Initialises Etel.
     */
    void InitialiseEtelL();

private:// data
    
    /**
     * Reference to container
     */
    CLbtContainer&                              iContainer; 
    
    /**
     * Reference to 
     */
    CLbtCleanupHandler&                         iCleanupHandler;
 
    /*
      * Reference to MLbtServerLogicTriggerChangeObserver
      */
    MLbtServerLogicTriggerChangeObserver&        iTriggerChangeObserver;
  
    /**
     * Handle to telephony server
     */
    RTelServer 			                        iTelServer; 
    
    /**
     * Handle to RMobilePhone
     */
    RMobilePhone 		                        iMPhone;
     
    /**
     * Handle to the sim property object
     *
     */	
    RProperty                                   iSimProperty;
    
    /**
     * Network mode
     */
    RMobilePhone::TMobilePhoneNetworkMode       iNetworkMode;    
    
    /**
     * Network info
     */
    RMobilePhone::TMobilePhoneNetworkInfoV1		iNwInfo;
    
    /**
     * Network info package
     */
    RMobilePhone::TMobilePhoneNetworkInfoV1Pckg	iNwInfoPckg; 
    
    /**
     * location info
     */
    RMobilePhone::TMobilePhoneLocationAreaV1	iLocArea; 
    
    RPointerArray<CLbtContainerTriggerEntry>    iTriggerList;
    
    RArray<TLbtTriggerId>                       iInvalidTriggers;
    
    RArray<TLbtTriggerId>                       iValidTriggers;
    
    TState                                      iState;
    
    
    TInt                                        iAoOpcode;
    
    };


#endif // C_LBTSIMCHANGEHANDLER_H
