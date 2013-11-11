// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "CCopyOneFile.h"

const TInt KCopyFileBufferSize=32768;

CCopyOneFile* CCopyOneFile::NewL(RFs &aFs)
	{
	CCopyOneFile *self=new (ELeave) CCopyOneFile(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return(self);
	}

void CCopyOneFile::Copy(const TFileName &aFrom, const TFileName &aTo,TRequestStatus& aStatus)
	{
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	TInt error=iFrom.Open(iFs,aFrom,EFileRead|EFileShareReadersOnly);
	if(error==KErrNone)
		{
		error=iTo.Replace(iFs,aTo,EFileWrite|EFileShareExclusive);
		if(error==KErrNone)
			{
			iFrom.Read(*iFileBuffer,KCopyFileBufferSize,iStatus);
			iReading=ETrue;
			SetActive();
			}
		}
	if(error!=KErrNone)
		Complete(error);
	}

void CCopyOneFile::Copy(RFile& aFrom, RFile& aTo, TRequestStatus& aStatus)
	{	
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	
	iFrom = aFrom;
	iTo = aTo;

	iFrom.Read(*iFileBuffer,KCopyFileBufferSize,iStatus);
	iReading=ETrue;
	SetActive();
	}

void CCopyOneFile::RunL()
	{
	User::LeaveIfError(iStatus.Int());

	if(iReading==EFalse)
		{
		if(iFileBuffer->Length()<KCopyFileBufferSize)
			// didn't read a full buffer last time, so its the end
			Complete(KErrNone);
		else
			{
			iFrom.Read(*iFileBuffer,iStatus);
			iReading=ETrue;
			SetActive();
			}
		}
	else
		{
		iTo.Write(*iFileBuffer,iStatus);
		iReading=EFalse;
		SetActive();
		}
	}

void CCopyOneFile::DoCancel()
	{
	Complete(KErrCancel);
	}

TInt CCopyOneFile::RunError(TInt aError)
	{
	Complete(aError);
	return(KErrNone);
	}

void CCopyOneFile::ConstructL()
	{
	iFileBuffer= new (ELeave) TBuf8<KCopyFileBufferSize>;
	}

CCopyOneFile::CCopyOneFile(RFs &aFs) : CActive(EPriorityStandard), iFs(aFs)
	{
	}

CCopyOneFile::~CCopyOneFile()
	{
	Cancel();
	delete iFileBuffer;
	}

void CCopyOneFile::Complete(TInt aError)
	{
	iFrom.Close();
	iTo.Close();
	if(iReportStatus) 
		User::RequestComplete(iReportStatus,aError);
	}

