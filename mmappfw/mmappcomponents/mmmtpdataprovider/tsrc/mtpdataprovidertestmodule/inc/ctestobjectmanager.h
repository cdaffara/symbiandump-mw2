/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Dummy object manager for test module
*
*/


#ifndef CTESTOBJECTMANAGER_H
#define CTESTOBJECTMANAGER_H

#include <stiflogger.h>
#include <mtp/mmtpobjectmgr.h> 
#include <mtp/cmtpobjectmetadata.h>
#include <mtp/tmtptypeuint32.h>

class CTestObjectManager : public MMTPObjectMgr, public CBase
    {
public:
    // destructor
    virtual ~CTestObjectManager();
    static CTestObjectManager* NewL();
    
    // from MMTPObjectMgr
    /**
    Provides a count of the number of object meta data records present 
    in the MTP data object enumeration store. This query can be targeted 
    at specific storage media and/or object data formats, or can be used 
    to query all MTP data objects on the device.
    @param aParams The query parameters.
    @return A count of the number of object meta data records.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TUint CountL( const TMTPObjectMgrQueryParams& aParams ) const;
    
    /**
    Commits into MTP data object enumeration store the object handle and 
    storage space previously reserved for the specified object. This is 
    typically used when either an MTP SendObjectInfo/SendObject, or 
    SendObjectPropList/SendObject operation sequence has completed 
    successfully.
    @param aObject The MTP data object information record to be commited.
    @leave One of the system wide error codes, if a processing failure occurs.
    @see ReserveObjectHandleL
    */
    void CommitReservedObjectHandleL( CMTPObjectMetaData& aObject );
    
    /**
    Provides an array of object handles present in the MTP data object 
    enumeration store. This query can be targeted at specific storage media 
    and/or object data formats, or can be used to query all MTP data objects 
    on the device. 
    
    Depending on the query parameter specified, this query may incur significant 
    RAM consumption, and so is intended to be called repeatedly in order to 
    retrieve all matching SUID data. On completion, @see aContext can be used
    to determine if there is more data to be retrieved.
    
    If there are no object handles which match the specified parameters then a 
    zero length SUID array is provided. 
    
    @param aParams The query parameters.
    @param aContext The query context data. On completion this data indicates 
    if there are additional object handle data to retrieve.
    @param aHandles The MTP object handles array buffer to be populated.
    @leave One of the system wide error codes, if a processing failure occurs.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void GetObjectHandlesL( const TMTPObjectMgrQueryParams& aParams,
    		RMTPObjectMgrQueryContext& aContext,
    		RArray<TUint>& aHandles ) const;
    
    /**
    Provides an array of object SUIDs present in the MTP data object 
    enumeration store. This query can be targeted at specific storage media 
    and/or object data formats, or can be used to query all MTP data objects 
    on the device. 
    
    Depending on the query parameter specified, this query may incur significant 
    RAM consumption, and so is intended to be called repeatedly in order to 
    retrieve all matching object SUID data. On completion, @see aContext can be used
    to determine if there is more data to be retrieved.
    
    If there are no object handles which match the specified parameters then a 
    zero length SUID array is provided. 
    
    @param aParams The query parameters.
    @param aContext The query context data. On completion this data indicates 
    if there are additional SUID data to retrieve.
    @param aSuids The object SUIDs array buffer to be populated.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void GetObjectSuidsL( const TMTPObjectMgrQueryParams& aParams,
    		RMTPObjectMgrQueryContext& aContext, CDesCArray& aSuids ) const;
    
    /**
    Provides the MTP object handle of the object with the specified internal 
    object unique ID.
    @param aObjectUid The internal object unique ID.
    @return The MTP object handle.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TUint32 HandleL( TInt64 aObjectUid ) const;
    
    /**
    Provides the MTP object handle of the object with the specified Persistent 
    Unique Identifier (PUID).
    @param aPuid The object PUID.
    @return The MTP object handle.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TUint32 HandleL( const TMTPTypeUint128& aPuid ) const;
    
    /**
    Provides the MTP object handle of the object with the specified specified 
    System Unique Identifier (SUID).
    @param aSuid The object SUID.
    @return The MTP object handle.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TUint32 HandleL( const TDesC& aSuid ) const;
    
    /**
    Inserts a single object information record into the MTP data object 
    enumeration store. A unique MTP object handle is assigned to the 
    data object.
    @param aObject The MTP data object information record to be inserted.
    @leave KErrAlreadyExists, if an object with the specified MTP object SUID
    already exists in the MTP data object enumeration store.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void InsertObjectL( CMTPObjectMetaData& aObject );
    
    /**
    Inserts multiple object meta data records in the MTP data object 
    enumeration store. A unique MTP object handle is assigned to each of the 
    data objects. 
    @param aObjects The MTP data object meta data records to be inserted.
    @leave KErrAlreadyExists, if an object with the one of the specified MTP 
    object SUIDs already exists in the MTP data object enumeration store.
    already exists in the MTP data object enumeration store.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void InsertObjectsL( RPointerArray<CMTPObjectMetaData>& aObjects );
    
    /**
    Modifies the specified MTP data object's object information record. 
    @param aObject The modified MTP data object information record values.
    */
    void ModifyObjectL( const CMTPObjectMetaData& aObject );
    
    /**
    Provides either the object information record associated with the 
    associated MTP object handle, or provides an indication that the specified
    object handle does not exist. 
    @param aHandle The MTP object handle associated with the object information 
    record to be provided.
    @param aObject On exit, the object information record of the requested object. 
    @return ETrue if the specified object handle exists, otherwise EFalse.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TBool ObjectL( const TMTPTypeUint32& aHandle,
    		CMTPObjectMetaData& aObject ) const;
    
    /**
    Provides the object information record associated with the associated MTP
    object SUID, or provides an indication that the specified object handle 
    does not exist. 
    @param aSuid The MTP object SUID associated with the object information 
    record to be provided.
    @param aObject On exit, the object information record of the requested object. 
    @return ETrue if the specified object handle exists, otherwise EFalse.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TBool ObjectL( const TDesC& aSuid, CMTPObjectMetaData& aObject ) const;
    
    /**
    Provides the ID of the data provider responsible for the specified MTP 
    data object.
    @param aHandle The MTP object handle.
    @return The responsible data provider ID.
    */
    TUint ObjectOwnerId( const TMTPTypeUint32& aHandle ) const;
    
    /**
    Provides the internal object unique ID of the object with the specified MTP
    object handle.
    @param aHandle The MTP object handle.
    @return The internal object unique ID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TInt64 ObjectUidL( TUint32 aHandle ) const;
    
    /**
    Provides the internal object unique ID of the object with the specified 
    System Unique Identifier (SUID).
    @param aSuid The object SUID.
    @return The internal object unique ID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TInt64 ObjectUidL( const TDesC& aSuid ) const;
    
    /**
    Provides the internal object unique ID of the object with the specified 
    Persistent Unique Identifier (PUID).
    @param aPuid The object PUID.
    @return The internal object unique ID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TInt64 ObjectUidL( TMTPTypeUint128 aPuid ) const;
    
    /**
    Provides the Persistent Unique Identifier (PUID) of the object with the 
    specified MTP object handle.
    @param aHandle The MTP object handle.
    @return The object PUID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TMTPTypeUint128 PuidL( TUint32 aHandle ) const;
    
    /**
    Provides the Persistent Unique Identifier (PUID) of the object with the 
    specified internal object unique ID.
    @param aObjectUid The internal object unique ID.
    @return The object PUID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TMTPTypeUint128 PuidL( TInt64 aObjectUid ) const;
    
    /**
    Provides the Persistent Unique Identifier (PUID) of the object with the 
    specified System Unique Identifier (SUID).
    @param aSuid The object SUID.
    @return The object PUID.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    TMTPTypeUint128 PuidL( const TDesC& aSuid ) const;
    
    /**
    Removes the object information record associated with the specified MTP 
    object handle from the MTP data object enumeration store.
    @param aHandle The MTP object handle associated with the object information 
    record to be removed.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void RemoveObjectL( const TMTPTypeUint32& aHandle );
    
    /**
    Removes the object information record associated with the specified object
    SUID from the MTP data object enumeration store.
    @param aSuid The object SUID associated with the object information 
    record to be removed.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void RemoveObjectL( const TDesC& aSuid );
    
    /**
    Removes multiple object meta data records from the MTP data object 
    enumeration store.
    @param aSuids The array of object SUIDs associated with the object 
    meta data records to be removed.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void RemoveObjectsL( const CDesCArray& aSuids );
    
    /**
    Removes from the MTP data object enumeration store all object meta data 
    records owned by the specified data provider.
    @param aDataProviderId The ID of the data provider owning the object 
    meta data records to be deleted.
    @leave One of the system wide error codes, if a processing failure occurs.
    */
    void RemoveObjectsL( TUint aDataProviderId );
    
    /**
    Reserves space for and assigns an object handle to the object described
    by the specified object information record. This is typically used when
    processing MTP SendObjectInfo or SendObjectPropList operations.
    @param aObject The object information record of the object. On 
    successful completion this is updated with the assigned object handle.
    @param aSpaceRequired The storage space to be reserved for the object.
    @leave KErrTooBig, if aSpaceRequired exceeds the available capacity of the 
    storage. 
    @leave KErrOverflow, if an object handle cannot be assigned.
    @leave One of the system wide error codes, if a processing failure occurs.
    @see CommitReservedObjectL
    @see UnReserveObjectHandleL
    */
    void ReserveObjectHandleL( CMTPObjectMetaData& aObject,
    		TUint64 aSpaceRequired );
    
    /**
    Releases the object handle and storage space previously reserved for the 
    specified object. This is typically used when either an MTP 
    SendObjectInfo/SendObject, or SendObjectPropList/SendObject operation 
    sequence has not completed successfully.
    @param aObject The MTP data object information record of the object.
    @leave One of the system wide error codes, if a processing failure occurs.
    @see ReserveObjectHandleL
    */
    void UnreserveObjectHandleL( const CMTPObjectMetaData& aObject );
private:
    CTestObjectManager();
    void ConstructL(); 
    
    void CopyObjectMetadataL( const CMTPObjectMetaData& aSource,
    		CMTPObjectMetaData& aTarget ) const;
private:
    CStifLogger* iLog;
    RPointerArray<CMTPObjectMetaData> iMTPObjects;
    
    TInt iHandle;
    };

#endif // CTESTOBJECTMANAGER_H
