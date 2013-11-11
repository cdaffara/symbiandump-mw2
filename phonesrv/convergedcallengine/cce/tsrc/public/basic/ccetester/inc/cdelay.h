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
* Description: Declaration of CDelay class.
*
*/

#ifndef CCCETESTERDELAY_H
#define CCCETESTERDELAY_H

// Include files

#include <e32base.h>

NONSHARABLE_CLASS( CDelay ): public CTimer
	{
	public:
	
	CDelay();
	virtual ~CDelay();
	
	void ConstructL();
	
	static CDelay* NewL();
	//static CreateStart();
		
	void Start( TInt aDelay );
	
	//void DoCancel();
	
	void Cancel();
	
	void RunL();
	
	CActiveSchedulerWait* iWaitLoop;
	
	};
	

#endif //CCCETESTERDELAY_H