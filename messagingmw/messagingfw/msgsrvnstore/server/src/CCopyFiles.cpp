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

#include "CCopyFiles.h"
#include "CCopyOneFile.h"



CCopyFiles* CCopyFiles::NewL(RFs &aFs)
	{
	CCopyFiles *self=new (ELeave) CCopyFiles(aFs);
	CleanupStack::PushL(self);
	self->ConstructL();
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return(self);
	}

void CCopyFiles::CopyDir(const TFileName &aFrom, const TFileName &aTo,TRequestStatus& aStatus)
	{
	aStatus=KRequestPending;
	iReportStatus=&aStatus;
	iFromPath=&aFrom;
	iToPath=&aTo;
	iStartedCopy=EFalse;
	delete iNext;
	iNext=NULL;
	iPos=0;
	TRequestStatus *status=&iStatus;
	User::RequestComplete(status,KErrNone);
	SetActive();
	}


void CCopyFiles::RunL()
	{
	if(iStartedCopy==EFalse)
		{
		iStartedCopy=ETrue;
		iDirScan->SetScanDataL(*iFromPath,KEntryAttNormal,ESortNone);
		}

	if(iStatus!=KErrNone)
		User::RequestComplete(iReportStatus,iStatus.Int());
	else
		if(iNext!=NULL && iPos<iNext->Count())
			CopyFileL();
		else
			NextDirL();
	}


void CCopyFiles::CopyFileL()
	{
	/* If you are copying c:\System\ to c:\copyto\
	   AbbrevitedPath() would return \Apps\Shell\
	   e.iName would contain Shell.ini
	   and FullPath would contain c:\System\Apps\Shell\
	   so we copy from <FullPath><e.iName>
	   to <Target dir><AbbriviatedPath><e.iName>
	   it overflows if the target would be greater than 256 characters
	  which is the maximum length of a filepath in epoc */

	const TEntry& e = (*iNext)[iPos];
	iTo->Zero();
	TPtrC abrevPath=iDirScan->AbbreviatedPath();
	if((*iToPath).Length() + abrevPath.Length()+e.iName.Length()> iTo->MaxLength())
		User::Leave(KErrOverflow);
	iTo->Append(*iToPath);
	if((*iTo)[iTo->Length()-1]=='\\')
		iTo->SetLength(iTo->Length()-1);
	iTo->Append(abrevPath);
	iTo->Append(e.iName);

	iFrom->Zero();
	TPtrC fullPath=iDirScan->FullPath();
	if(fullPath.Length()+e.iName.Length()> iFrom->MaxLength())
		User::Leave(KErrOverflow);

	iFrom->Append(fullPath);
	iFrom->Append(e.iName);
	iCopyOneFile->Copy(*iFrom,*iTo,iStatus);
	iPos++;
	SetActive();
	}



void CCopyFiles::NextDirL()
	{
	delete iNext;
	iNext=NULL;
	iDirScan->NextL(iNext);

	if(iNext!=NULL)
		{
		// Create the target directory,
		// see comment in void CCopyFiles::CopyFileL() for how this works,
		iTo->Zero();		
		TPtrC abrevPath=iDirScan->AbbreviatedPath();
		if((*iToPath).Length() + abrevPath.Length() > iTo->MaxLength())
			User::Leave(KErrOverflow);
		iTo->Append(*iToPath);
		if((*iTo)[iTo->Length()-1]=='\\')
			iTo->SetLength(iTo->Length()-1);
		iTo->Append(abrevPath);
		iFs.MkDirAll(*iTo);

		// kick off my RunL
		TRequestStatus *status=&iStatus;
		User::RequestComplete(status,KErrNone);
		SetActive();
		}
	else // no more dirs so complete request
		Complete(KErrNone);
	}


void CCopyFiles::Complete(TInt aError)
	{
	delete iNext;
	iNext=NULL;
	User::RequestComplete(iReportStatus, aError);
	}

void CCopyFiles::DoCancel()
	{
	iCopyOneFile->Cancel();
	Complete(KErrCancel);
	}

TInt CCopyFiles::RunError(TInt aError)
	{
	if(iReportStatus)
		User::RequestComplete(iReportStatus,aError);
	return(KErrNone);
	}

void CCopyFiles::ConstructL()
	{
	iDirScan= CDirScan::NewL(iFs);
	iCopyOneFile=CCopyOneFile::NewL(iFs);
	iTo=new (ELeave) TFileName;
	iFrom=new (ELeave) TFileName;
	}

CCopyFiles::CCopyFiles(RFs &aFs) : CActive(EPriorityStandard), iFs(aFs)
	{
	}

CCopyFiles::~CCopyFiles()
	{
	Cancel();
	delete iDirScan;
	delete iCopyOneFile;
	delete iTo;
	delete iFrom;
	}

