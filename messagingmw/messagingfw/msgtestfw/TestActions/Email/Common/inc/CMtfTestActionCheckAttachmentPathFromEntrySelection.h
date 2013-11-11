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


#ifndef __CMTFTESTACTIONCHECKATTACHMENTPATHFROMENTRYSELECTION_H__
#define __CMTFTESTACTIONCHECKATTACHMENTPATHFROMENTRYSELECTION_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionCheckAttachmentPathFromEntrySelection, "CheckAttachmentPathFromEntrySelection");

class CMtfTestActionCheckAttachmentPathFromEntrySelection : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCheckAttachmentPathFromEntrySelection(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionCheckAttachmentPathFromEntrySelection(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONCHECKATTACHMENTPATHFROMENTRYSELECTION_H__
