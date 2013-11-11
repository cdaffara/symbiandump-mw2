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
// RemoveMedia
// [Action Parameters]
// TInt   Drive     <input>: External drive (between D: and Y:) that we want to remove.
// [Action Description]
// Tells the message server to pretend its media is missing next time it gets a disk notification.
// [APIs Used]
// RMsvServerSession::SetFailure
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionRemoveMedia.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


CMtfTestAction* CMtfTestActionRemoveMedia::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionRemoveMedia* self = new (ELeave) CMtfTestActionRemoveMedia(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionRemoveMedia::CMtfTestActionRemoveMedia(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionRemoveMedia::~CMtfTestActionRemoveMedia()
	{
	}

void CMtfTestActionRemoveMedia::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionRemoveMedia);
	RFs fs; 
	RMsvServerSession server;
	TFileName fsName;
	
	TInt drive = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0));

	fs.Connect();
	server.Connect(fs);

	// This tells the server to pretend that its media is missing next time it gets a disk notification
	server.SetFailure(EDiskFailure, ETrue);

	// Generate a disk change notification
	fs.FileSystemName(fsName, drive);
	fs.MountFileSystem(fsName, drive);

	// Close session
	server.Close();
	fs.Close();

	User::After(100000); // wait a bit...
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionRemoveMedia);
	TestCase().ActionCompletedL(*this);
	}


