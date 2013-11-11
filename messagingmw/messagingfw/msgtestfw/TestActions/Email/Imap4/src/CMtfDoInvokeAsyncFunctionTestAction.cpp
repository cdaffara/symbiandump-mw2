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


// User include
#include "CMtfDoInvokeAsyncFunctionTestAction.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "TMtfTestParameterType.h"
#include "MtfTestActionUtilsUser.h"


/**
  Constructor
*/
CMtfDoInvokeAsyncFunctionTestAction::CMtfDoInvokeAsyncFunctionTestAction
									 (CMtfTestCase& aTestCase)
	: CMtfTestAction(aTestCase), iOperation(NULL)
	{
	}


/** 
  DoCancel
  Cancels the pending asynchronous operation
*/
void CMtfDoInvokeAsyncFunctionTestAction::DoCancel()
	{
	iOperation->Cancel();
	delete iOperation;
	iOperation = NULL;	
	}


/**
  InvokeAsyncImap4PopulateCommandL
  Invokes asynchronous  IMAP4-specific operation by calling
  CImap4ClientMtm::InvokeAsyncFunctionL() for the IMAP4 command
  provided as a input parameter.
  @internalTechnology 
  @param	aImap4Mtm		 Reference to the IMAP4 Mtm
  @param	aImap4Command	 IMAP4-specific command
  @param	aMailSelection	 Reference to the Mail selection
  @param	aDownloadLimits  The CMtfTestParameter object holding a pointer of full
							 or partial download limits
  @pre		None
  @post		The asynchronous operation for the IMAP4 command is initiated
  @leave	KErrUnknown		 If download limits provided is not of type 
		 					 TImImap4GetMailInfo or TImImap4GetPartialMailInfo
*/	
void CMtfDoInvokeAsyncFunctionTestAction::InvokeAsyncImap4PopulateCommandL
										  ( 
										    TImap4Cmds aImap4Command,
										    CMsvEntrySelection& aMailSelection
										  )
	{
	/* Set the status to Request Pending before calling the asynchronous 
	  function.
	*/
	iStatus = KRequestPending;

	// Obtain input parameter IMAP4 Mtm
	CImap4ClientMtm*  paramMtm 	=	ObtainParameterReferenceL<CImap4ClientMtm>
									(TestCase(),ActionParameters().Parameter(1));
	
	
	/*********************************************************************
	 Get the download limits package.  The download limits can be of type 
	 TImImap4GetMailInfo or TImImap4GetPartialMailInfo.  Hence,
	 TestCase.ObtainParameter() is called directly to get the reference to
	 CMtfTestParameter object that stores the type and the address of the
	 download limits that is set.  The download limits object is retrieved
	 from the CMtfTestParameter object.
	**********************************************************************/

	const CMtfTestParameter& paramDownloadLimits = TestCase().ObtainParameterL
												(ActionParameters().Parameter(2));


	/*************************************************************************
	 Check if the download limits is of type Full download or Partial download.
	 If it is one of the above 2 types, package the download limits object and 
	 pass it to InvokeAsyncFunctionL().  If the type does not match, Leave with
	 error KErrUnknown.
	*************************************************************************/

	/************************************************************************* 
	 The CMtfTestParameter stores the TImImap4GetMailInfo or 
	 TImImap4GetPartialMailInfo object. To get the object from 
	 CMtfTestParameter, the packaged object needs to be unpacked. This operation 
	 is usually done in the ObtainValueParameterL(), but we need to perform 
	 this operation explicilty as we are retrieving the CMtfTestParameter itself,
	 insted of the specific type of the parameter stored in CMTfTestParameter.
	***************************************************************************/  
	HBufC8* buf=reinterpret_cast<HBufC8*>(paramDownloadLimits.Parameter());

	if(paramDownloadLimits.SpecificTypeId() == EMtfTImImap4GetMailInfo)
		{
		// Get Full download limits
		TImImap4GetMailInfo value;	
		TPckgC<TImImap4GetMailInfo> unpack(value);
		unpack.Set(*buf);
		TImImap4GetMailInfo fullDownloadLimits = unpack();
		
		// Package of the download limits
		TPckgBuf<TImImap4GetMailInfo> fullPack (fullDownloadLimits);

		// Invokes asynchronous IMAP4-specific operation 
		iOperation = paramMtm->InvokeAsyncFunctionL(aImap4Command,aMailSelection,
																fullPack,iStatus);
		}
	else if(paramDownloadLimits.SpecificTypeId() == EMtfTImImap4GetPartialMailInfo)
		{
		// Get Parital download limits
		TImImap4GetPartialMailInfo value;	
		TPckgC<TImImap4GetPartialMailInfo> unpack(value);
		unpack.Set(*buf);
		TImImap4GetPartialMailInfo partialDownloadLimits = unpack();

		// Package of the download limits
		TPckgBuf<TImImap4GetPartialMailInfo> partialPack(partialDownloadLimits);
		
		// Invokes asynchronous IMAP4-specific operation
		iOperation = paramMtm->InvokeAsyncFunctionL( aImap4Command, aMailSelection,
												    partialPack, iStatus);
		}
	else
		{
		// The type of the input parameter does not match
		User::Leave(KErrUnknown);
		}

	CActiveScheduler::Add(this);
	SetActive();
	}

/**
  RunL
  Handles the asynchronous request completion event. Informs the completion 
  of the Test Action to the Test Case.
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide error
*/
void CMtfDoInvokeAsyncFunctionTestAction::RunL()
	{
	TInt err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);
	User::LeaveIfError(err);
	
	delete iOperation;
	iOperation = NULL;
	
	TestCase().ActionCompletedL(*this);
	}
