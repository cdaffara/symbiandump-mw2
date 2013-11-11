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

#include "MTM.H"

EXPORT_C CMyMtm* CMyMtm::NewL(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll)
	{
	CMyMtm* mymtm=new(ELeave) CMyMtm(aMtmTypeUid,aRegisteredMtmDll);
	CleanupStack::PushL(mymtm);
	mymtm->ConstructL();
	CleanupStack::Pop();
	return mymtm;
	}

CMyMtm::~CMyMtm()
	{
	}

CMyMtm::CMyMtm(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll):
	CMyMtmBase(aMtmTypeUid,aRegisteredMtmDll)
	{
	__DECLARE_NAME(_S("CMyMtm"));
	}
		
void CMyMtm::ConstructL()
	{
	}

