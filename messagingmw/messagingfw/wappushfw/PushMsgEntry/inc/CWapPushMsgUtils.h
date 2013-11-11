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
//

#ifndef __CWAPPUSHMSGUTILS_H_
#define __CWAPPUSHMSGUTILS_H_

#include <e32std.h>
#include <e32base.h>
#include <msvapi.h>
#include <pushentry.h>


class mwappushlog;
class cwappushmessagefilter;

//  2 panic codes for Push Message Utils
/**
@internalComponent
@released
*/
enum TWapPushMsgUtilsPanic  {
							EPushMsgUtilsNotValidPushMsg,
							EPushMsgUtilsNotSiMsg
							};


_LIT(KMsgUtilsPanicTitle, "CWapPushMsgUtils");


class CWapPushMsgUtils: public CBase, public MMsvSessionObserver
/**
Utility class for messaging operations which is provided for use by the WAP Push Content 
Handlers and the UI. Has functions for locating and creating WAP Push Service. 

Contains a CMsvSession object, and allows other objects to get a reference to a session 
without having to inherit from the MMsvSessionObserver class.

@internalTechnology
@released
*/
	{
public:
	IMPORT_C static CWapPushMsgUtils* NewL();
	IMPORT_C virtual ~CWapPushMsgUtils();	
	IMPORT_C void GetServiceIdL(TUid aMtm, TMsvId& rServiceId); 

	inline CMsvSession& Session() const;
	inline static TBool IsDeleted(const TMsvEntry& aEntry);
	inline static TInt32 GetPushMsgType(const TMsvEntry& aEntry);

protected:
	IMPORT_C void ConstructL();
	IMPORT_C CWapPushMsgUtils();
	inline void HandleSessionEventL(TMsvSessionEvent , TAny* , TAny* , TAny* );
	
protected:
	CMsvSession* iMsvSession;
	CMsvEntry* iMsvEntry;
	TMsvId iPushServiceId;
	TMsvId iPushFolderId;
	};

class CWapPushMessageFilter;
 
class CWapPushFilterUtils : public CBase
	{
public:
	IMPORT_C static CWapPushMessageFilter* GetFilterPluginL();
	};

#include <push/cwappushmsgutils.inl>


#endif
