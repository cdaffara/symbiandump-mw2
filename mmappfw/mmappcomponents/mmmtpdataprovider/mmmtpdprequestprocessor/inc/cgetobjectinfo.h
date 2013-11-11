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
* Description:  Get object info operation
*
*/


#ifndef CGETOBJECTINFO_H
#define CGETOBJECTINFO_H

#include "crequestprocessor.h"

// forward declacration
class CMTPTypeObjectInfo;
class MMmMtpDpConfig;

/**
* Defines GetObjectInfo request processor
*/
class CGetObjectInfo : public CRequestProcessor
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
    IMPORT_C ~CGetObjectInfo();

protected:
    // From CRequestProcessor
    /**
    * GetObjectInfo request handler
    */
    void ServiceL();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aWrapper    medadata access interface
    */
    CGetObjectInfo( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Second-phase construction
    */
    IMPORT_C void ConstructL();

private:
    /**
    * Prepare object with existing data according to the handle from PC
    */
    void BuildObjectInfoL();

private:
    CMTPTypeObjectInfo* iObjectInfo;

    MMmMtpDpConfig& iDpConfig;

    };

#endif // CGETOBJECTINFO_H
