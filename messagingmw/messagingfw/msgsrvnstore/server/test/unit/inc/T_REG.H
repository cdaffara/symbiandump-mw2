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

#if !defined(__T_REG_H__)
#define __T_REG_H__

#include <msvreg.h>
#include "msvregip.h"

const TUid KDummyMtm1Version2Uid={0x112a4845};
const TUid KDummyMtm2Version2Uid={0x112a4846};
const TUid KDummyMtm3Version2Uid={0x112a4847};

class CMyMtmBase : public CBase
	{
public:
	virtual ~CMyMtmBase() {iRegisteredMtmDll.ReleaseLibrary();}
	TUid Type() const {return iMtmTypeUid;}
protected:
	CMyMtmBase(TUid aMtmTypeUid,CRegisteredMtmDll& aRegisteredMtmDll):iRegisteredMtmDll(aRegisteredMtmDll),iMtmTypeUid(aMtmTypeUid) {}
private:
	CRegisteredMtmDll& iRegisteredMtmDll;  // Required in order to call ReleaseLibrary()
	TUid iMtmTypeUid;
	};


#endif
