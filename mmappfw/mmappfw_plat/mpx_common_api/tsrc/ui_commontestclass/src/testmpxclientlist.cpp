/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: MPXClientList testing implementation
*
*/


#include <mpxclientlist.h>
#include "commontestclass.h"
#include <mpxplaybackmessagedefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectionframeworkdefs.h>

#include "mpxsubscriptiondefs.h" 		//added on 2009,2,12
#include <mpxmediageneraldefs.h>		//added on 2009,2,12


// Begin MPXClientList testing implementation (mpxclientlist.h)============================

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListNewL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListNewL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListNewL testing CMPXClientList::NewL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListNewL testing CMPXClientList::NewL() begin"));
    TInt err=KErrNone;
    if ( iClientList )
        {
        delete iClientList;
        iClientList = NULL;
        }
    iClientList = CMPXClientList::NewL();
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListD()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListDestruction(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListDestruction testing CMPXClientList::~ begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListDestruction testing CMPXClientList::~ begin"));
    TInt err=KErrNone;
    
    delete iClientList;
    iClientList = NULL;
   
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListClientCount()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListClientCount(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListClientCount testing CMPXClientList::ClientCount() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListClientCount testing CMPXClientList::ClientCount() begin"));
    TInt err=KErrNone;
    TInt expectedCount;
    
    // read in parameters
    if ( aItem.GetNextInt(expectedCount) )
        {
        iLog->Log(_L("Missing Parameter: Expected count."));
        return KErrBadTestParameter;
        }
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    
    TInt count = iClientList->ClientCount();
    // verification
    if ( count != expectedCount )
        {
        iLog->Log(_L("Verification Failed: count=%d, expectedCount=%d."), count, expectedCount);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListIsClient()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListIsClient(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListIsClient testing CMPXClientList::IsClient() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListIsClient testing CMPXClientList::IsClient() begin"));
    TInt err=KErrNone;
    TBool expectedResult;
    TThreadId threadId = 2222;

    // read in parameters
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    
    TBool result = iClientList->IsClient(threadId);
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListFindByMsg()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListFindByMsg(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListFindByMsg testing CMPXClientList::Find() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListFindByMsg testing CMPXClientList::Find() begin"));
    TInt err=KErrNone;
    TBool expectedResult;

    // read in parameters
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    if ( !iMessageQueue )
        {
        iLog->Log(_L("Error: MPXMsgQueue not created."));
        return KErrBadTestParameter;
        }
    
    TBool result = iClientList->Find(*iMessageQueue);
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListFindById()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListFindById(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListFindById testing CMPXClientList::Find() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListFindById testing CMPXClientList::Find() begin"));
    TInt err=KErrNone;
    TThreadId threadId = 2222;
    TBool expectedResult;

    // read in parameters
    if ( aItem.GetNextInt(expectedResult) )
        {
        iLog->Log(_L("Missing Parameter: Expected result."));
        return KErrBadTestParameter;
        }
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    
    TBool result = iClientList->Find(threadId);
    // verification
    if ( result != expectedResult )
        {
        iLog->Log(_L("Verification Failed: result=%d, expectedResult=%d."), result, expectedResult);
        err = KErrUnexpectedValue;
        }
    return err;
    }

// To test the destruction function of CCommonTestClass
TInt CCommonTestClass::DeleteMpxClientList()
	{
	FTRACE(FPrint(_L("CCommonTestClass::DeleteMpxClientList testing CMPXClientList::~CMPXClientList() begin")));
	iLog->Log(_L("CCommonTestClass::DeleteMpxClientList testing CMPXClientList::~CMPXClientList() begin"));
	TInt err=KErrNone;
	
	delete iClientList;
	iClientList = NULL;
	
	FTRACE(FPrint(_L("CCommonTestClass::DeleteMpxClientList testing CMPXClientList::~CMPXClientList() end err=%d"),err));
	iLog->Log(_L("CCommonTestClass::DeleteMpxClientList testing CMPXClientList::~CMPXClientList() end err=%d"),err);
	
	return err;
	}
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListAddClientL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListAddClientL()
	{
	FTRACE(FPrint(_L("CCommonTestClass::MpxClientListAddClient testing CMPXClientList::AddClientL() begin")));
	iLog->Log(_L("CCommonTestClass::MpxClientListAddClietn testing CMPXClientList::AddClientL() begin"));
	iClientList = CMPXClientList::NewL();
	if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
        
	if ( !iMessageQueue )
        {
        iMessageQueue = CMPXMessageQueue::NewL(); 
        }
	TRAPD(err, iClientList->AddClientL(  RThread().Id(), iMessageQueue ));
	return err;
	}
//To test CMPXClientList::AddClientL(TThreadId aId,TInt aMode,CMPXMessageQueue* aMsgQueue);
// Returns: Symbian OS errors.
TInt CCommonTestClass::MpxClientListAddClientLUsingMode()
	{
	FTRACE(FPrint(_L("CCommonTestClass::MpxClientListAddClientLUsingMode testing CMPXClientList::AddClientL() begin")));
	iLog->Log(_L("CCommonTestClass::MpxClientListAddClientLUsingMode testing CMPXClientList::AddClientL() begin"));
	TThreadId threadId = RThread().Id();// 2222;
	TInt err = KErrNone;
	TRAP(err , iClientList = CMPXClientList::NewL());
	if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
	
	if ( !iMessageQueue )
        {
        TRAPD(err,iMessageQueue = CMPXMessageQueue::NewL()); 
        }
	TRAP(err,iClientList->AddClientL(threadId,KErrUnknown,iMessageQueue));
	return err;
	}

//To test CMPXClientList::RemoveClient(TInt aIndex)
// Returns: Symbian OS errors.
TInt CCommonTestClass::MpxClientListRemoveClient()
	{
	FTRACE(FPrint(_L("CCommonTestClass::MpxClientListRemoveClient testing CMPXClientList::RemoveClient(TInt aIndex) begin")));
	iLog->Log(_L("CCommonTestClass::MpxClientListRemoveClient testing CMPXClientList::RemoveClient(TInt aIndex) begin"));
		
	TInt iIndex = 0;
	TRAPD(err,iClientList->RemoveClient(iIndex));
	
	return err;
	}

//To test CMPXClientList::ClientProcessList()
// Returns: Symbian OS errors.
TInt CCommonTestClass::MpxClientListClientProcessList()
	{
	FTRACE(FPrint(_L("CCommonTestClass::MpxClientListClientProcessList testing CMPXClientList::ClientProcessList() begin")));
	iLog->Log(_L("CCommonTestClass::MpxClientListClientProcessList testing CMPXClientList::ClientProcessList() begin"));
	
	TInt err = KErrNone;
	TArray<TProcessId> iClientProcess = iClientList->ClientProcessList();
	
	FTRACE(FPrint(_L("CCommonTestClass::MpxClientListClientProcessList testing CMPXClientList::ClientProcessList() end err=%d"), err));
	iLog->Log(_L("CCommonTestClass::MpxClientListClientProcessList testing CMPXClientList::ClientProcessList() end err=%d"), err);
	
	return err;
	}

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListClientMode()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListClientMode(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListClientMode testing CMPXClientList::ClientMode() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListClientMode testing CMPXClientList::ClientMode() begin"));
    TInt err=KErrNone;
    TInt aIndex=0;
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    
   iClientList->ClientMode(aIndex);
    
    iLog->Log(_L("CMPXClientList::ClientMode() return"),err);   
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListSendMsg()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListSendMsg(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListSendMsg testing CMPXClientList::SendMsg() begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListSendMsg testing CMPXClientList::SendMsg() begin"));
    TInt err=KErrNone;
    CMPXMessage* aMsg(NULL);
    TInt aError=KErrNone;
    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
   iClientList->SendMsg(aMsg,aError);
    iLog->Log(_L("CMPXClientList::SendMsg() return"),err);   
    return err;

    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListSendMsgaIndex()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListSendMsgaIndex(CStifItemParser& /*aItem*/)

    {
      FTRACE(FPrint(_L("CCommonTestClass::MpxClientListSendMsgaIndex testing CMPXClientList::SendMsg() begin")));
      iLog->Log(_L("CCommonTestClass::MpxClientListSendMsgaIndex testing CMPXClientList::SendMsg() begin"));
      TInt err=KErrNone;
      TInt aError=KErrNone;
      TInt aIndex=0;
      TMPXMessage message;
      message = TMPXPlaybackMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);
      if ( iClientList== NULL )
            {
            iLog->Log(_L("Error: MPXClientList not created."));
            return KErrBadTestParameter;
            }
      CMPXMessage* cmp = iClientList->ConvertMsgLC(message);
      TRAP(err,iClientList->SendMsg(aIndex, cmp, KErrNone));
      CleanupStack::PopAndDestroy(cmp);
      iLog->Log(_L("CMPXClientList::SendMsg() return"),err);  
      return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SendMsgLByMsgCMPXClientList()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendMsgLByMsgCMPXClientList(CStifItemParser&)
       {
       FTRACE(FPrint(_L("CCommonTestClass::SendMsgLByMsgCMPXClientList testing CMPXClientList::SendMsgL(const TMPXMessage& aMsg) begin")));
       iLog->Log(_L("CCommonTestClass::SendMsgLByMsgCMPXClientList testing CMPXClientList::SendMsgL(const TMPXMessage& aMsg) begin"));
	   TInt err= KErrNone;     	
	   TMPXMessage aMsg; //=TMPXMessage::TMPXMessage();          	
	//    TInt aError=KErrNone;	
	   if ( !iClientList )	
	        {	
	        iLog->Log(_L("Error: MPXClientList not created."));	
	        return KErrBadTestParameter;	
	        }
	   TRAP(err,iClientList->SendMsgL(aMsg));	
	   if(err!=KErrNone)	
	      {	
	      iLog->Log(_L("CMPXClientList::SendMsgL() return"),err);	      	
	      }	
	   return err;
       }

// -----------------------------------------------------------------------------
// CCommonTestClass::MPXClientListConvertMsgLC()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MPXClientListConvertMsgLC(CStifItemParser& )
    {
    FTRACE(FPrint(_L("CCommonTestClass::MPXClientListConvertMsgLC testing CMPXClientList::ConvertMsgLC() begin")));
    iLog->Log(_L("CCommonTestClass::MPXClientListConvertMsgLC testing CMPXClientList::ConvertMsgLC() begin"));
    TInt err=KErrNone;
    //TMPXMessage aMsg =TMPXMessage::TMPXMessage();          
    TMPXMessage message;
    message = TMPXPlaybackMessage(static_cast<TMPXPlaybackMessage::TEvent>(KMPXMessageContentIdGeneral),
                                  EMPXMessageGeneralId,
                                  KMPXMessagePbMediaChanged);
    if ( iClientList== NULL )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
    CMPXMessage* cmp = iClientList->ConvertMsgLC(message);
    CleanupStack::PopAndDestroy(cmp);
    iLog->Log(_L("CMPXClientList::ConvertMsgLC() return"),err);  
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::SendMsgLIndexAndTMPMsgCMPXClientList()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::SendMsgLIndexAndTMPMsgCMPXClientList(CStifItemParser&)
    {
    FTRACE(FPrint(_L("CCommonTestClass::SendMsgLIndexAndTMPMsgCMPXClientList testing CMPXClientList::SendMsgL(TInt aIndex, const TMPXMessage& aMsg) begin")));
    iLog->Log(_L("CCommonTestClass::SendMsgLIndexAndTMPMsgCMPXClientList testing CMPXClientList::SendMsgL(TInt aIndex, const TMPXMessage& aMsg) begin"));
    TInt err= KErrNone;     
    TInt index = 0;
    TMPXMessage aMsg ;//=TMPXMessage::TMPXMessage();          

    if ( !iClientList )
        {
        iLog->Log(_L("Error: MPXClientList not created."));
        return KErrBadTestParameter;
        }
  
    TRAP(err,iClientList->SendMsgL(index,aMsg));

    if(err!=KErrNone)
       {
       iLog->Log(_L("CMPXClientList::SendMsgL() return"),err);       
       }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListAddSubscriptionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListAddSubscriptionL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListAddSubscriptionL testing CMPXClientList::AddSubscriptionL(TInt aIndex,CMPXSubscription* aSubscription) begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListAddSubscriptionL testing CMPXClientList::AddSubscriptionL(TInt aIndex,CMPXSubscription* aSubscription) begin"));
    TInt err= KErrNone;  
    
    TInt index = 0;
    iClientList->AddSubscriptionL(index,iSubscription); 
 
    if(err!=KErrNone)
       {
       iLog->Log(_L("CMPXClientList::AddSubscriptionL() return"),err);       
       }
    return err;
    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListRemoveSubscriptionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListRemoveSubscriptionL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListRemoveSubscriptionL testing CMPXClientList::RemoveSubscriptionL(TInt aIndex,CMPXSubscription* aSubscription) begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListRemoveSubscriptionL testing CMPXClientList::RemoveSubscriptionL(TInt aIndex,CMPXSubscription* aSubscription) begin"));
    TInt err= KErrNone;  
   
    TInt index = 0;
    TRAP(err,iClientList->RemoveSubscriptionL(index,*iSubscription));
    //iClientList->RemoveSubscriptionL(index,*iSubscription);

    if(err!=KErrNone)
       {
       iLog->Log(_L("CMPXClientList::RemoveSubscriptionL() return"),err);       
       }
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxClientListRemoveSubscriptionL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxClientListRemoveAllSubscriptionsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxClientListRemoveAllSubscriptionsL testing CMPXClientList::RemoveAllSubscriptionsL(TInt aIndex) begin")));
    iLog->Log(_L("CCommonTestClass::MpxClientListRemoveAllSubscriptionsL testing CMPXClientList::RemoveAllSubscriptionsL(TInt aIndex) begin"));
    TInt err= KErrNone;     
    
    TInt index = 0;  
    //iClientList->RemoveAllSubscriptionsL(index);    
    TRAP(err,iClientList->RemoveAllSubscriptionsL(index));   
   
    if(err!=KErrNone)
       {
       iLog->Log(_L("CMPXClientList::RemoveSubscriptionL() return error %d."),err);       
       }
    return err;
    }

// end of MPXClientList testing implementation (mpxclientlist.h)===========================
