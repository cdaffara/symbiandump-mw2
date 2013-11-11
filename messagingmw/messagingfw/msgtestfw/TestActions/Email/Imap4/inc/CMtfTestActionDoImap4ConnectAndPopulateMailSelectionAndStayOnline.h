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


#ifndef __CMTF_TEST_ACTION_DO_IMAP4_CONNECT_AND_POPULATE_MAIL_SELECTION_AND_STAY_ONLINE_H__
#define __CMTF_TEST_ACTION_DO_IMAP4_CONNECT_AND_POPULATE_MAIL_SELECTION_AND_STAY_ONLINE_H__


// User include
#include "CMtfDoInvokeAsyncFunctionWithSelectionTestAction.h"


/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline,
					"DoImap4ConnectAndPopulateMailSelectionAndStayOnline");
/*@}*/



/**
  Implements the action to invoke the asynchronous operation for IMAP4-specific
  command KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline.

  @internalTechnology
*/
class CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline : 
							public CMtfDoInvokeAsyncFunctionWithSelectionTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* aActionParameters
							   );
public:
	// Implementing the pure virtual function of base class
	void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline
											(CMtfTestCase& aTestCase);

	};

#endif //__CMTF_TEST_ACTION_DO_IMAP4_CONNECT_AND_POPULATE_MAIL_SELECTION_AND_STAY_ONLINE_H__
