// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <caf/cafplatform.h>
#include "Reftestagentdata.h"
#include "drmfiles.h"

using namespace ContentAccess;

CRefTestAgentData* CRefTestAgentData::NewL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode)
	{
	CRefTestAgentData* self = NewLC(aVirtualPath, aShareMode);
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentData* CRefTestAgentData::NewLC(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode)
	{
	CRefTestAgentData* self=new(ELeave) CRefTestAgentData();
	CleanupStack::PushL(self);
	self->ConstructL(aVirtualPath, aShareMode);
	return self;
	}

CRefTestAgentData* CRefTestAgentData::NewL(RFile& aFile, const TDesC& aUniqueId)
	{	
	CRefTestAgentData* self = NewLC(aFile, aUniqueId);
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentData* CRefTestAgentData::NewLC(RFile& aFile, const TDesC& aUniqueId)
	{
	CRefTestAgentData* self=new(ELeave) CRefTestAgentData();
	CleanupStack::PushL(self);
	self->ConstructL(aFile, aUniqueId);
	return self;
	}

CRefTestAgentData::CRefTestAgentData()
	{
	}

CRefTestAgentData::~CRefTestAgentData()
	{
	iServer.Close();
	}
  
void CRefTestAgentData::ConstructL(const TVirtualPathPtr& aVirtualPath, TContentShareMode aShareMode)
	{
	// default share mode of EFileShareReadersOnly
	TUint mode = EFileStream | EFileRead;

	if(aShareMode == EContentShareReadWrite)
		{
		mode |= EFileShareReadersOrWriters;
		}
	else if (aShareMode == EContentShareExclusive)
		{
		mode |= EFileShareExclusive;
		}
	else
		{
		mode |= EFileShareReadersOnly;
		}

	User::LeaveIfError(iServer.Open(aVirtualPath.URI(), aVirtualPath.UniqueId(), mode));
	}


void CRefTestAgentData::ConstructL(RFile& aFile, const TDesC& aUniqueId)
	{
	User::LeaveIfError(iServer.Open(aFile, aUniqueId));
	}

void CRefTestAgentData::DataSizeL(TInt &aSize)
	{
	User::LeaveIfError(iServer.DataSize(aSize));
	}

TInt CRefTestAgentData::EvaluateIntent(TIntent aIntent)
	{
	// Always allow EPeek
	TInt err = KErrNone;
	if(aIntent != EPeek)
		{
		TRAP(err, iServer.EvaluateIntentL(aIntent));
		}
	return err;
	}

TInt CRefTestAgentData::ExecuteIntent(TIntent aIntent)
	{
	// Always allow EPeek
	TInt err = KErrNone;
	if(aIntent != EPeek)
		{
		TRAP(err, iServer.ExecuteIntentL(aIntent));
		}
	return err;
	}

TInt CRefTestAgentData::Read(TDes8& aDes) 
	{
	return iServer.Read(aDes, aDes.MaxLength());
	}

TInt CRefTestAgentData::Read(TDes8& aDes,TInt aLength) 
	{
	return iServer.Read(aDes, aLength);
	}

void CRefTestAgentData::Read(TDes8& aDes,TRequestStatus& aStatus) 
	{
	TInt err = Read(aDes);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

void CRefTestAgentData::Read(TDes8& aDes,
							 TInt aLength, 
							 TRequestStatus& aStatus) 
	{
	TInt err = Read(aDes, aLength);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	}

TInt CRefTestAgentData::Seek(TSeek aMode, TInt& aPos) 
	{
	return iServer.Seek(aMode, aPos);	
	}

TInt CRefTestAgentData::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrCANotSupported;
	}

TInt CRefTestAgentData::GetAttribute(TInt aAttribute, TInt& aValue)
	{
	return iServer.GetAttribute(aAttribute, aValue);
	}

TInt CRefTestAgentData::GetAttributeSet(RAttributeSet& aAttributeSet)
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iServer.GetAttributeSetL(aAttributeSet));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

TInt CRefTestAgentData::GetStringAttribute(TInt aAttribute, TDes& aValue)
	{
	return iServer.GetStringAttribute(aAttribute, aValue);
	}

TInt CRefTestAgentData::GetStringAttributeSet(RStringAttributeSet& aStringAttributeSet)
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iServer.GetStringAttributeSetL(aStringAttributeSet));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}
TInt CRefTestAgentData::Read(TInt aPos, TDes8& aDes, TInt aLength, TRequestStatus& aStatus)
	{
	TInt err = iServer.Read(aPos, aDes, aLength);
	TRequestStatus* status = &aStatus;
	User::RequestComplete(status, err);
	return KErrNone;
	}

