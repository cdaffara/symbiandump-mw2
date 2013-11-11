/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Central repository keys for OMA SUPL configuration parameters
*
*/


#ifndef EPOS_OMASUPLCONFIGURATIONKEYS_H
#define EPOS_OMASUPLCONFIGURATIONKEYS_H

//CR Keys for OMA SUPL holds OMA SUPL Timeout Values
const TUid KCRUidOMASuplConfiguration = {0x102750A0 };  
const TUint32 KOMASuplConfigurationUT1 =		  0x00000001;
const TUint32 KOMASuplConfigurationUT2 = 		  0x00000002;
const TUint32 KOMASuplConfigurationUT3 = 		  0x00000003;
const TUint32 KOMASuplConfigurationPrivacyTimer = 0x00000004;
const TUint32 KOMASuplConfigurationSuplMode = 				  0x00000005;

// CR Key for local variation settings
const TUint32 KOMASuplClientProvHandlerVariant = 0x00000006;
// CR Key for local variation settings
const TUint32 KOMASuplSyncMLDMProvHandlerVariant = 0x00000007;
// CR Key for IMSI variation
const TUint32 KOMASuplProtocolHandlerVariant = 0x00000008;
const TUint32 KOMASuplConfigurationPersistentFailureTimer =    0x00000009;
const TUint32 KOMASuplConfigurationSuplPermissionQueryUsage =    0x0000000a;
const TUint32 KOMASuplNiDialogTimeOutValue =    0x0000000b;

#endif //EPOS_OMASUPLCONFIGURATIONKEYS_H
