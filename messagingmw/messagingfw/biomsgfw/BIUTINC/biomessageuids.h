// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description: Contains the TUid defines for the Bio Messaging
// biomessageuids.h
//

/**
 * @file
 * @publishedPartner
 * @released
 */
#if !defined(__BIOMESSAGEUIDS_H__)
#define __BIOMESSAGEUIDS_H__
/**
* UID of the Smart Message Parser DLL.
* @publishedPartner
* @released
*/
const TUid KUidSmartMessageParserDll    = { 0x10001251};
/**
* BIO specific error/panic code base. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageBaseError				= -505;
/**
* BIO message not found error. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageNotFound				= KBIOMessageBaseError;
/**
* BIO server MTM already active error. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageSvrMtmNotInactive		= KBIOMessageBaseError-1;
/**
* No BIO parser error. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageNoParserCreated		= KBIOMessageBaseError-2;  
/** 
* BIO parser in use error. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageParserDllStillInUse	= KBIOMessageBaseError-3;  
/**    
* BIO operation already active error. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageOperationNotInactive	= KBIOMessageBaseError-4;
/**
* BIO server MTM operation not supported. 
* @publishedPartner
* @released
*/
const TInt KBIOMessageSvrNotSupported		= KBIOMessageBaseError-5;

#endif //__BIOMESSAGEUIDS_H__