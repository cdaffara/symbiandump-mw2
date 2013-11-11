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
* Description:  Inline functions for media object
*
*/



// -----------------------------------------------------------------------------
// The value for a specific attribute
// -----------------------------------------------------------------------------
//
template<typename T>
inline T CMPXMedia::ValueTObjectL(const TMPXAttribute& aAttribute) const
    {
    TPckgBuf<T> v;
    TMPXAttributeType type = GetValue(aAttribute, v);
    ASSERT(type == EMPXTypeTObject || type == EMPXTypeTInt || type == EMPXTypeUnknown);
    return v();// creates empty object if none exists!
    }

// -----------------------------------------------------------------------------
// The value for a specific attribute
// -----------------------------------------------------------------------------
//
template<typename C>
inline C* CMPXMedia::ValueNoNewLCObjectL(const TMPXAttribute& aAttribute) const
    {
    ASSERT(iData&&iClientHandle);
    if (Type(aAttribute)==EMPXTypeError)
        return NULL;
    TInt i=iData->Index(iClientHandle,aAttribute);
    TPtrC8 data=iData->Value(iClientHandle,i);
    C* obj=NULL;
    TInt s=data.Size();
    if (s)
        {
        CBufBase* b=CBufFlat::NewL(s);
        CleanupStack::PushL(b);
        b->ResizeL(s);
        b->Write(0,data,s);
        ::CreateFromBufferL(*b,obj);
        CleanupStack::PopAndDestroy(b);
        }
    else
        {
        // This construction is not always possible (e.g. CDesCArray)
        // obj=new(ELeave)C;
        }
    return obj;
    }

// -----------------------------------------------------------------------------
// The value for a specific attribute
// -----------------------------------------------------------------------------
//
template<typename C>
inline C* CMPXMedia::ValueCObjectL(const TMPXAttribute& aAttribute) const
    {
    ASSERT(iData&&iClientHandle);
    if (Type(aAttribute)==EMPXTypeError)
        return NULL;
    TInt i=iData->Index(iClientHandle,aAttribute);
    TPtrC8 data=iData->Value(iClientHandle,i);
    return ValueL<C>(data);
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ValueL
// -----------------------------------------------------------------------------
//
template<typename T>
inline T* CMPXMedia::ValueL(const TDesC8& aData) const
    {
    T* obj=NULL;
    TInt s=aData.Size();
    if (s)
        {
        CBufBase* b=CBufFlat::NewL(s);
        CleanupStack::PushL(b);
        b->ResizeL(s);
        b->Write(0,aData,s);
        ::NewFromBufferL<T>(*b,obj);
        CleanupStack::PopAndDestroy(b);
        }
    return obj;
    }

// -----------------------------------------------------------------------------
// The value for a specific attribute
// -----------------------------------------------------------------------------
//
template<typename T>
inline T* CMPXMedia::Value(const TMPXAttribute& aAttribute) const
    {
    if (Type(aAttribute)==EMPXTypeError)
        return NULL;
    return (T*)ValuePtr(aAttribute);
    }

// -----------------------------------------------------------------------------
//  Add a new attribute value to this object, or modifies existing
//  value if already present
// -----------------------------------------------------------------------------
//
template<typename T>
inline void CMPXMedia::SetTObjectValueL(const TMPXAttribute& aAttribute,T aValue)
    {
    TPtrC8 value((TUint8*)&aValue,sizeof(T));
    SetValueL(aAttribute,value,EMPXTypeTObject);
    }

// -----------------------------------------------------------------------------
// Add a new attribute value to this object, or modifies existing
// value if already present
// -----------------------------------------------------------------------------
//
template<typename C>
inline void CMPXMedia::SetNoNewLCObjectL(
    const TMPXAttribute& aAttribute,C* aValue)
    {
    SetCObjectValueL(aAttribute,aValue);
    }

// -----------------------------------------------------------------------------
// Add a new attribute value to this object, or modifies existing
// value if already present
// -----------------------------------------------------------------------------
//
template<typename C>
inline void CMPXMedia::SetCObjectValueL(
    const TMPXAttribute& aAttribute,C* aValue)
    {
    if (aValue)
        {
        CBufBase* b=NULL;
        ::CreateBufferL(*aValue,b);
        CleanupStack::PushL(b);
        SetValueL(aAttribute,b->Ptr(0),EMPXTypeCObject);
        CleanupStack::PopAndDestroy(b);
        }
    else
        {
        SetValueL(aAttribute,KNullDesC8,EMPXTypeCObject);
        }
    }

// -----------------------------------------------------------------------------
// CMPXMedia::Data
// -----------------------------------------------------------------------------
//
inline TInt CMPXMedia::Data() const
    {
    return static_cast<TInt>(iDataHandle);
    }

// END OF FILE
