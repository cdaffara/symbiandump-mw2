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
// DoImap4ConnectAndPopulateMailSelectionAndStayOnline
// [Action Parameters]
// TMsvId				paramServiceId	<input>		: Value of IMAP4 Service Id
// CImap4ClientMtm		paramMtm		<input>		: Reference to Imap4 client MTM
// TImImap4GetMailInfo or TImImap4GetMailInfo	paramDownloadLimits	<input>	
// : The Full or Partial download 
// download limits
// CMsvEntrySelection	paramSelection  <input>		: Reference to the message selection
// [Action Description]
// Establishes a connection with the IMAP4 server and populates the mails present 
// in the mail selection. A Full or Partial download of mails is perfomred based 
// on the type of limits that is set.  The connection with the IMAP4 server is 
// retained after the download of messages.
// Calls the CImap4ClientMtm::InvokeAsyncFunctionL() with the IMAP4 command 
// KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline by calling the 
// ExecuteIMAP4CommandL() function of the base class 
// CMtfDoInvokeAsyncFunctionWithSelectionTestAction.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntrySelection::AppendL
// CImap4ClientMtm::InvokeAsyncFunctionL
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// EPOC include
#include <impcmtm.h>

// User include
#include "CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline.h"
#include "CMtfTestCase.h"



/**
  NewL()
  Constructs a CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline
  @pre    None
  @post   CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline object is created
*/
CMtfTestAction* CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline::
			NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline* self = 
		new (ELeave) 
			CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline::
		CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline
												(CMtfTestCase& aTestCase)
	: CMtfDoInvokeAsyncFunctionWithSelectionTestAction(aTestCase)
	{
	}


/**
  Function : ExecuteActionL
  Invokes the asynchronous operation for IMAP4 specific command 
  KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline by calling the base class
  function ExecuteIMAP4CommandL().  ExecuteIMAP4CommandL() function retrieves
  the Full or Partidal download limits package from the HBufC8 descriptor and
  calls the CImap4ClientMtm::InvokeAsyncFunctionL() function.
  @internalTechnology 
  @return None
  @pre    None
  @post   None
  @leave  KErrUnknown If package is not of type TImImap4GetMailInfo or 
		  TImImap4GetPartialMailInfo
*/
void CMtfTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline);
	TestCase().INFO_PRINTF1(_L("KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline Imap4 command execution"));
	//Call the base class function to invoke the IMAP4 command asynchronously
	ExecuteIMAP4CommandL(KIMAP4MTMConnectAndPopulateMailSelectionAndStayOnline);
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionDoImap4ConnectAndPopulateMailSelectionAndStayOnline);
	}
