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
* This is the header file for CT_MsgObserverCkEventNCount
* 
*
*/



#ifndef __T_IMAP4OBSERVER_CK_EVENT_N_COUNT_H__
#define __T_IMAP4OBSERVER_CK_EVENT_N_COUNT_H__



// User include
#include "T_MsgAsyncStepIMAP.h"



// Forward Declaration
class CT_MsgImapServer;

// Literals Used
_LIT(KCheckEventNCount,"CheckEventNCount");


// This implements a test step degregisters an observer on the inbox
class CT_MsgObserverCkEventNCount : public CT_MsgAsyncStepIMAP
	{
public:
    CT_MsgObserverCkEventNCount (CT_MsgSharedDataImap& aSharedDataIMAP);
    ~CT_MsgObserverCkEventNCount();
    TVerdict doTestStepL();

private:
     void ProgressL(TBool bFinal);
     void CancelStep();
	
private:
	CMsvEntry* iInboxEntry;
	CMsvEntry* iImapServiceEntry;
	CMsvEntrySelection* iEntrySelection;
	};


#endif //__T_IMAP4OBSERVER_CK_EVENT_N_COUNT_H__