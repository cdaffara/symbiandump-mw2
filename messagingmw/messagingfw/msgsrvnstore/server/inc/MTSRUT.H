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

#if !defined(__MTSRUT_H__)
#define __MTSRUT_H__
#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined(__MSVREG_H__)
#include "MSVREG.H"
#endif


// forward declarations

class RWriteStream;
class RReadStream;

class CInstalledMtmGroup : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CInstalledMtmGroup* NewL(TUidType aUidType,CMtmGroupData* aMtmGroupData,const TDesC &aFileName);
	~CInstalledMtmGroup();
private:	
	CInstalledMtmGroup(TUidType aUidType, CMtmGroupData* aMtmGroupData);
public:
	CMtmGroupData* iMtmGroupData;
	TUidType iUidType;
	TInt iClientUsageCount;
	TBool iIsInstalled;
	HBufC* iFilename;
	};

#endif
