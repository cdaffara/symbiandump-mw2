/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer interface for the plugin handler.
*
*/


#ifndef MPXPLUGINHANDLEROBSERVER_H
#define MPXPLUGINHANDLEROBSERVER_H

// INCLUDES
#include <e32std.h>

/**
*  Mix-in class for callback.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXPluginHandlerObserver)
    {
public:
    enum TPluginHandlerEvents
        {
        EPluginAdd,             // generated after the plugin DLL is copied
        EPluginUpdateStart,     // generated before the plugin DLL is unloaded (if already loaded)
        EPluginUpdateEnd,       // generated after the new plugin version is loaded (if old version was loaded)
        EPluginRemove           // generated after the plugin DLL is removed but before the plugin instance is unloaded
        };

public:
    /**
    * Indicates a change in the plugins managed by the handler.
    *
    * @since S60 3.2.3
    * @param aEvent event code
    * @param aPluginUid plugin this event is related to
    * @param aLoaded ETrue if the plugin is loaded, EFalse otherwise
    * @param aData data associated with the event, its meaning depends on the event:
    *          EPluginAdd - new plugin version
    *          EPluginUpdateStart - new plugin version
    *          EPluginUpdateEnd - new plugin version
    *          EPluginRemove - old plugin version
    */
    virtual void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent, const TUid& aPluginUid,
        TBool aLoaded, TInt aData) = 0;
    };

#endif // MPXPLUGINHANDLEROBSERVER_H
