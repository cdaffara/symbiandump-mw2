// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// There are a Unit test and OOM test that demonstrates the use of the SendAs2 APIs for the PREQ 1881 to send messages. 
// 
//

/**
 @file
*/
#include "T_SendAs2_Message_Send_SetCharset.h"
#include <mtclbase.h>
#include <gsmuelem.h> 
#include "sendas2test.h"
RTest test(_L("SendAs2 Message Send Start."));

_LIT(KBodyText, "You are limited only by your fears. Remember it is attitude, more than aptitude,that determines your altitude");
_LIT(KAddress1, "test1@symbian.com");
_LIT(KAlias1, "Alias1");
_LIT(KAlias2, "Alias2");
_LIT(KAccount1, " \n SendAs Account1 \n");
_LIT(KAccount2, " \n SendAs Account2 \n");
_LIT(KAccount3, " \n SendAs Account3 \n");
_LIT(KSmsClient, "SMS Client");

/**
Allocates and constructs a CSendAs2SetMessageCharset object.
Initialises all member data to their default values.
*/  
CSendAs2SetMessageCharset* CSendAs2SetMessageCharset::NewL()
    {
    CSendAs2SetMessageCharset* self = new (ELeave) CSendAs2SetMessageCharset();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
    
/**
Constructor
*/  
CSendAs2SetMessageCharset::CSendAs2SetMessageCharset()
    {      
    }

/**
Creating a console window
*/
void CSendAs2SetMessageCharset::ConstructL()
    {
    
    }   
    
/**
Cleans the application
    - closes the sendAs server
    - deletes the console window
    - deletes all other member variables
*/
void CSendAs2SetMessageCharset::Delete()
{

}

CSendAs2SetMessageCharset::~CSendAs2SetMessageCharset()
    {   
        
    delete iSelection;
    iSelection = NULL;
    
    delete iEntry;
    iEntry = NULL;
    
    delete iSession;
    iSession = NULL;
    
    delete iObserver;
    iObserver = NULL;
   
    
    iSendAs.Close();    
    }
    
// Unit test case for settting the 8 bit charset value. 
void CSendAs2SetMessageCharset::TestSet8BitCharsetSendMessageL(RSendAsMessage &message)
	{
    theUtils->WriteComment(_L("Set 8 bit Unicode charset for a message."));
    message.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabet8Bit);
	}

// Unit test case for settting the 7 bit charset value.	
void CSendAs2SetMessageCharset::TestSet7BitCharsetSendMessageL(RSendAsMessage &message)
	{
    theUtils->WriteComment(_L("Set 7 bit Unicode charset for a message."));
    message.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabet7Bit); 
	}
	
// Unit test case for settting the 16 bit Unicode charset value.	
void CSendAs2SetMessageCharset::TestSet16BitUnicodeCharsetSendMessageL(RSendAsMessage &message)
	{
    theUtils->WriteComment(_L("Set 16 bit Unicode charset for a message."));
    message.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabetUCS2);
	}
  
 
// OOM test for UCS2 charset value : This test is to test the API behavior during OOM condition.
void CSendAs2SetMessageCharset::TestOOMForUCS2(RSendAsMessage& aMessage)
	{
	// MEMORY TESTS
	TBool finished;
	TInt numEntries =10;
	
	theUtils->WriteComment(_L("Started OOM test for setting charset of a message."));

	for(TInt i=0; i<numEntries; i++)
		{
	#if defined(_DEBUG)	
		TInt failCount;
		failCount=0;
	#endif	
		finished = EFalse;
		// Typical pattern for OOM testing - loop until OOM test passes
		while (!finished)
			{
			// Call a SetCharacterSetL method of a Message.
			aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabetUCS2); // add charset value
__UHEAP_FAILNEXT(failCount++);
			TRAPD(ret, aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabetUCS2));
__UHEAP_RESET;

			if (ret == KErrNone)
				{
				finished = ETrue;
				}
			else
				{
				test(ret == KErrNoMemory);
				}  
			}
		}
	}
	
