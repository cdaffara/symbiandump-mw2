// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __MSVSEARCHSORTOPONHEADERBODY_H__
#define __MSVSEARCHSORTOPONHEADERBODY_H__

#include <msvsearchsortquery.h>
#include <e32base.h>
#include <msvenhancesearchsortutil.h>

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include "msvsearchsortconstants.h"
#endif

#if !defined(__MSVAPI_H__)
#include <msvapi.h>
#endif

#if !defined __MTCLBASE_H__
#include <mtclbase.h>
#endif

#if !defined(__MTMDEF_H__)
#include <mtmdef.h>
#endif

#include "MSVUIDS.H"
#include "MTCLREG.H"

// forward declaration
class CMsvSession;

const TInt KLengthSortData=50;

/**
CMsvSearchsortOpOnHeaderBody is used to perform the search sort operation on
Message header, body and attachment

@internalComponent
@released
*/
class CMsvSearchsortOpOnHeaderBody : public CActive
{
public:
	IMPORT_C static CMsvSearchsortOpOnHeaderBody* NewL(CMsvSession& aSession, TInt aPriority);
	IMPORT_C ~CMsvSearchsortOpOnHeaderBody();
	IMPORT_C void SearchSortOnHeaderBodyL(CMsvSearchSortQuery* aQuery, RArray<TMsvId> aSourceArray, TBool aSortOnIndexEntry, TRequestStatus& aStatus);
	IMPORT_C void DoGetEntryForAllIdL(const RArray<TMsvId>& aIdArray, TRequestStatus& aStatus);
	IMPORT_C void GetResultAsTMsvIdL(RArray<TMsvId>& aIdArray);
	IMPORT_C void GetResultAsTMsvIdWithSortFieldL(RArray<TMsvIdWithSortField>& aArrayOfIdWithSortField);
	IMPORT_C void GetResultAsInvalidTMsvIdL(RArray<TMsvId>& aInValidIdArray);
	IMPORT_C void GetResultAsTmsvEntryL(RArray<TMsvEntry>& aEntryArray);

private:
	enum TRequestType { EMsvSearchSortOnHeaderBody, EMsvGetEntryForIds };		
	//
private:
	CMsvSearchsortOpOnHeaderBody(CMsvSession& aSession, TInt aPriority);
	void ProcessQuery(CMsvSearchSortQuery* aQuery, const RArray<TMsvId> aIdArray, TBool aSortOnIndexEntry, TRequestStatus& aStatus);
	void FindInHeaderBodyL();
	void StartL();
	void ConstructL();
	void RunL();
	void DoRunL();
	void DoCancel();
	void CompleteSelf();
	void Complete(TInt aStatus);
	
	void InitializePartList();
	void DoCopySortDataL(CBaseMtm* aBaseMtm);
	TBool AddSortFlag(TMsvPartList aSortPartList);
	TBool AddSearchFlag(TMsvPartList aSearchPartList,TMsvPartList& aPartList);
	void RemoveSearchFlag(TMsvPartList aSearchPartList,TMsvPartList& aPartList);
	TBool IsOnlySortOperation();
	TBool DoAttachmentTypeSearchL(TInt aIndex);
	//
private:
	CMsvSession& iSession;
	TRequestStatus* iRequestStatus;
	CMsvSearchSortQuery* iQuery;
	TMsvEntry iEntry;
	TRequestType iRequestType;
	TMsvPartList iPartList;
	TBool iSortOnIndexEntry;
	
	CClientMtmRegistry* iClientRegistry;
	CArrayPtrFlat<CBaseMtm>* iMtmArray;
	CMsvEntry* iCMsvEntry;
	
	TInt iIdToSearch;
	TInt iQueryCount;
	
	RArray<TMsvId> iIdArray;
	RArray<TMsvEntry> iEntryArray;
	RArray<TMsvId> iResultIds;
	RArray<TMsvId> iInvalidIds;
	RArray<TMsvIdWithSortField> iSelectionArray;
	TBuf<KMsvMaxSearchTextLength> iTextToFind;
	TMsvEnhanceSearchSortUtil iEnhanceSearch;
	};


#endif //__MSVSEARCHSORTOPONHEADERBODY_H__
