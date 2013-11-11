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
* Description:  Get Object operation
*
*/


#ifndef CGETOBJECT_H
#define CGETOBJECT_H

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeFile;
class MMmMtpDpConfig;

/**
* Defines GetObject request processor
*/
class CGetObject : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper medadata access interface
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C ~CGetObject();

protected:
    // from CRequestProcessor
    /**
    * GetObject request handler
    */
    void ServiceL();

    /**
    * Handle the response phase of the current request
    * @return EFalse
    */
    TBool DoHandleResponsePhaseL();

    /**
    * Two-phase construction
    */
    void ConstructL();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    */
    CGetObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

    void GetObjectL( const TDesC& aFileName );

private:
    // File Object to encapsulate the object to send back
    CMTPTypeFile* iFileObject;

    // Object Info
    CMTPObjectMetaData* iObjectInfo;

    TMTPResponseCode iError;

    };

#endif // CGETOBJECT_H
