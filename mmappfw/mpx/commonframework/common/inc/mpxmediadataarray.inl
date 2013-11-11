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
* Description:  Implementation of array of variable size elements on global chunk
*
*/


// -----------------------------------------------------------------------------
// RMPXDataItem::RMPXDataItem
// -----------------------------------------------------------------------------
//
inline RMPXMediaDataItem::RMPXMediaDataItem()
    {
    iBufOffset = sizeof (RMPXMediaDataItem);
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItemArray::DataItem
// -----------------------------------------------------------------------------
//
inline RMPXMediaDataItem* RMPXMediaDataItemArray::MediaDataItem(
    TUint aClientHandle,
    TInt aIndex)
    {
    return static_cast<RMPXMediaDataItem*>(DataItem(aClientHandle,aIndex));
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItem::SetAttribute
// -----------------------------------------------------------------------------
//
inline void RMPXMediaDataItem::SetAttribute(
    const TMPXAttribute& aAttr,
    TMPXAttributeType aType)
    {
    iAttr=aAttr;
    iAttrType=aType;
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItem::Type
// -----------------------------------------------------------------------------
//
inline TMPXAttributeType RMPXMediaDataItem::Type() const
    {   
    return iAttrType;
    }

// -----------------------------------------------------------------------------
// RMPXMediaDataItem::Attribute
// -----------------------------------------------------------------------------
//
inline const TMPXAttribute& RMPXMediaDataItem::Attribute() const
    {   
    return iAttr;
    }

// End of file
