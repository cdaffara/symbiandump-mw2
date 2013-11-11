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
// VerifyRecipients
// [Action Parameters]
// CMsvEntry	paramMsgEntry	   <input>: Reference to the message entry object
// CMsvStore	paramMsgStore	   <input>: Reference to the message's store object
// TFieldList	paramRecipientType <input>: Enumeration specifying the message field type
// (One of the enumeration values: ETo, ECc EBcc.
// Other enumerations are currently not supported) 
// HBufC		Address1	 <input>: String containing Recipient Address
// HBufC		Address2	 <input>: String containing Recipient Address
// HBufC		addressN	 <input>: String containing Recipient Address
// [Action Description]
// VerifyRecipients Test Action is intended verify the recipient address set for
// the message using SendAs API, by comparing it with the recipient addresses provided
// as input to the Test Action.
// [APIs Used]
// CImHeader::RestoreL ()
// CImHeader::ToRecipients ()
// CImHeader::CcRecipients ()
// CImHeader::BccRecipients ()
// CSmsHeader::RestoreL ()
// CSmsHeader::Recipients()
// CSmsNumber::Address()
// CIrHeader::RestoreL ()
// CBtHeader::RestoreL()
// CObexMtmHeader::Addr ()
// __ACTION_INFO_END__
// 
//

/**
 @file 
 @internalTechnology 
*/


// User include
#include "CMtfTestActionVerifyRecipients.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#include "CMtfTestActionUtilsMessage.h"

#include "btmsgtypeuid.h"
#include "irmsgtypeuid.h"

//Granularity for CArrayFixFlat arrays
const TInt KArrayGranularity = 16;

/**
  NewL()
  Constructs a CMtfTestActionVerifyRecipients object.
  Uses two phase construction and leaves nothing on the CleanupStack.   
  @internalTechnology
  @param  aTestCase         Test Case to which this Test Action belongs
  @param  aActionParameters Action parameters, must not be NULL
  @return Created object of type CMtfTestActionVerifyRecipients
  @pre    None
  @post   CMtfTestActionVerifyRecipients object is created
*/
CMtfTestAction* CMtfTestActionVerifyRecipients::
		NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionVerifyRecipients* self = 
							new (ELeave) CMtfTestActionVerifyRecipients(aTestCase);

	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}
	

/**
  CMtfTestActionVerifyRecipients constructor
  Calls the base class' constructor
  @internalTechnology  
  @param  aTestCase  Test Case to which this Test Action belongs
  @pre    None
  @post   None
*/ 
CMtfTestActionVerifyRecipients::CMtfTestActionVerifyRecipients(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}

/**
  Function : ~CMtfTestActionVerifyRecipients
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionVerifyRecipients::~CMtfTestActionVerifyRecipients()
	{
	}

/**
  ExecuteActionL
  Verifies whther the recipient address of a specified type	(ETo, ECc EBcc)
  is set by comparing the set value(s) with the expected recipient address string(s)
  @internalTechnology 
  @pre    None
  @post   None
  @leave  System wide errors
*/
void CMtfTestActionVerifyRecipients::ExecuteActionL()
	{
	if((TestCase().TestStepResult()) == EPass)
		{
		TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionVerifyRecipients);
			
		// Get Test Action input parameters
		CMsvEntry*	paramMsgEntry = ObtainParameterReferenceL<CMsvEntry>(TestCase(),
										ActionParameters().Parameter(0));
										
		TImHeaderEncodingInfo::TFieldList paramRecipientType = 
								   ObtainValueParameterL<TImHeaderEncodingInfo::TFieldList>(TestCase(), ActionParameters().Parameter(1));

		if (paramMsgEntry == NULL)
			{
			TestCase().ERR_PRINTF1(_L("Invalid parameter: Message Entry value is NULL"));
			TestCase().SetTestStepResult(EFail);
			}
			
		if((TestCase().TestStepResult()) == EPass)	
			{
			CMsvStore * msvStore = NULL;
			
			if(paramMsgEntry->HasStoreL())
				{
				msvStore = paramMsgEntry->ReadStoreL();
				CleanupStack::PushL(msvStore);
				}
			
			CDesC16ArrayFlat* recipientArray = new (ELeave) CDesC16ArrayFlat(KArrayGranularity);
			CleanupStack::PushL(recipientArray);
		
			// Read the recipient address and alias names from the ini file
			TRAPD(result, ObtainRecipientsL(paramMsgEntry , *recipientArray));
			
			if (result != KErrNone)
				{
				TestCase().ERR_PRINTF2(_L("Failure while reading the recipients from ini file: %d"), result);
				TestCase().SetTestStepResult(EFail);
				}
			else
				{
				// Call function to verify the recipient address for the message
			 	result = VerifyRecipientsL(*paramMsgEntry, *msvStore, paramRecipientType, *recipientArray);
			 	if (result == KErrNone)
					{
					TestCase().INFO_PRINTF2(_L("Verification of recipient address was successful: %d"), result);
					}
				else if (result == KErrNotFound)
					{
					TestCase().ERR_PRINTF1(_L("Recipient address was not found in the message"));
					TestCase().SetTestStepResult(EFail);			
					}
				else
					{
					TestCase().ERR_PRINTF2(_L("Verification of Recipient address failed with error: %d"), result);
					TestCase().SetTestStepResult(EFail);			
					}
				}
			CleanupStack::PopAndDestroy(recipientArray);
			CleanupStack::PopAndDestroy(msvStore);
			}
		}
			
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionVerifyRecipients);
	TestCase().ActionCompletedL(*this);
	}
	
	
