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
* This contains ths class CT_MsgSharedDataBase which creates a session object
* 
*
*/





#ifndef __T_MSG_SHARED_DATA_BASE_H__
#define __T_MSG_SHARED_DATA_BASE_H__


/* Epoc includes */
#include <mtclreg.h>
#include <mtclbase.h>


/* 
 * This implements a class which creates and returns a session object which
 * can be shared across test steps.It serves as a base class for other shared
 * data objects which require the session object
 */
class CT_MsgSharedDataBase : public CBase, protected MMsvSessionObserver
	{
public:
	/*
	 * Destructor
	 */
	virtual ~CT_MsgSharedDataBase();

protected:
	CT_MsgSharedDataBase();

	/* 
	 * Constructs the required session, registry objects
	 * Initialises the MTM object to NULL
	 */
	void ConstructL();

	/* 
	 * To be implemented by the derived classes to creates
	 * the required MTM objects
	 */
	virtual CBaseMtm*	NewMtmL() = 0;

public:
	/* Derived from MMsvSessionObserver */
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};

public:
	/* Session object */
	CMsvSession*	iSession;
	/* Registry object required to create MTMs */
	CClientMtmRegistry*	iRegistry;
	/* Set to NULL.To be Set for each specific Mtm by deriving class. */
	CBaseMtm*	iMtm; 	
	};

#endif /* __T_MSG_SHARED_DATA_BASE_H__ */
