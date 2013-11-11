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

#include "refpolicyevaluator.h"
#include <ecom/implementationproxy.h>
#include <ups/cliententity.h>
#include <ups/fingerprint.h>
#include <ups/upsdb.h>

using namespace UserPromptService;

static const TUint KRefPolicyEvaluatorImplementationId = 0x10283698;

CPolicyEvaluator* CRefPolicyEvaluator::CreatePolicyEvaluatorL()
/**
Factory method that instantiates a new policy evaluator ECOM plug-in.

@return A pointer to the new reference policy evaluator object.
*/
	{
	CRefPolicyEvaluator* self = new (ELeave)CRefPolicyEvaluator();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

static const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KRefPolicyEvaluatorImplementationId, CRefPolicyEvaluator::CreatePolicyEvaluatorL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
/**
Standard ECOM factory
*/
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}	
	

CRefPolicyEvaluator::CRefPolicyEvaluator()
/**
Constructor
*/
	: CPolicyEvaluator()
	{
	CActiveScheduler::Add(this);
	}
	
CRefPolicyEvaluator::~CRefPolicyEvaluator()
/**
Destructor
*/
	{
	Deque();
	delete iDigest;
	delete iClientEntity;
	}

void CRefPolicyEvaluator::ConstructL()
/**
Second phase constructor, creates the message digest
*/
	{	
	iDigest = CMessageDigestFactory::NewDigestL(CMessageDigest::EMD5);
	}


// From CActive
void CRefPolicyEvaluator::DoCancel()
	{	
	// Logically should Cancel the internal outstanding requst, but
	// currently GenerateFingerprints has already completed it.

	// And need to complete the clients request
	if (iClientStatus)
		{
		User::RequestComplete(iClientStatus, KErrCancel);
		}
	}
	
TInt CRefPolicyEvaluator::RunError(TInt aError)
	{
	if (iClientStatus)
		{
		User::RequestComplete(iClientStatus, aError);
		}
	return KErrNone;
	}
	
void CRefPolicyEvaluator::RunL()
	{
	// This plug-in doesn't use the opaque data	
	TInt opaqueDataLen = iRequest->OpaqueData().Length();
	if (opaqueDataLen > 0)
		{
		RDebug::Printf("Received %d bytes of opaque data", opaqueDataLen);
		}
		
	SetClientEntityL();
	
	// Create most specific hash first i.e. HASH(destination)
	// N.B. Normally, the destination field should be parsed so that only relevant data is hashed.		
	iDigest->Reset();
	const TDesC& d = iRequest->Destination();
	TPtrC8 p(reinterpret_cast<const TUint8*>(d.Ptr()), d.Length() * 2);
	TPtrC8 h(iDigest->Hash(p));
	
	CFingerprint* f = CFingerprint::NewLC(h, d);
	iFingerprints->AppendL(f);
	CleanupStack::Pop(f);
	
	// Create a fingerprint that matches ALL destinations (not actually a wildcard)
	// N.B. Since the format of the fingerprint is internal to the policy evaluator
	// there is no requirement to hash this value.
	_LIT8(KAllDestinations8, "*");
	_LIT16(KAllDestinations16, "*");
	f = CFingerprint::NewLC(KAllDestinations8, KAllDestinations16);
	iFingerprints->AppendL(f);
	CleanupStack::Pop(f);
		
	User::RequestComplete(iClientStatus, KErrNone);
	}

void CRefPolicyEvaluator::SetClientEntityL()
/**
Sets the name of the entity within the client process that made the request.
This is optional functionality designed to allow permissions to be set for scripts
where the scripting host process can execute a number of different scripts.
*/
	{
	// This doesn't really support client entities, so for testing purposes we
	// extract the client entity name from the opaque data.
	_LIT8(KClientEntityTagStart, "<ce>");
	_LIT8(KClientEntityTagEnd, "</ce>");
	TInt startPos;
	TInt endPos;
	if ((startPos = iRequest->OpaqueData().FindF(KClientEntityTagStart)) != KErrNotFound)
		{
		startPos += KClientEntityTagStart().Length();
		if ((endPos = iRequest->OpaqueData().FindF(KClientEntityTagEnd)) != KErrNotFound)	
			{
			if (endPos > startPos)	
				{
				TPtrC8 entityName = iRequest->OpaqueData().Mid(startPos, endPos - startPos);
				iClientEntity = CClientEntity::NewL(entityName);
				*iClientEntityPtr = iClientEntity;			
				}	
			}
		}
	}
	
void CRefPolicyEvaluator::GenerateFingerprints(
	const CPromptRequest& aRequest, const CPolicy& aPolicy, 
	RPointerArray<CFingerprint>& aFingerprints, const CClientEntity*& aClientEntity, 
	const TAny*& aDialogCreatorParams, TRequestStatus& aStatus)
	{
	iRequest = &aRequest;
	iPolicy = &aPolicy;
	iFingerprints = &aFingerprints;	
	
	iClientEntityPtr = &aClientEntity;	
	aDialogCreatorParams = 0;	// Not used by this reference plug-in
	
	iClientStatus = &aStatus;
	aStatus = KRequestPending;
	
	// Kick off policy evaluator state machine
	iStatus = KRequestPending;
	TRequestStatus* status = &iStatus;
	SetActive();
	User::RequestComplete(status, KErrNone);
	}

TBool CRefPolicyEvaluator::ForcePromptL(const CDecisionRecord& aDecision, TUint& aNewEvaluatorInfo)
	{
	// An alternative implementation of this function would be to return ETrue
	// if the usage has exceeded a threshold level.
	// N.B. It is likely that the usage figures need to be held externally to 
	// the User Prompt Service and implementors should be careful not to unecessarily
	// block this API.
	(void) aDecision;
	aNewEvaluatorInfo = aDecision.iEvaluatorInfo + 10;
#if 1
	return EFalse;
#else
	return ETrue;
#endif
	}