/** 
 Reads the recipient address and the alias names from the ini file and formats the 
 recipient address if the alias type is specified
*/ 
void CMtfTestActionVerifyRecipients::ObtainRecipientsL(CMsvEntry* aMsvEntry, CDesC16ArrayFlat&  aRecipientArray)
	{
	TInt numberOfParameters = ActionParameters().Count();

	for(TInt index = 2; index < numberOfParameters ; index+=2)
		{
		HBufC* recipientAddr = ObtainParameterReferenceL<HBufC>(TestCase(),
									ActionParameters().Parameter(index));
		
		HBufC* aliasBuf = ObtainParameterReferenceL<HBufC>(TestCase(),
									ActionParameters().Parameter(index+1),NULL);
									
		// If alias is not provided, then copy the address into the array
		// For IR, BT and SMS, ignore the alias type set in the ini file							
		if((aliasBuf == NULL) || (aMsvEntry->Entry().iMtm != (KUidMsgTypeSMTP)))
			{
			TPtrC ptr16 = recipientAddr->Des();
			aRecipientArray.AppendL(ptr16);	
			}
		else
			{
			// For SMTP message, format the recipient address with the alias
			if((aMsvEntry->Entry().iMtm) == (KUidMsgTypeSMTP))
				{
				// To be done: String formating to include both alias and address
				TPtrC ptr16 = recipientAddr->Des();
				_LIT(KFormat,"\"%S\" <%S>");
				TBuf<256> x;
				x.Format(KFormat,aliasBuf,recipientAddr);
				aRecipientArray.AppendL(x);
				}
			}		
		}
	}


/**
 Based on the type of the message, calls the corresponding utility function
 to verify the specified type of recipient address
*/
TInt CMtfTestActionVerifyRecipients::VerifyRecipientsL (CMsvEntry& aMsgEntry,
								CMsvStore & aMsgStore,
								TImHeaderEncodingInfo::TFieldList recipientType,
								CDesC16Array&  aRecipientArray)
	{
	TInt returnValue = KErrNone;
	TUid msgMtmType = aMsgEntry.Entry().iMtm;

	if( (msgMtmType == KUidMsgTypeIMAP4) || (msgMtmType == KUidMsgTypePOP3) || (msgMtmType == KUidMsgTypeSMTP) )
		{
		returnValue = CMtfTestActionUtilsMessage::VerifyEmailRecipientsL(aMsgStore, aRecipientArray,recipientType);
		}
	else if((msgMtmType == KUidMsgTypeBt) || (msgMtmType == KUidMsgTypeIrUID))
		{
		returnValue = CMtfTestActionUtilsMessage::VerifyObexRecipientL(aMsgEntry, aMsgStore, aRecipientArray);	
		}
	else if (msgMtmType == KUidMsgTypeSMS)
		{
		returnValue = CMtfTestActionUtilsMessage::VerifySmsRecipientsL(aMsgStore,aRecipientArray);	
		}
	else
		{
		returnValue = KErrUnknown;
		}
		
	return returnValue;
	}
	
		
