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
// OBSVREG.CPP
//

#include "MSVRUIDS.H"
#include "MTCLREG.H"
#include "MTCLBASE.H"
#include "MSVPANIC.H"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"				
#endif

/** Constructor.

@param aMsvSession Message server session
@param aDllTypeUid MTM component type UID, e.g. KUidMtmClientComponent to handle client MTMs
@param aTimeoutMicroSeconds32 Time to wait before unloading MTM DLLs 
*/
EXPORT_C CObserverRegistry::CObserverRegistry(CMsvSession& aMsvSession,TUid aDllTypeUid,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	: CMtmDllRegistry(aMsvSession.FileSession(), aDllTypeUid, aTimeoutMicroSeconds32), iMsvSession(aMsvSession)
	{
	// initialise registration data
	};


/**
Destructor.
*/
EXPORT_C CObserverRegistry::~CObserverRegistry()
	{
	if (iIsAdded)
		iMsvSession.RemoveObserver(*this);
	};


/** Second-phase constructor.
*/
EXPORT_C void CObserverRegistry::ConstructL()
	{
	// announce our presence to the session
	iMsvSession.AddObserverL(*this);
	iIsAdded=ETrue;

	// load up all the installed DLLs that we need
	User::LeaveIfError(iMsvSession.iSession->FillRegisteredMtmDllArray(iMtmDllTypeUid,iRegisteredMtmDllArray,iTimeoutMicroSeconds32));
	};


/** Implements the session observer interface to update the registry information 
when MTM groups are installed or uninstalled.

@param aEvent Event type
@param aArg1 Event type-specific argument value (unused)
@param aArg2 Event type-specific argument value
@param aArg3 Event type-specific argument value (unused)
@see MMsvSessionObserver::HandleSessionEventL()
*/
EXPORT_C void CObserverRegistry::HandleSessionEventL(TMsvSessionEvent aEvent, TAny* /*aArg1*/, TAny* aArg2, TAny* /*aArg3*/)
	{
	switch (aEvent)
		{
		case EMsvMtmGroupInstalled:
			{
			TUid* mtmtypeuid=(TUid*) aArg2;
			if (!IsPresent(*mtmtypeuid))
				{
				CMtmGroupData* mtmgroupdata=iMsvSession.iSession->GetMtmGroupDataL(*mtmtypeuid);

				TUid mtmdlltypeuid[KMsvNumMtmDllTypes];  //  There must be an easier way to construct the array
				mtmdlltypeuid[EMtsrServerComponentIndex]	=KUidMtmServerComponent;
				mtmdlltypeuid[EMtsrClientComponentIndex]	=KUidMtmClientComponent;
				mtmdlltypeuid[EMtsrUiComponentIndex]		=KUidMtmUiComponent;
				mtmdlltypeuid[EMtsrUiDataComponentIndex]	=KUidMtmUiDataComponent;
/*
				mtmdlltypeuid[EMtsrDllTupe1Index]=KMsvDllType1Uid;
				mtmdlltypeuid[EMtsrDllType2Index]=KMsvDllType2Uid;
				mtmdlltypeuid[EMtsrDllType3Index]=KMsvDllType3Uid;
				mtmdlltypeuid[EMtsrDllType4Index]=KMsvDllType4Uid;
				mtmdlltypeuid[EMtsrDllType5Index]=KMsvDllType5Uid;
				mtmdlltypeuid[EMtsrDllType6Index]=KMsvDllType6Uid;
*/
				TInt index=0;
				for (; (index<KMsvNumMtmDllTypes) && (iMtmDllTypeUid!=mtmdlltypeuid[index]); index++)
					{
					}
				if (!(index==KMsvNumMtmDllTypes))
					User::LeaveIfError(AddRegisteredMtmDll(mtmgroupdata->MtmTypeUid(),mtmgroupdata->TechnologyTypeUid(),*mtmgroupdata->MtmDllInfoArray()[index],*iMsvSession.iSession));
				delete mtmgroupdata;
				}
			break;
			}
		case EMsvMtmGroupDeInstalled:
			{
			TUid* mtmtypeuid=(TUid*) aArg2;
			if (IsPresent(*mtmtypeuid))
				RemoveRegisteredMtmDll(*mtmtypeuid);         
			break;
			}
		default:
			break;
		}
	};
