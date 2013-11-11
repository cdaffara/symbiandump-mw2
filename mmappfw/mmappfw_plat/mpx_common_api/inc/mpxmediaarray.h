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
* Description:  Encapsulates array of media objects
*
*/



#ifndef CMPXMEDIAARRAY_H
#define CMPXMEDIAARRAY_H


#include "mpxmediabase.h"

class CMPXMedia;

/**
*  Array of media objects.
*
*  @lib mpxcommon.lib
*/
class CMPXMediaArray : public CMPXMediaBase
    {
public:

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @return new instance of object
    */
    IMPORT_C static CMPXMediaArray* NewL();

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aArray array to copy
    * @return new instance of object
    */
    IMPORT_C static CMPXMediaArray* NewL(const CMPXMediaArray& aArray);

    /**
    *  Destructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C ~CMPXMediaArray();

    /**
    *  Total number of objects contained in array.
    *
    *  @since S60 3.2.3
    *  @return number of contained media objects
    */
    IMPORT_C TInt Count() const;

    /**
    *  Array assessor.
    *  This method returns NULL if local heap is out of memory. Clients should
    *  check returned pointer before using it.
    *
    *  @since S60 3.2.3
    *  @param aIndex index into the array
    *  @return media object at index, ownership not transferred
    */
    IMPORT_C CMPXMedia* operator[](TInt aIndex) const;

    /**
    *  Get a pointer to the media object at the specified position within the array.
    *
    *  @since S60 3.2.3
    *  @param aIndex index into the array
    *  @leave KErrNoMemory if local heap is out of memory or other system error.
    *  @assert User 0 is the aIndex is out of bound
    *  @return media object at index, ownership not transferred
    */
    IMPORT_C CMPXMedia* AtL(TInt aIndex) const;

    /**
    *  Appends media object to array.
    *  Note: Ownership of aMedia transferred.
    *
    *  @since S60 3.2.3
    *  @param aMedia object to add to the array
    */
    IMPORT_C void AppendL(const CMPXMedia* aMedia);

    /**
    *  Appends media object to array.
    *  A new media object will be copied from aMedia. The new media object
    *  created on global heap and added into the array.
    *
    *  @since S60 3.2.3
    *  @param aMedia object to add to the array
    */
    IMPORT_C void AppendL(const CMPXMedia& aMedia);

    /**
    *  Resets the media array.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C void Reset();

    /**
    * Removes the object pointer at the specified
    * position from the array and deletes the
    * object whose pointer is removed.
    *
    * @since S60 3.2.3
    * @param aIndex The position within the array
    *        from where the object pointer is to be
    *        removed. The position is relative to
    *        zero, i.e. zero implies that a pointer
    *        at the beginning of the array is to be
    *        removed.
    */
    IMPORT_C void Remove(TInt aIndex);

    /**
    * Inserts the object pointer into the array at
    * the specified position.
    * Note: Ownership of aMedia transferred.
    *
    * @since S60 3.2.3
    * @param aMedia object to be inserted to the
    *        array
    * @param aPos the position within the array
    *        where the object pointer is to be inserted.
    *        The position is relative to zero, i.e.
    *        zero implies that a pointer is inserted
    *        at the beginning of the array.
    *        USER 131, if aPos is negative, or is
    *        greater than the number of object
    *        pointers currently in the array.
    * @return KErrNone, if the insertion is
    *        successful, otherwise one of the system
    *        wide error codes.
    * @leave The function leaves with one of the system wide error codes,
    */
    IMPORT_C TInt Insert(const CMPXMedia *aMedia, TInt aPos);

    /**
    * Inserts the object pointer into the array at
    * the specified position.
    * A new media object will be copied from aMedia. The new media object
    * created on global heap and added into the array.
    *
    * @since S60 3.2.3
    * @param aMedia object to be inserted to the
    *        array
    * @param aPos the position within the array
    *        where the object pointer is to be inserted.
    *        The position is relative to zero, i.e.
    *        zero implies that a pointer is inserted
    *        at the beginning of the array.
    *        USER 131, if aPos is negative, or is
    *        greater than the number of object
    *        pointers currently in the array.
    * @return KErrNone, if the insertion is
    *        successful, otherwise one of the system
    *        wide error codes.
    * @leave The function leaves with one of the system wide error codes,
    */
    IMPORT_C TInt Insert(const CMPXMedia& aMedia, TInt aPos);

    /**
    * Inserts the object pointer into the array at
    * the specified position. The function leaves
    * with one of the system wide error codes, if
    * the operation fails.
    * Note: Ownership of aMedia transferred.
    *
    * @since S60 3.2.3
    * @param aMedia object to be inserted to the
    *        array
    * @param aPos the position within the array
    *        where the object pointer is to be inserted.
    *        The position is relative to zero, i.e.
    *        zero implies that a pointer is inserted
    *        at the beginning of the array.
    *        USER 131, if aPos is negative, or is
    *        greater than the number of object
    *        pointers currently in the array.
    */
    IMPORT_C void InsertL(const CMPXMedia *aMedia, TInt aPos);

    /**
    * Inserts the object pointer into the array at
    * the specified position. The function leaves
    * with one of the system wide error codes, if
    * the operation fails.
    * A new media object will be copied from aMedia. The new media object
    * created on global heap and added into the array.
    *
    * @since S60 3.2.3
    * @param aMedia object to be inserted to the
    *        array
    * @param aPos the position within the array
    *        where the object pointer is to be inserted.
    *        The position is relative to zero, i.e.
    *        zero implies that a pointer is inserted
    *        at the beginning of the array.
    *        USER 131, if aPos is negative, or is
    *        greater than the number of object
    *        pointers currently in the array.
    */
    IMPORT_C void InsertL(const CMPXMedia& aMedia, TInt aPos);

    /**
    * Sets the value at a specified position; the previous
    * value is deleted.
    *
    * @since S60 3.2.3
    * @param aMedia object to be set to the
    *        array
    * @param aPos the position within the array
    *        where the object pointer is to be set.
    *        The position is relative to zero, i.e.
    *        zero implies that a pointer is inserted
    *        at the beginning of the array.
    *        USER 131, if aPos is negative, or is
    *        greater than the number of objects
    *        currently in the array.
    */
    IMPORT_C void Set(const CMPXMedia& aMedia, TInt aPos);

public:
    /**
    * Externalize media objects.
    * @since S60 3.2.3
    * @param aStream write stream
    */
    IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

    /**
    * Internalize media objects.
    *
    * @since S60 3.2.3
    * @param aStream read stream
    */
    IMPORT_C void InternalizeL(RReadStream& aStream);

private:
    /**
    *  Value at a specific index.
    *
    *  @since S60 3.2.3
    */
    CMPXMedia* Value(TInt aIndex) const;

    /**
    *  Value at a specific index.
    *
    *  @since S60 3.2.3
    */
    CMPXMedia* ValueL(TInt aIndex) const;

    /**
    *  Inserts/Updates a value at aPos.
    *
    *  @since S60 3.2.3
    */
    TInt AddToArray(const CMPXMedia& aMedia,TInt aPos,TBool aReplace=EFalse);
    };

#endif // CMPXMEDIAARRAY_H
