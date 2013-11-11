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


#ifndef __CMTF_TEST_ACTION_DO_IMAP4_POPULATE_H__
#define __CMTF_TEST_ACTION_DO_IMAP4_POPULATE_H__


//User include
#include "CMtfDoInvokeAsyncFunctionWithSelectionTestAction.h"


/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionDoImap4Populate,"DoImap4Populate");
/*@}*/


/**
  Implements the action to invoke the asynchronous operation for IMAP4-specific 
  command KIMAP4MTMPopulate.

  @internalTechnology
*/
class CMtfTestActionDoImap4Populate : 
					public CMtfDoInvokeAsyncFunctionWithSelectionTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* aActionParameters
							   );
public:
	// Overriding the pure virtual function of the base class
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionDoImap4Populate(CMtfTestCase& aTestCase);

	};

#endif //__CMTF_TEST_ACTION_DO_IMAP4_POPULATE_H__
