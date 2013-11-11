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
* Description:  class declaration of container trigger change observer.
*
*/
#ifndef LBTSERVERLOGICTRIGGERCHANGEOBSERVER_H_
#define LBTSERVERLOGICTRIGGERCHANGEOBSERVER_H_

/**
 * Class that needs to implement this interface to listen
 * to events that come from the sim change handler and app
 * change handler for trigger deletion and modification
 * event.
 *
 * @since S60 v4.0
 */
class MLbtServerLogicTriggerChangeObserver
    {
public: 
    /**
     * 
     */
    virtual void HandleTriggersChange( RArray<TLbtTriggerId>& aTriggerIds,
                                       RArray<TUid>& aManagerUids,
                                       TLbtTriggerEventMask aEventMask ) = 0;
    };
#endif /*LBTSERVERLOGICTRIGGERCHANGEOBSERVER_H_*/
