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

#ifndef __CMTFTESTACTIONCOMPAREEMAILS_H__
#define __CMTFTESTACTIONCOMPAREEMAILS_H__

// System include
#include <msvapi.h>


// User include
#include "CMtfSynchronousTestAction.h"
#include "CMtfTestActionUtilsEmailMessage.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCompareEmails,"CompareEmails");
/*@}*/


// class forward declaration
class CMtfTestParameter;


/**
  Implements the test action to compare the email messages
  passed as the selections as parameter
  @internalTechnology
*/
class CMtfTestActionCompareEmails : public CMtfSynchronousTestAction
	{
public:

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );
	// Destructor
	virtual ~CMtfTestActionCompareEmails(); 

	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionCompareEmails( CMtfTestCase& aTestCase );
	
	void CompareEmailsL( const CMsvEntrySelection& aMsgSelection1,
							  	const CMsvEntrySelection& aMsgSelection2);
	void CMtfTestActionCompareEmails::ComparePlainBodyTextL();
	void CompareFilesL(const TPtrC aFilePath1, const TPtrC aFilePath2,
											TInt& aError, TBool& aDiffFlag);
	void CMtfTestActionCompareEmails::DeleteEmailMessageInfo();
	void CMtfTestActionCompareEmails::SetEmailMessageInfoL(CMsvEntry& aMsvEntry1, CMsvEntry& aMsvEntry2);
	void CMtfTestActionCompareEmails::CompareHtmlL();
	void CMtfTestActionCompareEmails::CompareAttachmentsL(CMsvEntry& aMsvEntry1, CMsvEntry& aMsvEntry2);
	TBool CMtfTestActionCompareEmails::CompareAttachmentFilesL(RFile aFile1, RFile aFile2);
	// Check if the Test Case result is EPass
	inline TBool IsTestCaseStatusPass();
private:
	/**  Session object */
	CMsvSession* iParamSession;

	/**	Utility class object storing the information of the email message*/ 
	CMtfTestActionUtilsEmailMessage* iMessageInfo1;	
	CMtfTestActionUtilsEmailMessage* iMessageInfo2;
	};

#include "CMtfTestActionCompareEmails.inl"

#endif //__CMTFTESTACTIONCOMPAREEMAILS_H__