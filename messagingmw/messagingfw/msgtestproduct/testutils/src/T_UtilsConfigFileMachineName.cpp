// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

// User include
#include <t_utilsconfigfilemachinename.h>

// Epoc include
#include <f32file.h>


//Literals used
_LIT(KFileName,"c:\\msgtest\\MachineName.txt");
_LIT(KAtEmailAddress,"@msexchange2k.closedtest.intra");


/**
NewL()
Allocates and creates a new CT_MsgConnectPop3Server object

@param aPtrEmailAdd
@leave KErrNoMemory
@return
The newly created CT_MsgUtilsConfigFileMachineName object
*/
EXPORT_C CT_MsgUtilsConfigFileMachineName* CT_MsgUtilsConfigFileMachineName::NewL(const TDesC& aPtrEmailAdd)
	{
	CT_MsgUtilsConfigFileMachineName* self = new (ELeave) CT_MsgUtilsConfigFileMachineName();
	CleanupStack::PushL(self);
	self->ConstructL(aPtrEmailAdd);
	CleanupStack::Pop(self);
	return self;	
	}


/**
NewLC()
Allocates and creates a new CT_MsgUtilsConfigFileMachineName object, leaving the 
object on the cleanup stack.

@param aPtrEmailAdd
@leave KErrNoMemory
@return
The newly created CT_MsgUtilsConfigFileMachineName object
*/
EXPORT_C CT_MsgUtilsConfigFileMachineName* CT_MsgUtilsConfigFileMachineName::NewLC(const TDesC& aPtrEmailAdd)
	{
	CT_MsgUtilsConfigFileMachineName* self = new (ELeave) CT_MsgUtilsConfigFileMachineName();
	CleanupStack::PushL(self);
	self->ConstructL(aPtrEmailAdd);
	return self;	
	}


/**
CT_MsgUtilsConfigFileMachineName()
Constructor
*/
EXPORT_C CT_MsgUtilsConfigFileMachineName::CT_MsgUtilsConfigFileMachineName()
	{	
	}


/**
ConstructL()
Reads the machine name from the Machine config file("c:\\msgtest\\MachineName.txt")
and constructs the data members.

@param aPtrEmailAdd
*/	
EXPORT_C void CT_MsgUtilsConfigFileMachineName::ConstructL(const TDesC& aPtrEmailAdd)
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


/**
MachineNameEmail()
Returns an email address with the host machine name.

@return
Email address of the machine
*/
EXPORT_C const TDesC& CT_MsgUtilsConfigFileMachineName::MachineNameEmail()
	{
	return (*iMachineNameEmail);
	}


/**
MachineName()
Returns the host machine name

@return
Machine name
*/
EXPORT_C const TDesC8& CT_MsgUtilsConfigFileMachineName::MachineName()
	{
	return (*iMachineName);	
	}
	

/**
~CT_MsgUtilsConfigFileMachineName()
Destructor
*/
CT_MsgUtilsConfigFileMachineName::~CT_MsgUtilsConfigFileMachineName()
  {
  delete iMachineName;	
  delete iMachineNameEmail;
  }

