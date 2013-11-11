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
// Contributors:
//
// Description:
// A revised version of the CRegisteredParserDll class.  This no longer uses a 
// reference to a CParserRegistrationData object.  Instead a UidType is passed in 
// the  constructor, or through a set method.  The class is used to find the file 
// which contains a parser, to load and maintain a reference count of the number of 
// objects using the dll, so that it can be unloaded when no longer required. 
// It will also unload the dll when a new parser is required.
// 
//



#ifndef __REGPSDLL_H__
#define __REGPSDLL_H__



#include <e32base.h>

// forward declarations
class RWriteStream;
class RReadStream;
class RFs;

//
//  Real non-derivable class
//
class CRegisteredParserDll : public CTimer
/** Finds, loads and maintains a reference count of client processes for a BIO 
parser DLL.

When the reference count falls to 0, the class is also responsible for unloading 
the DLL. The public interface for this class is used only by CBIOServerMtm. 
@publishedAll
@released
*/
	{
public:
    IMPORT_C static CRegisteredParserDll* NewL(TDesC& aParserName);

    IMPORT_C ~CRegisteredParserDll();

	inline TInt DllRefCount() const;
	inline TUidType UidType() const;
	inline void SetUidType(const TUidType& aUidType);

	IMPORT_C TInt GetLibrary(RFs& aFs,RLibrary& aDllLibrary);   // Increments reference count if successful or returns error 
	IMPORT_C void ReleaseLibrary();                                   // Decrements access count
	                                                                  // Should be called from destructor of any objects created from the library 
private:
	CRegisteredParserDll(const TDesC& aParserName);
    void ConstructL();

	void LoadLibraryL(RFs& aFs);   
	TInt FindFullName(RFs& aFs,TDes& aFullName); 

private:
	TUidType iUidType;
    TPtrC iParserName;
	RLibrary iParserDllLibrary;
	TInt iDllRefCount;

    void RunL();
    enum { KReleaseLibraryTimeout = 30000000 };
	};

#include <regpsdll.inl>
#endif
