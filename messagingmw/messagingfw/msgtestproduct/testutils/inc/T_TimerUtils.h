/**
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
* This is the header file which contains the utility functions for the CTimer derived objects
* 
*
*/



/**
 @file
*/


#ifndef __T_TIMER_UTILS_H__
#define __T_TIMER_UTILS_H__


// epoc includes
#include <e32base.h>

/**
This class provides the implementation of the Timer utilities
*/
class CT_MsgTimerUtils : public CTimer
	{
public:
	IMPORT_C static CT_MsgTimerUtils* NewL();
	~CT_MsgTimerUtils();
	IMPORT_C void After(TInt aSec,TRequestStatus& aStatus);

protected:
	void RunL();

private:
	CT_MsgTimerUtils();

private:
	TRequestStatus* iRequestStatus;
	};
#endif //__T_TIMER_UTILS_H__
