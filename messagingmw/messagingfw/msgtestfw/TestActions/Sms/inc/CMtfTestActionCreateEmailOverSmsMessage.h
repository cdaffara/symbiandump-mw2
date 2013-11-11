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
//



#ifndef __CMTFTESTACTIONCREATEEMAILOVERSMSMESSAGE_H__
#define __CMTFTESTACTIONCREATEEMAILOVERSMSMESSAGE_H__


#include "CMtfSynchronousTestAction.h"

class CMsvEntry;
class TMsvEntry;

_LIT(KTestActionCreateEmailOverSmsMessage,"CreateEmailOverSmsMessage");

/**
This action creates a simple SMS message.

The test action first gets the configuration settings from an SMS message configuration file.
It then creates an Email over SMS message entry, and stores the CSmsHeader object and CRichText object.
Right now, it only supports single recipient message.  After creating the message, the 
action stores the message entry ID to the supplied parameter.

@internalTechnology
*/
class CMtfTestActionCreateEmailOverSmsMessage : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionCreateEmailOverSmsMessage(CMtfTestCase& aTestCase);
	void InitializeHeaderFromConfigL(TMsvEntry* indexEntry, CMsvEntry* aEntry);
	};


#endif
