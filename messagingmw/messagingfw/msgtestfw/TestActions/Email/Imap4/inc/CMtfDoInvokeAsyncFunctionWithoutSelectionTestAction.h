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


#ifndef __CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCTION_WITHOUT_SELECTION_TEST_ACTION_H__
#define __CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCTION_WITHOUT_SELECTION_TEST_ACTION_H__


// EPOC include
#include <msvstd.h>
#include <impcmtm.h>
#include <e32des8.h>

// User include
#include "CMtfDoInvokeAsyncFunctionTestAction.h"


/**
  Abstrast base class for Test Actions that invoke IMAP4 commands related
  to 'copy new', 'copy all', 'move new', 'move all', 'populate new', 'populate all'.
  
  The parameters requried for the CImap4ClientMtm::InvokeAsyncFucntionL() are 
  obtained and the call to CImap4ClientMtm::InvokeAsyncFucntionL() is made by calling
  the base class CMtfDoInvokeAsyncFunctionTestAction::InvokeAsyncImap4PopulateCommandL()
  function.
  
  Derived class need to override ExecuteActionL() and RunL() functions. 
  DoCancel() function need not be overridden.

  @internalTechnology
*/
class CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction : public CMtfDoInvokeAsyncFunctionTestAction
	{
private:
	void DoCancel();
protected:
	// Constructor
	CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction(CMtfTestCase& aTestCase);	
	// Destructor
	virtual ~CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction();
	// Invokes asynchronous IMAP4 command
	void ExecuteIMAP4CommandL(TImap4Cmds aImap4Command);
protected:
	CMsvEntrySelection* iMailSelection;
	};

#endif //__CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCTION_WITHOUT_SELECTION_TEST_ACTION_H__