// OOM test for 8 Bit charset value	
void CSendAs2SetMessageCharset::TestOOMFor8Bit(RSendAsMessage& aMessage)
	{
	// MEMORY TESTS
	TBool finished;
	TInt numEntries =10;
	
	theUtils->WriteComment(_L("Started OOM test for setting charset of a message."));

	for(TInt i=0; i<numEntries; i++)
		{
	#if defined(_DEBUG)	
		TInt failCount;
		failCount=0;
	#endif	
		finished = EFalse;
		// Typical pattern for OOM testing - loop until OOM test passes
		while (!finished)
			{
			// Call a SetCharacterSetL method of a Message.
			aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabet8Bit); // add charset value
__UHEAP_FAILNEXT(failCount++);
			TRAPD(ret, aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabetUCS2));
__UHEAP_RESET;

			if (ret == KErrNone)
				{
				finished = ETrue;
				}
			else
				{
				test(ret == KErrNoMemory);
				}  
			}
		}
	}

// OOM test for 7 Bit charset value	
void CSendAs2SetMessageCharset::TestOOMFor7Bit(RSendAsMessage& aMessage)
	{
	// MEMORY TESTS
	TBool finished;
	TInt numEntries =10;
	
	theUtils->WriteComment(_L("Started OOM test for setting charset of a message."));

	for(TInt i=0; i<numEntries; i++)
		{
	#if defined(_DEBUG)	
		TInt failCount;
		failCount=0;
	#endif	
		finished = EFalse;
		// Typical pattern for OOM testing - loop until OOM test passes
		while (!finished)
			{
			// Call a SetCharacterSetL method of a Message.
			aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabet7Bit); // add charset value
__UHEAP_FAILNEXT(failCount++);
			TRAPD(ret, aMessage.SetCharacterSetL(TSmsDataCodingScheme::ESmsAlphabetUCS2));
__UHEAP_RESET;

			if (ret == KErrNone)
				{
				finished = ETrue;
				}
			else
				{
				test(ret == KErrNoMemory);
				}  
			}
		}
	}
// end of OOM test.
    
