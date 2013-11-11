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
//


#include <f32file.h>
#include <barsc.h> 
#include <barsread.h>
#include <ecom/implementationproxy.h>
#include <barsread.h>
#include "class0sms.h"

//UID for Class 0 SMS Notifier
const TUid KUidClass0SmsNotifier = {0x2000C382};


//
// DLL Global Methods
//

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x2000BDF1, CClass0Sms::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}
	

CClass0Sms* CClass0Sms::NewL()
	{
	CClass0Sms* self= new (ELeave) CClass0Sms();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
//CClass0Sms constructor	
CClass0Sms::CClass0Sms()
	{
	//nothing to do here
	}
	
//CClass0Sms destructor
CClass0Sms::~CClass0Sms()
	{
	//needs to close the notifier
	iNotifier.Close();
    }
	
//CClass0Sms ConstructL
void CClass0Sms::ConstructL()
	{
	//Connects to the RNotifier server. KErrNone, if it succeeds.
	User::LeaveIfError(iNotifier.Connect());
	}

/**
DisplayMessageHandler method wil calling the StartNotifier/UpdateNotifier according to aIsCompleteMsg flag. Here it passes the Class 0 SMS Message.
@param aSmsMessage will contains Starting & Ending PDU position of decoded message,
    and boolean value indicates that, is this last incomplete message. And it contains Class0SMS data.
	This needs be Internalize in client side, in the same order(i.e TInt aStartPos, TInt endPos, TBool aIsLastMessage, TPtr aSmsData).
@param aComplete indicates, is it a partial messsage or full message.  
*/
void CClass0Sms::DisplayMessageHandler(TDesC8& aSmsMessage, TBool aIsCompleteMsg)
	{
	TPckgBuf<TInt> 	response;
	
	if(aIsCompleteMsg)
		{
		iNotifier.StartNotifier(KUidClass0SmsNotifier, aSmsMessage);		
		}
	else
		{
		iNotifier.UpdateNotifier(KUidClass0SmsNotifier, aSmsMessage, response);
		}
	}
