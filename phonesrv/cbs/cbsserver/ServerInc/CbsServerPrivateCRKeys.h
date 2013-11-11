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
* Description:  This file contains the header file of private CR keys
*
*/



#ifndef __CBSSERVER_PRIVATE_CR_KEYS_H__
#define __CBSSERVER_PRIVATE_CR_KEYS_H__

const TUid KCRUidCellBroadcast = {0x1020298F};

/**
* This key setting enables CBS messages reception if it is turned on.
*
* Possible integer values:
* 0 Reception off
* 1 Reception on
*
* Default value: 0
*/
const TUint32 KCbsReception = 0x00000001;

/**
* If this key value is set on and the topic of received message is 
* not in the topic list, the topic is added to the topic list.
*
* Possible integer values:
* 0 Topic detection off
* 1 Topic detection on
*
* Default value: 0
*/
const TUint32 KCbsTopicDetection = 0x00000002;

/**
* This key holds information of message languages 
* that are allowed to be received. Default value means that 
* all messages with all languages are allowed to be received.
*
* Possible values:
* Each character in the string can be either '0' or '1'
*
* Default value: 0000000000000000000001
*/
const TUint32 KCbsLanguages = 0x00000003;

/**
* With this key value user sets feature on, which limits message 
* reception with a time frame. Time frame is defined separately by UI.
*
* Possible integer values:
* 0 Time limits off
* 1 Time limits on
*
* Default value: 0
*/
const TUint32 KCbsLimitedReception = 0x00000004;

/**
* Time when last automatic cleanup was excecuted.
*
* Possible integer values:
* Interger representation of the time.
*
* Default value: 0
*/
const TUint32 KCbsCleanupTime = 0x00000005;
	
#endif // __CBSSERVER_PRIVATE_CR_KEYS_H__

