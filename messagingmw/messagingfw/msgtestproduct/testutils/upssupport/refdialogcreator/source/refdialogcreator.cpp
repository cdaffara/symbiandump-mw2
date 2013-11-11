// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "refdialogcreator.h"
#include <ecom/implementationproxy.h>
#include <apaid.h>
#include <apgcli.h>
#include <ups/promptrequest.h>

static const TUint KRefDialogCreatorImplementationId = 0x10283694;

//static const TUint KRefNotifierImplementationId = 0x1028369B;

CDialogCreator* CRefDialogCreator::CreateDialogCreatorL()
/**
Factory method that instantiates a new dialog creator ECOM plug-in.

@return A pointer to the new reference dialog creator object.
*/
	{
	CRefDialogCreator* self = new (ELeave)CRefDialogCreator();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

static const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KRefDialogCreatorImplementationId, CRefDialogCreator::CreateDialogCreatorL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
/**
Standard ECOM factory
*/
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}	
	

CRefDialogCreator::CRefDialogCreator() 
/**
Constructor
*/
	: CDialogCreator(), iPromptData(), iPromptDataPckg(iPromptData),
	iPromptResultPckg(iPromptResult), iState(EIdle)
	{
	CActiveScheduler::Add(this);
	}
	
CRefDialogCreator::~CRefDialogCreator()
/**
Destructor
*/
	{
	Deque();
	}

void CRefDialogCreator::ConstructL()
/**
Second phase constructor
*/
	{
	//connect to notifier server
	}

void CRefDialogCreator::DoCancel()
	{
	if (iState == EProcessResult)
		{
		//cancel the notifier operation
		}
	if (iClientStatus)
		{
		User::RequestComplete(iClientStatus, KErrCancel);
		}
	}
	
TInt CRefDialogCreator::RunError(TInt aError)
	{
	if (iClientStatus)
		{
		User::RequestComplete(iClientStatus, aError);
		}
	return KErrNone;
	}

void CRefDialogCreator::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	switch (iState)
		{
		case EPrepareDialog:
			DoPrepareDialogL();
			break;
		case EDisplayDialog:
			DoDisplayDialogL();
			break;
		case EProcessResult:
			DoProcessResultL();
			break;
		default:
			ASSERT(EFalse);			
		}
	}
	
	
	
void CRefDialogCreator::DoPrepareDialogL()
	{
	// Only one state at the moment but more should be
	// added for long running operators e.g. querying the SIS registry
	// or resolving the client entity.
	ResolveClientNameL();
	
	// Get the vendor name for the client process
	ResolveVendorNameL(iRequest->ClientVid());
	
	// Server / Service localized names generated in notifier plug-in. 
	iPromptData.iServerSid = iRequest->ServerSid();
	iPromptData.iServiceId = iRequest->ServiceId();
	
	// Use the options specified by the policy
	iPromptData.iOptions = iPolicy->Options();
	
	// Add the descriptions of the fingerprints 
	// The reference implementation only allows two fingerprints for a prompt
	// but other implementations could support more.
	// E.g. a specific phone number OR all phone numbers
	if (iFingerprints->Count() > 0)
		{
		iPromptData.iDestination1.Copy((*iFingerprints)[0]->Description());
		iPromptData.iNumDestinations++;
		}
		
	if (iFingerprints->Count() > 1)
		{
		iPromptData.iDestination2.Copy((*iFingerprints)[1]->Description());
		iPromptData.iNumDestinations++;
		}
	User::RequestComplete(iClientStatus, KErrNone);		
	// DisplayDialog is invoked by the UPS, this just verifies 
	// that PrepareDialog was called first.
	iState = EDisplayDialog;
	}
 
	
 void CRefDialogCreator::DoDisplayDialogL() 
/**
Uses the notifier framework to display the dialog.
*/
	{
	TRequestStatus* status = &iStatus;
	SetActive();
	User::RequestComplete(status, KErrNone);
	iState = EProcessResult;
	}

	
