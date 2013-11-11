/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCP plugin undertaker
*
*/



#ifndef MCCPPLUGINDEATHOBSERVER_H
#define MCCPPLUGINDEATHOBSERVER_H

/**
 *  Undertaker notifier used to transfer 
 *  information about plugin death
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */


class MCCPPluginDeathObserver
    {
    
public:

    /**
     * Notificatin of plugin thread "crash"
     *
     * @since S60 v3.2
     * @param aPluginUid TUid of plugin
     * @param aType Type of event
     * @param aReason Reason or error code
     */
    virtual void CCPPluginDiedEvent(TUid aPluginUid, TInt aType, TInt aReason) = 0;

    /**
     * Notificatin of plugin initialisation failure
     * Informs observer that plugin is not functional and can be removed
     *
     * @since S60 v3.2
     * @param aPluginUid TUid of plugin
     * @param aError error occurred
     */
    virtual void CCPPluginInitialisationFailed(TUid aPluginUid, TInt aError) = 0;
    
    };

#endif      // MCCPPLUGINDEATHOBSERVER_H

// End of File

