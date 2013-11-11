// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Description:
// This file contains the codes for WAP Push CO Watcher and Security Plugin
// 
//

/**
 @file PUSHSTD.H
*/

#ifndef __PUSHSTD_H__
#define __PUSHSTD_H__

/**
 * Base values of code ranges.
 */
enum{
	EWapPushRetCodeBase     = -20000,  //todo...
	};

/**
 * Return/leave codes for Wap Push CO Watcher and Security Plugin.
 */
typedef enum 
/**
@internalComponent
@released
*/
	{
	EWapPushGeneralError			  = EWapPushRetCodeBase,	  // Unspecified error condition
	EWapPushCannotFindPlugin		  = EWapPushRetCodeBase - 1,  // Plug-in server failed to create a Security plugin
	EWapPushWtlsConfigFailed		  = EWapPushRetCodeBase - 2,  // Failed to configure the WTLS layer of the WAP Stack
	EWapPushWtlsBadServerCert		  = EWapPushRetCodeBase - 3,  // WTLS rejected the certificate obtained from the WAP GW
	EWapPushWtlsServerCertRejected	  = EWapPushRetCodeBase - 4,  // The user rejected the certificate from the WAP GW
	EWapPushWtlsCipherRejected		  = EWapPushRetCodeBase - 5,  // The user rejected the cipher strength negotiated in WTLS
	EWapPushGatewayCannotBeReached	  = EWapPushRetCodeBase - 6, // A session could not be established with the WAP Gateway
	EWapPushGatewayTransactionAbort   = EWapPushRetCodeBase - 7, // A transaction was aborted by the gateway or the stack
	EWapPushGatewaySessionDisconnect  = EWapPushRetCodeBase - 8, // The session was disconnected by the WAP Gateway
	EWapPushCancellationAbort		  = EWapPushRetCodeBase - 9  // Error code for leave when cancellation has occured during RunL()
	} TWapPushReturnCode;


#endif //__PUSHSTD_H__
