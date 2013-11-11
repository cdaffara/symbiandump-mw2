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

#ifndef __MSVREGIP_H__
#define __MSVREGIP_H__

#include <e32base.h>




class RWriteStream;
class RReadStream;

class CMtmGroupData;
class CRegisteredMtmDllArray;
class MRegisteredMtmDllObserver;


class TMsvPackedRegisteredMtmDllArray
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C static void PackRegisteredMtmDllArrayL(RWriteStream& aWriteStream, const CRegisteredMtmDllArray& aRegisteredMtmDllArray);
	IMPORT_C static void UnpackRegisteredMtmDllArrayL(RReadStream& aReadStream, CRegisteredMtmDllArray& aRegisteredMtmDllArray, const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32, MRegisteredMtmDllObserver& aRegisteredMtmDllObserver);
	};

#endif	// __MSVREGIP_H__
