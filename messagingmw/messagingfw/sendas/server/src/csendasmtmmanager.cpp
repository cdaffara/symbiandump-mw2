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

#include "csendasmtmmanager.h"

#include <mtclreg.h>
#include <mtclbase.h>
#include <mtmuids.h>
#include <csendasmessagetypes.h>

#include "csendasserver.h"

// constants
const TInt KSendAsArrayGranularity = 8; // should be big enough to hold all standard mtms

CSendAsMtmManager* CSendAsMtmManager::NewL(CSendAsServer& aServer)
	{
	CSendAsMtmManager* self = new(ELeave) CSendAsMtmManager(aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CSendAsMtmManager::~CSendAsMtmManager()
	{
	// free resources
	iClientMtmArray.ResetAndDestroy();
	iClientMtmArray.Close();
	iMtmUidArray.Close();
	delete iClientRegistry;	
	}
	
CSendAsMtmManager::CSendAsMtmManager(CSendAsServer& aServer) : 
	iServer(aServer), iMtmUidArray(KSendAsArrayGranularity), 
	iClientMtmArray(KSendAsArrayGranularity)
	{
	}

void CSendAsMtmManager::ConstructL()
	{
	iClientRegistry = CClientMtmRegistry::NewL(iServer.GetMsvSessionL());
	PopulateL();
	}

/** Populates the array of available MTMs capable of sending messages

*/
void CSendAsMtmManager::PopulateL()
	{
	TInt count = iClientRegistry->NumRegisteredMtmDlls();
	for( TInt i=0; i<count; ++i )
		{
		TBool addMtmInfo = EFalse;
		TUid mtmUid = iClientRegistry->MtmTypeUid(i);
		const CMtmDllInfo& dllInfo = iClientRegistry->RegisteredMtmDllInfo(mtmUid);
		if( dllInfo.CapabilitiesAvailable() )
			{
			if( dllInfo.MessagingCapability() )
				{
				addMtmInfo = ETrue;
				}
			}
		else
			{
			CBaseMtm* mtm = NULL;
			TRAPD(err, mtm = FindStoredMtmL(mtmUid));
 			if( err != KErrPermissionDenied )
   				{
 				if( err == KErrNone )
 					{
 					TInt ignored = 0;
 					if( mtm->QueryCapability(KUidMtmQueryCanSendMsg, ignored) != KErrNotSupported )
 						{
 						addMtmInfo = ETrue;
 						}
 					}
 				else if( err != KErrNotFound )
   					{
 					User::Leave(err);
   					}
   				}
			}
		if( addMtmInfo )
			{
			iMtmUidArray.AppendL(mtmUid);
			}
		}
	}

/** Requests creation of a Client-side MTM object for the specified MTM UID.

@param  aMtmUid
The client MTM UID.

@return
CBaseMtm pointer to the created MTM.
*/
CBaseMtm* CSendAsMtmManager::GetClientMtmL(TUid aMtmUid)
	{
	return iClientRegistry->NewMtmL(aMtmUid);
	}

RArray<TUid>& CSendAsMtmManager::GetMtmUidArray()
	{
	return iMtmUidArray;
	}
	
CClientMtmRegistry* CSendAsMtmManager::GetClientMtmRegistry()
	{
	return iClientRegistry;
	}

/** Searches for previously loaded MTM.

The client MTM array is searched for MTM matching the requested UID.
If the MTM is not found, an attempt is made to create it.
 
@param  aMtmUid
The client MTM UID.

@return
CBaseMtm pointer to the requested MTM.
*/
CBaseMtm* CSendAsMtmManager::FindStoredMtmL(TUid aMtmUid)
	{
	// search mtm array for matching mtm by uid
	CBaseMtm* mtm = NULL;
	TInt count = iClientMtmArray.Count();
	for( TInt i=0; i<count; ++i )
		{
		mtm = iClientMtmArray[i];
		if( mtm && mtm->Type() == aMtmUid )
			{
			return mtm;
			}
		}

	// not found in mtm array so attempt to create it
	mtm = GetClientMtmL(aMtmUid);
	CleanupStack::PushL(mtm);

	// add to mtm array
	iClientMtmArray.AppendL(mtm);
	CleanupStack::Pop(mtm);
	return mtm;
	}

/** Empties and repopulates the client MTM array.

*/
void CSendAsMtmManager::RefreshMtmUidArrayL()
	{
	// empty list of mtm uids
	iMtmUidArray.Reset();
	// repopulate list
	PopulateL();
	}
