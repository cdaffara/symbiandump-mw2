/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Encapsulates message subscriptions.
*
*/



#ifndef MPXSUBSCRIPTION_H
#define MPXSUBSCRIPTION_H

// INCLUDES
#include <mpxmedia.h>
#include <mpxmediaarray.h>

/**
* MPX subscription item class.
*
* #defined to CMPXMedia for now.
*/
typedef CMPXMedia CMPXSubscriptionItem;

/**
* A subscription instance includes one or more subscription items.
* An MPX client can have one or more subscriptions.
*
* @lib MPXCommon.lib
*/
class CMPXSubscription :
    public CMPXMedia
    {
    public:
        /**
        * Two phased constructor.
        *
        * @since S60 3.2.3
        * @return New CMPXSubscription instance.
        */
        IMPORT_C static CMPXSubscription* NewL();
 
        /**
        * Two phased constructor.
        *
        * @since S60 3.2.3
        * @param aMediaArray Subscription item array to initialize with.
        * @return New CMPXSubscription instance.
        */
        IMPORT_C static CMPXSubscription* NewL(CMPXMediaArray& aMediaArray); 
 
        /**
        * Adds a new subscription item to the subscription.
        *
        * @since S60 3.2.3
        * @param aItem Subscription item to add.
        */
        IMPORT_C void AddItemL(const CMPXSubscriptionItem& aItem);

        /**
        * Returns the subscription item array.
        * 
        * @since S60 3.2.3
        * @leave KErrNoMemory if out of memory
        * @return subsription items array
        */
        IMPORT_C const CMPXMediaArray* ItemsL() const;
 
        /**
        * Comparison operator.
        *
        * @since S60 3.2.3
        * @param aSubscription subscription to compare with
        */
        IMPORT_C TBool operator==(const CMPXSubscription& aSubscription) const;
 
        /**
        * Prints the subscription.
        *
        * @since S60 3.2.3
        */
        void DebugPrint() const;

    private: 
        /**
        * Third phase constructor.
        *
        * @since S60 3.2.3
        */
        void InitializeL(); 

        /**
        * Third phase constructor.
        *
        * @since S60 3.2.3
        * @param aMediaArray Subscription item array to initialize with.
        */
        void InitializeL(CMPXMediaArray& aMediaArray);
    };

#endif // MPXSUBSCRIPTION_H
