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
* Description:  Class definition of Strategy Engine Observer
*
*/



#ifndef C_LBTSTRATEGYENGINEOBSERVER_H
#define C_LBTSTRATEGYENGINEOBSERVER_H


#include "lbttriggerstatusobserver.h"


/**
 * This is an observer class that is implemented by CLbtStrategy class instance. 
 * This class defines the set of callback methods used by the instance of 
 * CLbtStrategyEngineto to notify trigger fire events, triggering supervision
 * status and retrieve triggering settings information.
 *              
 * @since Series 60 4.0
 */
class MLbtStrategyEngineObsrvr : public MLbtTriggerStatusObsrvr

    {
    
public:    
    
    /**
     * Reports triggering supervision status to the instance of CLbtStrategy
     */
    virtual void ReportTriggeringSupervisionStatus( 
                    const TLbtStrategySupervisionDynamicInfo& aDynInfo ) = 0;

    /**
     * Retrieves triggering supervision settings
     */
    virtual void RetrieveTriggeringSettings( 
                    TLbtStrategyTriggeringSupervisionSettings& aSettings ) = 0;
    
    };

#endif  // C_LBTSTRATEGYENGINEOBSERVER_H
