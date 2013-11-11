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
// dcffile.cpp
// 
//

#include <caf/caf.h>
#include "TestAgentFile.h"

_LIT8(KDrmMimeType, "application/testagent.drm");

CTestAgentFile* CTestAgentFile::NewL(RFile& aFile)
	{
	CTestAgentFile *self = new (ELeave) CTestAgentFile(aFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTestAgentFile::CTestAgentFile(RFile& aFile) : iFile(aFile)
	{
	}

CTestAgentFile::~CTestAgentFile()
	{	
	}	

void CTestAgentFile::ConstructL()
	{

	}

TInt CTestAgentFile::Read(TDes8& aDes,TInt aLength)
	{
	return iFile.Read(aDes, aLength);
	}

void CTestAgentFile::Read(TDes8& aDes, TInt aLength, TRequestStatus& aStatus)
	{
	iFile.Read(aDes, aLength, aStatus);
	}

TInt CTestAgentFile::Seek(TSeek aMode, TInt& aPos)
	{
	return iFile.Seek(aMode, aPos);
	}

TPtrC8 CTestAgentFile::ContentMimeType()
	{
	return KDrmMimeType();
	}

TInt CTestAgentFile::GetAttribute(TInt aAttribute, TInt& aValue)
	{
	TInt err = KErrNone;
	
	switch(aAttribute)
		{
	case ContentAccess::EIsProtected:
		aValue = EFalse;
		break;
	case ContentAccess::EIsForwardable:
		aValue = ETrue;
		break;
	case ContentAccess::EIsModifyable:
		aValue = EFalse;
		break;
	case ContentAccess::EIsCopyable:
		aValue = ETrue;
		break;
	case ContentAccess::EContentCDataInUse:
		aValue = KErrCANotSupported;
		break;
	case ContentAccess::ECopyPaste:
		aValue = ETrue;	
		break;
	case ContentAccess::ECanMove:
		aValue = ETrue;
		break;
	case ContentAccess::ECanRename:
		aValue = ETrue;
		break;
	default:
		err = KErrCANotSupported;
		break;
		};
	return err;
	}
	
TInt CTestAgentFile::GetStringAttribute(TInt aAttribute, TDes& aValue)
	{
	TInt err = KErrNone;
	
	// the file itself
	switch(aAttribute)
		{
		case ContentAccess::EMimeType:
			aValue.Copy(KDrmMimeType());
			break;
		default:
			err = KErrCANotSupported;
			break;
		};
	return err;	
	}
	
void CTestAgentFile::DataSizeL(TInt &aSize)
	{
	iFile.Size(aSize);
	}
	

