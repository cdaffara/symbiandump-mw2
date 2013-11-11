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

#include "CWappBookmark.h"
#include <ipaddr.h>

CWappBookmark* CWappBookmark::NewL()
	{
	CWappBookmark* self = CWappBookmark::NewLC();
	CleanupStack::Pop();
	return self;
	}

CWappBookmark* CWappBookmark::NewLC()
	{
	CWappBookmark* self = new (ELeave) CWappBookmark;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CWappBookmark::ConstructL()
	{
	SetNameL(KNullDesC);
	SetUrlL(KNullDesC);
	}

CWappBookmark::~CWappBookmark()
	{
	delete iBmkName;
	delete iBmkUrl;
	}

const TDesC& CWappBookmark::Name() const
	{
	return *iBmkName; //return a constant pointer descriptor
	}

//	
const TDesC& CWappBookmark::Url() const
	{
	return iBmkUrl->Addr();
	}

//	SetNameL()	- Allocate memory and set Name field, delete 
//				  any currently allocated memory, 				  
//
void CWappBookmark::SetNameL(const TDesC& aDes)
	{
	HBufC* name = aDes.AllocL();
	delete iBmkName;
	iBmkName = name;
	}

//	SetUrlL()	- Sets URL field, deleting any currently allocated memory, setting  
//				  pointer to newly allocated HBufC
//
void CWappBookmark::SetUrlL(const TDesC& aDes)
	{
	CIpAddress* tempAddr = CIpAddress::NewL(aDes);
	delete iBmkUrl;
	iBmkUrl = tempAddr;
	}
