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
// @file
// [TestStep Name]
// InsertMedia
// [InsertMedia]
// Drive     		: External drive (between D: and Y:) that we want to insert.
// Insert the media
// [APIs Used]
// RMsvServerSession::SetFailure
// 
//


#include "T_InsertMedia.h"


// Literals used
_LIT(KDrive,"Drive");


/**
  Function 		: CT_InsertMedia
  Description 	: Constructor
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_InsertMedia::CT_InsertMedia(CT_MsgSharedDataImap& aSharedDataIMAP)
:CT_MsgAsyncStepIMAP(aSharedDataIMAP)
	{
	SetTestStepName(KInsertMedia);
	}

/**
  Function 		: ~CT_InsertMedia
  Description 	: Destructor
  @return 		: N/A
*/
CT_InsertMedia::~CT_InsertMedia()
	{}

/**
  Function 		: NewL
  Description 	: Creates an object of CT_InsertMedia
  @param 		: aSharedDataIMAP	Reference to CT_MsgSharedDataImap
  @return 		: N/A
*/
CT_InsertMedia* CT_InsertMedia::NewL(CT_MsgSharedDataImap& aSharedDataIMAP)
	{
	CT_InsertMedia* self = new(ELeave) CT_InsertMedia(aSharedDataIMAP);
	CleanupStack::PushL(self);
	self->ConstructL();		// Call CT_MsgAsyncStepIMAP::ConstructL()
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function 		: ProgressL
  Description 	: Displays the status of the asynchronous operation
  @param 		: bFinal	Variable of TBool
  @return 		: N/A
*/
void CT_InsertMedia::ProgressL(TBool /*bFinal*/)
	{}

/**
  Function 		: CancelStep
  Description 	: Cancels the asynchronous operation
  @return 		: N/A
*/
void CT_InsertMedia::CancelStep()
	{}

/**
  Function 		: doTestStepL
  Description 	: 
  @return 		: TVerdict - Test step result
*/
TVerdict CT_InsertMedia::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step: InsertMedia"));

	RFs fs; 
	RMsvServerSession server;
	TFileName fsName;
	
	TInt drive = 0;
	GetIntFromConfig(ConfigSection(), KDrive, drive);

	fs.Connect();
	server.Connect(fs);

	// This tells the server to pretend that its media is missing next time it gets a disk notification
	server.SetFailure(EDiskFailure, EFalse);

	// Generate a disk change notification
	fs.FileSystemName(fsName, drive);
	fs.MountFileSystem(fsName, drive);

	// Close session
	server.Close();
	fs.Close();

	User::After(100000); // wait a bit...
	
	return TestStepResult();
	}


