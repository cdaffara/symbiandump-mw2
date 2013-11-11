/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file 
 @internalTechnology 
*/


#ifndef __CMTF_TEST_ACTION_CREATE_PARTIALLY_DOWNLOADED_MAIL_SELECTION_H__
#define __CMTF_TEST_ACTION_CREATE_PARTIALLY_DOWNLOADED_MAIL_SELECTION_H__


// User include
#include "CMtfSynchronousTestAction.h"


/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionCreatePartiallyDownloadedMailSelection,
							"CreatePartiallyDownloadedMailSelection");
/*@}*/


/**
  CMtfSynchronousTestAction creates a selection of partially downloaded IMAP4 
  messsage in the parent folder.  Overrides only the pure virtual funtion 
  ExecuteActionL() of class CMtfSynchronousTestAction.
  @internalTechnology
*/
class CMtfTestActionCreatePartiallyDownloadedMailSelection : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* aActionParameters
							   );
	
public:
	// From CMtfSynchronousTestAction, override the base class virtual function
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionCreatePartiallyDownloadedMailSelection(CMtfTestCase& aTestCase);
	};

#endif // __CMTF_TEST_ACTION_CREATE_PARTIALLY_DOWNLOADED_MAIL_SELECTION_H__
