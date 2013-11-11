/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   SendUI central repository key definitions
*
*/




#ifndef _SENDUIPRIVATECRKES_H
#define _SENDUIPRIVATECRKES_H

// CONSTANTS

/**
* Send UI Service API
*/
const TUid KCRUidSendUi = {0x10282823};

/**
* Number of sending services
*/
const TInt32 KKeySendUiServiceCount         = 0x01000000;
const TInt32 KKeySendUiServiceLanguage      = 0x01000001;
const TInt32 KKeySendUiServiceMtmCount      = 0x01000002;
const TInt32 KKeySendUiServiceEcomCount     = 0x01000003;
const TInt32 KKeySendUiFeatureManagerServices = 0x01000004;

// Sendui service list is updated, if Postcard, Audio Message or MMS Upload service state changes according to FeatureManager
const TInt32 KSendUiPostcard     = 0x00000001;
const TInt32 KSendUiAudioMessage = 0x00000002;
const TInt32 KSendUiMmsUpload = 0x00000004;

/**
* Elements of the sending service structure
*/
const TInt32 KSendUiServiceId               = 0x00000000;
const TInt32 KSendUiServiceProviderId       = 0x00000001;
const TInt32 KSendUiServiceFlags            = 0x00000002;
const TInt32 KSendUiServiceBodySize         = 0x00000003;
const TInt32 KSendUiServiceMessageSize      = 0x00000004;
const TInt32 KSendUiServiceTechnologyType   = 0x00000005;
const TInt32 KSendUiServiceName             = 0x00000006;
const TInt32 KSendUiServiceMenuName         = 0x00000007;
const TInt32 KSendUiServiceFeatures         = 0x00000008;
const TInt32 KSendUiServiceAddress          = 0x00000009;

/**
* Service index "position":
*
* 1st sending service:
*  0x0000
*  0x0001
*  0x0002
*  ...
* 2nd sending service:
*  0x0100
*  0x0101
*  0x0102
*  ...
* 3rd sending service:
*  0x0200
*  0x0201
*  0x0202
*  ...
* etc.
*/
const TInt32 KSendUiServiceIndex            = 0x00000100;

/**
* Maximum length for the name of the sending service
*/
const TInt32 KSendUiServiceMaxNameLength    = 40;

#endif