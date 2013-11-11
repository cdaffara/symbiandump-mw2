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
*
*/



/**
 @file
*/


#ifndef __T_MSG_SHARED_DATA_POP_H__
#define __T_MSG_SHARED_DATA_POP_H__

// Epoc include
#include <mtclbase.h>

// User includes
#include "T_MsgSharedDataBase.h"

/**
This implements a class which creates a pointer to the POP MTM object
*/
class CT_MsgSharedDataPop : public CT_MsgSharedDataBase
	{
public:
	static CT_MsgSharedDataPop* NewL();
	~CT_MsgSharedDataPop();

protected:
	virtual CBaseMtm*	NewMtmL();

private:
	void ConstructL();
	CT_MsgSharedDataPop();

public:
	CPeriodic* 	iPeriodic;
 	TInt  		iMaxRamValue;
 	TInt  		iMinRamValue;
 	TInt  		iStartRamValue;
 	TInt  		iEndRamValue;
 	TBool 		iMark;
	};

#endif // __T_MSG_SHARED_DATA_Pop_H__
