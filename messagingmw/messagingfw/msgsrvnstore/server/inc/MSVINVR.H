// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#if !defined( __MSVINVR_H )
#define __MSVINVR_H

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#define __DECLARE_DEBUG  __DECLARE_TEST

#if defined(_DEBUG)

template <class T>
struct CheckInvariant
	{
	static void Check(TAny* aPtr)
		{((T*)aPtr)->__DbgTestInvariant();}
	};

template <class T>
inline TCleanupOperation CheckInvariantFunction(T*)
	{return CheckInvariant<T>::Check;}

class __DbgInvariant
	{
public:
	inline __DbgInvariant(TCleanupOperation aOp,TAny* aPtr)
		:iOp(aOp), iPtr(aPtr)
		{}
	inline ~__DbgInvariant()
		{iOp(iPtr);}
private:
	TCleanupOperation iOp;
	TAny* iPtr;
	};

class __DbgInvariantL
	{
public:
	inline __DbgInvariantL(TCleanupOperation aOp,TAny* aPtr)
		{CleanupStack::PushL(TCleanupItem(aOp,aPtr));}
	inline ~__DbgInvariantL()
		{CleanupStack::PopAndDestroy();}
	};

#define __DEBUG_INVARIANT_ONEXIT    __DbgInvariant __invariant(CheckInvariantFunction(this),this)
#define __DEBUG_INVARIANT_ONEXIT_L  __DbgInvariantL __invariant(CheckInvariantFunction(this),this)

#else // defined( _DEBUG )

#define __DEBUG_INVARIANT_ONEXIT    
#define __DEBUG_INVARIANT_ONEXIT_L  
//#define __DECLARE_DEBUG

#endif

#endif // __MSVINVR_H
