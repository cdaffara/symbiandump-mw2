/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata engine client session
*
*/


#include "mdesession.h"

#include "mdesessionimpl.h"


EXPORT_C CMdESession* CMdESession::NewL(MMdESessionObserver& aObserver)
	{
	CMdESession* self = CMdESession::NewLC(aObserver);
	CleanupStack::Pop(self);
	return self;
	}


EXPORT_C CMdESession* CMdESession::NewLC(MMdESessionObserver& aObserver)
	{
	CMdESessionImpl* self = new (ELeave) CMdESessionImpl(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


CMdESession::CMdESession()
	{
	}


void CMdESession::SessionConstruct()
	{
	}


CMdESession::~CMdESession()
	{
	}
	
