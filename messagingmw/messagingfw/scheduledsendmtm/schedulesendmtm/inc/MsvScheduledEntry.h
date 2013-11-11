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

#ifndef __MSVSCHEDULEDENTRY_H__
#define __MSVSCHEDULEDENTRY_H__

#include <msvsenderroraction.h>
#include <msventryscheduledata.h>

/**
Abstract base class which stores the schedule data and recipients of a message. 

CMsvScheduledEntry does not force the Server MTM to store the message 
recipients in a certain way. However, it is recommended that the Server MTM 
derive its recipients class from CMsvRecipient, because the class already has 
support for the number of retries to send a message to a recipient. 

CMsvScheduledEntry stores the TMsvEntry to which it relates to provide quick 
access to the scheduling-related index data without needing calls to 
CMsvServerEntry::SetEntry(). 
However, CMsvScheduledEntry does not provide access to all the members of 
the TMsvEntry. This is to stop the user of the CMsvScheduledEntry from 
accessing the descriptor members of the TMsvEntry while the server entry is 
not set to the TMsvEntry. 

@publishedAll
@released
*/

class CMsvScheduledEntry : public CBase
	{
public:
	/**
	Tests if any of the recipients of this message can still be sent to.
	
	@param aErrorActions
	Error-actions
	
	@param aAction 
	Default error-action
	
	@return True if at least one of the recipients has not been sent 
	the message.
	*/
	virtual TBool CanSendToAnyRecipients(const CMsvSendErrorActions& aErrorActions, TMsvSendErrorAction& aAction) = 0;

	/**
	Tests if any of the recipients of this message can still be sent to.
	
	@param aAction
	Default error-action
	
	@return True if at least one of the recipients has not been sent 
	the message.
	*/
	virtual TBool CanSendToAnyRecipients(const TMsvSendErrorAction& aAction) = 0;
	
	/**
	Sets all the recipients' retries to zero.
	*/
	virtual void RecipientsResetRetries() = 0;
	
	/**
	Increases all the recipients' retries by one.
	*/
	virtual void RecipientsIncreaseRetries() = 0;
	
	/**
	Sets all the recipients' flags to sending failed.
	*/
	virtual void RecipientsSetFailed() = 0;
	
	/**
	Tests if all recipients of the message have been sent successfully. 
	
	@return True if all recipients have been sent successfully.
	*/
	virtual TBool RecipientsAllSent() const = 0;

	IMPORT_C virtual void StoreL(CMsvStore& aStore) const;
	IMPORT_C virtual void RestoreL(CMsvStore& aStore);

	/**
	Stores the recipients.
	
	@param aStore
	Store to write to.
	*/
	virtual void RecipientsStoreL(CMsvStore& aStore) const = 0;
	
	/**
	Restores the recipients.
	
	@param aStore
	Store to read from.
	*/
	virtual void RecipientsRestoreL(CMsvStore& aStore) = 0;

	inline const TTime& ScheduleDate() const;
	inline void SetScheduleDate(const TTime& aDate);

	inline const TBool Scheduled() const;
	inline void SetScheduled(const TBool aScheduled);

	inline const TInt SendingState() const;
	inline void SetSendingState(TInt aState);

	inline const TInt Error() const;
	inline const TMsvId Id() const;
	inline const TBool OffPeak() const;

	inline void SetFailed(const TBool aFail);
	inline const TBool Failed() const;
	
	IMPORT_C TBool PendingConditions() const;
	IMPORT_C void SetPendingConditions(TBool aPendingConditions);

	inline const TUid Mtm() const;

	IMPORT_C void Entry(TMsvEntry& aEntry) const;

protected:

	IMPORT_C CMsvScheduledEntry(const TMsvEntry& aEntry);

public:

	/** Message schedule data. */
	TMsvEntryScheduleData iData;

private:

	TMsvEntry iEntry;
	};

/**
Dynamic array of CMsvScheduledEntry objects. 

@publishedAll
@released
*/
typedef CArrayPtrFlat<CMsvScheduledEntry> CMsvScheduledEntries;

#include <msvscheduledentry.inl>

#endif
