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
* Description: Request processor which handle common property description
*
*/

#ifndef CGETOBJECTPROPDESC_H
#define CGETOBJECTPROPDESC_H

#include "crequestprocessor.h"

// forward declaration
class CMTPTypeObjectPropDesc;
class MMmMtpDpConfig;

/**
* Defines GetObjectPropDesc request processor for common
*/
class CGetObjectPropDesc : public CRequestProcessor
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
    IMPORT_C virtual ~CGetObjectPropDesc();

private:
    /**
    * Standard c++ constructor
    * @param aFramework    The data provider framework
    * @param aConnection   The connection from which the request comes
    * @param aWrapper      Medadata access interface
    */
    CGetObjectPropDesc( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Second phase constructor
    */
    void ConstructL();

protected:
    // from CRequestProcessor
    /**
    * Check the current request
    * @return EMTPRespCodeOK if the reqeust is good, otherwise,
    *     one of the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * GetObjectPropDesc request handler
    */
    IMPORT_C void ServiceL();

private:
    /**
    * Ensures the object format operation parameter is valid
    * (done for CheckRequestL)
    * @return Response code of the check result
    */
    TMTPResponseCode CheckFormatL() const;

    /**
    * Ensures the object prop code operation parameter is valid
    * (done for CheckRequestL)
    * @return Response code of the check
    */
    TMTPResponseCode CheckPropCodeL() const;

private:
    TUint iFormatCode;

    MMmMtpDpConfig& iDpConfig;

    TUint iPropCode;

    // property description
    CMTPTypeObjectPropDesc* iPropertyDesc;

    };

#endif // CGETOBJECTPROPDESC_H
