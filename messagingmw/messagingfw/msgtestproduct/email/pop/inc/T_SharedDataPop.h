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


#ifndef __T_SHARED_DATA_POP_H__
#define __T_SHARED_DATA_POP_H__

// Epoc include
#include <mtclbase.h>

// User includes
#include "t_msgshareddatabase.h"


/**
This implements a class which creates a pointer to the POP MTM object
*/
class CT_MsgSharedDataPop : public CT_MsgSharedDataBase
	{
public:
	static CT_MsgSharedDataPop* NewL();

protected:
	virtual CBaseMtm*	NewMtmL();

private:
	CT_MsgSharedDataPop();
	};

#endif // __T_SHARED_DATA_POP_H__
