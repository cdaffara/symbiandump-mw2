// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// 
//

#include "t_setsubsttestwrapper.h"


/*@{*/

// Commands
_LIT(KNew,									"New");
_LIT(KCreateDrive,							"CreateDrive");
_LIT(KDeleteDrive,							"DeleteDrive");

// Command parameters
_LIT(KObjectValue,							"object_value");

_LIT(KDrive,								"drive");

_LIT(KDrivePath, 							"C:\\Drive%d\\");


/*@}*/

/**
Purpose: Constructor of CT_SetSubstTestWrapper class

@internalComponent
*/
CT_SetSubstTestWrapper::CT_SetSubstTestWrapper()
:	iObject(NULL)
	{
	}
/**
Purpose: Destructor of CT_SetSubstTestWrapper class

@internalComponent
*/
CT_SetSubstTestWrapper::~CT_SetSubstTestWrapper()
	{
	delete iObject;
	iObject = NULL;

	iFs.Close();

	}
/**
Purpose: Command fuction of CT_SetSubstTestWrapper class

@internalComponent
*/
CT_SetSubstTestWrapper* CT_SetSubstTestWrapper::NewL()
	{
	CT_SetSubstTestWrapper*	ret = new (ELeave) CT_SetSubstTestWrapper();
	CleanupStack::PushL(ret);
	ret->ConstructL();
	CleanupStack::Pop(ret);
	return ret;
	}
/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
void CT_SetSubstTestWrapper::ConstructL()
	{
	iObject			= new (ELeave) TInt;
	iFs.Connect();
	}

/**
Purpose: Command fuction for a wrapper class

@internalComponent
*/
TBool CT_SetSubstTestWrapper::DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex)
	{
	TBool ret = ETrue;

	// Print out the parameters for debugging
	INFO_PRINTF2( _L("<font size=2 color=990000><b>aCommand = %S</b></font>"), &aCommand );
	INFO_PRINTF2( _L("aSection = %S"), &aSection );
	INFO_PRINTF2( _L("aAsyncErrorIndex = %D"), aAsyncErrorIndex );



	if(KNew() == aCommand)
		{
		DoCmdNewL(aSection);
		}
	else if(KCreateDrive() == aCommand)
		{
		DoCmdCreateDrive(aSection);
		}
	else if(KDeleteDrive() == aCommand)
		{
		DoCmdDeleteDrive(aSection);
		}
	else
		{
		ret = EFalse;
		}

	return ret;
	}

/**
Purpose: To create a new object of the CTEFTest type through the API.

Ini file options:
	iniData - The data from the ini file at the section provided.

@internalComponent
@param  aSection Current ini file command section
*/
void CT_SetSubstTestWrapper::DoCmdNewL(const TDesC& aSection)
	{
	TInt objectValue = 0;
	if (!GetIntFromConfig(aSection, KObjectValue(), objectValue))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KObjectValue());
		SetBlockResult(EFail);
		}
	else
		{
		delete iObject;
		iObject = new (ELeave) TInt(objectValue);
		}
	}

/**
Purpose: Create a logical drive

@internalComponent
@param  aSection Current ini file command section
*/
void CT_SetSubstTestWrapper::DoCmdCreateDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
	}

	TBuf<20> driveDir;
	driveDir.Format(KDrivePath, drive);

	iFs.MkDir(driveDir);

	iFs.SetSubst(driveDir, drive);

	SetBlockResult( EPass );
	}


/**
Purpose: Delete a logical drive

@internalComponent
@param  aSection Current ini file command section
*/
void CT_SetSubstTestWrapper::DoCmdDeleteDrive(const TDesC& aSection)
	{
	TInt drive = 0;
	if (!GetIntFromConfig(aSection, KDrive(), drive))
		{
		ERR_PRINTF2(_L("<font color=FF0000>No parameter %S</font>"), &KDrive());
		SetBlockResult(EFail);
	}

	iFs.SetSubst(_L(""), drive);

	TBuf<20> driveDir;
	driveDir.Format(KDrivePath, drive);

	iFs.RmDir(driveDir);

	SetBlockResult( EPass );
	}


