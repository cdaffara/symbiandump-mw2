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
*/


#ifndef __CMTF_TEST_ACTION_SET_IMAP4_PARTIAL_DOWNLOAD_LIMITS_H__
#define __CMTF_TEST_ACTION_SET_POP3_PARTIAL_DOWNLOAD_LIMITS_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSetImap4PartialDownloadLimits,"SetImap4PartialDownloadLimits");
/*@}*/


/**
  Overview: 
  CMtfTestActionSetImap4PartialDownloadLimits class derived from CMtfSynchronousTestAction
  Implements the action to set members variables of TImImap4GetPartialMailInfo and packages the  
  object into a modifiable buffer descriptor.  The package is stored in a HBufC8 descriptor.
  The heap descriptor is then stored an output parameter of the Test Action.
  @internalTechnology
*/
class CMtfTestActionSetPop3PartialDownloadLimits : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters );
	virtual ~CMtfTestActionSetPop3PartialDownloadLimits(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSetPop3PartialDownloadLimits(CMtfTestCase& aTestCase);
	};

#endif //__CMTF_TEST_ACTION_SET_IMAP4_PARTIAL_DOWNLOAD_LIMITS_H__
