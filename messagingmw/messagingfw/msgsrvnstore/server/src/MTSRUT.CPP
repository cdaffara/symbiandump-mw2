// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <s32file.h>

#include "MTSRUT.H"
#include "MSVPANIC.H"



CInstalledMtmGroup* CInstalledMtmGroup::NewL(TUidType aUidType,CMtmGroupData* aMtmGroupData,const TDesC& aFileName)
	{
	CleanupStack::PushL(aMtmGroupData); // we take ownership of this
	CInstalledMtmGroup* self=new(ELeave) CInstalledMtmGroup(aUidType, aMtmGroupData);
	CleanupStack::Pop(aMtmGroupData); // constructor took ownership
	CleanupStack::PushL(self);
	self->iFilename=aFileName.AllocL();
	CleanupStack::Pop();
	return self;
	}

CInstalledMtmGroup::~CInstalledMtmGroup()
	{
	__ASSERT_DEBUG(iClientUsageCount==0,PanicServer(EMtsrInstalledMtmGroupStillInUse));
	delete iMtmGroupData;
	delete iFilename;
	}


CInstalledMtmGroup::CInstalledMtmGroup(TUidType aUidType, CMtmGroupData* aMtmGroupData):
	iMtmGroupData(aMtmGroupData),
	iUidType(aUidType),
	iIsInstalled(ETrue)
	{
	__DECLARE_NAME(_S("CInstalledMtmGroup"));
	}