/**
Main function which calls other functions.
Creates a session observer and sets the entry on drafts folder.
Also, gets a selection containing the IDs of all the context children.
@leave KErrNoMemory, system wide error codes    
*/
void CSendAs2SetMessageCharset::StartL()
    {
    iObserver = new(ELeave) CDummyObserver;
    iSession = CMsvSession::OpenSyncL(*iObserver);

    TMsvId aFolderId = KMsvDraftEntryId;
    iEntry = CMsvEntry::NewL(*iSession, aFolderId, 
        TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
    
    iSelection = iEntry->ChildrenL();
    
    Connect();
    theUtils->Start(_L("Message Async Send"));
    RSendAsMessage message;

    CleanMessageFolderL();
    theUtils->WriteComment(_L("Create a message."));
    CreateL(message);
    DisplayAccountL();
    
    // Setting the 8-bit charset value.
    TestSet8BitCharsetSendMessageL(message);
    // Setting the 7-bit charset value.
 	TestSet7BitCharsetSendMessageL(message);
 	// Setting the 16-bit Unicode charset value.
 	TestSet16BitUnicodeCharsetSendMessageL(message);
 	
	// OOM test for UCS2 charset value
	TestOOMForUCS2(message);
		 	
	// OOM test for 8 Bit charset value
	TestOOMFor8Bit(message);
		 	
	// OOM test for 7 Bit Charset value
	TestOOMFor7Bit(message);

    SendL(message);
    theUtils->Complete();

    }

/**
Creates a session
    - based on the time slots
    - connects to the server
    @leave system wide error codes
*/
void CSendAs2SetMessageCharset::Connect()
    {
       TInt err = iSendAs.Connect(KSendAsDefaultMessageSlots);
    }

/**
Clears the message folder passed in the entry before sending any new messages.
*/
void CSendAs2SetMessageCharset::CleanMessageFolderL()
    {
    TMsvLocalOperationProgress progress;
    if (iSelection->Count() > 0)
        {
        iEntry->DeleteL(*iSelection, progress);
        }   
    }

/**
Creates a SendAs message
@param aMessage An RSendAsMessage reference, used to create a message
*/
void CSendAs2SetMessageCharset::CreateL( RSendAsMessage& aMessage)
    {
        
    CSendAsMessageTypes* messageTypes = CSendAsMessageTypes::NewL();
    CleanupStack::PushL(messageTypes);  

    // Filter all MTMs that can send messages, currently using SMS MTM 
    iSendAs.FilteredMessageTypesL(*messageTypes);

    TUid sendAsMtmUid;
        
    // Returning the message UID based on the message type 
    // such as 0 for SMTP Client, 1 for SMS Client
    // 2 for Infrared client MTM and 3 for Bluetooth client MTM.
    sendAsMtmUid = messageTypes->UidFromNameL(KSmsClient);
    
    CleanupStack::PopAndDestroy(messageTypes);  
        
    TInt count = iSelection->Count();
    iEntry->SetEntryL(KMsvDraftEntryId);
    
    aMessage.CreateL(iSendAs, sendAsMtmUid);
    
    /** Set the body text of this message using a plain descriptor. 
     The object must have an open message to use this method.
    */
    aMessage.SetBodyTextL(KBodyText);
    
    // Add recipients to this message.
    aMessage.AddRecipientL(KAddress1, KAlias1, RSendAsMessage::ESendAsRecipientTo);

    CMsvEntry* entry = CMsvEntry::NewL(*iSession, KMsvDraftEntryId, 
        TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
    CleanupStack::PushL(entry);
    
    // Sets the context to the specified entry.
    entry->SetEntryL(KMsvDraftEntryId);
    if(iSelection->Count() >= 1)
        {
        entry->SetEntryL((*iSelection)[0]);
        };
        
    CleanupStack::PopAndDestroy(entry);
    }
    
/**
Displays the names of all the available accounts.
Creates accounts, if not already created.
*/
void CSendAs2SetMessageCharset::DisplayAccountL()
    {
    CSendAsAccounts* accounts = CSendAsAccounts::NewL();
    CleanupStack::PushL(accounts);
    
    // Append 3 new accounts and associated name pair.
    if(accounts->Count()==0)
        {
        accounts->AppendAccountL(KAccount1, 0xaaaaaaaa); 
        }
    
    if(accounts->Count()==1)
        {
        accounts->AppendAccountL(KAccount2, 0x55555555); 
        }
        
    if(accounts->Count()==2)
        {
        accounts->AppendAccountL(KAccount3, 0x22222222); 
        }
    
    
    // Display the array of names of accounts for this message type
    for(TInt i = 0; i < 3; i++)
        {
        TPtrC array = accounts->AccountNames().MdcaPoint(i);
            }
        
    CleanupStack::PopAndDestroy(accounts);
    
    }
    
/**
Sends the sendAs2 message
@param aMessage An RSendAsMessage reference, used to send a message
*/
void CSendAs2SetMessageCharset::SendL(RSendAsMessage& aMessage)
    {
    
    CMsvOperationWait* wait = CMsvOperationWait::NewLC();
    wait->iStatus = KRequestPending;
    wait->Start();
    
    // Asynchronously send the message.
    aMessage.SendMessage(wait->iStatus);
    
    CleanupStack::PopAndDestroy(wait);
    }

LOCAL_C void MainL()
    {
    // Creates an Active Scheduler to handle asychronous calls.
      
    CSendAs2SetMessageCharset* app = CSendAs2SetMessageCharset::NewL();
    CleanupStack::PushL(app);
    
    // Calls the main function in which other functions are called.
    app->StartL();
    CleanupStack::PopAndDestroy(app);
    
    }
  
  
  
  void doTestsL()
	{
	MainL();
	}


