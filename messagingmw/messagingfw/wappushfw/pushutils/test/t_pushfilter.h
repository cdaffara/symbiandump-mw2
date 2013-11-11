// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Defines panic function and enum and test filter plugin classes
// 
//


#ifndef	__T_PUSHFILTER_H__
#define	__T_PUSHFILTER_H__

#include "CWapPushMessageFilter.h"

const TUid KUIDTestPushMessageFilter	= { 0x101F3E6E };
_LIT(KTestPushFilterPlugin,"*");

enum TTestPushMessageFilterPanic 
	{
	EPluginIndexInvalid
	};

GLREF_C void Panic(TTestPushMessageFilterPanic aPanic);





/** A test push message filter.
	
	This has a trivial implementation of FilterMessage and is purely
	for testing the filter plugin hook in the wap push watchers
*/
class CTestPushMessageFilter : public CWapPushMessageFilter
	{
public:
	static CTestPushMessageFilter* NewL();
	virtual ~CTestPushMessageFilter();

	// from CWapPushMessageFilter
	virtual void FilterMessage(const CPushMessage& aPushMessage, TRequestStatus& aStatus, TBool& aAllowMessage);
	virtual void CancelFilter();

private:
	CTestPushMessageFilter();

private:	// BC-proofing

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	virtual void CWapPushMessageFilter_Reserved1();		

/**
	Intended Usage	:	Reserved for future expansion.
	@since			6.0
 */
	virtual void CWapPushMessageFilter_Reserved2();		

	/** Reserved for future expansion
	 */
	TAny*		iCWapPushMessageFilter_Reserved;
	};

#endif	// __T_PUSHFILTER_H__
