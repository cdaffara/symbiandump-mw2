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

#include "CMtfTestActionSendEMsvMtmCommand.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>

#include <smtpcmds.h>
	// for KSMTPMTMIsConnected
#include <mtclbase.h>
	// For MTM's

#include <smscmds.h>
	// for ESmsMtmCommandReadSimParams

#include "MCLIENT.H"
#include "MSERVER.H"



#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include "IPCCommandForCapsPermissions.h"
#include "SendProxyClient.h"
#include "SendProxyserver.h"


/**
  Function : NewL
  Description : 
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @param : aActionParams - CMtfTestActionParameters 
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvMtmCommand object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvMtmCommand::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvMtmCommand* self = new (ELeave) CMtfTestActionSendEMsvMtmCommand(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvMtmCommand
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvMtmCommand::CMtfTestActionSendEMsvMtmCommand(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvMtmCommand
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvMtmCommand::~CMtfTestActionSendEMsvMtmCommand()
	{
	}

/**
  Function : ExecuteActionL
  Description : Entry point for the this test action in the test framework
  @internalTechnology
  @param : none
  @return : void
  @pre none 
  @post none
*/

void MTMCommandReportPassOrFailForCapsPermissions( CMtfTestCase& aTestCase, const TDesC& aTestName, TInt aError ,TInt aPass )
	{
	
	if ( aError == KErrDisconnected )
		{
		if (aPass==1)
			{			
			aTestCase.INFO_PRINTF2(_L("%S :: PASS :: Test Expected to Pass and Passes") , &aTestName );
			aTestCase.SetTestStepResult(EPass);
			}
		else if (aPass==0)
			{
				
			aTestCase.ERR_PRINTF2(_L("%S ::  FAIL :: Test Expected to Fail but Passes") , &aTestName  );
			aTestCase.SetTestStepResult(EFail);
			}
		else
			{
			aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Should Specify PASS/FAIL") , &aTestName );
			aTestCase.SetTestStepResult(EFail);						
			}
		}
	else if ( aError == KErrPermissionDenied )
		{
		if (aPass==1)
			{			
			aTestCase.ERR_PRINTF2(_L("%S ::  FAIL :: Test Expected to Pass but Fails") , &aTestName  );
			aTestCase.SetTestStepResult(EFail);
			}
		else if (aPass==0)
			{
			aTestCase.INFO_PRINTF2(_L("%S :: PASS :: Test Expected to Fail and Passes") , &aTestName );
			aTestCase.SetTestStepResult(EPass);				
			}
		else
			{
			aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Should Specify PASS/FAIL") , &aTestName );
			aTestCase.SetTestStepResult(EFail);						
			}
		}
	else
		{			
		aTestCase.ERR_PRINTF3(_L("%S :: FAIL Unknown error (%d) ") ,  &aTestName , aError );
		aTestCase.SetTestStepResult(EFail);						
		}
	
	}

void CdmaSmsMTMCommandReportPassOrFailForCapsPermissions( CMtfTestCase& aTestCase, const TDesC& aTestName, TInt aError ,TInt aPass )
	{
	
	if ( aError == KErrNotSupported )
		{
		if (aPass==1)
			{			
			aTestCase.INFO_PRINTF2(_L("%S :: PASS :: Test Expected to Pass and Passes") , &aTestName );
			aTestCase.SetTestStepResult(EPass);
			}
		else if (aPass==0)
			{
				
			aTestCase.ERR_PRINTF2(_L("%S ::  FAIL :: Test Expected to Fail but Passes") , &aTestName  );
			aTestCase.SetTestStepResult(EFail);
			}
		else
			{
			aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Should Specify PASS/FAIL") , &aTestName );
			aTestCase.SetTestStepResult(EFail);						
			}
		}
	else if ( aError == KErrPermissionDenied )
		{
		if (aPass==1)
			{			
			aTestCase.ERR_PRINTF2(_L("%S ::  FAIL :: Test Expected to Pass but Fails") , &aTestName  );
			aTestCase.SetTestStepResult(EFail);
			}
		else if (aPass==0)
			{
			aTestCase.INFO_PRINTF2(_L("%S :: PASS :: Test Expected to Fail and Passes") , &aTestName );
			aTestCase.SetTestStepResult(EPass);				
			}
		else
			{
			aTestCase.ERR_PRINTF2(_L("%S :: FAIL :: Should Specify PASS/FAIL") , &aTestName );
			aTestCase.SetTestStepResult(EFail);						
			}
		}
	else
		{			
		aTestCase.ERR_PRINTF3(_L("%S :: FAIL Unknown error (%d) ") ,  &aTestName , aError );
		aTestCase.SetTestStepResult(EFail);						
		}
	}

