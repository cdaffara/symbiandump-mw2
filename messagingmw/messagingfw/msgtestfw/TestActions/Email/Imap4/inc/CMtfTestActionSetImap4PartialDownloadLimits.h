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


#ifndef __CMTF_TEST_ACTION_SET_IMAP4_PARTIAL_DOWNLOAD_LIMITS_H__
#define __CMTF_TEST_ACTION_SET_IMAP4_PARTIAL_DOWNLOAD_LIMITS_H__


// User include
#include "CMtfSynchronousTestAction.h"


/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSetImap4PartialDownloadLimits,"SetImap4PartialDownloadLimits");
/*@}*/


/**
  Implements the action to set members variables of TImImap4GetPartialMailInfo,
  then store it as an output parameter of the Test Action.
  @internalTechnology
*/
class CMtfTestActionSetImap4PartialDownloadLimits : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* aActionParameters
							   );
	
public:
	// Implementing the pure virutal function of CMtfSynchronousTestAction
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionSetImap4PartialDownloadLimits(CMtfTestCase& aTestCase);
	};

#endif //__CMTF_TEST_ACTION_SET_IMAP4_PARTIAL_DOWNLOAD_LIMITS_H__
