// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Class 0 SMS Message 
// 
//

#ifndef __CLASS0SMS_H_
#define __CLASS0SMS_H_

#include <e32base.h>
#include <csmsclass0base.h>

class CClass0Sms : public CSmsClass0Base 
	{
	public:
		void DisplayMessageHandler(TDesC8& aSmsMessage, TBool aComplete);
		static CClass0Sms* NewL();
		~CClass0Sms();
		
	private:
		CClass0Sms();
		void ConstructL();

	private:
		RNotifier	iNotifier;
	};

#endif

