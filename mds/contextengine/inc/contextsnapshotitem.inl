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
* Description:  Inline implementations CContextSnapshotItem.
*
*/


#ifndef CCONTEXTSNAPSHOTITEM_INL
#define CCONTEXTSNAPSHOTITEM_INL

inline CContextSnapshotItem::CContextSnapshotItem()
    : iObserver( NULL ), iHD( NULL ), iHDArray( NULL )
    {
    }

inline CContextSnapshotItem::~CContextSnapshotItem() // notice that objects are not destroyed
    {
    } 

inline CContextSnapshotItem* CContextSnapshotItem::NewL(
    MContextSnapshotObserver* aObserver, CHarvesterData* aHD )
    {
    CContextSnapshotItem* self = new (ELeave) CContextSnapshotItem();
    self->SetObserver( aObserver );
    self->SetItem( aHD );
    return self;
    }

inline CContextSnapshotItem* CContextSnapshotItem::NewL(
    MContextSnapshotObserver* aObserver, RPointerArray<CHarvesterData>* aHDArray )
    {
    CContextSnapshotItem* self = new (ELeave) CContextSnapshotItem();
    self->SetObserver( aObserver );
    self->SetItemArray( aHDArray );
    return self;
    }

inline void CContextSnapshotItem::SetObserver( MContextSnapshotObserver* aObserver )
    {
    iObserver = aObserver;
    }

inline MContextSnapshotObserver* CContextSnapshotItem::GetObserver()
    {
    return iObserver;
    }

inline void CContextSnapshotItem::SetItem( CHarvesterData* aHD )
    {
    iHD = aHD;
    iHDArray = NULL;
    }

inline CHarvesterData* CContextSnapshotItem::GetItem()
    {
    return iHD;
    }

inline void CContextSnapshotItem::SetItemArray( RPointerArray<CHarvesterData>* aHDArray )
    {
    iHDArray = aHDArray;
    iHD = NULL;
    }

inline RPointerArray<CHarvesterData>* CContextSnapshotItem::GetItemArray()
    {
    return iHDArray;
    }

#endif // CCONTEXTSNAPSHOTITEM_INL
