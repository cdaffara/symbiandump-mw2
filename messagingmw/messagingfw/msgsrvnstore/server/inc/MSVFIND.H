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
//

#if !defined(__MSVFIND_H__)
#define __MSVFIND_H__

#if !defined(__MSVAPI_H__)
#include <msvapi.h>
#endif

#if !defined __MTCLBASE_H__
#include <mtclbase.h>
#endif

#if !defined(__MTMDEF_H__)
#include <mtmdef.h>
#endif

//**********************************
// TMsvFindResult
//**********************************
//
// Contains the result of a find operation for a particular entry
//

class TMsvFindResult
/** A single result from a text search operation. The class encapsulates information 
which:

1. identifies the message in which the search text is found

2. identifies the parts of the message that contain the search text. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C TMsvFindResult();
	IMPORT_C TMsvFindResult(TMsvPartList aPartList, TMsvId aId);
	//
public:
	/** The parts of the message that contain the search text. */
	TMsvPartList iPartList;
	/** The entry Id of the message that contains the search text. */
	TMsvId iId;
	};

//**********************************
// CMsvFindResultSelection
//**********************************
//
// A list of find operation results with an accessor to find  
// the position for a particular entry
//

class CMsvFindResultSelection : public CArrayFixFlat<TMsvFindResult>
/** Collection of results from a text search operation. The collection is organized 
as an array. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C CMsvFindResultSelection();
	//
public:
	IMPORT_C CMsvFindResultSelection* CopyL()  const;
	IMPORT_C CMsvFindResultSelection* CopyLC() const;
	IMPORT_C TInt Find(TMsvId aId) const;
	};

//**********************************
// TMsvFindOperationProgress
//**********************************
//
// Find operation progress
//

class TMsvFindOperationProgress
/** Encapsulates progress information for a text search operation. The class has 
public data members. 
@publishedAll
@released
*/
	{
public:
	IMPORT_C TMsvFindOperationProgress();
	//
public:
	/** KErrNone if the search operation completed successfully otherwise one of the 
	system-wide error codes.
	
	This has no meaning while the search operation is in progress. */
	TInt iError;
	/** The number of messages already searched. */
	TInt iCompleted;
	/** The number of messages remaining to be searched.
	
	If the search operation is for text within a specified root, folder or service, 
	then this value is zero until the total number of messages has been determined. */
	TInt iRemaining;
	/** The entry Id identifying the message currently being searched */
	TMsvId iCurrentId;
	};

class CClientMtmRegistry;
class CMsvChildMessages;

//**********************************
// CMsvFindOperation
//**********************************
//
// The find operation. Created by calling the relevant static function
// The results for all of these are added to the CMsvFindResultSelection which
// can be accessed via the accessor function GetFindResult
//

class CMsvFindOperation : public CMsvOperation
/** Encapsulates a text search operation.

The class offers an interface for locating messages which contain specified 
text. Messages may contain data created for, or received from, any of the 
transport protocols that are supported by the Messaging Architecture (e.g. 
Email, FAX and SMS).

Clients create a new instance for each search operation to be performed. They 
are also responsible for destroying the instance once a search operation is 
complete.

Clients can derive from this class, typically to implement their own version 
of the function IsValid(). 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvFindOperation* FindInChildrenL(CMsvSession& aSession, const TDesC& aTextToFind, TMsvId aParentId, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus);
	IMPORT_C static CMsvFindOperation* FindInSelectionL(CMsvSession& aSession, const TDesC& aTextToFind, const CMsvEntrySelection& aSelection, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus);
	IMPORT_C ~CMsvFindOperation();
	//
	IMPORT_C const TDesC8& ProgressL();
	IMPORT_C const TDesC8& FinalProgress();
	//
	inline const CMsvFindResultSelection& GetFindResult() const;
	//
protected:
	IMPORT_C CMsvFindOperation(CMsvSession& aSession, const TDesC& aTextToFind, TMsvPartList aPartList, TRequestStatus& aObserverRequestStatus);
	IMPORT_C void ConstructFindInChildrenL(TMsvId aId);
	IMPORT_C void ConstructFindInSelectionL(const CMsvEntrySelection& aSelection);
	//
private:
	void ConstructL();
	//
	IMPORT_C void DoCancel();
	IMPORT_C void RunL();
	//
	void DoRunL();
	void FindL();
	void Complete(TInt aStatus);
	void StartL();
	//
	virtual TBool IsValid(const TMsvEntry& aEntry) const;
	//
private:
	enum { EMsvFindingText, EMsvExpandingFolders } iState;
	TBuf<KMsvMaxFindTextLength> iTextToFind;
	TMsvPartList iPartList;
	CMsvFindResultSelection* iFindResultSel;
	TPckgBuf<TMsvFindOperationProgress> iProgress;
	CMsvEntrySelection* iSelection;
	CClientMtmRegistry* iClientRegistry;
	CArrayPtrFlat<CBaseMtm>* iMtmArray;
	CMsvChildMessages* iChildMessages;
	TMsvId iCurrentId;
	TMsvEntry iEntry;
	};

#include <msvfind.inl>

#endif

