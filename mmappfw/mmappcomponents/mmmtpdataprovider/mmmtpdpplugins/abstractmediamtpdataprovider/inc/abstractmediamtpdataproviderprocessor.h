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
* Description:
*
*/


#ifndef ABSTRACTMEDIAMTPDATAPROVIDERPROCESSOR_H
#define ABSTRACTMEDIAMTPDATAPROVIDERPROCESSOR_H

// class forward
class MMTPConnection;
class TMTPTypeRequest;
class MMmRequestProcessor;
class MMmMtpDpConfig;

typedef MMmRequestProcessor* ( *TAbstractMediaMtpDataProviderRequestProcessorCreateFunc )(
    MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig );

typedef struct
    {
    TUint16 iOperationCode;
    TAbstractMediaMtpDataProviderRequestProcessorCreateFunc iCreateFunc;
    } TAbstractMediaMtpDataProviderRequestProcessorEntry;

class AbstractMediaMtpDataProviderProcessor
    {
public:
    /**
    * Create a request processor that matches the request
    * @param aFramework The reference to the data provider framework
    * @param aRequest    The request to be processed
    * @param aConnection The connection from which the request comes from
    * @param aMetadataWrapper  medadata access interface
    * @return a pointer to the request processor
    */
    static MMmRequestProcessor* CreateL( MMTPDataProviderFramework& aFramework,
        const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    };

#endif // ABSTRACTMEDIAMTPDATAPROVIDERPROCESSOR_H
