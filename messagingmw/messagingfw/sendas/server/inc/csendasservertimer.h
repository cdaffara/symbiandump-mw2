// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASSERVERTIMER_H__
#define __CSENDASSERVERTIMER_H__

#include <e32base.h>

/**
Utility class for the sendas server to shut down after a period with no open
sessions.

@internalComponent
@released
*/
class CSendAsServerTimer : public CTimer
	{
public:
	static CSendAsServerTimer *NewL();
private:	// from CTimer
	virtual void RunL();
private:
	CSendAsServerTimer();
	};

#endif	// __CSENDASSERVERTIMER_H__
