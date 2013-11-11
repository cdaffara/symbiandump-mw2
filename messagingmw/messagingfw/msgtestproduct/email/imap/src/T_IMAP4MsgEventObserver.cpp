// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// This file contains a generic CMsvEntry Entry Observer for IMAP4
// 
//



#include "T_IMAP4MsgEventObserver.h" 
#include <test/testexecutestepbase.h>
#include <t_utilscentralrepository.h>

//epoc includes
#include <msvids.h>

//literals used

_LIT(KNewChildren,"newChildren");
_LIT(KDeletedChildren,"deletedChildren");
_LIT(KMovedChildren,"movedChildren");

TBufC<12> KNew ( KNewChildren );
TBufC<16> KDelete ( KDeletedChildren );
TBufC<14> KMoved ( KMovedChildren );


/**
  Function : CT_MsgEventObserver
  Description :Constructor			
  @return : n/a
*/
CT_MsgEventObserver::CT_MsgEventObserver()
:iArrayIndex(0)
,iNumberOfMessages(0)
,iObserverAdded(EFalse)
,iServiceId(KMsvNullIndexEntryId)
,iEntryId(KMsvNullIndexEntryId)
,iServiceEntry(NULL)
,iFolderEntry(NULL)
,iObserverEntry(KMsvNullIndexEntryId)
,iEntrySelection(NULL)
{



	


	   	
   
}

/**
  Function : ~CT_MsgEventObserver
  Description : Destructor
  @internalTechnology
  @return : N/A
*/

CT_MsgEventObserver::~CT_MsgEventObserver()
{
	
     if(iObserverAdded)
     {
	   RemoveObserverEntry();
     }
   
		delete iFolderEntry;
		iFolderEntry = NULL;
   
    	delete iEntrySelection;
		iEntrySelection = NULL;	
  
		delete iServiceEntry;
		iServiceEntry=NULL;

		delete iObserverEntry;
		iObserverEntry=NULL;

	
}

