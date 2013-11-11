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



#ifndef LBTTRIGGERCHANGEEVENTOBSERVER_H
#define LBTTRIGGERCHANGEEVENTOBSERVER_H


#include <lbttriggerchangeevent.h>

/**
 * The class defines the interface for receiving 
 * trigger change event.
 *
 * This interface is used together with class 
 * CLbtTriggerChangeEventNotifier.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class MLbtTriggerChangeEventObserver
    {
public:
    /**
     * This function is called when one or more triggers
     * are changed.
     *
     * Note, the leave of this function is trapped and 
     * ignored in class CLbtTriggerChangeEventNotifier. 
     * 
     * @param[out] aEvent Will contain the information when 
     * a trigger change event occurs.
     */
    virtual void TriggerChangedL(
        const TLbtTriggerChangeEvent& aEvent) = 0;
    };

#endif //LBTTRIGGERCHANGEEVENTOBSERVER_H
