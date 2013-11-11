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
* This contains the class CT_MsgSharedDataImap
* 
*
*/





#ifndef __T_MSG_SHARED_DATA_IMAP_H__
#define __T_MSG_SHARED_DATA_IMAP_H__

//Epoc include 
#include <mtclbase.h>

// User includes 
#include "T_IMAP4MsgEventObserver.h" 
#include "T_MsgSharedDataBase.h"

/* This implements a class which creates a pointer to the IMAP MTM object */
class CT_MsgSharedDataImap : public CT_MsgSharedDataBase
	{
public:
	// Creates an object of type CT_MsgSharedDataImap 
	static CT_MsgSharedDataImap* NewL();


protected:

	// Creates an object of IMAP MTM 
	virtual CBaseMtm* NewMtmL(); 
	
private:

	CT_MsgSharedDataImap();
	~CT_MsgSharedDataImap();
	void ConstructL();
	
public: 	
		CT_MsgEventObserver* iObserver;
		CPeriodic* 	iPeriodic;
 		TInt  		iMaxRamValue;
 		TInt  		iMinRamValue;
 		TInt  		iStartRamValue;
 		TInt  		iEndRamValue;
 		TBool 		iMark;
	
	};
#endif // __T_MSG_SHARED_DATA_IMAP_H__
