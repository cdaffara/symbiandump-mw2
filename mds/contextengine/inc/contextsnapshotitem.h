/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A simple class to store context observers and related
*                metadata objects.
*
*/


#ifndef CCONTEXTSNAPSHOTITEM_H
#define CCONTEXTSNAPSHOTITEM_H

#include <e32base.h>

class CHarvesterData;

/**
* A simple storage class used to store queued context observers and their objects.
*/
class CContextSnapshotItem : public CBase
    {
    public:

        /** NewL for creating new initialized CContextSnapshotItem. */
        static CContextSnapshotItem* NewL( MContextSnapshotObserver* aObserver,
            CHarvesterData* aHD );

        /** NewL for creating new initialized CContextSnapshotItem for multiple items. */
        static CContextSnapshotItem* NewL( MContextSnapshotObserver* aObserver,
            RPointerArray<CHarvesterData>* aObjects );

        /** Destructor */
        virtual ~CContextSnapshotItem();

        /** Sets observer for this item */
        void SetObserver( MContextSnapshotObserver* aObserver );

        /** Gets observer from this item */
        MContextSnapshotObserver* GetObserver();

        /** Sets item pointer for this item */
        void SetItem( CHarvesterData* aHD );

        /** Gets item pointer for this item */
        CHarvesterData* GetItem();

        /** Sets item array pointer for this item */
        void SetItemArray( RPointerArray<CHarvesterData>* aObjects );

        /** Gets item array pointer for this item */
        RPointerArray<CHarvesterData>* GetItemArray();

    private:

        /** Default constructor */
        CContextSnapshotItem();

    private: // data

        MContextSnapshotObserver* iObserver;
        CHarvesterData* iHD;
        RPointerArray<CHarvesterData>* iHDArray;
    };


#include "contextsnapshotitem.inl"


#endif // CCONTEXTSNAPSHOTITEM_H
