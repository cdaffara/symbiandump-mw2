/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Contains tracing functionality
*
*/


#include "epos_csupltrace.h"
#include "epos_comasupltrace.h"

COMASuplTrace* COMASuplTrace::NewL()
{
	 COMASuplTrace* self =  new ( ELeave ) COMASuplTrace();
	 CleanupStack::PushL( self );
	 self->ConstructL();
	 CleanupStack::Pop(self);
	 return self;
}

void COMASuplTrace::ConstructL()
{
	iTrace = CSuplTrace::NewL();
}
void COMASuplTrace::Trace(const TDesC& aDescription,const TDesC& aFilename,
              			  const TInt aLineNumber)
{	
    TRAP_IGNORE(iTrace->TraceL(aDescription, aFilename, aLineNumber));
}

COMASuplTrace::~COMASuplTrace()
{
	delete iTrace;
}
