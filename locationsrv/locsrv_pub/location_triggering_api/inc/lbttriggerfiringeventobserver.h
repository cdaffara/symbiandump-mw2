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
* Description:  location triggering server client library
*
*/



#ifndef LBTTRIGGERFIRINGEVENTOBSERVER_H
#define LBTTRIGGERFIRINGEVENTOBSERVER_H


#include <lbtcommon.h>

/**
 * The class defines the interface for receiving 
 * trigger firing event. 
 *
 * This interface is used together with class 
 * CLbtTriggerFiringEventNotifier.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class MLbtTriggerFiringEventObserver
    {
public:
    /**
     * This function is called when a trigger(either a session
     * trigger or a start-up trigger) is fired.
     *
     * Note, the leave of this function is trapped and 
     * ignored in class CLbtTriggerFiringEventNotifier. 
     * 
     * @param[out] aFireInfo Contains the fired  
     * trigger's firing information.
     */
    virtual void TriggerFiredL(
        const TLbtTriggerFireInfo& aFireInfo
        ) = 0;
    };

#endif //LBTTRIGGERFIRINGEVENTOBSERVER_H
