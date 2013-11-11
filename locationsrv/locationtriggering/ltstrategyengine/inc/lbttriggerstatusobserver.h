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
* Description:  Class definition of Trigger Status Observer
*
*/



#ifndef C_LBTTRIGGERSTATUSOBSERVER_H
#define C_LBTTRIGGERSTATUSOBSERVER_H


/**
 * This is an observer class that is implemented by CLbtStrategy. This class 
 * defines a callback method used by the instance of CLbtTriggerView to notify 
 * trigger fire events when suggested by Strategy Engine. When strategy engine
 * evaluates the trigger and updates the trigger fired status, trigger view 
 * reflects these changes in the container and upon successful completion of
 * the operation on container reports the trigger fire event to CLbtStrategy
 * which inturn notifies Server Logic
 *              
 * @since Series 60 4.0
 */
class MLbtTriggerStatusObsrvr

    {
    
public:    
    
    /**
     * Reports trigger fire event to the instance of CLbtStrategy
     */
    virtual void TriggerFired( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
                                        const TPositionInfo& aPosInfo ) = 0;
    
    };

#endif  // C_LBTTRIGGERSTATUSOBSERVER_H
