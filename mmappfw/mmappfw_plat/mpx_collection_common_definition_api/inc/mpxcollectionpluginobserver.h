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
* Description:  Collection plugin observer
*
*/


#ifndef MMPXMCOLLECTIONPLUGINOBSERVER_H
#define MMPXMCOLLECTIONPLUGINOBSERVER_H


#include <mpxcollectionframeworkdefs.h>
#include <mpxmessage2.h>
#include <mpxcollectionenginecommonobserver.h>

class CMPXCollectionPath;

// CONSTANTS

// CLASS DECLARATION

/**
 *  MMPXCollectionPluginObserver class
 *
 *  Observer interface for Collection plugin
 */
NONSHARABLE_CLASS(MMPXCollectionPluginObserver) :
    public MMPXCollectionEngineCommonObserver
    {
public:

   /**
    * Handle message from plug-in
    *
    * @param aMessage, message from the plug-in
    * @deprecated Please use HandleMessage(CMPXMessage* aMsg, TInt aError)
    */
    virtual void HandleMessage(const CMPXMessage& aMsg)
        {
        HandleMessage(const_cast<CMPXMessage*>(&aMsg), KErrNone);
        }

    /**
     * Handle message from plug-in.
     * Note: make it as pure virtual when previous callback removed
     *
     * @since S60 3.2.3
     * @param aMessage message from the plug-in. Ownership not transferred.
     * @param aError error code.
     */
     virtual void HandleMessage(CMPXMessage* /*aMsg*/, TInt /*aError*/)
         {}

    /**
     * Handle opening a group, e.g. an artist.
     *
     * @since S60 3.2.3
     * @param aMedia media object returned. NULL will be returned if error
     *               Ownership not transferred.
     * @param aErr error code
     */
    virtual void HandleOpen(CMPXMedia* aMedia, TInt aErr) = 0;

    /**
     * Handle opening an item.
     *
     * @since S60 3.2.3
     * @param aPath path to the selected item, NULL will be returned if error
     *              Ownership not transferred
     * @param aErr error code
     */
    virtual void HandleOpen(CMPXCollectionPath* aPath, TInt aErr) = 0;

    /**
     * Handle opening a group, e.g. an artist.
     *
     * @since S60 3.2.3
     * @param aMedia media object returned. NULL will be returned if error.
     *        Ownership not transferred.
     * @param aPath full path to to current level. Including top level nodes as well.
     *         NULL will be returned if error. Ownership not transferred
     *         Note. The path does not contain any state info besides a list of ids.
     * @param aErr error code
     */
    virtual void HandleOpen(CMPXMedia* aMedia,
                            const CMPXCollectionPath* aPath,
                            TInt aErr) = 0;
    };

#endif      // MMPXMCOLLECTIONPLUGINOBSERVER_H
