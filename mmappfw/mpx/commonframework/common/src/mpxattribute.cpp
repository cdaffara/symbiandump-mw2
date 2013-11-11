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
* Description:  mpx attribute class
*
*/

#include "mpxattribute.h"
// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// Overloadded operator
// ----------------------------------------------------------------------------
//
EXPORT_C TBool TMPXAttributeData::operator& (
    const TMPXAttributeData& aData) const
    {
    ASSERT(iContentId == aData.iContentId);
    return iAttributeId & aData.iAttributeId;
    }

// ----------------------------------------------------------------------------
// Overloadded operator
// ----------------------------------------------------------------------------
//
EXPORT_C TBool TMPXAttributeData::operator& (TUint& aData) const
    {
    return iAttributeId & aData;
    }
// ----------------------------------------------------------------------------
// Overloadded operator
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttributeData TMPXAttributeData::operator| (
    const TMPXAttributeData& aData) const
    {
    ASSERT(iContentId == aData.iContentId);
    TMPXAttributeData ret={iContentId, iAttributeId | aData.iAttributeId};
    return ret;
    }

// ----------------------------------------------------------------------------
// C++ default constructor
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttribute::TMPXAttribute()
    {
    iData.iContentId=0;
    iData.iAttributeId=0;
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttribute::TMPXAttribute(TInt aContentId, TUint aAttributeId)
    {
    iData.iContentId=aContentId;
    iData.iAttributeId=aAttributeId;
    }

// ----------------------------------------------------------------------------
// Copy constructor
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttribute::TMPXAttribute(const TMPXAttribute& aId)
    {
    iData.iContentId=aId.ContentId();
    iData.iAttributeId=aId.AttributeId();
    }

// ----------------------------------------------------------------------------
// Copy constructor
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttribute::TMPXAttribute(const TMPXAttributeData& aData)
:   iData(aData)
    {
    }

// ----------------------------------------------------------------------------
// Overloaded assignment operator
// ----------------------------------------------------------------------------
//
EXPORT_C TMPXAttribute& TMPXAttribute::operator=(const TMPXAttribute& aId)
    {
    if (this != &aId)
        {
        iData.iContentId = aId.ContentId();
        iData.iAttributeId = aId.AttributeId();
        }
    return *this;
    }

// ----------------------------------------------------------------------------
// Overloaded equal operator
// ----------------------------------------------------------------------------
//
EXPORT_C TBool TMPXAttribute::operator==(const TMPXAttribute& aId) const
    {
    TBool ret = EFalse;
    if (iData.iAttributeId == aId.iData.iAttributeId && 
        iData.iContentId == aId.iData.iContentId)
        {
        ret = ETrue;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// Externalize
// -----------------------------------------------------------------------------
//
EXPORT_C void TMPXAttribute::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteInt32L(iData.iContentId);
    aStream.WriteUint32L(iData.iAttributeId);
    }

// -----------------------------------------------------------------------------
// Internalize
// -----------------------------------------------------------------------------
//
EXPORT_C void TMPXAttribute::InternalizeL(RReadStream& aStream)
    {
    iData.iContentId=aStream.ReadInt32L();
    iData.iAttributeId=aStream.ReadUint32L();
    }

// ----------------------------------------------------------------------------
// Return content id
// ----------------------------------------------------------------------------
//
EXPORT_C TInt TMPXAttribute::ContentId() const
    {
    return iData.iContentId;
    }

// ----------------------------------------------------------------------------
// Return attribute id
// ----------------------------------------------------------------------------
//
EXPORT_C TUint TMPXAttribute::AttributeId() const
    {
    return iData.iAttributeId;
    }
    
// -----------------------------------------------------------------------------
// Helper function to do match in the RArray of the objects of this class
// -----------------------------------------------------------------------------
//
EXPORT_C TBool TMPXAttribute::Match(
    const TMPXAttribute& aFirst, 
    const TMPXAttribute& aSecond)
    {
    return aFirst==aSecond;
    }

// -----------------------------------------------------------------------------
// Helper function to do match in the RArray of the objects of this class by
// content id
// -----------------------------------------------------------------------------
//
EXPORT_C TBool TMPXAttribute::MatchContentId(
    const TMPXAttribute& aFirst, 
    const TMPXAttribute& aSecond)
    {
    return aFirst.ContentId()==aSecond.ContentId();
    }

// End of File
