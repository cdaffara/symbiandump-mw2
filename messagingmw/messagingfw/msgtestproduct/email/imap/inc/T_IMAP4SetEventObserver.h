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
* This is the header file for CT_MsgSetInboxObserver 
* 
*
*/



#ifndef __T_IMAP4_SET_EVENT_OBSERVER_H__
#define __T_IMAP4_SET_EVENT_OBSERVER_H__



// User include
#include "T_MsgAsyncStepIMAP.h"



// Forward Declaration
class CT_MsgImapServer;

// Literals Used
_LIT(KSetEventObserver,"SetEventObserver");


// This implements a test step allows notification of new mails in clients account
class CT_MsgSetEventObserver: public CT_MsgAsyncStepIMAP
	{
public:
    CT_MsgSetEventObserver (CT_MsgSharedDataImap& aSharedDataIMAP);
   ~CT_MsgSetEventObserver ();
    TVerdict doTestStepL();
	
private:
     void ProgressL(TBool bFinal);
     void CancelStep();
protected:

	
	
private:

	TInt iNumberOfMessages;
	CMsvEntry* iInboxEntry;
	CMsvEntry* iImapServiceEntry;
	CMsvEntrySelection* iEntrySelection;




	};





#endif //__T_IMAP4_SET_EVENT_OBSERVER_H__