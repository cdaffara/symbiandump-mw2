/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
* @file
* This contains the class CT_MsgEventObserver
* 
*
*/




//Epoc include
#include <mtclbase.h>//Specifies function IDs for standard MTM functions
//userincludes
#include "T_IMAP4MsgEventStore.h"
#ifndef __T_IMAP4_MSG_EVENT_OBSERVER_H__
#define __T_IMAP4_MSG_EVENT_OBSERVER_H__





class   CT_MsgEventObserver :  public  MMsvEntryObserver
{

	public:
	    
	    static CT_MsgEventObserver* NewL ();
		void HandleEntryEventL(TMsvEntryEvent aEvent, TAny *aArg1, TAny *aArg2, TAny *aArg3);
	    ~CT_MsgEventObserver();
	    void SetAccountL(TDes& aAccountName);
	    TMsvId GetServiceIdL();
	    void SetServiceEntryL(CMsvSession& aSession);
	    TMsvId FindFolderL(CMsvSession& aSession,TDesC& aEntryName,TDesC& aParentEntryName, TDesC& aRootParentEntryName,TMsvId aId);
	    
	    void RemoveObserverEntry();
	    void SetFolderId(TMsvId aEntryId);
	    TInt CheckEventAndCountL(TDesC& aEvent);
	  
	   
	  
	
	private:
	     
	     CT_MsgEventObserver();
	     void ConstructL();
	     void AddInfoL(TMsvEntryEvent aEntryEvent, TInt aNum,TInt aIndex);
	     void ResetObserver();
	     void SetObserverEntryL(TMsvId aEntryId, CMsvSession& aSession);
	      
	protected: 
	
		void DoHandleEntryEventL(TMsvEntryEvent aEvent, TAny *aArg1, TAny *aArg2, TAny *aArg3);
	    
	    
	private:
	
	
	    TInt iArrayIndex,iNumberOfMessages;
	    TBool iObserverAdded;
	    TMsvId iServiceId,iEntryId;
	    CMsvEntry* iServiceEntry;
	    CMsvEntry* iFolderEntry;
	    CMsvEntry* iObserverEntry;
	    CMsvEntrySelection* iEntrySelection;
	    CMsvSession* iSession;
	    RPointerArray<CT_MsgEventStore> iDataArray;
	   	
};








#endif//__T_IMAP4_MSG_EVENT_OBSERVER_H__