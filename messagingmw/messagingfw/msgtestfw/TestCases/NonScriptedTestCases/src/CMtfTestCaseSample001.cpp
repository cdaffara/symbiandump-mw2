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

#include "CMtfTestCaseSample001.h"
#include "CMtfTestActionSampleActionA.h"
#include "CMtfTestActionSampleActionB.h"
#include "CMtfTestActionSampleActionC.h"
#include "CMtfTestActionSampleActionD.h"
#include "CMtfTestActionSampleActionE.h"
#include "CMtfTestActionSampleActionF.h"
#include "CMtfTestActionParameters.h"

void CMtfTestCaseSample001::CheckPostConditionsL()
/**
 * Override of base class virtual
 */
{
	CMtfTestActionSampleActionE::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
	CMtfTestActionSampleActionF::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
}

CMtfTestCase* CMtfTestCaseSample001::NewL(const CMtfTestServer& aTestServer)
{
	CMtfTestCaseSample001* self = new (ELeave) CMtfTestCaseSample001(aTestServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
}

CMtfTestCaseSample001::CMtfTestCaseSample001(const CMtfTestServer& aTestServer)
: CMtfTestCase(aTestServer,EFalse,ETrue)
{
}

void CMtfTestCaseSample001::SetupTestCaseL()
/**
 * Override of base class virtual
 */
{
	CMtfTestActionSampleActionA::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
	CMtfTestActionSampleActionB::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
}


void CMtfTestCaseSample001::ExecuteTestCaseL()
/**
 * Override of base class pure virtual
 */
{
	CMtfTestActionSampleActionC::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
	CMtfTestActionSampleActionD::NewL(*this,CMtfTestActionParameters::NewLC());
	CleanupStack::Pop();
}
