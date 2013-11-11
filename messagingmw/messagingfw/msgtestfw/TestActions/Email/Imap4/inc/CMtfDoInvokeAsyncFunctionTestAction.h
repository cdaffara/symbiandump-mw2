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


#ifndef __CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCTION_TEST_ACTION_H__
#define __CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCTION_TEST_ACTION_H__


// EPOC include
#include <impcmtm.h>


// User include
#include "CMtfTestAction.h"

// Forward declaration
class CMtfTestParameter;


/**
  Abstract base class which calls the asynchronous IMAP4-specific commands.
  The class stores the CMsvOperation object which control asynchronous commands 
  of IMAP4.
  
  InvokeAsyncImap4PopulateCommandL() function in the class can be called to 
  invoke IMAP4 commands related to Populate mails.
  
  Derived classes need to override ExecuteActionL() of CMtfTestAction.
  This class implements the DoCancel() and RunL() methods.

  @internalTechnology
*/
class CMtfDoInvokeAsyncFunctionTestAction : public CMtfTestAction
	{
protected:
	// Constructor
	CMtfDoInvokeAsyncFunctionTestAction(CMtfTestCase& aTestCase);

	// Invokes the asynchronous IMAP4 command related to Populate All mails, New mails
	// and mail selection
	void InvokeAsyncImap4PopulateCommandL(TImap4Cmds			aImap4Command,
										  CMsvEntrySelection&	aMailSelection
										 );
	// Overriding the virtual function from CMtfTestAction
	virtual void DoCancel();

	// Overriding the virtual function from CMtfTestAction	
	virtual void RunL();

protected:
	/** Interface for controlling asynchronous commands */
	CMsvOperation* iOperation;
	};

#endif //__CMTF_TEST_ACTION_DO_INVOKE_ASYNC_FUNCION_TEST_ACTION_H__
