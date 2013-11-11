/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Sub-Session class of LBT server
*
*/


// INCLUDES
#include <lbterrors.h>
#include "lbtsession.h"
#include "lbtsubsession.h"
#include "lbtservercore.h"
#include "lbtserverlogiccontainer.h"
#include "lbtserverlogicbase.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtSubSession::NewL
// Symbian Two - phase constructor
// ---------------------------------------------------------------------------
//
CLbtSubSession* CLbtSubSession::NewLC(CLbtServerLogicContainer& aContainer, TSubSessionType aType)
	{
	FUNC_ENTER("CLbtSubSession::NewLC");	
	CLbtSubSession* self = new ( ELeave ) CLbtSubSession(aContainer);
    CleanupStack::PushL( self );
    self->ConstructL(aType);    
    return self;
	}

// ---------------------------------------------------------------------------
// CLbtSubSession::CLbtSubSession
// ---------------------------------------------------------------------------
//
CLbtSubSession::CLbtSubSession(CLbtServerLogicContainer& aContainer) 
	: iContainer(aContainer)
	{
	
	}

// ---------------------------------------------------------------------------
// CLbtSubSession::~CLbtSubSession
// ---------------------------------------------------------------------------
//
CLbtSubSession::~CLbtSubSession()
	{
	FUNC_ENTER("CLbtSubSession::~CLbtSubSession");
	}

// ---------------------------------------------------------------------------
// CLbtSubSession::CLbtSubSession
// ---------------------------------------------------------------------------
//
void CLbtSubSession::ConstructL(TSubSessionType aType)
	{
	iType = aType;
	}

// ---------------------------------------------------------------------------
// CLbtSubSession::ServiceL
// ---------------------------------------------------------------------------
//
void CLbtSubSession::ServiceL(const RMessage2 &aMessage)
	{
	FUNC_ENTER("CLbtSubSession::ServiceL");	
	CLbtServerLogicBase* interface = iContainer.ServerLogic();
	CLbtServerLogicBase::TSubSessionType type;
	if(iType == TLbtClientLibrary)
		{
		type = CLbtServerLogicBase::TLbtClientLibrary;
		}
	else
		{
		type = CLbtServerLogicBase::TLbtManagementLibrary;
		}
		
	if(interface)
		{
		interface->ServiceL(aMessage, type);		
		}
	else
		{
		aMessage.Complete(KErrNotReady);
		}
	}

// end of file


