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
* Description:  Collection message
*
*/

 
#ifndef TMPXCOLLECTIONMESSAGE_H
#define TMPXCOLLECTIONMESSAGE_H

#include <e32msgqueue.h>
#include <mpxmessage.h>
/**
 *  Collection message type
 */
class TMPXCollectionMessage : public TMPXMessage
    {
public:
    enum TEvent 
        { 
        ENoEvent,
        EError,  //EError must be 1. aType=TMPXCollectionServerOp, aData = error code
        EMediaChanged,// aData=the index of changed property
        EItemChanged, //aType=TMPXCollectionChangeType, 
                      //aData=id of currently opened item
        EPathChanged, //aType=TMPXCollectionPathChangeType
                      // For EMcPathChangedByOpen, aData = TMPXCollectionOpenEntry
                      // For EMcPathChangedByCollectionChange, aData not used        
        ECollectionChanged, // aData=UID of new collection
        EBroadcastEvent,    //aType=enum to event type
        EFocusChanged,  // Focused item changed, aData = index of focused item
        EAsyncOpComplete, // aType=TMPXCollectionServerOp, aData = error code
        //
        // Following enum is odd to have in a public API, however this whole
        // class is now deprecated and there will only ever be the "extended"
        // message API:
        // MMPXCollectionObserver::HandleCollectionMessageL(const CMPXMessage* aMsg)
        //
        EExtendedMessage // (Internal use) aData=MMPXData data handle
        };
public:
    /**
     *  C++ constructor.
     *
     *  @since S60 3.2.3
     *  @param aEvent event
     *  @param aType event type
     *  @param aData optional data 
     */
    inline TMPXCollectionMessage(TEvent aEvent,TInt aType = 0, TInt aData = 0);

    /**
     *  C++ constructor.
     *
     *  @since S60 3.2.3
     */
    inline TMPXCollectionMessage();
    };

#include "mpxcollectionmessage.inl"

#endif // TMPXCOLLECTIONMESSAGE_H
