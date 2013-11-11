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

#if !defined(__MSVARRAY_H__)
#define __MSVARRAY_H__

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

//*************************
//  class TKeyArrayFixPtr
//*************************

class TKeyArrayFixPtr : public TKeyArrayFix
// 
// Generic Key class for sorting arrays of pointers in flat arrays. 
//
/**
@internalComponent
@released
*/
	{ 
public:
	inline TKeyArrayFixPtr(TInt anOffset,TKeyCmpText aType);
	inline TKeyArrayFixPtr(TInt anOffset,TKeyCmpText aType,TInt aLength);
	inline TKeyArrayFixPtr(TInt anOffset,TKeyCmpNumeric aType);
	// At returns dereferenced pointer to objects (but not index)
	IMPORT_C TAny* At(TInt anIndex) const;
	};


//**********************************
// CMsvEntryArray
//**********************************
//
// The array of TMsvEntry pointers
//

class CMsvEntryArray : public CArrayPtrFlat<const TMsvEntry>
/**
@internalComponent
@released
*/
	{ 
public: 
	IMPORT_C static CMsvEntryArray* NewL(const CArrayFix<TUid>& iMtmList); 
	IMPORT_C static CMsvEntryArray* NewLC(const CArrayFix<TUid>& iMtmList); 
	IMPORT_C void SortL(TMsvSelectionOrdering aOrdering);
	IMPORT_C ~CMsvEntryArray();
private:
	enum TGroupCriterion
		{
		EGroupByStandardFolders, 
		EGroupByType, 
		EGroupByPriority, 
		EGroupByMtm, 
		EStopGrouping
		};
private:
	TInt NumberOfArraysToSplitIntoL(TGroupCriterion aGroupCriterion);
	TBool OkToGroup(TGroupCriterion aGroupCriterion,TMsvSelectionOrdering aOrdering) const;
	TInt ArrayId(const TMsvEntry* aEntry,TGroupCriterion aGroupCriterion) const;
	void GroupL(TGroupCriterion aGroupCriterion,TMsvSelectionOrdering aOrdering,TBool aDoSort);
	void ReverseOrder(TMsvSorting aSortType);
	void ReverseOrder();
	CMsvEntryArray(const CArrayFix<TUid>& iMtmList);
	TKeyArrayFixPtr MessageSortKey(TMsvSorting aSortType) const;

	TInt FindSubjectStart(const TDesC& aSubject, const TDesC& aSubjectSkipString) const;
	void SubjectBasedSortL(TBool aReverse, const TDesC& aSubjectSkipString);
	void DetailBasedSortL();

private:
	const CArrayFix<TUid>& iOrigMtmList;
	CArrayFix<TUid>* iActualMtmList;
	};

// inline

inline TKeyArrayFixPtr::TKeyArrayFixPtr(TInt anOffset,TKeyCmpText aType)
	: TKeyArrayFix(anOffset,aType)
	{}

inline TKeyArrayFixPtr::TKeyArrayFixPtr(TInt anOffset,TKeyCmpText aType,TInt aLength)
	: TKeyArrayFix(anOffset,aType,aLength)
	{}

inline TKeyArrayFixPtr::TKeyArrayFixPtr(TInt anOffset,TKeyCmpNumeric aType)
	: TKeyArrayFix(anOffset,aType)
	{}


#endif
