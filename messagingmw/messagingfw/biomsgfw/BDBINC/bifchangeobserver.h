// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Description: bifchangeobserver.h
// 
// Author	
// Date		 
//
/**
 * @file 
 * Callback interface implementation for BIF files changes.
 *
 * @publishedPartner
 * @released
 */

#ifndef __BIFCHANGEOBSERVER_H__
#define __BIFCHANGEOBSERVER_H__

class CBIODatabase;

/** Callback interface implemented by classes to receive notifications of BIF files 
changes from CBifChangeObserver. 

@publishedPartner
@released
*/
class MBifChangeObserver 
	{
public:
	/** BIF change events. */
	enum TBifChangeEvent
		{
		/** Unknown change. */
		EBifChangeUnknown = 0,
		/** BIF added. */
		EBifAdded,
		/** BIF deleted. */
		EBifDeleted,
		/** BIF changed. */
		EBifChanged
		};

public: 
	/** Called when a BIF change occurs.
	
	@param aEvent Change event type
	@param aBioID BIO message type of changed BIF */
	virtual void HandleBifChangeL(TBifChangeEvent aEvent, TUid aBioID)=0;
	};

/** Active object that watches for changes made to the installed BIF files. 

@publishedPartner
@released
*/
class CBifChangeObserver : public CActive
	{
public: 
	IMPORT_C static CBifChangeObserver* NewL(MBifChangeObserver& aObserver, RFs& aFs);
	IMPORT_C void Start();
	~CBifChangeObserver();
	
	static void CleanupBifArray(TAny* aBifArray);

private:
	// from CActive
	virtual void RunL();
	virtual void DoCancel();

private:
	CBifChangeObserver(MBifChangeObserver& aObserver, RFs& aFs);
	void ConstructL();
	
	void NotifyObserverL();
	void WaitForFileNotification();
	void DoRunL();
	void CopyEntriesL(const CDir& aDir, CArrayFixFlat<TEntry>& aEntries);
	TBool CompareReaders(const CBioInfoFileReader& aReader1, const CBioInfoFileReader& aReader2) const;
	
	TInt FindEntry(const CBifEntry& aBifEntry, const RPointerArray<CBifEntry>& aEntries, TInt& aIndex) const;
	
private:
	MBifChangeObserver&	iChangeObserver;
	RFs&			iFs;

	RPointerArray<CBifEntry> iEntries;

	CBIODatabase*	iBioDB;
	RTimer	iTimer;
	TInt iRetryCount;
	};

#endif // __BIFCHANGEOBSERVER_H__
