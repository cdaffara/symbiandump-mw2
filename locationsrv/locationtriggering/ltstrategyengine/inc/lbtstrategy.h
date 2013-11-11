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
* Description:  Class definition of Location Acquisition Strategy
*
*/



#ifndef C_LBTSTRATEGY_H
#define C_LBTSTRATEGY_H


#include "lbtstrategybase.h"
#include "lbtstrategyengineobserver.h"


class CLbtStrategyEngine;


/**
 * CLbtStrategy is an ECom plugin that implements CLbtStrategyBase class. This 
 * class defines a containment for a single instance of CLBtStrategyEngine that
 * is responsible for supervising triggers in the system. Notifications from 
 * CLbtStrategyEngine about firing of triggers, status of trigger supervision 
 * is received and forwarded to location triggering server logic.
 *
 * @since S60 v4.0
 */
NONSHARABLE_CLASS( CLbtStrategy ) : public CLbtStrategyBase,
                                    public MLbtStrategyEngineObsrvr

    {

public:

    /**
     * Factory function that instantiates an object of CLbtStrategy
     *
     * @param aConstructionParameters - 
     */
    static CLbtStrategy* NewL( TAny* aConstructionParameters );
    
    /**
     * Destructor
     */
    ~CLbtStrategy();
    
public:     // from base class CLbtStrategyBase

    /**
     * From CLbtStrategyBase
     * Called by Location Triggering Server Logic to issue request to 
     * start supervision
     */
    virtual void StartSupervision();

    /**
     * From CLbtStrategyBase
     * Called by Location Triggering Server Logic to issue request to
     * stop the supervision process. 
     */
    virtual void StopSupervision();

    /**
     * From CLbtStrategyBase
     * Called by Location Triggering Server Logic when settings related to 
     * trigger supervision changes
     */
    virtual void TriggeringSupervisionSettingsChanged();
    
public:     // from observer class MLbtStrategyEngineObsrvr

    /**
     * From MLbtStrategyEngineObsrvr
     * Called by Strategy Engine to report trigger fire events
     */
    void TriggerFired(CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, const TPositionInfo& aPosInfo );
    
    /**
     * From MLbtStrategyEngineObsrvr
     * Called by Strategy Engine to report triggering supervision status
     */
    void ReportTriggeringSupervisionStatus( 
                    const TLbtStrategySupervisionDynamicInfo& aDynInfo );
    
    /**
     * From MLbtStrategyEngineObsrvr
     * Called by Strategy Engine to retrieve triggering settings information
     */
    void RetrieveTriggeringSettings( 
                    TLbtStrategyTriggeringSupervisionSettings& aSettings );
    
private:

    /**
     * 2nd phase constuctor for instantiating member variables
     *
     * @param aConstructionParameters - 
     */
    void ConstructL( TAny* aConstructionParameters );
    
    /**
     * Default constructor
     */
    CLbtStrategy();
    
private:

    /**
     * Ownership of the handle to Strategy Engine is maintained in the 
     * instance of this class
     */
    CLbtStrategyEngine*     iStrategyEngine;
    
    };

#endif
