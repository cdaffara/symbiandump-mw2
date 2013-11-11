// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <f32file.h>

#include "CMtfTestActionUtilsConfigFileMachineName.h"

_LIT(KFileName,"c:\\msgtest\\MachineName.txt");
_LIT(KAtEmailAddress,"@msexchange2k.closedtest.intra");

CMtfTestActionUtilsConfigFileMachineName* CMtfTestActionUtilsConfigFileMachineName::NewL(const TDesC& aPtrEmailAdd)
	{
	CMtfTestActionUtilsConfigFileMachineName* self = new (ELeave) CMtfTestActionUtilsConfigFileMachineName();
	CleanupStack::PushL(self);
	self->ConstructL(aPtrEmailAdd);
	CleanupStack::Pop(self);
	return self;	
	}

CMtfTestActionUtilsConfigFileMachineName* CMtfTestActionUtilsConfigFileMachineName::NewLC(const TDesC& aPtrEmailAdd)
	{
	CMtfTestActionUtilsConfigFileMachineName* self = new (ELeave) CMtfTestActionUtilsConfigFileMachineName();
	CleanupStack::PushL(self);
	self->ConstructL(aPtrEmailAdd);
	return self;	
	}


CMtfTestActionUtilsConfigFileMachineName::CMtfTestActionUtilsConfigFileMachineName()
	{	
	}
	
void CMtfTestActionUtilsConfigFileMachineName::ConstructL(const TDesC& aPtrEmailAdd)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	RFile file;	
	User::LeaveIfError(file.Open(fs, KFileName, EFileRead));
	
	CleanupClosePushL(file);
	TInt  fileSize;
	User::LeaveIfError(file.Size(fileSize));
	iMachineName = HBufC8::NewL(fileSize);
	TPtr8 bufferPtr = iMachineName->Des();
	User::LeaveIfError(file.Read(bufferPtr));
	
	if (aPtrEmailAdd.Compare(_L("NONE"))==0)
		iMachineNameEmail = HBufC::NewL(iMachineName->Length() + KAtEmailAddress.iTypeLength);
	else
		iMachineNameEmail = HBufC::NewL(iMachineName->Length() + aPtrEmailAdd.Length() + 1);
	iMachineNameEmail->Des().Copy(*iMachineName);
	if (aPtrEmailAdd.Compare(_L("NONE"))==0)
		iMachineNameEmail->Des().Append(KAtEmailAddress);	
	else
		{
		iMachineNameEmail->Des().Append(_L("@"));
		iMachineNameEmail->Des().Append(aPtrEmailAdd);
		}
	
	CleanupStack::PopAndDestroy(2,&fs);
	}

const TDesC& CMtfTestActionUtilsConfigFileMachineName::MachineNameEmail()
	{
	return (*iMachineNameEmail);
	}

const TDesC8& CMtfTestActionUtilsConfigFileMachineName::MachineName()
	{
	return (*iMachineName);	
	}

CMtfTestActionUtilsConfigFileMachineName::~CMtfTestActionUtilsConfigFileMachineName()
  {
  delete iMachineName;	
  delete iMachineNameEmail;
  }
