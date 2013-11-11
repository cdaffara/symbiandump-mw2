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

#if !defined(__MTM_H__)
#define __MTM_H__

#include "T_REG.H"

class CMyMtm : public CMyMtmBase
	{
public:
	IMPORT_C static CMyMtm* NewL(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll);
	~CMyMtm();

private:
	CMyMtm(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll);
	void ConstructL();
	};

#endif
