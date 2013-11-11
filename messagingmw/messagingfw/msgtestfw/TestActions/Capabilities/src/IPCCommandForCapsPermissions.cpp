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

#include "IPCCommandForCapsPermissions.h"
#include "pigeonservermtm.h"
#include <e32std.h>
#include <e32base.h>


#include "MCLIENT.H"
#include "MSERVER.H"

#include "SendProxyClient.h"


#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"



void IPCCommandReportPassOrFailForCapsPermissions( CMtfTestCase& aTestCase, const TDesC& aTestName, TInt aError ,TInt aPass )
	{
	
	if ( aError == KErrNone )
		{
		if (aPass==1)
			{			
			aTestCase.INFO_PRINTF2(_L("%S :: PASS :: Test Expected to Fail and Fails") , &aTestName );
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


_LIT(KDescription , "CreateEntry" );
_LIT(KNewAddress , "Symbian" );


void FillInSimpleMessageEntry( TMsvId aMessageId, TMsvId aParentId, TMsvId aServiceId, TMsvEntryForward& aEntry)
{
	
	aEntry.SetId(aMessageId);
	aEntry.SetParent( aParentId );		
	aEntry.iType = KUidMsvMessageEntry;
	aEntry.iMtm = KUidMsgTypeSMTP;	
	aEntry.iServiceId = aServiceId;
	
	aEntry.iDescription.Set(KDescription);
	
	aEntry.iDetails.Set(KNewAddress);
	aEntry.iDate.HomeTime();

}

void FillInServiceEntry( TMsvId aMessageId, TMsvEntryForward& aEntry)
{
	// This is a service entry, so we need to set parent to root and service ID to
	// the entry id (ie serviceId is same as entry id)
	aEntry.SetId(aMessageId);
	aEntry.SetParent( KMsvRootIndexEntryId );		
	aEntry.iType = KUidMsvServiceEntry;
	aEntry.iMtm = KUidMsgTypeSMTP;	
	aEntry.iServiceId = aMessageId;
	
	aEntry.iDescription.Set(KDescription);
	aEntry.iDetails.Set(KNewAddress);
	aEntry.iDate.HomeTime();
}


void FillInRemoteMessageEntry( TMsvId aMessageId, TMsvId aParentId, TMsvId aServiceId, TMsvEntryForward& aEntry)
{
	aEntry.SetId(aMessageId);
	aEntry.SetParent( aParentId );		
	aEntry.iType = KUidMsvMessageEntry;
	aEntry.iMtm = KUidMsgTypePigeon;	
	aEntry.iServiceId = aServiceId;
	
	aEntry.iDescription.Set(KDescription);
	
	aEntry.iDetails.Set(KNewAddress);
	aEntry.iDate.HomeTime();

}

_LIT(KNewAddress1 , "OverwriteMe" );
	

// Useful routine just to see and check what has actally gone into the messaginging server.
// Note must have correct capabilities.
	
void CheckWentInStoreL(CMtfTestCase& aTestCase, const TDesC& aTestName,  
	RProxyServerSession& RPSS , TMsvId id,  TMsvEntryForward& entry , TMsvOp& operationId )
	{
		
	HBufC8* iBuffer = HBufC8::NewLC(KMsvSessionBufferLength);						
	TPtr8 ptr=iBuffer->Des();
	TMsvEntryForward entryStore=entry;
	entryStore.SetId(id);
	
	entryStore.iDetails.Set( KNewAddress1 );
	
	TMsvId aService(0); 
	TPckg<TMsvId> service(aService);

	operationId++;
	TInt error = RPSS.SendReceive(EMsvGetEntry,TIpcArgs(entryStore.Id(),&ptr,&service));
	if (error!=KErrNone)
		{
		aTestCase.ERR_PRINTF2(_L("%S :: PROBLEM :: Unable to read the message back.") , &aTestName );
		User::LeaveIfError(error);		
		}
		
		
	TMsvPackedEntry packedEntry(iBuffer);
	packedEntry.UnpackEntry(entryStore);		
	
	if ( entryStore.iDetails == KNewAddress1 )	
		{
		aTestCase.ERR_PRINTF2(_L("%S :: PROBLEM :: Message was not modified in the store.") , &aTestName );
		User::LeaveIfError(error);				
		}		
	else 
		{			
		//RDebug::Printf( " CheckWentInStoreL :: WORKS!!!!!!!!!!!!!!!!!!!!!!!!\n" ); //?????
		}
	
	CleanupStack::Check(iBuffer);
	CleanupStack::PopAndDestroy();
	
	}
