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
* Description:  Implementation of media
*
*/


#include <mpxlog.h>
#include <mpxuser.h>
#include "mpxmedia.h"
#include "mpxmediaarray.h"
#include "mpxcollectionpath.h"

// -----------------------------------------------------------------------------
// CMPXMedia::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::NewL()
    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->CMPXMediaBase::ConstructL();
    CleanupStack::Pop(m);
    return m;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::NewL(const TArray<TInt>& aSupportedIds)
    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->ConstructL(aSupportedIds);
    CleanupStack::Pop(m);
    return m;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::NewL(RReadStream& aStream)
    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->ConstructL(aStream);
    CleanupStack::Pop(m);
    return m;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::NewL(TInt aDataHandle)

    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->CMPXMediaBase::ConstructL(aDataHandle);
    CleanupStack::Pop(m);
    return m;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::NewL(const CMPXMedia& aMedia)
    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->CMPXMediaBase::ConstructL(aMedia);
    CleanupStack::Pop(m);
    return m;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::CopyL
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia* CMPXMedia::CopyL(const CMPXMedia& aMedia)
    {
    CMPXMedia* m=new(ELeave)CMPXMedia();
    CleanupStack::PushL(m);
    m->CopyConstructL(aMedia);
    CleanupStack::Pop(m);
    return m;
    }

