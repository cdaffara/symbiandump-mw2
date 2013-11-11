/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Encapsulates media base class
*
*/



#ifndef CMPXMEDIABASE_H
#define CMPXMEDIABASE_H

// INCLUDES
#include <s32strm.h>
#include <mpxdata.h>
#include <e32hashtab.h>

//  FORWARD DECLARATIONS

/**
*  Base class of global data classes.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(CMPXMediaBase) : public CBase
    {
protected:
    /**
    * Encapsulates a local value, uid and corresponding attribute type.
    *
    * @lib mpxcommon.lib
    */
    class TValue
        {
    public:
        /**
         * C++ constuctor.
         *
         * @since S60 3.2.3
         * @param aValue pointer to the object of the media attribute
         * @param aUid uid of the object
         * @param aType type of the media attribute
         */
        inline TValue(TAny* aValue,TInt aUid,TMPXAttributeType aType);
        
    public:
        TAny* iValue;
        TInt iUid;
        TMPXAttributeType iType;
        };
protected:
     /**
     * C++ default constructor.
     *
     * @since S60 3.2.3
     */
    CMPXMediaBase();

    /**
     * Destructor.
     *
     * @since S60 3.2.3
     */
    ~CMPXMediaBase();

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    */
    void ConstructL();

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aBase media base object to be copied
    */
    void ConstructL(const CMPXMediaBase& aBase);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aDataHandle data handle to the global object to be copied
    */
    void ConstructL(TInt aDataHandle);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aDataHandle data handle to the global oject to be copied
    *  @param aData data object to be copied
    *  @leave KErrArgument DataHandle is zero
    */
    void ConstructL(TInt aDataHandle,MMPXData& aData);

    /**
    *  2nd phase contructor.
    *
    *  @since S60 3.2.3
    *  @param aBase media base object to be copied. Deep copy.
    */
    void CopyConstructL(const CMPXMediaBase& aBase);

protected:
    /**
    * Clears all data associated with this object; ready to
    * be assigned new data.
    *
    * @since S60 3.2.3
    */
    void Clear();

    /**
    * Clears locally cached contents.
    *
    * @since S60 3.2.3
    */
    virtual void ResetLocal();

    /**
    * Locally stored value, or NULL if not stored locally.
    *
    * @since S60 3.2.3
    * @param aUid uid of the media attribute
    * @return TValue object which holds local copy of the object
    */
    const TValue* LocalValue(TInt aUid) const;
    
    /**
    * Set locally held data.
    * 
    * @since S60 3.2.3
    * @param aValue value to be held
    * @return KErrNone or KErrNoMemory
    */
    int SetLocal(const TValue& aValue);

    /**
     * Delete locally by index.
     *
     * @since S60 3.2.3
     * @param aIndex index to the values array
     */
    void DeleteLocal(TInt aIndex);

    /**
    * Delete a pointer.
    *
    * @since S60 3.2.3
    * @param aValue value to be deleted
    * @aType media attribute type
    */
    void DeletePtr(TAny* aValue,TMPXAttributeType aType);

    /**
    * Externalize media object.
    *
    * @since S60 3.2.3
    * @param aStream reference to the stream object
    * @param aType object type
    */
    void DoExternalizeL(RWriteStream& aStream,MMPXData::TMPXObjectType aType) const;

    /**
    * Internalize media object.
    *
    * @since S60 3.2.3
    * @param aStream reference to the stream object
    * @param aType object type
    * @leave KErrArgument aType is invalid
    */
    void DoInternalizeL(RReadStream& aStream,MMPXData::TMPXObjectType aType);

    /**
    * Number of values.
    *
    * @since S60 3.2.3
    * @return number of media attributes in the global data
    */
    inline TInt Count() const;

    /**
    *  Lock heap and push UnlockHeap into cleanupstack.
    *
    * @since S60 3.2.3
    */
    void LockHeapLC() const;

    /**
    *  Cleanup operation which will be invoked by the CleanupStack::PopAndDestroy().
    *
    *  @since S60 3.2.3
    *  @param aMediaBase a media based object
    */
    static void UnlockHeap(TAny* aMediaBase);

private:
    /**
    * Delete locally held data.
    * 
    * @since S60 3.2.3
    * @param aUid Uid of the value to be deleted
    */
    void DeleteLocalByUid(const TInt aUid);

protected:
    TUint iClientHandle; // Thread relative
    TUint iDataHandle; // Global
    MMPXData* iData; // Thread relative
private:    
    RHashMap<TInt, TValue> iValues;
    };

#include "mpxmediabase.inl"

#endif // CMPXMEDIABASE_H
