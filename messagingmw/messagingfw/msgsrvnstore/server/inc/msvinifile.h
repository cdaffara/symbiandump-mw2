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
//

#if !defined(__CMSVINIDATA_H__)
#define __CMSVINIDATA_H__

#include <e32base.h>




/**
 *Defines the interface to acess to ini data file
 *
 *@internalAll Component
 *@released
 */

NONSHARABLE_CLASS (CMsvIniData): public CBase 
	{
public:
	// Constructor, pass in name of ini file to open
	// Default search path is 'c:\system\data' on target filesystem
	// ie. 'NewL(_L("c:\\system\\data\\ttools.ini"))' is equivalent
	// to 'NewL(_L("ttools.ini"))'
	static CMsvIniData* NewL(const TDesC& aName);
	virtual ~CMsvIniData();

	TBool FindVar(const TDesC& aKeyName, TPtrC& aResult);

	TBool FindVar(const TDesC& aKeyName, TInt& aResult);

	TBool FindVar(const TDesC& aSection, const TDesC& aKeyName,	TPtrC& aResult);

	TBool FindVar(const TDesC& aSection, const TDesC& aKeyName,	TInt& aResult);	

protected:
	CMsvIniData();
	void ConstructL(const TDesC& aName);

private:
	HBufC* iName;
	HBufC* iToken;
	TPtr iPtr;
private:
#ifdef SYMBIAN_MESSAGESTORE_UNIT_TESTCODE
	friend class CTestIniData; //only for testing
#endif
	};

#endif

