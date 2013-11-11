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
* Description:  MPX subscription class
*
*/


// INCLUDES
#include <mpxlog.h>
#include "mpxmediaarray.h"
#include "mpxsubscriptiondefs.h"
#include "mpxsubscription.h"

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CMPXSubscription::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXSubscription* CMPXSubscription::NewL()
    {
    CMPXSubscription* self = static_cast<CMPXSubscription*>(CMPXMedia::NewL());
    CleanupStack::PushL(self);
    self->InitializeL();
    CleanupStack::Pop(self); 
    return self; 
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::NewL
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXSubscription* CMPXSubscription::NewL(
    CMPXMediaArray& aMediaArray)
    {
    CMPXSubscription* self = static_cast<CMPXSubscription*>(CMPXMedia::NewL());
    CleanupStack::PushL(self);
    self->InitializeL(aMediaArray);
    CleanupStack::Pop(self); 
    return self; 
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::AddItemL
// ----------------------------------------------------------------------------
//
EXPORT_C void CMPXSubscription::AddItemL(
    const CMPXSubscriptionItem& aItem)
    {
    // append the subscription item to the media array
    CMPXMediaArray* items = Value<CMPXMediaArray>(KMPXSubscriptionItems);
    User::LeaveIfNull(items);
    items->AppendL(aItem);
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::Items
// ----------------------------------------------------------------------------
//
EXPORT_C const CMPXMediaArray* CMPXSubscription::ItemsL() const
    {
    MPX_ASSERT(IsSupported(KMPXSubscriptionItems));
    CMPXMediaArray* array = Value<CMPXMediaArray>(KMPXSubscriptionItems);
    User::LeaveIfNull(array);
    return array;
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::operator==
// ----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXSubscription::operator==(
    const CMPXSubscription& aSubscription) const
    {
    TBool match(ETrue);
 
    const CMPXMediaArray* array1 = aSubscription.ItemsL();
    const CMPXMediaArray* array2 = ItemsL();

    TInt count(array1->Count());
    if (count != array2->Count())
        {
        match = EFalse;
        }
    else
        {
        for (TInt index = 0; (index < count) && match; ++index)
            {
            CMPXSubscriptionItem* subItem1( array1->AtL(index) );
            CMPXSubscriptionItem* subItem2( array2->AtL(index) );
 
            TInt attrCount(subItem1->Count());
            if (attrCount != subItem2->Count())
                {
                match = EFalse;
                break;
                }
 
            for (TInt attrIndex = 0; attrIndex < attrCount; ++attrIndex) 
                {
                const TMPXAttribute& attr( subItem1->Attribute( attrIndex ));
                if ( !subItem1->Match( *subItem2, attr ) )
                    {
                    match = EFalse;
                    break;
                    }
                }
            }
        }
 
    return match; 
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::InitializeL
// ----------------------------------------------------------------------------
//
void CMPXSubscription::InitializeL()
    {
    // create an empty subscription item array
    CMPXMediaArray* dummy = CMPXMediaArray::NewL();
    CleanupStack::PushL(dummy); 
    SetCObjectValueL(KMPXSubscriptionItems, dummy); 
    CleanupStack::PopAndDestroy(dummy);
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::InitializeL
// ----------------------------------------------------------------------------
//
void CMPXSubscription::InitializeL(
    CMPXMediaArray& aMediaArray)
    {
    SetCObjectValueL(KMPXSubscriptionItems, &aMediaArray); 
    }

// ----------------------------------------------------------------------------
// CMPXSubscription::DebugPrint
// ----------------------------------------------------------------------------
//
void CMPXSubscription::DebugPrint() const
    {
    const CMPXMediaArray* array(NULL);
    TRAPD(err,  array = ItemsL());
    if (KErrNone == err)
        {
        MPX_ASSERT(array);
        TInt count(array->Count());
        for (TInt index = 0; index < count; ++index)
            {
            CMPXSubscriptionItem* subItem = (*array)[index];
            if (subItem)
                {
                TInt attrCount(subItem->Count());
 
                for (TInt attrIndex = 0; attrIndex < attrCount; ++attrIndex)
                    {
                    const TMPXAttribute& attr = subItem->Attribute(attrIndex);
                    MPX_DEBUG3("Attr: %x, %x", attr.ContentId(), attr.AttributeId());
                    }
                }
            else
                {
                break;
                }
            }
        }
    }

// End of File
