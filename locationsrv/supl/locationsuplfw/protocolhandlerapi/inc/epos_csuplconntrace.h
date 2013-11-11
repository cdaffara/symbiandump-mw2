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


#ifndef C_CSUPLTRACE_H
#define C_CSUPLTRACE_H

#include <e32base.h>

class CSuplTrace;
NONSHARABLE_CLASS(CSuplConnTrace) : public CBase
{
	public:
	 
	static CSuplConnTrace* NewL();
	void Trace(const TDesC& aDescription,const TDesC& aFilename,
               const TInt aLineNumber);
    ~CSuplConnTrace();
	
	private:
	
	void ConstructL();
	private:
	
	//Tracing utility
	CSuplTrace* iTrace;

};


#endif
