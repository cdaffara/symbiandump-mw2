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


#ifndef MEDIAMTPDATAPROVIDERPROCESSOR_H
#define MEDIAMTPDATAPROVIDERPROCESSOR_H

// class forward
class MMTPConnection;
class TMTPTypeRequest;
class CMTPDataProviderPlugin;
class MMTPRequestProcessor;
class CMediaMtpDataProvider;
class MMmMtpDpConfig;
class MMmRequestProcessor;

typedef MMmRequestProcessor* ( *TMediaMtpDataProviderRequestProcessorCreateFunc )(
    MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig );

typedef struct
    {
    TUint16 iOperationCode;
    TMediaMtpDataProviderRequestProcessorCreateFunc iCreateFunc;
    } TMediaMtpDataProviderRequestProcessorEntry;

class MediaMtpDataProviderProcessor
    {
public:
    /**
    * Create a request processor that matches the request
    * @param aFramework The reference to the data provider framework
    * @param aRequest    The request to be processed
    * @param aConnection The connection from which the request comes from
    * @param aDpConfig  Dp configurations
    * @return a pointer to the request processor
    */
    static MMmRequestProcessor* CreateL( MMTPDataProviderFramework& aFramework,
        const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection,
        MMmMtpDpConfig& aDpConfig );

    };

#endif // MEDIAMTPDATAPROVIDERPROCESSOR_H