void CMtfTestActionSendEMsvMtmCommand::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvMtmCommand);
	// input params
	
	CBaseMtm* paramMtm = ObtainParameterReferenceL<CBaseMtm>(TestCase(),ActionParameters().Parameter(0));
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(1) );


	RProxyServerSession RPSS;
	TInt error = RPSS.Connect();
	// Will auto start the server if not started.
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to start proxy server. !"), &KTestActionSendEMsvMtmCommand);
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
				
		TInt error = 0;
		CleanupClosePushL(RPSS);
		
		
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);

		CMsvEntrySelection* entrySelection =  new (ELeave) (CMsvEntrySelection);
		CleanupDeletePushL(entrySelection);
		
		entrySelection->SetReserveL(1);
		
		CMsvEntry& entry = paramMtm->Entry();
		entrySelection->AppendL(entry.EntryId());			
					
		TMsvPackedOperation packedOperation(buffer);

		if (paramMtm->Type() != KUidMsgTypeSMS)
			{
			error = packedOperation.Pack(*entrySelection, KSMTPMTMIsConnected);
			}
		else
			{
			error = packedOperation.Pack(*entrySelection, ESmsMtmCommandReadSimParams);
			}

		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to pack the message") , &KTestActionSendEMsvMtmCommand );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
		
		TMsvOp operationId=1;
		TBuf8<32> param;
		
		
		error=RPSS.SendReceive(EMsvCommandData, TIpcArgs(operationId, buffer, &param));
		if (error)
			{	
			TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to send the message data") , &KTestActionSendEMsvMtmCommand );
			TestCase().SetTestStepResult(EFail);						
			User::LeaveIfError(error);			
			}
				
		error=RPSS.SendReceive(EMsvMtmCommand , TIpcArgs(operationId) );
			// We always expect 
			//		KErrDisconnected (-36) as we have never made a connection to the SMTP server.
			//   or KErrPermissionDenied if the caps checking finds problems.
			
				
		if (paramMtm->Type() != KUidMsgTypeSMS)
			{						
			MTMCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvMtmCommand, error , pass );		

			if ( (pass==0) && (error!=KErrDisconnected) )	
				{ // expected to fail and does fail should not cancel.							
				}
			else
				{					
				TBuf8<64> complete;	
				TInt ret =RPSS.SendReceive(EMsvCancelOperation , 
								TIpcArgs(operationId,  &complete ) );
					// Now lets cancel to stop any panic. We will have data in buffers etc that needs to cleaned out.
					
				if (ret>=0)
					{ // We are happy some sort of length returned.
					}
				else if (ret<0)
					{	
					TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to cancel command") , &KTestActionSendEMsvMtmCommand );
					TestCase().SetTestStepResult(EFail);						
					User::LeaveIfError(error);			
					}			
				}
			}
		else
			{ // Cdma Sms Mtm is used
			CdmaSmsMTMCommandReportPassOrFailForCapsPermissions( TestCase(), KTestActionSendEMsvMtmCommand, error , pass );		

			if (error == KErrNone)
				{
				// Now lets cancel to stop any panic, KErrNone is not expected
				TBuf8<64> complete;	
				TInt ret =RPSS.SendReceive(EMsvCancelOperation , 
								TIpcArgs(operationId,  &complete ) );
					
				TestCase().ERR_PRINTF2(_L("%S :: FAIL :: Unable to cancel command") , &KTestActionSendEMsvMtmCommand );
				TestCase().SetTestStepResult(EFail);						
				}
			}
			
		CleanupStack::Check(entrySelection);
		CleanupStack::PopAndDestroy();		

		CleanupStack::Check(buffer);
		CleanupStack::PopAndDestroy();
		
		CleanupStack::Check(&RPSS);
		CleanupStack::PopAndDestroy();			
		}
			
		
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvMtmCommand);
	TestCase().ActionCompletedL(*this);
			
	}
