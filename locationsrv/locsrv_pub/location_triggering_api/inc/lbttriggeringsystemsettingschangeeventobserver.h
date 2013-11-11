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



#ifndef LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTOBSERVER_H
#define LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTOBSERVER_H


#include <lbttriggeringsystemsettings.h>

/**
 * The class defines the interface for receiving 
 * triggering system settings change event.
 *
 * This interface is used together with class 
 * CLbtTriggeringSystemSettingsChangeEventNotifier.
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class MLbtTriggeringSystemSettingsChangeEventObserver
    {
public:
    /**
     * This function is called when triggering
     * system settings are changed.
     *
     * Note, the leave of this function is trapped and 
     * ignored in class 
     * CLbtTriggeringSystemSettingsChangeEventNotifier. 
     * 
     * @param[out] aSettings Contains the new 
     * triggering system settings. 
     */
    virtual void TriggeringSystemSettingsChangedL(
        const TLbtTriggeringSystemSettings& aSettings 
       ) = 0;
    };

#endif //LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTOBSERVER_H
