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
//

/**
 @file
*/


#include "CMtfTestActionSampleActionA.h"
#include "CMtfTestActionManager.h"
#include "CMtfTestServer.h"
#include "CMtfTestCase.h"
#include <msvapi.h>
#include <msvstd.h>
//#include "CMtfTestParameterStore.h"

CMtfTestAction* CMtfTestActionSampleActionA::NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* aActionParameters)
{
	CMtfTestActionSampleActionA* self = new (ELeave) CMtfTestActionSampleActionA(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
}
	
CMtfTestActionSampleActionA::CMtfTestActionSampleActionA(CMtfTestCase& aTestCase)
: CMtfSynchronousTestAction(aTestCase)
{
}

void CMtfTestActionSampleActionA::ExecuteActionL()
{
	_LIT(KParam,"param");
	_LIT(KParam2,"p2");
	_LIT(KParam3,"p3");
	_LIT(KNum,"453");
	_LIT(KTest,"\"test\"");
	TBuf<5> buf(KParam);
	CMsvEntryFilter* filter = CMsvEntryFilter::NewL();
	HBufC* buf2=buf.AllocL();
	TInt i(5);
	StoreParameterL<HBufC>(TestCase(),*buf2,KParam3);
	StoreParameterL<TInt>(TestCase(),i,KParam2);
	StoreParameterL<CMsvEntryFilter>(TestCase(),*filter,KParam);
	CMsvEntryFilter* e= ObtainParameterReferenceL<CMsvEntryFilter>(TestCase(),KParam);
	HBufC* b= ObtainParameterReferenceL<HBufC>(TestCase(),KParam3);
	e++;
	TInt tt=ObtainValueParameterL<TInt>(TestCase(),KParam2);
	TInt tt2=ObtainValueParameterL<TInt>(TestCase(),KNum);
	tt2++;	
	tt++;
	b++;
	HBufC* b2=ObtainParameterReferenceL<HBufC>(TestCase(),KTest);
	b2++;
	TestCase().ActionCompletedL(*this);
}

CMtfTestActionSampleActionA::~CMtfTestActionSampleActionA()
{
}

