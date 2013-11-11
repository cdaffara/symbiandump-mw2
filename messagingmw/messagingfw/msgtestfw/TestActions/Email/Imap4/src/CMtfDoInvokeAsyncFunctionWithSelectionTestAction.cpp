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
#include "CMtfDoInvokeAsyncFunctionWithSelectionTestAction.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**   Constructor  */
CMtfDoInvokeAsyncFunctionWithSelectionTestAction::
		CMtfDoInvokeAsyncFunctionWithSelectionTestAction(CMtfTestCase& aTestCase)
	: CMtfDoInvokeAsyncFunctionTestAction(aTestCase), iMailSelection(NULL)
	{
	}


/**
  Destructor.
*/
CMtfDoInvokeAsyncFunctionWithSelectionTestAction::
	~CMtfDoInvokeAsyncFunctionWithSelectionTestAction()
	{
	delete iMailSelection;
	}

/** 
  DoCancel
  Cancels the pending asynchronous operation
*/
void CMtfDoInvokeAsyncFunctionWithSelectionTestAction::DoCancel()
	{
	delete iMailSelection;
	iMailSelection = NULL;
	CMtfDoInvokeAsyncFunctionTestAction::DoCancel();
	}


/**
  ExecuteIMAP4CommandL
  Obtains the Test Action input parameters, appends IMAP4 
  service Id to the mail selection and calls the base class  
  function InvokeAsyncImap4PopulateCommandL()
  @internalTechnology 
  @param	aImap4Command	IMAP4-specific command
  @pre		None
  @post		Asynchronous operation for the IMAP4 command is initiated
  @leave	System wide errors
*/	
void CMtfDoInvokeAsyncFunctionWithSelectionTestAction::ExecuteIMAP4CommandL
														 (TImap4Cmds aImap4Command)
	{
	// Obtain input parameter IMAP4 Service Id
	TMsvId			  paramServiceId  =	ObtainValueParameterL<TMsvId>(TestCase(),
										  ActionParameters().Parameter(0));

		// // Obtain input parameter mail selection
	CMsvEntrySelection* paramMailSelection=	ObtainParameterReferenceL<CMsvEntrySelection>
										(TestCase(),ActionParameters().Parameter(3));
	
									  
	/************************************************************************
	 A selection of mails that need to be downloaded to a local folder
	 The first entry in this selection MUST be the service. 
	 For populating a selection of mails, the selection of mails should
	 be appended after the service ID. The selection of mails MUST have the
	 same parent i.e. MUST be in the same folder.
	*************************************************************************/
	
	iMailSelection =  new (ELeave) CMsvEntrySelection;
	
	if(aImap4Command != KIMAP4MTMPopulate)
		{
		iMailSelection->InsertL(0, paramServiceId);
		}	

	TInt mailCount = paramMailSelection->Count();

	for (TInt selecIndex=0; selecIndex < mailCount; selecIndex++)	
		{
		TMsvId msvId = paramMailSelection->At(selecIndex);
		iMailSelection->AppendL(msvId);
		}
		
	// Call the base class function to invokes asynchronous IMAP4-specific operation
	InvokeAsyncImap4PopulateCommandL(aImap4Command, *iMailSelection);
	}
