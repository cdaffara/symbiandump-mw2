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
* Description:  Plugin monitor
*
*/


#ifndef CMPXPLUGINMONITOR_H
#define CMPXPLUGINMONITOR_H

#include <ecom/ecom.h>
#include <badesca.h>
#include <mpxpluginmonitorobserver.h>

/**
*  Monitors a designated plugin interface. When a plugin for that interface is
*  added or removed, if a client is interested in the event, client will be
*  notified through MMPXPluginMonitorObserver interface.
*
*  @lib mpxcommon.lib
*/
class CMPXPluginMonitor : public CActive
    {

public:
    /**
    *  Two-phased constructor.
    *
    *  @since S60 3.2.3
    *  @param aInterfaceUid The UID of the plugin interface to be monitored by
    *                       this instance of CMPXPluginMonitor
    *  @return Constructed object
    */
    IMPORT_C static CMPXPluginMonitor* NewL(const TUid& aInterfaceUid);

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C ~CMPXPluginMonitor();

    /**
    *  Add a observer.
    *
    *  @since S60 3.2.3
    *  @param aObs observer to be added
    */
    void AddObserverL(MMPXPluginMonitorObserver& aObs);

    /**
    *  Remove a observer.
    *
    *  @since S60 3.2.3
    *  @param aObs observer to be removed
    */
    void RemoveObserverL(MMPXPluginMonitorObserver& aObs);

    /**
    *  Request to notify all observers that they should unload a specified plugin
    *  because it is being updated.
    *
    *  @since S60 3.2.3
    *  @param aPluginUid plugin that is being updated
    */
    void NotifyUnload(const TUid& aPluginUid);

private:

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    *  @param aInterfaceUid The UID of the plugin interface to be monitored by
    *                       this instance of CMPXPluginMonitor
    */
    CMPXPluginMonitor(const TUid& aInterfaceUid);

    /**
    *  From CActive.
    *  Handles an active object's request completion event.
    *
    *  @since S60 3.2.3
    */
    void RunL();

    /**
    *  From CActive.
    *  Handles a leave occurring in the request completion event handler RunL().
    *
    *  @since S60 3.2.3
    *  @param aError error code
    *  @return system error code
    */
    TInt RunError(TInt aError);

    /**
    * From CActive.
    * Implements cancellation of an outstanding request.
    *
    * @since S60 3.2.3
    */
    void DoCancel();

private:
    TUid iInterfaceUid;
    RPointerArray<MMPXPluginMonitorObserver> iObservers;  // items not owned
    REComSession iECs;
    };

#endif // CMPXPLUGINMONITOR_H
