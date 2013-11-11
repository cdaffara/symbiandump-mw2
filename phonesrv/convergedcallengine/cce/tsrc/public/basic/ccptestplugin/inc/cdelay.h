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
* Description:  Timer*
*/
/*
* ===================================================================
*  Name:       CDelay.h
*
*  Description: Timer

* ===================================================================
*/

#ifndef CCPTESTDELAY_H
#define CCPTESTDELAY_H

// Include files

#include <e32base.h>

class CDelay : public CTimer
	{
	public:
	
	CDelay();
	~CDelay();
	
	void ConstructL();
	
	static CDelay* NewL();
			
	void Start( TInt aDelay );
	
	void Cancel();
	
	void RunL();
	
	CActiveSchedulerWait* iWaitLoop;
	
	};
	

#endif //CCPTESTDELAY_H