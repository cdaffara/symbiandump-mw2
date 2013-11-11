/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer for monitoring trigger creation and trigger firing
*
*/

#ifndef __LBTCONTEXTTRIGGEROBSERVER_H__
#define __LBTCONTEXTTRIGGEROBSERVER_H__

/*
 * This class observes the state of triggers and notifies the listening
 * class about the state of triggers. 
 *  TriggerFired    -    Notifies listener about a trigger being fired.
 *  TriggerCreatedL -    Notifies listener about successful creation of a trigger
 *  NotifyError     -    Called on unsuccessful creation of a trigger.
 */
class MLBTContextTriggerObserver
    {
    
public :
        
    /*
     * Alerts listening class about a trigger fire event,
     * giving the trigger id of fired trigger
     * 
     * param[out] aTriggerId Trigger id of fired trigger
     */        
    virtual void ContextTriggerFiredL( TLbtTriggerId aTriggerId ) = 0;
    
    /*
     * Alerts listening class about successful trigger creation
     * 
     * param[out] aTrigId Trigger id of created trigger
     * param[out] aContextName SetName of created trigger.
     * param[out] aTriggerName Name of created trigger
     */
    virtual void TriggerCreatedL( TLbtTriggerId& aTrigId , 
                                TPtrC& aContextName, 
                                TPtrC& aTriggerName ) = 0;
    
    /*
     * Notified listening class about errors during trigger creation.
     * 
     * param[out] aError Cause for failure to create a trigger.
     */
    virtual void NotifyError( TInt aError ) = 0;
    
    };

#endif // __LBTCONTEXTTRIGGEROBSERVER_H__
