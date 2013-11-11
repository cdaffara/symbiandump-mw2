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
// Base class for Class 0 SMS plugin
// 
//

/**
 @publishedAll
 @released
*/

#ifndef __CSMSCLASS0BASE_H_
#define __CSMSCLASS0BASE_H_

#include <e32base.h>

class CSmsClass0Base : public CBase
	{

public:
	virtual void DisplayMessageHandler(TDesC8& aSmsMessage, TBool aComplete);
	~CSmsClass0Base();

protected:
	CSmsClass0Base();
	
	};

#endif