/**
  Function : SetAccountL
  Description : retrieves the service id from the repository via the account name.
                SetAccountL
  @param : TDes& aAccountName
  @return : void
*/


 void CT_MsgEventObserver::SetAccountL(TDes& aAccountName)
 {
 		iServiceId = CT_MsgUtilsCentralRepository::GetImapServiceIdL(aAccountName);
        
 }
 
 
 /**
  Function : GetServiceIdL
  Description : returns service id, can be used by a test step.
  @return : TMsvId
*/
TMsvId CT_MsgEventObserver::GetServiceIdL()
{
	// do error checking here i.e KMsvNullIndexEntryId
	if (iServiceId==KMsvNullIndexEntryId)
	{
		//error Leave!
		User::Leave(KErrGeneral);
	}
		return iServiceId;
}
 
 
  /**
  Function : SetServiceEntryL
  Description : creates a cmsventry object using the service id
  @param :CMsvSession&
  @return : void
*/
 
 void CT_MsgEventObserver::SetServiceEntryL(CMsvSession& aSession)
 {

 	 iServiceEntry = CMsvEntry::NewL(aSession, GetServiceIdL(),TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
 }
 
 
  /**
  Function : FindFolderL
  Description : Finds the TMsvId of the folder specified, and sets an observer on it.
  				Depending on the scenario types listed below, uses of the parameters aEntryName,
  				aParentEntryName, aRootParentEntryName will need to be as follows.
  				
  				SCENARIO A:
                If the  folder searched  for is just under the account entry i.e at the same level
                as special folders (such as inbox).
                In this scenario aRootParentEntryName is the accountname in question, aParentEntryName 
                is also accountname, and aEntryName is the entry being look for.
                						
                							EXAMPLE A folder to search for is inbox 
                							PATH: account/inbox
                							aRootParentEntryName = [accountname]   
                							aParentEntryName = [accountname]
                							aEntryName= "inbox"
                
               								EXAMPLE B folder to search for is jobs
               								(and jobs is on the same level as the inbox)
               								PATH: account/jobs
                 							aRootParentEntryName = [accountname]   
                							aParentEntryName = [accountname]
                							aEntryName= "jobs"
                
                SCENARIO B:
                
                The folder being search for is further down the hierarchy, in that it is NOT just under
                the account  entry.In this scenario aRootParententry is the entry immediately under 
                the account entry, aParentEntryName is the immediate parent of the folder entry in question,
                and finally aEntryName is the folder entry in question.
                Examples to clarify. 
                EXAMPLE A: folder to search is boo
                PATH: account/inbox/zanzibar/far/boo
                aRootParentEntryName =inbox
                aParentEntryName =far
                aEntryName=boo
                 
                EXAMPLE B: folder to search is syrup
                PATH: account/tree/maple/syrup
                aRootParentEntryName =tree
                aParentEntryName =maple
                aEntryName=syrup
                  
                EXAMPLE C folder to search is zoo
                PATH: account/drafts/zoo
                aRootParentEntryName =drafts
                aParentEntryName =drafts
                aEntryName=zoo
          
  @param :CMsvSession& aSession
  @param :TDesC& aEntryName
  @param :TDesC& aParentEntryName
  @param :TDesC& aRootParentEntryName
  @param :TMsvId aId
  @return :TMsvId
*/
 
TMsvId CT_MsgEventObserver::FindFolderL(CMsvSession& aSession,TDesC& aEntryName, TDesC& aParentEntryName, TDesC& aRootParentEntryName,TMsvId aId) 
  {
  
 	 // Set the context to the Parent
	iFolderEntry= CMsvEntry::NewL(aSession, aId,TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
 	iFolderEntry->SetEntryL(aId);
    TMsvId entryId=KMsvNullIndexEntryId;
    
 	//retrieve all children from the  parent entry
 	iEntrySelection=NULL;
	iEntrySelection=iFolderEntry->ChildrenL();
				
 	TInt count = iEntrySelection->Count();
 	
  	for(TInt i=0;i<count;++i)
  	{
  	
  	// Set the context to the child entry
	iFolderEntry->SetEntryL((*iEntrySelection)[i]);
	// Set the context to the Parent
	CMsvEntry* parentEntry = CMsvEntry::NewL(aSession, iFolderEntry->Entry().Parent(),TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
 	CleanupStack::PushL(parentEntry);
  	//check is this the current folder the folder we are looking for
  	if (( aEntryName.CompareF( iFolderEntry->Entry().iDescription ) == 0 ) ||
    		( aEntryName.CompareF( iFolderEntry->Entry().iDetails ) == 0 ))
   		{
   		    	
		//does its parent folder match our parent folder
		if (( aParentEntryName.CompareF( parentEntry->Entry().iDescription ) == 0 ) ||
     		( aParentEntryName.CompareF( parentEntry->Entry().iDetails ) == 0 ))
 			{
 			entryId = (*iEntrySelection)[i];
 			CleanupStack::PopAndDestroy(parentEntry);
 			break;
 			}
 			
   		}	
   			
   	//check if current folder is the parent then traverse down
  	else if (( aParentEntryName.CompareF( iFolderEntry->Entry().iDescription ) == 0 ) ||
    			( aParentEntryName.CompareF( iFolderEntry->Entry().iDetails ) == 0 ))
  			{
  			//traverse into specifc child folder
   	     	entryId= FindFolderL(aSession, aEntryName,aParentEntryName,aRootParentEntryName,(*iEntrySelection)[i]); 
   			}
   						
  	//check if current folder is the root parent then traverse down
   	else if (( aRootParentEntryName.CompareF( iFolderEntry->Entry().iDescription ) == 0 ) ||
    			( aRootParentEntryName.CompareF( iFolderEntry->Entry().iDetails ) == 0 ))
   			{
   			//traverse into 
   			entryId= FindFolderL(aSession, aEntryName,aParentEntryName,aRootParentEntryName,(*iEntrySelection)[i]); 
   			}
	
   	
   	CleanupStack::PopAndDestroy(parentEntry);
   		
  	}
 
 
 	//set  observer
 	SetObserverEntryL(entryId, aSession);
 	return entryId;

  	
  }
  
  /**
  Function : SetObserverEntryL
  Description :Adds an entry observer to the requested folder by creating
  			   a cmsventry obj. It is called iternally by FindFolderL		
  @param : TMsvId aEntryId
  @param : CMsvSession& 
  @return : void

*/	
 void CT_MsgEventObserver::SetObserverEntryL(TMsvId aEntryId, CMsvSession& aSession)
 {      
        //create a cmsventry entry
   		iObserverEntry= CMsvEntry::NewL(aSession, aEntryId,TMsvSelectionOrdering(KMsvNoGrouping, EMsvSortByNone, ETrue));
		//add an entry observer to it
		iObserverEntry->AddObserverL(*this);
		SetFolderId(aEntryId);
	    iObserverAdded=ETrue;    

 }
 
 
  /**
  Function : SetFolderId
  Description : Simple setter function to set the folder id
  			    called internally by SetObserverEntryL
  @param :TMsvId aEntryId
  @return : void

*/

 void CT_MsgEventObserver::SetFolderId(TMsvId aEntryId)
 {
 	iEntryId=aEntryId;
 }
 
 
 
 /**
  Function :  RemoveObserverEntry
  Description : Deregistering entry notifications on the folder specified	
  				we call function ResetObserver	
  @return : void

*/
 
 void CT_MsgEventObserver::RemoveObserverEntry()
 {  
    //remove observer & reset
    iObserverEntry->RemoveObserver(*this);
 	iObserverAdded=EFalse;
    ResetObserver();
 }
 
 
 
 
 /**
  Function :ResetObserver
  Description : Called by RemoveObserverEntry, this resets
  				msg counter, array index,& array
  @return : void

*/
 
 void CT_MsgEventObserver::ResetObserver()
 {
 	iNumberOfMessages=0;//reset msg counter
 	iArrayIndex=0;//reset array index
 	iDataArray.ResetAndDestroy();//reset array;	
 }
 
 
 
/**
  Function : NewL
  Description : Creates an object of CT_MsgEventObserver 				 
  @return : N/A
*/

CT_MsgEventObserver* CT_MsgEventObserver::NewL()
{
	CT_MsgEventObserver* self = new(ELeave) CT_MsgEventObserver();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}


/**
  Function : ConstructL
  Description : N/A future placeholder at the moment
  @return : N/A
*/
	
	
void CT_MsgEventObserver::ConstructL()
{
	//future placeholder

	

}

 

/**
  Function : HandleEntryEventL
  Description : Observer call back.Called by CMsvEntry when a messaging event has occurred.			
  @param : TMsvEntryEvent aEvent
  @param : TAny* aArg1
  @param : TAny* aArg2
  @param : TAny* aArg3
  @return : void

*/
void CT_MsgEventObserver::HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	
		DoHandleEntryEventL(aEvent, aArg1, aArg2, aArg3);
		
	
	}
	


/**
  Function : CheckEventAndCountL
  Description : This function is called by a test step
  				and checks and counts the number of entires effected by a specified event.
  				events that are checked are EMsvNewChildren,EMsvDeletedChildren,EMsvEntryMoved.
  				literals used as comparison are "newChildren","deletedChildren","movedChildren". 								
  @param : TDesC& aEvent
  @return : TInt
*/

TInt CT_MsgEventObserver::CheckEventAndCountL(TDesC& aEvent)
{



   	TMsvEntryEvent aEventType;
   	TPtrC newChildren= KNew.Des();
   	TPtrC deletedChildren=KDelete.Des();
   	TPtrC movedChildren=KMoved.Des();
   	
   
   	TInt aMsgsEffected=0;
   
   	if (aEvent==((TDesC&)newChildren))
   		{
   	  	aEventType=EMsvNewChildren;
   		}
   		
   else if(aEvent==((TDesC&)deletedChildren))
   		{
   		aEventType=EMsvDeletedChildren;
   		}
   		
   else if (aEvent==((TDesC&)movedChildren))
   		{
   		aEventType=EMsvEntryMoved;
   		}
   
   else 
   		{
   		//error checking here 
   		User::Leave(KErrGeneral);
   		}
   
   for(TInt count=0; count<iArrayIndex;++count)
   		{
   	
   		 if (iDataArray[count]->iEventType==aEventType)
   	 		{
   	 		aMsgsEffected=iDataArray[count]->iNumofMsg;
   	 		}
   	 	
   		iNumberOfMessages=iNumberOfMessages+aMsgsEffected;
   	
   		}

 return	iNumberOfMessages;
}



/**
  Function : AddInfoL
  Description : This function is called by the handler DoHandleEntryEventL,
  				it records the event type and number of entries effected and places
  				them in an array of type CT_MsgEventStore
  @param : TMsvEntryEvent aEvent
  @param : TInt aNum
  @param : TInt aIndex
  @return : void

*/

void CT_MsgEventObserver::AddInfoL(TMsvEntryEvent aEvent, TInt aNum,TInt aIndex)
{
 
 	//create my object to place into iDataArray
    CT_MsgEventStore* info=new (ELeave)CT_MsgEventStore(aEvent,aNum);
    CleanupStack::PushL(info);
    //add my object to iDataArray;
    iDataArray.InsertL(info,aIndex);
    CleanupStack::Pop(info);
  	
}

/**
  Function : DoHandleEntryEventL
  Description : Called by HandleEntryEventL when a messaging event has occurred.
                Events that are recorded by this callback are only  EMsvNewChildren,EMsvDeletedChildren,EMsvEntryMoved						
  @param : TMsvEntryEvent aEvent
  @param : TAny* aArg1
  @param : TAny* aArg2 this parameter is not being used within this function.
  @param : TAny* aArg3 this parameter is not being used within this function.
  @return : void

*/

	
void CT_MsgEventObserver::DoHandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* /*aArg2*/, TAny* /*aArg3*/)
{
	

   
     iEntrySelection=NULL;
  
       	// The messages in the folder have been updated - check them.
	if(  aEvent == EMsvNewChildren|| aEvent==EMsvDeletedChildren || aEvent==EMsvEntryMoved )
		{
		iEntrySelection = static_cast<CMsvEntrySelection*>(aArg1);
		AddInfoL(aEvent, iEntrySelection->Count(),iArrayIndex);
        iArrayIndex=iArrayIndex+1;
		}
	
     
}



 
 
 
 
 
 
	
	

	  


