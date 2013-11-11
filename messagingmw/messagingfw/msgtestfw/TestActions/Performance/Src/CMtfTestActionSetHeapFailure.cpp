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

/**
 @file
*/


#include "CMtfTestActionSetHeapFailure.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"
#include <msvapi.h>
//#include <msvstd.h>
#include "CMtfTestParameterStore.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>


class CSessionObserver : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};


CMtfTestAction* CMtfTestActionSetHeapFailure::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetHeapFailure* self = new (ELeave) CMtfTestActionSetHeapFailure(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	
CMtfTestActionSetHeapFailure::CMtfTestActionSetHeapFailure(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
	{
	}

void CMtfTestActionSetHeapFailure::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetHeapFailure);
	RFs fs; 
	RMsvServerSession server;
	
	fs.Connect();
	server.Connect(fs);

	server.SetFailure(EHeapFailure, RHeap::ETrueRandom);

	CSessionObserver* sessionObserver = new (ELeave) CSessionObserver;

	TRAPD(err, CMsvSession* paramSession = CMsvSession::OpenSyncL(*sessionObserver));
	ASSERT(err==KErrNoMemory);
	delete sessionObserver;
	server.SetFailure(EHeapFailure, RHeap::ENone);
	server.Close();
	fs.Close();

	User::After(100000); // wait
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetHeapFailure);
	TestCase().ActionCompletedL(*this);
	}

CMtfTestActionSetHeapFailure::~CMtfTestActionSetHeapFailure()
	{
	}
