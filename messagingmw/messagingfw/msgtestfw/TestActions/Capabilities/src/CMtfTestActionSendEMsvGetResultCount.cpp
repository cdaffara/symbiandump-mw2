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

#include "CMtfTestActionSendEMsvGetResultCount.h"

#include <e32std.h>
#include <e32base.h>

#include <msvipc.h>
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
  @return : CMtfTestAction* - a base class pointer to the newly created CMtfTestActionSendEMsvGetEntry object
  @pre none
  @post none
*/
CMtfTestAction* CMtfTestActionSendEMsvGetResultCount::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSendEMsvGetResultCount* self = new (ELeave) CMtfTestActionSendEMsvGetResultCount(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function : CMtfTestActionSendEMsvGetEntry
  Description : Constructor
  @internalTechnology
  @param : aTestCase - CMtfTestCase for the CMtfTestAction base class
  @return : N/A
  @pre none
  @post none
*/
CMtfTestActionSendEMsvGetResultCount::CMtfTestActionSendEMsvGetResultCount(CMtfTestCase& aTestCase) : CMtfSynchronousTestAction(aTestCase)
	{
	}
	
/**
  Function : ~CMtfTestActionSendEMsvGetEntry
  Description : Destructor
  @internalTechnology
  @param :
  @return : 
  @pre 
  @post 
*/
CMtfTestActionSendEMsvGetResultCount::~CMtfTestActionSendEMsvGetResultCount()
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




void CMtfTestActionSendEMsvGetResultCount::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSendEMsvGetResultCount);
	TInt pass = ObtainValueParameterL<TInt>(TestCase(),ActionParameters().Parameter(0) );

	RProxyServerSession RPSS;
		// Will auto start the ProxyServer( which will start the MessagingServer(MSExe.exe) ) if not started.
	
	TInt error = RPSS.Connect();
	CleanupClosePushL(RPSS);
	
	if (error!=KErrNone)
		{
		TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetEntry :: Unable to start proxy server. !"));
		TestCase().SetTestStepResult(EFail);
		}
	else
		{
		TInt32 serverCaps;
		RPSS.GetServerCapabilities(serverCaps);
		TestCase().INFO_PRINTF2(_L("Proxy Server has capabilities: %x"), serverCaps);
						
		HBufC8* buffer = HBufC8::NewLC(KMsvSessionBufferLength);
		if (buffer==NULL)
			{
			TestCase().ERR_PRINTF1(_L("CMtfTestActionSendEMsvGetResultCount :: Unable to allocate mem. !"));
			TestCase().SetTestStepResult(EFail);
			}
		
		
		CMsvSearchSortQuery* searchQuery = CMsvSearchSortQuery::NewL();
		CleanupStack::PushL(searchQuery);
	
		searchQuery->SetParentId(KMsvGlobalOutBoxIndexEntryIdValue);
		searchQuery->SetResultType(EMsvResultAsTMsvId);
		searchQuery->SetWholeWord(ETrue);
		searchQuery->SetCaseSensitiveOption(ETrue);
		searchQuery->SetWildCardSearch(EFalse);
		
		
		TInt error = KErrNone;
	
				
		
		error = 0;
		TPtr8 ptr=buffer->Des();
		
		TBool markquery=0;
		TInt iterator=0;
		TInt queryid=0;
		
		TMsvPackQuery packedQuery(buffer);
		error = packedQuery.PackQuery(searchQuery);
		while(error!=KErrNone)
			{
			// increase the size of the buffer and try again
			buffer->Des().SetLength(0); // to avoid copying contents
			buffer = buffer->ReAllocL(buffer->Des().MaxSize() + KMsvSessionBufferLength);
			error = packedQuery.PackQuery(searchQuery);
			}
		TInt aMarkQuery = 0;
		TInt aOperationId =78;
   	    error=RPSS.SendReceiveProxyAsync(EMsvSearchSortOperation, TIpcArgs(aOperationId, buffer, aMarkQuery));
   	    
   	    if(!error)
   	    	{
   	    	TestCase().ERR_PRINTF2(_L("Executed ::  %S"), &KTestActionSearchSortOperation);
   	    	}
   	    
   	    aOperationId++;
   	    error=RPSS.SendReceiveProxyAsync(EMsvSearchSortOnIndexEntry, TIpcArgs(aOperationId, buffer, markquery,iterator));
   	    if(!error)
   	       	{
   	       	TestCase().ERR_PRINTF2(_L("Executed ::  %S"), &KTestActionSearchSortOnIndexEntry);
   	       	}
   	    
   	    aOperationId++;
		error=RPSS.SendReceiveProxyAsync(EMsvSearchSortQueryId, TIpcArgs(aOperationId, queryid, iterator));
		if(!error)
		   	{
		   	TestCase().ERR_PRINTF2(_L("Executed ::  %S"), &KTestActionSearchSortQueryId);
		   	}
		
		aOperationId++;
		TInt aQueryId=1001;
		error=RPSS.SendReceiveProxyAsync(EMsvQueryData, TIpcArgs(aOperationId, aQueryId, &ptr));
		if(!error)
			{
			TestCase().ERR_PRINTF2(_L("Executed ::  %S"), &KTestActionQueryData);
			}
		
				
		aOperationId++;
		error=RPSS.SendReceiveProxyAsync(EMsvCancelSearchSortOp, TIpcArgs(aOperationId));
		if(!error)
		   	{
		   	TestCase().ERR_PRINTF2(_L("Executed ::  %S"), &KTestActionCancelSearchSortOp);
		   	}
		
	  	    	
		IPCCommandReportPassOrFailForCapsPermissions( TestCase(), KMsvServerName, error , pass );
		
		CleanupStack::PopAndDestroy(searchQuery);
		CleanupStack::PopAndDestroy(buffer);
		

		}
	
	CleanupStack::Check(&RPSS);
	CleanupStack::PopAndDestroy(); // RPSS
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSendEMsvGetResultCount);
	TestCase().ActionCompletedL(*this);	
	
	}
