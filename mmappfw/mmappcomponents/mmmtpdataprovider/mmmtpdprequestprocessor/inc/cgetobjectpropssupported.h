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
* Description:  Get object properties supported.
*
*/


#ifndef CGETOBJECTPROPSSUPPORTED_H
#define CGETOBJECTPROPSSUPPORTED_H

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeArray;
class CMmMtpDpMetadataAccessWrapper;
class MMmMtpDpConfig;

/**
* Defines GetObjectPropsSupported request processor
*/
class CGetObjectPropsSupported : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework    The data provider framework
    * @param aConnection    The connection from which the request comes
    * @param aDpConfig, The interface of dataprovider configuration
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL(MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );


    /**
    * Destructor.
    */
    IMPORT_C ~CGetObjectPropsSupported();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection   The connection from which the request comes
    * @param aDpConfig, The interface of dataprovider configuration
    */
    CGetObjectPropsSupported( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

protected:
    // from CRequestProcessor
    /**
    * GetObjectPropsSupported request handler
    */
    IMPORT_C void ServiceL();

private:
    CMTPTypeArray* iObjectPropsSupported;
    MMmMtpDpConfig& iDpConfig;

    };

#endif // CGETOBJECTPROPSSUPPORTED_H
