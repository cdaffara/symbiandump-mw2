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
* Description:  Encapsulates media
*
*/



#ifndef CMPXMEDIA_H
#define CMPXMEDIA_H

// INCLUDES
#include <s32strm.h>
#include <mpxcmn.h>
#include <mpxmediabase.h>

//  FORWARD DECLARATIONS

/**
*  Encapsulates arbitrary data. The ID is a unique identifier for the type
*  of content contained within this object: there may be several supported IDs.
*  Supported IDs provide a means whereby a client who receives a media object
*  can request additional media objects from the same source using another of
*  the supported IDs. Attributes are referenced by flags which are unique to
*  the type of content contained in the object (i.e ID). The media object may
*  contain the full range of attribute values or some subset, depending both
*  on the attributes requested by the client and the capabilities of the source
*  of the data.
*
*  @lib mpxcommon.lib
*/

class CMPXMedia : public CMPXMediaBase
    {
public:
    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aSupportedIds, denotes all possible content that can be provided
    *  by the source of this media object
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* NewL(const TArray<TInt>& aSupportedIds);

    /**
    * Two-phased constructor.
    * @since S60 3.2.3
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* NewL();

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aStream create object from stream
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* NewL(RReadStream& aStream);

    /**
    * Two-phased constructor. Creates new instance of CMPXMedia
    * which shares underlying data from aMedia; lifetime of
    * data is reference counted, i.e. all CMPXMedia instances which
    * reference same data must be deleted before underlying data is deleted.
    *
    * @since S60 3.2.3
    * @param aMedia create object from media
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* NewL(const CMPXMedia& aMedia);

    /**
    * Two-phased constructor. Creates object from existing data handle.
    * The data handle is used to indentify the data in a media object.
    *
    * @since S60 3.2.3
    * @param aDataHandle handle to the data
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* NewL(TInt aDataHandle);

    /**
    * Two-phased constructor. Creates new instance of CMPXMedia
    * which duplicates underlying data from aMedia.
    *
    * @since S60 3.2.3
    * @param aMedia create object from media
    * @return new instance of object
    */
    IMPORT_C static CMPXMedia* CopyL(const CMPXMedia& aMedia);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    IMPORT_C ~CMPXMedia();

    /**
    * Assignment operator.
    * It copies data handle and increase the reference count of the aMedia only.
    *
    * @since S60 3.2.3
    * @param aMedia source media
    * @return reference to updated object
    */
    IMPORT_C CMPXMedia& operator=(const CMPXMedia& aMedia);

    /**
    * Gets the total size and the currently used size of the global heap.
    *
    * @since S60 3.2.3
    * @param aTotal Total size of memory (out parameter)
    * @param aUsed Used size of memory (out parameter)
    */
    IMPORT_C static void HeapMemoryInfoL( TInt& aTotal, TInt& aUsed );

 public:
    /**
    *  Possible content IDs that denote the type of content the source of this
    *  object can supply.
    *
    *  @since S60 3.2.3
    *  @return IDs indicating type of content that can be contained within
    *   this object
    */
    IMPORT_C const TArray<TInt> SupportedIds() const;

    /**
    *  The attributes provided in this media object.
    *
    *  @since S60 3.2.3
    *  @return array of attributes indicating the attributes for the values
    *  contained in this object
    */
    IMPORT_C const TArray<TMPXAttribute> Attributes() const;

    /**
    *  Checks if this object contains the value for a given attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the specified attribute
    *  @return whether supported or not
    */
    IMPORT_C TBool IsSupported(const TMPXAttribute& aAttribute) const;

    /**
    *  The number of attribute values provided in this media object.
    *
    *  @since S60 3.2.3
    *  @return count of attribute values in this object
    */
    IMPORT_C TInt Count() const;

    /**
    *  The attribute for a specific index.
    *
    *  @since S60 3.2.3
    *  @param aIndex the index from 0 to Count()-1, covering all the values
    *  provided in this object
    *  @return the attribute corresponding to the value at the specified index
    */
    IMPORT_C const TMPXAttribute& Attribute(TInt aIndex) const;

    /**
    *  The attributes set for a specific content id.
    *
    *  @since S60 3.2.3
    *  @param aContentId content Id
    *  @return the attributes set corresponding to content id
    */
    IMPORT_C TUint AttributesSet(TInt aContentId) const;

    /**
    *  The index of a given attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the specified attribute
    *  @return the index, or KErrNotFound if not present in this object
    */
    IMPORT_C TInt Index(const TMPXAttribute& aAttribute) const;

    /**
    *  The type of data stored for the index.
    *
    *  @since S60 3.2.3
    *  @param aIndex the index whose type of value is queried
    *  @return the data type at that index
    */
    IMPORT_C TMPXAttributeType Type(TInt aIndex) const;

    /**
    *  The type of data stored for the attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose type of value is queried
    *  @return the data type of the attribute,
    *          EMPXTypeUnknown returned if the attribute does not exist
    */
    IMPORT_C TMPXAttributeType Type(const TMPXAttribute& aAttribute) const;

    /**
    * Clears all the attributes and values.
    *
    * @since S60 3.2.3
    */
    IMPORT_C void Reset();

    /**
    *  Deletes a given attribute and its value; No effect
    *  if attribute is not present.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute to delete
    */
    IMPORT_C void Delete(const TMPXAttribute& aAttribute);

     /**
    *  Deletes a given attribute and its value; No effect
    *  if attribute is not present.
    *
    *  @since S60 3.2.3
    *  @param aIndex the index from 0 to Count()-1
    */
    IMPORT_C void Delete(TInt aIndex);

    /**
    *  The value for a specific attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return value of the attribute
    */
    IMPORT_C const TDesC& ValueText(const TMPXAttribute& aAttribute) const;

    /**
     *  Add a new attribute value to this object, or modifies existing
     *  value if already present.
     *
     *  @since S60 3.2.3
     *  @param aAttribute the attribute whose value is added/modified
     *  @param aValue text value to be set
     */
    IMPORT_C void SetTextValueL(const TMPXAttribute& aAttribute,
                                 const TDesC& aValue);

    /**
     *  Merges in the attributes of the provided media.
     *
     *  @since S60 3.2.3
     *  @param aMedia Media object with attributes to merge in
     */
    IMPORT_C void MergeMediaL( const CMPXMedia& aMedia );

    /**
     * Compares if the specified media's attribute matches this one.
     *
     * @since S60 3.2.3
     * @param aMedia Media object to match
     * @param aAtt Attribute to match
     * @return ETrue if matches, else EFalse
     */
    IMPORT_C TBool Match(
        const CMPXMedia& aMedia,
        const TMPXAttribute& aAtt );

 public:

     /**
     *  The value for a specific attribute. it may return NULL
     *
     *  This method supports all T types and CMPXMediaBase classes like
     *  CMPXMedia and CMPXMediaArray.
     *
     *  This method returns NULL (ASSERT happens on UDEB image) for CBase
     *  objects other than CMPXMedia, CMPXMediaArray.
     *
     *  @since S60 3.2.3
     *  @param aAttribute the attribute whose value is queried
     *  @return value of the attribute, or NULL if not present
     *          Note: Ownership is not transferred to caller
     *  @deprecated if an error code is set for the attribue, use method:
     *           CMPXMedia::ValueTObjectL(const TMPXAttribute& aAttribute),
     *           CMPXMedia::ValueNoNewLCObjectL(const TMPXAttribute& aAttribute),
     *           CMPXMedia::ValueCObjectL(const TMPXAttribute& aAttribute).
     *
     *           For CMPXCollectionPath, use method:
     *           CMPXMedia::ValueCObjectL(const TMPXAttribute& aAttribute).
     */
     template<typename T>
     inline T* Value(const TMPXAttribute& aAttribute) const;

    /**
    *  A copy of the value for a specific attribute.
    *  If the media attribute does not exist, the default T object will be returned.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return copy of the value of the attribute
    */
    template<typename T>
    inline T ValueTObjectL(const TMPXAttribute& aAttribute) const;

    /**
    *  Copy an object out of media object.
    *  Note: C++ default contructor of class C must be public.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return new copy object of class C
    */
    template<typename C>
    inline C* ValueNoNewLCObjectL(const TMPXAttribute& aAttribute) const;

    /**
    *  Copy an object out of media object.
    *  Note: class C must implement the function NewL() and streamable.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return new copy object of class C
    */
    template<typename C>
    inline C* ValueCObjectL(const TMPXAttribute& aAttribute) const;

    /**
    *  Add a new attribute value to this object, or modifies existing
    *  value if already present.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is added/modified
    *  @param aValue attribute value of the T object.
    */
    template<typename T>
    inline void SetTObjectValueL(const TMPXAttribute& aAttribute, T aValue);

    /**
    *  Add a new attribute value to this object, or modifies existing
    *  value if already present.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is added/modified
    *  @param aValue pointer to the C object
    */
    template<typename C>
    inline void SetNoNewLCObjectL(const TMPXAttribute& aAttribute,C* aValue);

    /**
    *  Add a new attribute value to this object, or modifies existing
    *  value if already present.
    *  Uses CTypeObject::NewL( RReadStream& aStream ).
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is added/modified
    *  @param aValue pointer to the C object
    */
    template<typename C>
    inline void SetCObjectValueL(const TMPXAttribute& aAttribute,C* aValue);

    /**
    *  A representation (handle) of the data stored in this object.
    *
    *  @since S60 3.2.3
    *  @return TInt handle to the data
    */
    inline TInt Data() const;

    /**
    *  Set an error code to an attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose error code is setted.
    *  @param aError error code of the attribute.
    *  Note: if error code is set for an attribute, call to Value<T> for
    *  the attribute will return NULL. You MUST be careful to check the
    *  return value of Value<T>. The Value<T> is deprecated for this reason
    *  and you should use ValueTObject instead.
    */
    IMPORT_C void SetErrorL(const TMPXAttribute& aAttribute, TInt aError);

    /**
    *  Get the error code of an attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose error code is retrieved.
    *  @return TInt error code of the attribute, or KErrNone if no error code is set.
    */
    IMPORT_C TInt Error(const TMPXAttribute& aAttribute) const;

public:

    /**
    * Externalize media object.
    *
    * @since S60 3.2.3
    * @param aStream write stream
    */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

    /**
    * Internalize media object.
    *
    * @since S60 3.2.3
    * @param aStream read stream
    */
    IMPORT_C void InternalizeL(RReadStream& aStream);

private:

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aSupportedIds support content ids
    */
    void ConstructL(const TArray<TInt>& aSupportedIds);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aStream read stream
    */
    void ConstructL(RReadStream& aStream);

private:
    /**
    *  Set data.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @param aValue the value of the media attribute
    *  @param aType object type of the media attribute
    *
    */
    IMPORT_C void SetValueL(const TMPXAttribute& aAttribute,
                            const TDesC8& aValue,
                            TMPXAttributeType aType);

    /**
    *  The value for a specific attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return value of the attribute, or NULL if not present
    *          Note: ownerhip of returned pointer is not transferred.
    */
    IMPORT_C TAny* ValuePtr(const TMPXAttribute& aAttribute) const;
    
    
    /**
    *  Get the binary value for a specific attribute.
    *
    *  @since S60 3.2.3
    *  @param aAttribute the attribute whose value is queried
    *  @return type of the attribute or EMPXTypeUnknown 
    *          if not present
    */
    IMPORT_C TMPXAttributeType GetValue(const TMPXAttribute& aAttribute, TDes8& aValue) const;
    
private:
    /**
    * Reconstructs local attribute array.
    *
    * @since S60 3.2.3
    */
    void RefreshAttributes();

    /**
    * Frees all local data.
    *
    * @since S60 3.2.3
    */
    void ResetLocal();

    /**
    *  Copy an object out of a descriptor buffer.
    *  Note: Cbased class must implement the function NewL().
    *
    *  @since S60 3.2.3
    *  @return new copy object of class T
    */
    template<typename T>
    inline T* ValueL(const TDesC8& aData) const;

    /**
    *  Copy an object out of media object.
    *
    *  @since S60 3.2.3
    *  @return pointer to new copy of object, ownership transferred
    */
    TAny* ValueCObjectL(const TDesC8& aData) const;

private:
    RArray<TMPXAttribute> iAttributes;
    };

#include "mpxmedia.inl"

#endif // CMPXMEDIA_H
