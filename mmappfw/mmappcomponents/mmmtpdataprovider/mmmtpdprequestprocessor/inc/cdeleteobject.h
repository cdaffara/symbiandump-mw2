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
* Description:  Delete object operation
*
*/


#ifndef CDELETEOBJECT_H
#define CDELETEOBJECT_H

#include <d32dbms.h>

#include "crequestprocessor.h"

//forward declaration
class MMmMtpDpConfig;

/**
* Defines DeleteObject request processor
*/
class CDeleteObject : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CDeleteObject();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    */
    CDeleteObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

protected:
    //from CRequestProcessor
    /**
    * DeleteObject request handler
    */
    IMPORT_C void ServiceL();

    //from CActive
    /**
    * AO Run method, deletes a selection of files on the system
    */
    IMPORT_C void RunL();

    /**
    * Used to cancel the deletion of the files
    */
    IMPORT_C void DoCancel();

    /**
    * Completeing phase Handler
    */
    IMPORT_C TBool DoHandleCompletingPhaseL();

private:
    void ConstructL();
    /**
    * Control the deletion
    */
    void StartL();
    /**
    * Delete all child objects of an association recursively.
    */
    void DeleteAllChildrenL();

    /**
    * Delete all of the associations if file deletion was successful
    * Then signals that the deletion has been completed
    */
    void ProcessFinalPhaseL();

    /**
    * Gets all object handles (for GetObjectHandlesL)
    */
    void GetObjectHandlesL( TUint32 aStorageId, TUint32 aParentHandle );

    /**
    * Deletes an object from the file system and the object store
    */
    void DeleteObjectL( const CMTPObjectMetaData& aObjectInfo );

private:
    //  Array of all the objects to delete
    RArray<TUint> iObjectsToDelete;

    // Keeps track of any errors that occur
    TInt iDeleteError;

    // Keeps track of whether any files where deleted
    TBool iIsMultiDelete;

    // Keeps track of the respone code when it is EMTPRespCodeDeviceBusy
    TBool iDeviceBusy;

    // Keeps track of the count of EMTPRespCodeDeviceBusy that return to PC
    TInt iCountDeviceBusyError;

    // Accessor of DB
    MMmMtpDpConfig& iDpConfig;

    };

#endif // CDELETEOBJECT_H
