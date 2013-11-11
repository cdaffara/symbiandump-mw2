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
// SetImap4PartialDownloadLimits
// [Action Parameters]
// TInt     TotalSizeLimit       <input>: Value of the total size limit
// Default value is KMaxTInt
// TInt     BodyTextSizeLimit    <input>: Value of the body text limit
// Default value is KMaxTInt
// TInt     AttachmentSizeLimit  <input>: Value of the attachment size limit
// Default value is KMaxTInt
// TInt     PartialMailSizeOptions <input>: Enumerated values defined in 
// TImImap4PartialDownloadMailSizeOptions
// Default value is ENoSizeLimits
// TMsvId   DestinationFolder    <input>: Value of the destination folder ID
// TMsvId	ServiceId			<input>: Value of the Imap service Id to use
// TImImap4GetMailInfo  Imap4GetMailInfo <output-initiation>: Partial Download limits
// [Action Description]
// Sets the member variables of TImImap4GetPartialDownloadInfo, creates a package of
// of the object. The package is stored in a Heap Buffer and provided as an output
// of this Test Action.
// [APIs Used]
// None
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/

// User inlcude
#include <cemailaccounts.h>
#include <iapprefs.h>

#include <imapset.h>

#include "CMtfTestActionSetImap4PartialDownloadLimits.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

// EPOC inlcude 


/**
  NewL()
  Constructs a CMtfTestActionSetImap4PartialDownloadLimits object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionSetImap4PartialDownloadLimits
  @pre    None
  @post   CMtfTestActionSetImap4PartialDownloadLimits object is created
*/
CMtfTestAction* CMtfTestActionSetImap4PartialDownloadLimits::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSetImap4PartialDownloadLimits* self = new (ELeave) CMtfTestActionSetImap4PartialDownloadLimits(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	
/**
  CMtfTestActionSetImap4PartialDownloadLimits constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 	
CMtfTestActionSetImap4PartialDownloadLimits::CMtfTestActionSetImap4PartialDownloadLimits(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ExecuteActionL
  Sets the members variables of TImImap4GetPartialMailInfo and packages the  
  object into a modifiable buffer descriptor.  The package is stored in a HBufC8 descriptor.
  The heap descriptor is then stored an output parameter of the Test Action.
  @internalTechnology 
  @return None
  @pre    None
  @post   None
  @leave  System wide errors
*/



void CMtfTestActionSetImap4PartialDownloadLimits::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSetImap4PartialDownloadLimits);

	TInt   paramTotalSizeLimit      = ObtainValueParameterL<TInt>(TestCase(),
										ActionParameters().Parameter(0),KMaxTInt);


	TInt   paramBodyTextSizeLimit   = ObtainValueParameterL<TInt>(TestCase(),
										ActionParameters().Parameter(1),KMaxTInt);


	TInt   paramAttachmentSizeLimit = ObtainValueParameterL<TInt>(TestCase(),
										ActionParameters().Parameter(2),KMaxTInt);
	

	TImImap4PartialMailOptions  paramPartialMailOptions	= 
									  ObtainValueParameterL<TImImap4PartialMailOptions>(TestCase(),
										ActionParameters().Parameter(3),ENoSizeLimits);
	

	TMsvId paramDestinationFolder   = ObtainValueParameterL<TMsvId>(TestCase(),
										ActionParameters().Parameter(4));

	TMsvId paramServiceId   = ObtainValueParameterL<TMsvId>(TestCase(),
										ActionParameters().Parameter(5));
		
	//Store the partial donwload limit values
	TImImap4GetPartialMailInfo imap4GetPartialMailInfo;
	
	imap4GetPartialMailInfo.iBodyTextSizeLimit   = paramBodyTextSizeLimit;
	imap4GetPartialMailInfo.iAttachmentSizeLimit = paramAttachmentSizeLimit;
	imap4GetPartialMailInfo.iTotalSizeLimit      = paramTotalSizeLimit;
	imap4GetPartialMailInfo.iPartialMailOptions  = paramPartialMailOptions;
	imap4GetPartialMailInfo.iDestinationFolder   = paramDestinationFolder;
	
	// Get the Imap settings and save the partial download limits
	CEmailAccounts* emailAccounts = CEmailAccounts::NewLC();	
	CImImap4Settings* imapSettings =  new(ELeave) CImImap4Settings();
	CleanupStack::PushL(imapSettings);
	
	TImapAccount imapAccount;
	emailAccounts->GetImapAccountL(paramServiceId, imapAccount);
	emailAccounts->LoadImapSettingsL(imapAccount, *imapSettings);
	
	imapSettings->SetBodyTextSizeLimitL( imap4GetPartialMailInfo.iBodyTextSizeLimit );
	imapSettings->SetAttachmentSizeLimitL( imap4GetPartialMailInfo.iAttachmentSizeLimit );
	imapSettings->SetMaxEmailSize( imap4GetPartialMailInfo.iTotalSizeLimit );
	imapSettings->SetPartialMailOptionsL( imap4GetPartialMailInfo.iPartialMailOptions );
		
	emailAccounts->SaveImapSettingsL(imapAccount, *imapSettings);
	
	CleanupStack::PopAndDestroy(2, emailAccounts ); // imapSettings, emailAccounts

	// Provide TImImap4GetPartialMailInfo as output of the Test Action
	StoreParameterL<TImImap4GetPartialMailInfo>(TestCase(),imap4GetPartialMailInfo,
															ActionParameters().Parameter(6));
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSetImap4PartialDownloadLimits);
	TestCase().ActionCompletedL(*this);
	}
