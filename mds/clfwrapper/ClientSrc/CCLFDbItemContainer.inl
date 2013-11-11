/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*
*/



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCLFDbItemContainer::AppendL
// -----------------------------------------------------------------------------
//
inline void CCLFDbItemContainer::AddL( CCLFContainerItem* aItem )
    {
    // there could be debricated items 
    iItemArray.InsertInOrderAllowRepeatsL( aItem, iArraySorter );
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::IncreaseReferenceCount
// -----------------------------------------------------------------------------
//
inline void CCLFContainerItem::IncreaseReferenceCount()
    {
    ++iReferenceCount;
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::DbItem
// -----------------------------------------------------------------------------
//
inline const CMdEObject& CCLFContainerItem::DbItem() const
    {
    return *iMdEObject;
    }

// -----------------------------------------------------------------------------
// CCLFContainerItem::SetItemId
// -----------------------------------------------------------------------------
//
inline void CCLFContainerItem::SetItemId( TCLFItemId aItemId )
    {
    iItemId = aItemId;
    }
    
//  End of File
