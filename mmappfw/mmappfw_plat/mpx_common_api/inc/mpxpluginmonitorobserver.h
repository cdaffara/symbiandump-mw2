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
* Description:  plugin monitor observer
*
*/


#ifndef MMPXPLUGINMONITOROBSERVER_H
#define MMPXPLUGINMONITOROBSERVER_H

/**
*  Observer interface for a plugin monitor.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MMPXPluginMonitorObserver)
    {
    public:

        /**
        *  Callback whenever there is a change in loaded plugins. This is
        *  called from CMPXPluginMonitor::RunL and should not be a long
        *  running task.
        *
        *  @since S60 3.2.3
        */
        virtual void PluginsChangedL() = 0;

        /**
        *  Indicates that a plugin should be unloaded if it is loaded by the observer.
        *  This call is made when a plugin update is detected by a plugin handler and
        *  after all old plugin instances are unloaded the plugin handler can load the
        *  new plugin version. All plugin handlers should maintain the reference count
        *  for the plugin, as they will get a PluginsChangedL notification following one
        *  or multiple HandlePluginUnload calls and they will need to know what plugins
        *  were loaded before the update so they can be reloaded.
        *
        *  @since S60 3.2.3
        */
        virtual void HandlePluginUnload(const TUid& aPluginUid) = 0;
    };

#endif      // MMPXPLUGINMONITOROBSERVER_H


