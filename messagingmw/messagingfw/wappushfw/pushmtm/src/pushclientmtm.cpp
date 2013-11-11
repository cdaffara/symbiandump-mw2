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

#include <msvreg.h>
#include <msvapi.h>
#include <mtmdef.h>
#include <mtclbase.h>
#include <msvftext.h>
#include "pushclientmtm.h"


/**
 Standard static factory function.
 @param aRegisteredMtmDll of type CRegisteredMtmDll
 @param aSession of type CMsvSession
 @return pointer to a new instance of the class CWapPushClientMtm
*/

EXPORT_C CWapPushClientMtm* CWapPushClientMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
	{
	CWapPushClientMtm* self=new(ELeave) CWapPushClientMtm(aRegisteredMtmDll, aSession);
	return self;
	}

/**
 * Class destructor
 */
CWapPushClientMtm::~CWapPushClientMtm()
	{
	}


/**
 * Constructor
 */
CWapPushClientMtm::CWapPushClientMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvSession& aSession)
: CBaseMtm(aRegisteredMtmDll, aSession)
	{
	}

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::SaveMessageL()
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::LoadMessageL()
	{
	User::Leave(KErrNotSupported);
	}


/**
 * MTM API Not Supported
 */
CMsvOperation* CWapPushClientMtm::ReplyL  (TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

/**
 * MTM API Not Supported
 */
CMsvOperation* CWapPushClientMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartList*/, TRequestStatus& /*aCompletionStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

/**
 * MTM API Not Supported
 */
TMsvPartList CWapPushClientMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	return KMsvMessagePartNone;
	}

/**
 * Derived API for finding text in a stored Message
 * @param aTextToFind
 * @param aPartList
 */
TMsvPartList CWapPushClientMtm::Find(const TDesC& aTextToFind, TMsvPartList aPartList)
	{
	TMsvPartList retList = KMsvMessagePartNone;

	TRAP_IGNORE(retList = DoFindL(aTextToFind, aPartList));

	return retList;
	}

/**
 * Leaving function for finding text in a stored Message
 * @param aTextToFind
 * @param aPartList
 */
TMsvPartList CWapPushClientMtm::DoFindL(const TDesC& aTextToFind, TMsvPartList aPartList)
	{
	TMsvPartList  retList = KMsvMessagePartNone;
	CMsvFindText* text    = CMsvFindText::NewL();

	CleanupStack::PushL(text);

	if (aPartList & KMsvMessagePartBody)
		{
		if (text->FindRichTextL(aTextToFind, Body(), aPartList))
			retList |= KMsvMessagePartBody;
		}
	if (aPartList & KMsvMessagePartOriginator) 
		{
		if (text->FindTextL(aTextToFind, iMsvEntry->Entry().iDetails, aPartList))
			retList |= KMsvMessagePartOriginator;
		}
	if (aPartList & KMsvMessagePartDescription)	
		{
		if (text->FindTextL(aTextToFind, iMsvEntry->Entry().iDescription, aPartList))
			retList |= KMsvMessagePartDescription;
		}

	CleanupStack::PopAndDestroy();// text
	return retList;
	}

#ifndef __MESSAGING_API_V2__
const CDesCArray& CWapPushClientMtm::AddresseeList()
	{
	return *iAddresseeList;
	}
#endif	// __MESSAGING_API_V2__

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::AddAddresseeL(const TDesC& /*aRealAddress*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::AddAddresseeL(const TDesC& /*aRealAddress*/, const TDesC& /*aAlias*/)
	{
	User::Leave(KErrNotSupported);
	}


/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::RemoveAddressee(TInt /*aIndex*/)
	{
	}

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::ContextEntrySwitched()
	{
	}

/**
 * MTM API Not Supported
 */
void CWapPushClientMtm::InvokeSyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
CMsvOperation* CWapPushClientMtm::InvokeAsyncFunctionL(TInt /*aFunctionId*/,const CMsvEntrySelection& /*aSelection*/, TDes8& /*aParameter*/, TRequestStatus& /*aCompletionStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}


