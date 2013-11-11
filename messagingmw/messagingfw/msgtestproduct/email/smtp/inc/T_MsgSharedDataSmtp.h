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
* This contains CT_MsgSharedDataSmtp
* 
*
*/



/**
 @file
*/


#ifndef __T_MSG_SHARED_DATA_SMTP_H__
#define __T_MSG_SHARED_DATA_SMTP_H__


// User includes
#include "T_MsgSharedDataBase.h"


/**
Implements the class for sharing data across the test steps created by SMTP test server.  
*/
class CT_MsgSharedDataSmtp : public CT_MsgSharedDataBase
	{
public:
	static CT_MsgSharedDataSmtp* NewL();
	~CT_MsgSharedDataSmtp();

protected:
	virtual CBaseMtm*	NewMtmL();
	CT_MsgSharedDataSmtp();

private:
	void ConstructL();

public:
	CPeriodic* 	iPeriodic;
	TInt  		iMaxRamValue;
	TInt  		iMinRamValue;
	TInt  		iStartRamValue;
	TInt  		iEndRamValue;
	TBool 		iMark;
	};

#endif // __T_MSG_SHARED_DATA_SMTP_H__
