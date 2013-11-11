// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __CSENDASTESTEDITTIMER_H__
#define __CSENDASTESTEDITTIMER_H__

#include <msvapi.h>

// forward declarations
class CSendAsTestEditUtils;

class CSendAsTestEditTimer : public CTimer
	{
public:
	static CSendAsTestEditTimer* NewL(CSendAsTestEditUtils& aEditUtils);
	virtual ~CSendAsTestEditTimer();

	// methods from CActive
	virtual void RunL();

private:
	CSendAsTestEditTimer(CSendAsTestEditUtils& aEditUtils);

	CSendAsTestEditUtils&	iEditUtils;
	};

#endif	// __CSENDASTESTEDITTIMER_H__
