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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// DoImap4ConnectAndPopulateNewMailAndDisconnect
// [Action Parameters]
// TMsvId				paramServiceId	<input>		: Value of IMAP4 Service Id
// CImap4ClientMtm		paramMtm		<input>		: Reference to Imap4 client MTM
// TImImap4GetMailInfo or TImImap4GetMailInfo	paramDownloadLimits	<input>	
// : The Full or Partial download 
// download
// TMsvId				paramFolderId	<input>		: Destination folder id
// [Action Description]
// Connects to the IMAP4 server, populates New messages from the server and then 
// disconnects from the server. Does a Partial or Full download of mails based on the
// type of the download limits (TImImap4GetMailInfo  or  TImImap4GetPartialMailInfo)  
// provided as input to the Test Action.
// Calls the CImap4ClientMtm::InvokeAsyncFunctionL() with the IMAP4 command 
// KIMAP4MTMConnectAndPopulateNewMailAndDisconnect by calling the ExecuteIMAP4CommandL()
// function of the base class.
// [APIs Used]
// CMsvEntry::SetEntryL()
// CMsvEntrySelection::AppendL()
// CMsvEntrySelection::ResizeL()
// CImap4ClientMtm::InvokeAsyncFunctionL()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect.h"
#include "CMtfTestCase.h"


/**
  NewL()
  Constructs a CMtfTestActionDoImap4ConnectAndPopulateAllMailAndDisconnect object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect
  @pre    None
  @post   CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect object is created
*/
CMtfTestAction* CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect* self = 
				new (ELeave) CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect
																			(aTestCase);
	
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect::
				CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect
													(CMtfTestCase& aTestCase)
	: CMtfDoInvokeAsyncFunctionWithoutSelectionTestAction(aTestCase)
	{
	}


/**
  ExecuteActionL
  Invokes the asynchronous operation for IMAP4 command 
  KIMAP4MTMConnectAndPopulateNewMailAndDisconnect by calling the base class
  function ExecuteIMAP4CommandL()

  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect);
	TestCase().INFO_PRINTF1(_L("KIMAP4MTMConnectAndPopulateNewMailAndDisconnect Imap4 command execution"));
	// Call the base class function to invoke the IMAP4 command asynchronously 
	ExecuteIMAP4CommandL(KIMAP4MTMConnectAndPopulateNewMailAndDisconnect);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDoImap4ConnectAndPopulateNewMailAndDisconnect);
	}
