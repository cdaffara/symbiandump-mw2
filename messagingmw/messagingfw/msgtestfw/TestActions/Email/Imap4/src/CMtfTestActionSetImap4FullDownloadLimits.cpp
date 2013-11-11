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
// SetImap4FullDownloadLimits
// [Action Parameters]
// TInt     TotalSizeLimit          <input> : Value of the total size limit
// Default value is KMaxTInt
// TInt     PartialMailSizeOptions  <input> : Enumerated values defined in 
// TImap4GetMailOptions
// Default value is EGetImap4EmailHeaders
// TMsvId   DestinationFolder       <input> : Value of the destination folder ID
// TMsvId	ServiceId				<input> : Value of the Imap service id to use
// TImImap4GetMailInfo  Imap4GetMailInfo <output-initiation>: Full Download limits
// [Action Description]
// Sets the member variables of TImImap4GetMailInfo
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file
 @internalTechnology 
*/


#include <cemailaccounts.h>
#include <iapprefs.h>

// EPOC include
#include <imapset.h>

// User include
#include "CMtfTestActionSetImap4FullDownloadLimits.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"


/**
  NewL()
  Constructs a CMtfTestActionSetImap4FullDownloadLimits object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSetImap4FullDownloadLimits
  @pre    None
  @post   CMtfTestActionSetImap4FullDownloadLimits object is created
*/
CMtfTestAction* CMtfTestActionSetImap4FullDownloadLimits::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	
	CMtfTestActionSetImap4FullDownloadLimits* self = 
						new (ELeave) CMtfTestActionSetImap4FullDownloadLimits(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionSetImap4FullDownloadLimits constructor
  Calls the base class constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 	
CMtfTestActionSetImap4FullDownloadLimits::CMtfTestActionSetImap4FullDownloadLimits
											(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


/**
  ExecuteActionL
  Sets the values for TImImap4GetMailInfo memeber variables and then store it
  as an output parameter of the Test Action.
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionSetImap4FullDownloadLimits::ExecuteActionL()
	{	
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetImap4FullDownloadLimits);
	// Obtain Test Action input parameters
	TInt  paramTotalSizeLimit	  = ObtainValueParameterL<TInt>(TestCase(),
										ActionParameters().Parameter(0),KMaxTInt);

	TImap4GetMailOptions  paramMailOptions	 = ObtainValueParameterL<TImap4GetMailOptions>
												(TestCase(),ActionParameters().Parameter(1),
												EGetImap4EmailHeaders);

 	TMsvId	paramDestinationFolder  = ObtainValueParameterL<TMsvId>
										(TestCase(),ActionParameters().Parameter(2));
										
	TMsvId paramServiceId = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(3));

	// Store the full donwload limits values 
	TImImap4GetMailInfo  imap4GetMailInfo;
		
	imap4GetMailInfo.iMaxEmailSize      = paramTotalSizeLimit;
	imap4GetMailInfo.iGetMailBodyParts  = paramMailOptions;
	imap4GetMailInfo.iDestinationFolder = paramDestinationFolder;
	
	// Load the Imap account settings and save the download limits
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);

	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(paramServiceId, imapAccount);
	emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);
		
	imapSettings->SetMaxEmailSize( imap4GetMailInfo.iMaxEmailSize );
	imapSettings->SetGetMailOptions( imap4GetMailInfo.iGetMailBodyParts );
	
	emailAccounts->SaveImapSettingsL(imapAccount, *imapSettings);
	CleanupStack::PopAndDestroy( 2, emailAccounts ); // imapSettings, emailSettings
	

	// Store the full download limits as the ouput parameter of the Test Action		
	StoreParameterL<TImImap4GetMailInfo>(TestCase(),imap4GetMailInfo,
											ActionParameters().Parameter(4));

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetImap4FullDownloadLimits );
	TestCase().ActionCompletedL(*this);
	}
