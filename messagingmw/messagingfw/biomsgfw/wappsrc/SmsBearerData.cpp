// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "SmsBearerData.h"

CWapSmsBearerData*  CWapSmsBearerData::NewLC()
	{	
	CWapSmsBearerData*  self = new (ELeave) CWapSmsBearerData;
	CleanupStack::PushL(self);
	return self;
	}

CWapSmsBearerData*  CWapSmsBearerData::NewL()
	{
	CWapSmsBearerData*  self;
	self = CWapSmsBearerData::NewLC();
	CleanupStack::Pop();
	return self;
	}
