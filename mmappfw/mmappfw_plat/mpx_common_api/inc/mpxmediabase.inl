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
* Description:  Inline functions for media base
*
*/



// -----------------------------------------------------------------------------
// CMPXMediaBase::TValue::TValue
// -----------------------------------------------------------------------------
//    
inline CMPXMediaBase::TValue::TValue(
    TAny* aValue,
    TInt aUid,
    TMPXAttributeType aType)
    :iValue(aValue),iUid(aUid),iType(aType)
    {
    }

// -----------------------------------------------------------------------------
// CMPXMediaBase::Count
// -----------------------------------------------------------------------------
//    
inline TInt CMPXMediaBase::Count() const
    {
    ASSERT(iClientHandle&&iData);
    return iData->Count(iClientHandle);
    } 
// END OF FILE
