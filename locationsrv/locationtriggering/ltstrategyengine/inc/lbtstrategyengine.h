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



#ifndef C_LBTSTRATEGYENGINE_H
#define C_LBTSTRATEGYENGINE_H

#include <etelmm.h>
#include "lbtstrategy.h"
#include "lbttriggerview.h"	
#include "lbttriggerchangeobserver.h"
#include "lbtcoordinatesupervisor.h"
#include "lbtcellsupervisor.h"

class MLbtStrategyEngineObsrvr;
class CLbtCellChangeHandler;

/**
 * CLbtStrategyEngine is responsible for supervising triggers in the system. 
 * The instance of CLbtStrategyEngine acquires the terminal's location 
 * information using the instance of CLbtLocationRequestor and supervises 
 * triggers. It notifies location triggering server logic component when 
 * triggers are fired. 
 *
 * @since S60 v4.0
 */
NONSHARABLE_CLASS( CLbtStrategyEngine ) : public MLbtContainerChangeEventObserver,
                                          public MLbtTriggerViewObserver
						                      
    {
public:     // Methods

    /**
     * Factory function that instantiates an object of CLbtStrategyEngine
     *
     * @param aConstructionParameters - 
     */
    static CLbtStrategyEngine* NewL( TAny* aConstructionParameters, 
                                        MLbtStrategyEngineObsrvr* aObserver );

    /**
     * Destructor
     */
    ~CLbtStrategyEngine();

public:

    /**
     * Called by CLbtStrategy to issue request to start supervision
     */
    void StartSupervision();

    /**
     * Called by CLbtStrategy to issue request to stop the supervision process
     */
    void StopSupervision();

    /**
     * Called by CLbtStrategy when settings related to trigger supervision 
     * changes
     */
    void SettingsChanged();
    
public:     // from interface class MTriggerViewObserver

    /**
     * From MTriggerViewObserver
     * Called by CLbtTriggerView when the view is refreshed after a new 
     * position fix is provided or after trigger modifications are
     * notified by the Container
     */
    virtual void OperationComplete( 
                            MLbtTriggerViewObserver::TOperation aOperation );
    
public:     // from interface class MLbtContainerChangeEventObserver

    /**
     * From MLbtContainerChangeEventObserver
     * Called by Location Triggering Container when trigger(s) is / are 
     * modified or created
     *
     * @param aTriggerIds - ID(s) of the trigger(s) that was/were created or 
     * modified
     */
    virtual void TriggerStoreChanged( RArray<TLbtTriggerId>& aTriggerIds,
                                      TLbtTriggerEventMask aEventMask,
                                      TLbtContainerChangedAreaType aAreaType,
                                      RArray<TUid>& aManagerUids );


private:
	 void ConstructL( TAny* aConstructionParameters,
                                        MLbtStrategyEngineObsrvr* aObserver );

	CLbtStrategyEngine();
	
	/**
     * Initialises Etel.
     */
    void InitialiseEtelL();

private:
	
	CLbtTriggerView*				            iView;
	
	CLbtCoOrdinateSupervisor*                   iCoOrdinateSupervisor;
	
	CLbtCellSupervisor*                         iCellSupervisor;
	
	TBool                                       iSupervisionInitiated;
	
	TBool                                       iViewInitialised;
	
	MLbtContainerChangeEventObserver::TLbtContainerChangedAreaType iChangedAreaType;
	
	RTelServer 			                        iTelServer; 

    RMobilePhone 		                        iMPhone; 

    CLbtCellChangeHandler*                      iCellChangeHandler;
	};

#endif	// C_LBTSTRATEGYENGINE_H
