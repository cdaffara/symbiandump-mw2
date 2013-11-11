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
* Description:  Get Format Capabilities operation
*
*/
#ifndef CGETFORMATCAPABILITLIST_H
#define CGETFORMATCAPABILITLIST_H

#include <mtp/cmtptypeformatcapabilitylist.h>

#include "crequestprocessor.h"

// forward declaration
class MMmMtpDpConfig;
class CMTPTypeObjectPropDesc;

/**
Implements the file data provider CMTPGetFormatCapabilities request processor.
@internalComponent
*/
class CGetFormatCapabilities : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework The data provider framework
    * @param aConnection The connection from which the request comes
    * @param aDpConfig Configuration of data provider
    * @return The pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CGetFormatCapabilities();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection   The connection from which the request comes
    * @param aDpConfig, The interface of dataprovider configuration
    */
    CGetFormatCapabilities( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Constructor
    * Second phase constructor
    */
    void ConstructL();

protected:
    // from CMTPRequestProcessor
    /**
    * CGetFormatCapabilities request handler
    */
    IMPORT_C void ServiceL();

    /**
    * Check the current request
    * @return EMTPRespCodeOK if the reqeust is good, otherwise,
    *     one of the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

private:
    /**
     * Handle one format's capabilities
     */
    void ServiceOneFormatCapabilitiesL( TUint aFormatCode );

    /**
    * Handle one property description of one format
    * @param aPropCode    The Property Code
    * @return The MTPTypeObject PropDesc
    */
    CMTPTypeObjectPropDesc* ServiceOnePropDescL( TUint16 aPropCode );

private:
    /*
     * Format code.
     */
    TUint iFormatCode;

    /*
     * MMmMtpDpConfig.
     */
    MMmMtpDpConfig& iDpConfig;

    /*
     * CMTPTypeCapabilityList dataset.
     */
    CMTPTypeFormatCapabilityList* iCapabilityList;

    };

#endif // CMTPGETFORMATCAPABILITLIST_H
