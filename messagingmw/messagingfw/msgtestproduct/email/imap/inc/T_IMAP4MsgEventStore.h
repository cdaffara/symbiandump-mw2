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
* This contains the class  CT_MsgEventStore
* It is used to hold the type of event (TMsvEntryEvent),
* and number of cmventry objects effected (i.e emails/folders).
* 
*
*/





//Epoc include
#include <msvapi.h>//Specifies function IDs for standard MTM functions
#include <e32base.h>

#ifndef __T_IMAP4_MSG_EVENT_STORE_H__
#define __T_IMAP4_MSG_EVENT_STORE_H__




class CT_MsgEventStore : public CBase , public  MMsvEntryObserver
{
public:
	
	  CT_MsgEventStore(TMsvEntryEvent aEventType, TInt aNumofMsg);
	  ~CT_MsgEventStore();
	  void	HandleEntryEventL(TMsvEntryEvent , TAny*, TAny*, TAny*) {};
	  
public:
	
	TMsvEntryEvent iEventType;
	TInt iNumofMsg;
};

#endif//__T_IMAP4_MSG_EVENT_STORE_H__