// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __TNONOPERATIONMTMDATATYPE_H__
#define __TNONOPERATIONMTMDATATYPE_H__

#include <e32std.h>

/**
UID used by the Extension_ method for CBaseServerMtm to identify that the
non operation MTM data has been requested.

@internalComponent
@released
*/
const TUint KUidMsgNonOperationMtmData = 0x1028368e;

/**
Defines the types of non operation MTM data that can be requested from a
server MTM.

@internalComponent
@released
*/
enum TNonOperationMtmDataType
	{
	EMtmDataAccessPointId = 0
	};

/**
Structure to store IAP value for non operation MTM data IAP request

@internalComponent
@released
*/
struct TNonOperationMtmDataAccessPointId
	{
	TUint32 iAccessPointId;
	};

/**
Package to store result for non operation MTM data IAP request

@internalComponent
@released
*/
typedef TPckgBuf<TNonOperationMtmDataAccessPointId> TNonOperationMtmDataAccessPointIdBuffer;

#endif // #define __TNONOPERATIONMTMDATATYPE_H__
