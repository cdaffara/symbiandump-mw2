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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// GetStore
// [Action Parameters]
// Entry			<input>: Reference to the entry.
// ReadOnly			<optional>: Flag to indicate if read only access to store is required,default is 1.
// Store			<output>: Reference to store.
// [Action Description]
// Provides the store.
// [APIs Used]
// CMsvEntry::ReadStoreL
// CMsvEntry::EditStoreL
// __ACTION_INFO_END__
// 
//

#include <e32std.h>

#include "CMtfTestActionGetStore.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>


CMtfTestAction* CMtfTestActionGetStore::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetStore* self = new(ELeave) CMtfTestActionGetStore(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetStore::CMtfTestActionGetStore(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetStore::~CMtfTestActionGetStore()
	{
	}

void CMtfTestActionGetStore::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionAddGetStore);
	CMsvEntry* paramEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),ActionParameters().Parameter(0));
	TInt readOnlyStore = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1), 1);
	
	CMsvStore* store = NULL;
	
	if ( readOnlyStore == 1 )
		{
		if (paramEntry->HasStoreL())
			{					
			store = paramEntry->ReadStoreL();
			}
		else
			{
			User::Leave(KErrNotFound);
			}
		}
	else
		{
		store = paramEntry->EditStoreL();	
		}
		
	StoreParameterL<CMsvStore>(TestCase(),*store,ActionParameters().Parameter(2));
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionAddGetStore);
	TestCase().ActionCompletedL(*this);
	}



