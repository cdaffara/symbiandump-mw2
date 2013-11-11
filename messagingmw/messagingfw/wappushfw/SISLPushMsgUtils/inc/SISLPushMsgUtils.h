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

/**
 @file
 @publishedPartner
 @released
*/

#ifndef __SISLPUSHMSGUTILS_H__
#define __SISLPUSHMSGUTILS_H__

#include <e32base.h>
#include <msvapi.h>
#include <pushentry.h>
#include <push/cwappushmsgutils.h>

/** push folder description */
_LIT(KPushFolderDescription, "WAP Push Msg Folder Entry");

class MWapPushLog;
class CWapPushMessageFilter;


/**
Panic codes for Push Message Utils.

@publishedPartner
@released
*/
enum TSISLPushMsgUtilsPanic  {
                                /** Invlaid Push message */
							    ESISLPushMsgUtilsNotValidPushMsg,
                                /** Not SI message */
							    ESISLPushMsgUtilsNotSiMsg
							 };

/** push message ultils panic */
_LIT(KSISLPushMsgUtilsPanicTitle, "CSISLPushMsgUtils");

/**
SI and SL Push message utility class used by the SI and SL Content Handlers. 
The class has functions for locating and creating WAP Push Service. 

Contains a CMsvSession object, and allows other objects to get a reference to 
a session without having to inherit from the MMsvSessionObserver class.

@see	CSIContentHandler
@see	CSLContentHandler

@publishedPartner
@released
*/
class CSISLPushMsgUtils: public CWapPushMsgUtils
	{
public:
	IMPORT_C static CSISLPushMsgUtils* NewL();
	IMPORT_C virtual ~CSISLPushMsgUtils();

	IMPORT_C CMsvEntrySelection* FindSiIdLC (const TDesC& aSiId) ;
	IMPORT_C TMsvId FindUrlL(const TDesC& aUrl, TUid aPushType);
	IMPORT_C TInt GetActionL(TMsvId aId);
	IMPORT_C const TTime  GetExpiryL(TMsvId aId);
	IMPORT_C TMsvId GetHighestActionL(TUid aPushType);
	IMPORT_C TMsvId GetNextExpiryL();

	IMPORT_C void DeleteEntryL(TMsvId aId);
	IMPORT_C void DeleteEntryNowL(TMsvId aId);
	IMPORT_C void GetPushMsgFolderIdL(TMsvId& rFolderId);

public:
	inline TTime GetCreationDateL(TMsvId aId);
	inline TTime GetTransmissionDateL(TMsvId aId);

private:
	CSISLPushMsgUtils();
	void ConstructL();

	TMsvId CreatePushMsgFolderL();
	CPushMsgEntryBase* GetPushMsgEntryL(const TUid aPushType);
	CMsvEntrySelection* GetChildrenWithMtmLC(TUid aMtm);
	TTime GetDateL(TMsvId aId, const TUid aPushType);
	};


#include <push/sislpushmsgutils.inl>

#endif // __SISLPUSHMSGUTILS_H__
