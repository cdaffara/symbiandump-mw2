// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors: Sean Jesson of Teleca Ltd. August 1999.
//
// Description:
// Test Enviroment for compact business card parser.
//
//

#ifndef _TESTCBCP_H_
#define _TESTCBCP_H_

#include <e32std.h>
#include <e32test.h>
#include <c32comm.h>
#include <in_sock.h>

extern RTest Test;
extern RSocketServ SocketServer;
extern RHeap* TestHeap;

void CommInitL(TBool aEnhanced);

// class definitions

class CTestScheduler : public CActiveScheduler
	{
public:
	void Error(TInt anError) const;
	};


#endif
