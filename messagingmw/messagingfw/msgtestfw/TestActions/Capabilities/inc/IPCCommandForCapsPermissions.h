// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __IPCCOMMANDFORCAPSPERMISSIONS_H__
#define __IPCCOMMANDFORCAPSPERMISSIONS_H__


#include <e32base.h>
#include <e32std.h>


#include <msvipc.h>


class CMtfTestCase;

void IPCCommandReportPassOrFailForCapsPermissions( CMtfTestCase& aTestCase, const TDesC& aTestName, TInt aError ,TInt aPass );


//
// Require this to get access so can set parent etc. 
//
class TMsvEntryForward : public TMsvEntry
{
public :
	void SetParent(TMsvId aId) 
	{
		TMsvEntry::SetParent(aId); 
	};
	void SetId(TMsvId aId)
	{
		TMsvEntry::SetId(aId);
	}
	
};


enum TCreateEntryType
{
	ECreateService, 
	ECreateMessage,
	ECreateRemote,
	ECreateMessageSMS,
	ECreateServiceSMS,
	ECreateRemoteSMS
};


void FillInSimpleMessageEntry( TMsvId aMessageId, TMsvId aParentId, TMsvId aServiceId, TMsvEntryForward& aEntry);
void FillInServiceEntry( TMsvId aMessageId, TMsvEntryForward& aEntry);
void FillInRemoteMessageEntry( TMsvId aMessageId, TMsvId aParentId, TMsvId aServiceId, TMsvEntryForward& aEntry);

class RProxyServerSession;
void CheckWentInStoreL(CMtfTestCase& aTestCase, const TDesC& aTestName,  
	RProxyServerSession& RPSS , TMsvId id, TMsvEntryForward& entry , TMsvOp& operationId );


#endif  //__IPCCOMMANDFORCAPSPERMISSIONS_H__