// ----------------------------------------------------------------------------
// Overloaded assignment operator
// ----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia& CMPXMedia::operator=(const CMPXMedia& aMedia)
    {
//    MPX_FUNC_EX("CMPXMedia::operator=()");
    MPX_ASSERT(aMedia.iData&&aMedia.iDataHandle);
    MPX_ASSERT(iClientHandle);
    //
    if (this!=&aMedia)
        {
        Clear();
        iDataHandle=aMedia.iDataHandle;
        iData=aMedia.iData;
        iData->AddRef(iClientHandle);
        } // otherwise assign itself
    return *this;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CMPXMedia::~CMPXMedia()
    {
    iAttributes.Close();
    }

// -----------------------------------------------------------------------------
// CMPXMedia::HeapMemoryInfoL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::HeapMemoryInfoL( TInt& aTotal, TInt& aUsed )
    {
    return MMPXData::HeapMemoryInfoL( aTotal, aUsed);
    }

// -----------------------------------------------------------------------------
// Externalize
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::ExternalizeL(RWriteStream& aStream) const
    {
    DoExternalizeL(aStream,MMPXData::EMedia);
    }

// -----------------------------------------------------------------------------
// Internalize
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::InternalizeL(RReadStream& aStream)
    {
    DoInternalizeL(aStream,MMPXData::EMedia);
    }

// -----------------------------------------------------------------------------
// Clears data
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::Reset()
    {
//    MPX_FUNC_EX("CMPXMedia::Reset()");
    MPX_ASSERT(iClientHandle&&iData);
    //
    ResetLocal(); // Local data
    iData->Reset(iClientHandle); // Clears heap data for this object
    }

// -----------------------------------------------------------------------------
// Possible content IDs that denote the type of content the source of this
// object can supply;
// -----------------------------------------------------------------------------
//
EXPORT_C const TArray<TInt> CMPXMedia::SupportedIds() const
    {
    MPX_ASSERT(iClientHandle&&iData);
    return iData->SupportedIds(iClientHandle);
    }

// -----------------------------------------------------------------------------
// The attributes provided in this media object
// -----------------------------------------------------------------------------
//
EXPORT_C const TArray<TMPXAttribute> CMPXMedia::Attributes() const
    {
    const_cast<CMPXMedia*>(this)->RefreshAttributes();
    return iAttributes.Array();
    }

// -----------------------------------------------------------------------------
// Does this object contain the value for a given attribute
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXMedia::IsSupported(const TMPXAttribute& aAttribute) const
    {
    MPX_ASSERT(iClientHandle&&iData);
    return KErrNotFound!=iData->Index(iClientHandle,aAttribute);
    }

// -----------------------------------------------------------------------------
// The number of attribute values provided in this media object
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMedia::Count() const
    {
    return CMPXMediaBase::Count();
    }

// -----------------------------------------------------------------------------
// The attribute for a specific index
// -----------------------------------------------------------------------------
//
EXPORT_C const TMPXAttribute& CMPXMedia::Attribute(TInt aIndex) const
    {
    MPX_ASSERT(iClientHandle&&iData);
    MPX_ASSERT(aIndex>=0&&aIndex<Count());
    //
    return iData->Attribute(iClientHandle,aIndex);
    }

// -----------------------------------------------------------------------------
// The attributes set for a specific content
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CMPXMedia::AttributesSet(TInt aContentId) const
    {
    const_cast<CMPXMedia*>(this)->RefreshAttributes();
    TUint attributesSet(0);
    for (TInt i=iAttributes.Count();--i>=0;)
        {
        TMPXAttribute attr(iAttributes[i]);
        if (attr.ContentId()==aContentId)
            {
            attributesSet|=attr.AttributeId();
            }
        }
    return attributesSet;
    }

// -----------------------------------------------------------------------------
// The index of a given attribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMPXMedia::Index(const TMPXAttribute& aAttribute) const
    {
    MPX_ASSERT(iClientHandle&&iData);
    return iData->Index(iClientHandle,aAttribute);
    }

// -----------------------------------------------------------------------------
// The type of data stored for the attribute
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXAttributeType CMPXMedia::Type(TInt aIndex) const
    {
    MPX_ASSERT(iClientHandle&&iData);
    MPX_ASSERT(aIndex>=0&&aIndex<Count());
    //
    return iData->AttributeType(iClientHandle,aIndex);
    }

// -----------------------------------------------------------------------------
// The type of data stored for the attribute
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXAttributeType CMPXMedia::Type(const TMPXAttribute& aAttribute) const
    {
    MPX_ASSERT(iClientHandle&&iData);
    TMPXAttributeType type(EMPXTypeUnknown);
    TInt index(Index(aAttribute));
    if (KErrNotFound!=index)
        {
        type = Type(index);
        }
    return type;
    }

// -----------------------------------------------------------------------------
// The value for a specific attribute
// -----------------------------------------------------------------------------
//
EXPORT_C const TDesC& CMPXMedia::ValueText(const TMPXAttribute& aAttribute) const
    {
    TPtrC* text=Value<TPtrC>(aAttribute);
    return text?(const TDesC&)*text:KNullDesC;
    }

// -----------------------------------------------------------------------------
// Add a new attribute value to this object, or modifies existing
// value if already present
// -----------------------------------------------------------------------------
//
EXPORT_C  void CMPXMedia::SetTextValueL(
    const TMPXAttribute& aAttribute,
    const TDesC& aValue)
    {
    TPtrC8 data((TUint8*)aValue.Ptr(),aValue.Size());
    SetValueL(aAttribute,data,EMPXTypeText);
    }

// -----------------------------------------------------------------------------
// Merges in the attributes of the provided media
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::MergeMediaL( const CMPXMedia& aMedia )
    {
    // Do not merge the same data handle
    if( Data() != aMedia.Data() )
        {
        for ( TInt i = 0; i < aMedia.Count(); i++ )
            {
            TMPXAttributeType type( aMedia.Type( i ));
            TMPXAttribute att( aMedia.Attribute( i ));
            // Need to handle text differently since it's
            // possible to have KNullDesC, which will result
            // in an invalid TPtrC.
            if ( EMPXTypeText == type )
                {
                const TDesC& t( aMedia.ValueText( att ));
                if ( t == KNullDesC )
                    {
                    // Cannot use reference to KNullDesC, must
                    // use the literal directly.
                    SetTextValueL( att, KNullDesC );
                    }
                else
                    {
                    SetTextValueL( att, t );
                    }
                }
            else
                {
                TPtrC8 ptr( aMedia.iData->Value( aMedia.iClientHandle, i ));
                SetValueL( att, ptr, type );
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// Compares if the specified media's attribute matches this one
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMPXMedia::Match(
    const CMPXMedia& aMedia,
    const TMPXAttribute& aAtt )
    {
    TBool result( EFalse );
    TInt myIndex( Index( aAtt ));
    if ( KErrNotFound != myIndex )
        {
        TInt index( aMedia.Index( aAtt ));
        if ( KErrNotFound != index )
            {
            if ( Type( myIndex ) == aMedia.Type( index ))
                {
                TPtrC8 myPtr( iData->Value( iClientHandle, myIndex ));
                TPtrC8 ptr( aMedia.iData->Value( aMedia.iClientHandle, index ));
                result = ( myPtr == ptr );
                }
            }
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::Delete
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::Delete(const TMPXAttribute& aAttribute)
    {
//    MPX_FUNC_EX("CMPXMedia::Delete(const TMPXAttribute& aAttribute)");
    Delete(iData->Index(iClientHandle,aAttribute));
    }

// -----------------------------------------------------------------------------
// CMPXMedia::Delete
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::Delete(TInt aIndex)
    {
//    MPX_FUNC_EX("CMPXMedia::Delete(TInt aIndex)");
    if (KErrNotFound != aIndex)
        {
        MPX_ASSERT(aIndex>=0&&aIndex<Count());
        DeleteLocal(aIndex);
        iData->Delete(iClientHandle,aIndex);
        } // do nothing. the same behaviour as the v1 although it should panic
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMedia::ConstructL(const TArray<TInt>& aSupportedIds)
    {
    CMPXMediaBase::ConstructL();
    User::LeaveIfError(iData->SetSupportedIds(iClientHandle,aSupportedIds));
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ConstructL
// -----------------------------------------------------------------------------
//
void CMPXMedia::ConstructL(RReadStream& aStream)
    {
    MPX_ASSERT(!iData&&!iDataHandle);
    InternalizeL(aStream);
    }

// -----------------------------------------------------------------------------
// CMPXMedia::SetValueL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMPXMedia::SetValueL(
    const TMPXAttribute& aAttribute,
    const TDesC8& aValue,
    TMPXAttributeType aType)
    {
    MPX_ASSERT(iData&&iClientHandle);
    //
    LockHeapLC();
    TInt i=iData->Index(iClientHandle,aAttribute);
    if (i==KErrNotFound)
        {
        User::LeaveIfError(iData->Append(iClientHandle,aAttribute,aType,aValue));
        }
     else
        {
        DeleteLocal(i);
        iData->Set(iClientHandle,aAttribute,aType,aValue,i);
        }
    CleanupStack::PopAndDestroy(); //unlock heap
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ValuePtr. Returns NULL if (a) there is no such object with the
// given attribute; (b) object cannot be created (e.g. no memory); (c) it's an
// arbitrary CBase object (only CMPXMedia, CMPXMediaArray, and CMPXCollectionPath
// are supported. Otherwise, returns a pointer to the object which is also stored
// locally
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* CMPXMedia::ValuePtr(const TMPXAttribute& aAttribute) const
    {
    MPX_ASSERT(iData&&iClientHandle);
    MMPXData::LockHeap( iClientHandle );
    TAny* obj=NULL;
    TInt index=iData->Index(iClientHandle,aAttribute);
    TMPXAttributeType type=EMPXTypeUnknown;

    if (index!=KErrNotFound) // Check if the value exist or not
        {
        type=iData->AttributeType(iClientHandle,index);
        // Check local values to see if there and current
        //
        TInt uid=iData->Uid(iClientHandle,index);
        const TValue* v=LocalValue(uid);
        if (!v)
            {
            TPtrC8 data=iData->Value(iClientHandle,index);
            TInt size=data.Size();
            const TAny* ptr=data.Ptr();
            //
            switch(type)
                {
            case EMPXTypeText:
                obj=new TPtrC((TUint16*)ptr,size/2);
                break;
                //
            case EMPXTypeCObject:
                TRAP_IGNORE(obj=ValueCObjectL(data));
                break;
                //
            case EMPXTypeError: // Error type is TInt, flow down
            case EMPXTypeTObject:
                //
                // Create a decriptor pointing to the data. But the pointer
                // within the descriptor must be returned by this method
                //
                 obj=new TPtrC8((TUint8*)ptr,size);
                break;
                //
            default:
                MPX_ASSERT(0);
                break;
                }
            if (obj)
                {
                CMPXMedia& theMedia=*const_cast<CMPXMedia*>(this);
                TInt err=theMedia.SetLocal(TValue(obj,uid,type)); // Add new one
                if (err!=KErrNone)
                    {
                    theMedia.DeletePtr(obj,type);
                    obj=NULL;
                    }
                }
            }
        else
            {
            obj=v->iValue;
            }
        }
    MMPXData::UnlockHeap(iClientHandle);
    return (type==EMPXTypeTObject||type==EMPXTypeError)?(obj?(TAny*)static_cast<TPtrC8*>(obj)->Ptr():NULL):obj;
    }

EXPORT_C TMPXAttributeType CMPXMedia::GetValue(const TMPXAttribute& aAttribute, TDes8& aValue) const
    {
    MPX_ASSERT(iData&&iClientHandle);
    MMPXData::LockHeap( iClientHandle );
    TInt index=iData->Index(iClientHandle,aAttribute);
    TMPXAttributeType type=EMPXTypeUnknown;
    
    if (index!=KErrNotFound) // Check if the value exist or not
        {
        type=iData->AttributeType(iClientHandle,index);
        aValue = iData->Value(iClientHandle,index).Left(aValue.MaxLength());
        }
    MMPXData::UnlockHeap(iClientHandle);
    return type;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::RefreshAttributes
// -----------------------------------------------------------------------------
//
void CMPXMedia::RefreshAttributes()
    {
    // Reconstructs array of attributes
    //
    iAttributes.Reset();
    for (TInt i=iData->Count(iClientHandle);--i>=0;)
        {
        (void)iAttributes.Append(iData->Attribute(iClientHandle,i));
        }
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ResetLocal
// -----------------------------------------------------------------------------
//
void CMPXMedia::ResetLocal()
    {
    CMPXMediaBase::ResetLocal();
    iAttributes.Reset();
    }

// -----------------------------------------------------------------------------
// CMPXMedia::ValueCObjectL
// -----------------------------------------------------------------------------
//
TAny* CMPXMedia::ValueCObjectL(const TDesC8& aData) const
    {
    TAny* obj=NULL;
    TInt size=aData.Size();
    TUint* ptr=(TUint*)aData.Ptr();

#ifdef _DEBUG
    const TBool ptrAddrIsOnBoundary = ((TUint)ptr & 3) == 0;

    // Only dereference 4-byte word on 4-byte word boundary
    MPX_ASSERT(ptrAddrIsOnBoundary); // assert if this is not the case
#endif

    MMPXData::TMPXObjectType t=*(MMPXData::TMPXObjectType*)ptr;
    switch(t)
        {
        case MMPXData::EMedia:
            MPX_ASSERT(size==sizeof(TInt)+sizeof(TUint)); // i.e 8 bytes
            obj=CMPXMedia::NewL(*++ptr);
            break;
            //
        case MMPXData::EMediaArray:
            MPX_ASSERT(size==sizeof(TInt)+sizeof(TUint)); // i.e 8 bytes
            obj=ValueL<CMPXMediaArray>(aData);
            break;
            //
        case MMPXData::EPath:
            obj=ValueL<CMPXCollectionPath>(aData);
            break;
            //
        default:
            //
            // obj=NULL, i.e. arbitrary C objects
            // ARE NOT SUPPORTED by this method
            //
            MPX_ASSERT(0); // For now, to catch clients that use this
            break;
            }
        return obj;
    }

// -----------------------------------------------------------------------------
// CMPXMedia::SetErrorL
// -----------------------------------------------------------------------------
EXPORT_C void CMPXMedia::SetErrorL(const TMPXAttribute& aAttribute, TInt aError)
    {
    TPtrC8 value((TUint8*)&aError,sizeof(TInt));
    SetValueL(aAttribute,value,EMPXTypeError);
    }

// -----------------------------------------------------------------------------
// CMPXMedia::Error
// -----------------------------------------------------------------------------
EXPORT_C TInt CMPXMedia::Error(const TMPXAttribute& aAttribute) const
    {
    if (Type(aAttribute)==EMPXTypeError)
        {
        TInt* v=(TInt*)ValuePtr(aAttribute);
        return v?*v:KErrNone; // return KErrNone if none exists!
        }
    else
        return KErrNone;
    }

// END OF FILE
