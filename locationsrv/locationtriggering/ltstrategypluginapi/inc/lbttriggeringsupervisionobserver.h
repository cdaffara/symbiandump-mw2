/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Interface for communicating triggering events to
* the location triggering server
*
*/



#ifndef M_LBTTRIGGERINGSUPERVISIONOBSERVER_H
#define M_LBTTRIGGERINGSUPERVISIONOBSERVER_H


#include <e32base.h>
#include <lbs.h>
#include <lbtcommon.h>

#include "lbtstrategytriggeringsystemstatus.h"
#include "lbtstrategytriggeringsupervisionsettings.h"


/**
 * This is an observer class that is implemented by the location triggering 
 * server logic. This class defines the set of callback methods used by the
 * strategy plugin to inform server logic about triggering events.
 *              
 *  @since Series 60 4.0
 */
class MLbtTriggeringSupervisionObserver

    {
    
public:

   /**
    * Reports a trigger firing event
    *
    * @returns @p TReal
    */
   virtual void TriggerFiredL( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
        						const TPositionInfo& aPosInfo ) = 0;
   
   /**
    * Reports Triggering system status
    *
    * @returns @p TReal
    */
   virtual void SetTriggeringSupervisionDynamicInfo( 
   			const TLbtStrategySupervisionDynamicInfo& aStatus ) = 0;
   
   /**
    * Retrieves triggering supervision settings from server
    */
   virtual void GetTriggeringSupervisionSettings( 
 			TLbtStrategyTriggeringSupervisionSettings& aSettings ) = 0;
   		
    };

#endif // M_LBTTRIGGERINGSUPERVISIONOBSERVER_H
