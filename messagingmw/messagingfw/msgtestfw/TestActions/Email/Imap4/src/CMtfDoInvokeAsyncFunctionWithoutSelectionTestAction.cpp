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

/**
 @file 
 @internalTechnology 
*/


// EPOC include 
#include <imapset.h>

// User include
#include "CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  Constructor.
*/
CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction::
	CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction(CMtfTestCase& aTestCase)
	: CMtfDoInvokeAsyncFunctionTestAction(aTestCase),iMailSelection(NULL)
	{
	}

/**
  Destructor.
*/
CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction::
	~CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction()
	{
	delete iMailSelection;
	}

/** 
  DoCancel
  Cancels the pending asynchronous operation
*/
void CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction::DoCancel()
	{
	delete iMailSelection;
	iMailSelection = NULL;
	CMtfDoInvokeAsyncFunctionTestAction::DoCancel();
	}


/**
  ExecuteIMAP4CommandL
  Obtains the Test Action input parameters, creates a mail selection
  and calls the base class function InvokeAsyncImap4PopulateCommandL()
  @internalTechnology 
  @param	aImap4Command	IMAP4-specific command
  @pre		None
  @post	The asynchronous operation for the IMAP4 command is initiated
  @leave	System wide errors
*/	
void CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction::ExecuteIMAP4CommandL
													 (TImap4Cmds aImap4Command)
	{
	// Obtain input parameter IMAP4 Service Id
	TMsvId	paramServiceId  =	ObtainValueParameterL<TMsvId>(TestCase(),
									  ActionParameters().Parameter(0));


	// Obtain input parameter destination folder Id
	TMsvId	paramFolderId  =	ObtainValueParameterL<TMsvId>(TestCase(),
									  ActionParameters().Parameter(3));	

	/*********************************************************************
	 Create a mail selection. The first entry in this selection MUST be 
	 the service. For the 'populate new', 'populate all' situations, the 
	 folder from which the mails are to be populated should be appended
	 after the service ID.
	**********************************************************************/
	iMailSelection = new (ELeave) CMsvEntrySelection;
	
	iMailSelection->ResizeL(0);
	iMailSelection->InsertL(0,paramServiceId);
	iMailSelection->InsertL(1,paramFolderId); 

	// Call the base class function to invokes asynchronous IMAP4-specific operation
	InvokeAsyncImap4PopulateCommandL(aImap4Command, *iMailSelection);
	}
