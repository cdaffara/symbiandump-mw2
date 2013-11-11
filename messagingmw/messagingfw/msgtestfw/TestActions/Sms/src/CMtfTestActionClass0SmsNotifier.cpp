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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// Class0Notifier
// [Action Parameters]
// [Action Description]
// Notify the class 0 sms message
// __ACTION_INFO_END__
// 
//

#include "CMtfTestActionClass0SmsNotifier.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


_LIT(KTestClas0Sms, "\\logs\\testexecute\\class0sms.txt");
const TInt KTestStringLength = 200;

CMtfTestAction* CMtfTestActionClass0SmsNotifier::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionClass0SmsNotifier* self = new (ELeave) CMtfTestActionClass0SmsNotifier(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}


CMtfTestActionClass0SmsNotifier::CMtfTestActionClass0SmsNotifier(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionClass0SmsNotifier::~CMtfTestActionClass0SmsNotifier()
	{
	}


void CMtfTestActionClass0SmsNotifier::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionClass0SmsNotifier);
	
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	
	
	RFile file;
	file.Open(fs, KTestClas0Sms, EFileRead);
		
	TBuf8 <KTestStringLength> smsData;
	file.Read(smsData); 
		
	fs.Close();
	
	CleanupStack::PopAndDestroy(&fs);
	
	TestCase().INFO_PRINTF2(_L("Test Action %S Passed..."), &KTestActionClass0SmsNotifier);	
	TestCase().ActionCompletedL(*this);
	}
	



