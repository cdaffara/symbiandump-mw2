// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// InsertMedia
// [Action Parameters]
// None
// [Action Description]
// Resets the Message Server following a RemoveMedia test action.
// If RemoveMedia has not been executed previously the action has no effect.
// [APIs Used]
// RMsvServerSession::SetFailure
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionInsertMedia.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "MSVSERV.H"
#include <msvapi.h>

CMtfTestAction* CMtfTestActionInsertMedia::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionInsertMedia* self = new (ELeave) CMtfTestActionInsertMedia(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionInsertMedia::CMtfTestActionInsertMedia(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionInsertMedia::~CMtfTestActionInsertMedia()
	{
	}

void CMtfTestActionInsertMedia::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionInsertMedia);
	RFs fs; 
	RMsvServerSession server;
	TFileName fsName;
	
	TInt drive = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

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
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionInsertMedia);
	TestCase().ActionCompletedL(*this);
	}


