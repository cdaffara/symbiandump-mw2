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
* Description:  location triggering server client interface
*
*/



#ifndef LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTNOTIFIER_H
#define LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTNOTIFIER_H

#include <lbt.h>
#include <lbttriggeringsystemsettingschangeeventobserver.h>

/**
 * Helper class to receive triggering system settings change event 
 * through observer interface. 
 *
 * @see RLbt::NotifyTriggeringSystemSettingChange
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */
class CLbtTriggeringSystemSettingsChangeEventNotifier : public CActive
    {
public:
    /**
     * Construct a triggering system settings change event notifier object. 
     *
     * When the object is constructed. The triggering
     * system settings change  notification request is not 
     * issued to the Location Triggering Server. Client 
     * shall call 
     * CLbtTriggeringSystemSettingsChangeEventNotifier::Start() 
     * to start notification.
     *
     * @param[in] aLbt A referece to RLbt object. The subsession 
     * must be opened. Otherwise a panic is raised.
     * @param[in] aObserver A referece to the 
     * observer object to receive triggering system setting 
     * change event.
     * @param[in] aPriority An integer specifying the 
     * priority of this active object. CActive::TPriority 
     * defines a standard set of priorities. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the 
     * subsession of aLbt is not opened.
     */
     IMPORT_C static CLbtTriggeringSystemSettingsChangeEventNotifier* NewL(
        RLbt& aLbt,
        MLbtTriggeringSystemSettingsChangeEventObserver& aObserver,
        TInt aPriority = CActive::EPriorityStandard);

    /**
     * Destructor. 
     *
     * If the notification is started, the 
     * destructor will cancel the notification.
     */
    IMPORT_C ~CLbtTriggeringSystemSettingsChangeEventNotifier();

    /**
     * Start triggering system settings change event notification. 
     *
     * After this function is called, when triggering 
     * system settings are changed, the
     * client will be notified from the observer interface.
     * 
     * Client shall call Cancel() function to stop the
     * triggering system settings change event notification.
     */
    IMPORT_C void Start();

private:
    //Derived from CActive
    void RunL();
    TInt RunError( TInt aError );
    void DoCancel();

private:
    /**
     * Default constructor
     */
    CLbtTriggeringSystemSettingsChangeEventNotifier(
        RLbt& aLbt,
        MLbtTriggeringSystemSettingsChangeEventObserver& aObserver,
        TInt aPriority = CActive::EPriorityStandard );

    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();

private:
    /**
     * Triggering system setting.
     */
    TLbtTriggeringSystemSettings iSettings;
    
    /**
     * Reference to Lbt subsession.
     */
    RLbt& iLbt;
    
    /**
     * Reference to triggering system settings change observer,
     */
    MLbtTriggeringSystemSettingsChangeEventObserver& iObserver; 
    };

#endif //LBTTRIGGERINGSYSTEMSETTINGSCHANGEEVENTNOTIFIER_H
