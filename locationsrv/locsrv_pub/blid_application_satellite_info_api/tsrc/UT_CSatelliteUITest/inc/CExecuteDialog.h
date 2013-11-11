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
* Description: ExeculteLD module declaraion
*
*/

#ifndef CEXECUTEDIALOG_H_
#define CEXECUTEDIALOG_H_

#include <e32base.h>
#include <EIKENV.h>

class CExecuteDialog : public CActive
	{
public:	
	CExecuteDialog();
	virtual ~CExecuteDialog();
	void SimulateExitKeyL();
	
private:
	void RunL();
	void DoCancel();
	
private:
	TBool iFirstTime;
	RTimer mTimeoutTimer;
	//data
	};

#endif /* CEXECUTEDIALOG_H_ */
