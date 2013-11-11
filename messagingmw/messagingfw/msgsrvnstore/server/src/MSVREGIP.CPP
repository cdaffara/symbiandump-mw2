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

#include "MSVREGIP.H"
#include "MSVREG.H"
#include "MSVPANIC.H"

#include <f32file.h>

#include <s32strm.h>



LOCAL_C void doPackRegisteredMtmDllL(RWriteStream& aWriteStream, const CRegisteredMtmDll& aRegisteredMtmDll)
	{
	aWriteStream.WriteInt32L(aRegisteredMtmDll.MtmTypeUid().iUid);
	aWriteStream.WriteInt32L(aRegisteredMtmDll.TechnologyTypeUid().iUid);
	aWriteStream << aRegisteredMtmDll.MtmDllInfo();
	}
	
EXPORT_C void TMsvPackedRegisteredMtmDllArray::PackRegisteredMtmDllArrayL(RWriteStream& aWriteStream, const CRegisteredMtmDllArray& aRegisteredMtmDllArray)
	{
	const TInt count = aRegisteredMtmDllArray.Count();
	aWriteStream.WriteInt32L(count);
	
	for (TInt ii=0; ii<count; ++ii)
		{
		doPackRegisteredMtmDllL(aWriteStream, *aRegisteredMtmDllArray[ii]);		
		}
	}
	
LOCAL_C CRegisteredMtmDll* unpackRegisteredMtmDllL(RReadStream& aReadStream, const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32, MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	TUid mtmTypeUid = TUid::Uid(aReadStream.ReadInt32L());
	TUid technologyTypeUid = TUid::Uid(aReadStream.ReadInt32L());
	
	CMtmDllInfo* mtmDllInfo = CMtmDllInfo::NewL(aReadStream);
	CleanupStack::PushL(mtmDllInfo);
	CRegisteredMtmDll* registeredMtmDll = CRegisteredMtmDll::NewL(mtmTypeUid, technologyTypeUid, *mtmDllInfo, aTimeoutMicroSeconds32, aRegisteredMtmDllObserver);
	CleanupStack::PopAndDestroy(mtmDllInfo); // mtmDllInfo has been copied
	return registeredMtmDll;
	}
	
LOCAL_C void resetAndDestroyArray(TAny* aRegisteredMtmDllArray)
	{
	reinterpret_cast<CMtmDllInfoArray*>(aRegisteredMtmDllArray)->ResetAndDestroy();
	}

EXPORT_C void TMsvPackedRegisteredMtmDllArray::UnpackRegisteredMtmDllArrayL(RReadStream& aReadStream, CRegisteredMtmDllArray& aRegisteredMtmDllArray, const TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32, MRegisteredMtmDllObserver& aRegisteredMtmDllObserver)
	{
	__ASSERT_DEBUG(aRegisteredMtmDllArray.Count()==0,PanicServer(EMsvRegisteredMtmDllArrayNotEmpty));

	CleanupStack::PushL(TCleanupItem(resetAndDestroyArray, &aRegisteredMtmDllArray));
	TInt count = aReadStream.ReadInt32L();
	for (TInt ii=0; ii<count; ++ii)
		{
		CRegisteredMtmDll* registeredMtmDll = unpackRegisteredMtmDllL(aReadStream, aTimeoutMicroSeconds32, aRegisteredMtmDllObserver);
		aRegisteredMtmDllArray.AddRegisteredMtmDllL(registeredMtmDll);
		}
	CleanupStack::Pop(&aRegisteredMtmDllArray);
	}
	