void CRefDialogCreator::DoProcessResultL()
/**
Set the user response by reading strings form the file a
*/
	{
 	_LIT(KUserResPonseFile,"c:\\UserResponse.txt");
 	_LIT8(KUserResponseYes, "YES");
 	_LIT8(KUserResponseSession, "SESSION");
 	_LIT8(KUserResponseAlways, "ALWAYS");
// 	_LIT8(KUserResponseNo, "NO"); Not required as the default response would be NO
 	_LIT8(KUserResponseNever, "NEVER");
 	
 
 	RFs fileServerSession;
 	fileServerSession.Connect();
 	RFile file;
 	
 	
 	file.Open(fileServerSession,KUserResPonseFile,EFileRead);
 	TBuf8<8> userResponse;
 	file.Read(userResponse);
 	file.Close();
 	userResponse.UpperCase();
 	TPromptResult policyEnum;
	if(userResponse.CompareC(KUserResponseYes) == 0)
		{
		iPromptResult.iSelected=CPolicy::EYes;
		}
	else if(userResponse.CompareC(KUserResponseSession) == 0)
		{
		iPromptResult.iSelected = CPolicy::ESession;
		}
	else if(userResponse.CompareC(KUserResponseAlways) == 0)
		{
		iPromptResult.iSelected = CPolicy::EAlways;
		}
	else if(userResponse.CompareC(KUserResponseNever) == 0)
		{
		iPromptResult.iSelected = CPolicy::ENever;
		}
	else
		{
		iPromptResult.iSelected = CPolicy::ENo;
		}
		
	if(iPromptResult.iSelected == CPolicy::EAlways || iPromptResult.iSelected == CPolicy::ENever)
	 	{ 
		// The Always or Never option was selected so return the fingerprint 
		// for the new decision record.
		// 
		// In this implementation a copy of the original fingerprint is returned. However,
		// it is permitted to return a different fingerprint e.g. a modifier description.		
		if (iPromptResult.iDestination >= 0 && iPromptResult.iDestination < iFingerprints->Count())		
			{
			*iFingerprint = (*iFingerprints)[iPromptResult.iDestination];
			}
		else
			{
			ASSERT(EFalse);
			}

	 	}
	*iOptionSelected =	iPromptResult.iSelected;
	*iEvaluatorInfo = 0;	// TODO add example
	iState = EIdle;
	User::RequestComplete(iClientStatus, KErrNone);		
	}

void CRefDialogCreator::ResolveVendorNameL(TVendorId aVid)
/**
Looks up the localized vendor name for the client process and writes
this to iPromptData.iVendorName.

Typically, this would be resolved from the SIS registry or a lookup table.

@param aVid	The vendor id of the client process.
*/
	{
	// TODO SIS registry lookup	
	if (aVid.iId == 0x70000001)
		{
		_LIT(KSymbian, "Symbian Software Ltd");
		iPromptData.iVendorName.Copy(KSymbian);
		}
	else 
		{
		_LIT(KUnknown, "Unknown vendor");
		iPromptData.iVendorName.Copy(KUnknown);
		}
	}
	
void CRefDialogCreator::ResolveClientNameL()
/**
Generates a human readable name for the client process. In order of 
preference the following data is returned
*/
	{
	RThread clientThread;
	User::LeaveIfError(clientThread.Open(iRequest->ClientThreadId()));
	CleanupClosePushL(clientThread);
	
	RProcess clientProcess;
	User::LeaveIfError(clientThread.Process(clientProcess));
	CleanupClosePushL(clientProcess);
	iPromptData.iClientName.Copy(clientProcess.FileName());
	
	CleanupStack::PopAndDestroy(2, &clientThread); // clientThread, clientProcess	
	}


// From CDialogCreator
void CRefDialogCreator::PrepareDialog(
	const UserPromptService::CPromptRequest& aRequest, const CPolicy& aPolicy,			
	const RPointerArray<CFingerprint>& aFingerprints, const CClientEntity* aClientEntity,
	const TAny* aEvalPrivateData, TRequestStatus& aStatus)
	{
	aStatus = KRequestPending;
	iClientStatus = &aStatus;
	
	iRequest = &aRequest;
	iPolicy = &aPolicy;
	iFingerprints = &aFingerprints;
	iEvalPrivateData = aEvalPrivateData;
	(void) aClientEntity;

	// Kick off dialog creator state machine
	iState = EPrepareDialog;
	iStatus = KRequestPending;
	TRequestStatus* status = &iStatus;
	SetActive();
	User::RequestComplete(status, KErrNone);
	}
	
void CRefDialogCreator::DisplayDialog(CPolicy::TOptions& aOptions, const CFingerprint*& aFingerprint,
		TUint& aEvaluatorInfo, TRequestStatus& aStatus)
	{	
	aStatus = KRequestPending;
	iClientStatus = &aStatus;
	
	iOptionSelected = &aOptions;
	iFingerprint = &aFingerprint;
	aFingerprint = 0;
	iEvaluatorInfo = &aEvaluatorInfo;
	iClientStatus = &aStatus;
	
	// Start state machine
	ASSERT(iState == EDisplayDialog); // PrepareDialog should have been called first
	iStatus = KRequestPending;
	TRequestStatus* status = &iStatus;
	SetActive();
	User::RequestComplete(status, KErrNone);
	}

