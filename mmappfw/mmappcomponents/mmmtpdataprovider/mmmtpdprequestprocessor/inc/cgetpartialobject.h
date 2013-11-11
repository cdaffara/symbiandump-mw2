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
* Description:  GetParitialObject operation
*
*/


#ifndef CGETPARTIALOBJECT_H
#define CGETPARTIALOBJECT_H

#include "crequestprocessor.h"

class CMTPTypeFile;
class CMmMtpDpMetadataAccessWrapper;
class MMmMtpDpConfig;

/**
* Defines GetPartialObject request processor
*/
class CGetPartialObject : public CRequestProcessor
    {
public:
    /**
    * Two-phase construction method
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    * @param aWrapper     medadata access interface
    * @return a pointer to the created request processor object
    */
    IMPORT_C static MMmRequestProcessor* NewL( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    /**
    * Destructor
    */
    IMPORT_C virtual ~CGetPartialObject();

protected:
    /**
    * Standard c++ constructor
    * @param aFramework   The data provider framework
    * @param aConnection  The connection from which the request comes
    */
    CGetPartialObject( MMTPDataProviderFramework& aFramework,
        MMTPConnection& aConnection );

protected:
    // from CRequestProcessor
    /**
    * Check the GetPartialObject reqeust
    * @return EMTPRespCodeOK if the request is good, otherwise, one of the error response codes
    */
    IMPORT_C TMTPResponseCode CheckRequestL();

    /**
    * GetPartialObject request handler
    * Send the partial object data to the initiator
    */
    IMPORT_C void ServiceL();

    /**
    * Signal to the initiator how much data has been sent
    * @return EFalse
    */
    IMPORT_C TBool DoHandleResponsePhaseL();

private:
    /**
    * Second-phase construction
    */
    void ConstructL();

private:
    //helper
    /**
    * Verify if the parameter of the request (i.e. offset) is good.
    * @return ETrue if the parameter is good, otherwise, EFalse
    */
    TBool VerifyParametersL();

    /**
    * Populate the partial data object
    */
    void BuildPartialDataL();

private:
    MMTPDataProviderFramework& iFramework;
    TUint iOffset;
    TUint iPartialDataLength;

    CMTPTypeFile* iFileObject;
    TUint32 iObjectHandle;

    };

#endif // CGETPARTIALOBJECT_H
