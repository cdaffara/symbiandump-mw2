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
* Description:  Observer class for Publish and Subscribe handling
*
*/


#ifndef M_MPXPSKEYOBSERVER_H
#define M_MPXPSKEYOBSERVER_H

/**
 *  Abstract interface to the PS key watcher.
 *
 *  @lib mpxcommon.lib
 */
NONSHARABLE_CLASS(MMPXPSKeyObserver)
    {
public:

    /**
    * Handle PS event
    *
    * @since S60 3.2.3
    * @param aUid The UID that identifies the property category
    * @param aKey The property sub-key
    */
    virtual void HandlePSEvent(TUid aUid, TInt aKey) = 0;
    };


#endif // M_MPXPSKEYOBSERVER_H
