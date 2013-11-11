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
// This contains the class  CT_MsgEventStore
// It is used to hold the type of event (TMsvEntryEvent),
// and number of cmventry objects effected (i.e emails/folders).
// 
//


#include "T_IMAP4MsgEventStore.h"


/**
  Function :CT_MsgEventStore
  Description :Constructor	
  @param :	TMsvEntryEvent	
  @param :  TInt
  @return : n/a
*/

	
CT_MsgEventStore::CT_MsgEventStore(TMsvEntryEvent aEventType, TInt aNumofMsg)
:CBase()
,iEventType(aEventType)
,iNumofMsg(aNumofMsg)
{
		
}

/**
  Function : ~CT_MsgEventStore()
  Description : Destructor
  @return : N/A
*/
CT_MsgEventStore:: ~CT_MsgEventStore()
{
	 	
}

