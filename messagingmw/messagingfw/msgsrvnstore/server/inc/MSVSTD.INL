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

inline TMsvId TMsvEntry::Id() const
/** Gets the value of the entry ID.

@return The value of the entry ID */
	{
	return iId;
	}

inline void TMsvEntry::SetId(TMsvId aId)
	{
	iId = aId;
	}

inline TMsvId TMsvEntry::Parent() const
/** Gets the value of the parent ID.

@return The value of the parent ID */
	{
	return iParentId;
	}

inline void TMsvEntry::SetParent(TMsvId aParentId)
	{
	iParentId = aParentId;
	}

inline TBool TMsvEntry::Complete () const
/** Gets the value of the message complete flag.

Note that for email download without size limits, the complete flag will only
be set to true once all the parts of the message (body text and attachments)
have been downloaded.

@return The value of the message complete flag */
	{
	return !(iData&KMsvEntryNotCompleteFlag);
	}

inline void TMsvEntry::SetComplete(TBool aComplete)
/** Sets the message complete flag.

Note that for email download without size limits, the complete flag will only
be set to true once all the parts of the message (body text and attachments)
have been downloaded.

@param aComplete Value for the message complete flag */
	{
	iData = (iData & ~KMsvEntryNotCompleteFlag) | ((!aComplete)?KMsvEntryNotCompleteFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::New() const
/** Gets the value of the new message flag.

@return The value of the new message flag */
	{
	if(iData&KMsvEntryNewFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetNew(TBool aNew)
/** Sets the message new flag.

@param aNew Value for the message new flag */
	{
	iData = (iData & ~KMsvEntryNewFlag) | (aNew?KMsvEntryNewFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Unread() const
/** Gets the value of the message unread flag.

@return The value of the message unread flag */
	{
	if(iData&KMsvEntryUnreadFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetUnread(TBool aUnread)
/** Sets the message unread flag.
	
@param aUnread Value for the message unread flag */
	{
	iData = (iData & ~KMsvEntryUnreadFlag) | (aUnread?KMsvEntryUnreadFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Failed() const
/** Gets the value of the message send failed flag.

@return The value of the message send failed flag */
	{
	if(iData&KMsvEntryFailedFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetFailed(TBool aFailed)
/** Sets the message send failed flag.

@param aFailed Value for the message send failed flag */
	{
	iData = (iData & ~KMsvEntryFailedFlag) | (aFailed?KMsvEntryFailedFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Operation() const
/** Gets the value of the disconnected operation queued flag.

@return The value of the disconnected operation queued flag */
	{
	if(iData&KMsvEntryOperationFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetOperation(TBool aOperation)
/** Sets the disconnected operation queued flag.

@param aOperation Value for the disconnected operation queued flag */
	{
	iData = (iData & ~KMsvEntryOperationFlag) | (aOperation?KMsvEntryOperationFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Owner() const
/** Gets the value of the owner flag.

@return The value of the owner flag */
	{
	if(iData&KMsvEntryOwnerFlag)
		{
		return ETrue;
		}
	return EFalse;		
	}

inline void TMsvEntry::SetOwner(TBool aOwner)
	{
	iData = (iData & ~KMsvEntryOwnerFlag) | (aOwner?KMsvEntryOwnerFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Attachment() const
/** Gets the value of the attachment flag.

@return The value of the attachment flag */
	{
	if(iData&KMsvEntryAttachmentFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetAttachment(TBool aAttachmentFlag)
/** Sets the attachment flag.

@param aAttachmentFlag Value for the attachment flag */
	{
	iData = (iData & ~KMsvEntryAttachmentFlag) | (aAttachmentFlag?KMsvEntryAttachmentFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Visible() const
/** Gets the value of the entry visible flag.

@return The value of the entry visible flag */
	{
	return !((iData&KMsvEntryInvisibleFlag) || (iData&KMsvEntryPendingDeleteFlag));
	}

inline void TMsvEntry::SetVisible(TBool aVisble)
/** Sets the entry visible flag.
	
@param aVisble Value for the entry visible flag */
	{
	iData = (iData & ~KMsvEntryInvisibleFlag) | ((!aVisble)?KMsvEntryInvisibleFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::MultipleRecipients() const
/** Gets the value of the multiple recipients flag.

@return The value of the multiple recipients flag */
	{
	if(iData&KMsvEntryMultipleRecipientFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetMultipleRecipients(TBool aMultipleRecipient)
/** Sets the multiple recipients flag.
	
@param aMultipleRecipient Value for the multiple recipients flag */
	{
	iData = (iData & ~KMsvEntryMultipleRecipientFlag) | (aMultipleRecipient?KMsvEntryMultipleRecipientFlag:KMsvEntryClearFlag);
	}


inline TBool TMsvEntry::ReadOnly() const
/** Gets the value of the read-only flag.

@return The value of the read-only flag */
	{
	if(iData&KMsvEntryReadOnlyFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetReadOnly(TBool aReadOnly)
/** Sets the read-only flag.

@param aReadOnly Value for the read-only flag */
	{
	iData = (iData & ~KMsvEntryReadOnlyFlag) | (aReadOnly?KMsvEntryReadOnlyFlag:KMsvEntryClearFlag);
	}


/** 
Sets the flag for the TMsvEntry's visible folder(CMsvCacheVisibleFolder), if the entry is visible 
while creation.
@param aVisble Value for the visible flag 
*/
inline void TMsvEntry::SetVisibleFolderFlag(TBool aVisible)

	{
	iData = (iData & ~KMsvVisibleFolderNodeFlag) | ((!aVisible)?KMsvVisibleFolderNodeFlag:KMsvEntryClearFlag);
	}

/** 
Gets the value of the TMsvEntry's visible folder flag.
@return The value of the entry visible flag 
*/
inline TBool TMsvEntry::VisibleFolderFlag() const
	{
	return !(iData&KMsvVisibleFolderNodeFlag);
	}

	
inline TBool TMsvEntry::Deleted() const
/** Gets the value of the deleted flag.

@return The value of the deleted flag */
	{
	if(iData&KMsvEntryDeletedFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetDeleted(TBool aDeletedFlag)
	{
	iData = (iData & ~KMsvEntryDeletedFlag) | (aDeletedFlag?KMsvEntryDeletedFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::StandardFolder() const
/** Gets the value of the standard folder flag.

@return The value of the standard folder flag */
	{
	if(iData&KMsvEntryStandardFolderFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetStandardFolder(TBool aStandardFolder)
	{
	iData = (iData & ~KMsvEntryStandardFolderFlag) | (aStandardFolder?KMsvEntryStandardFolderFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Connected() const
/** Gets the value of the remote server connection flag.

@return The value of the remote server connection flag */
	{
	if(iData&KMsvEntryConnectedFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetConnected(TBool aConnected)
/** Sets the remote server connection flag.

@param aConnected Value for the remote server connection flag */
	{
	iData = (iData & ~KMsvEntryConnectedFlag) | (aConnected?KMsvEntryConnectedFlag:KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::InPreparation() const
/** Gets the value of the in preparation flag.

@return The value of the in preparation flag */
	{
	if(iData&KMsvEntryInPreparationFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetInPreparation(TBool aInPreparation)
/** Sets the in-preparation flag.

@param aInPreparation Value for the in-preparation flag */
	{
	iData = (iData & ~KMsvEntryInPreparationFlag) | (aInPreparation?KMsvEntryInPreparationFlag:KMsvEntryClearFlag);
	}

inline TInt TMsvEntry::PcSyncCount() const
/** Gets the value of the PC synchronisation field.

@return The value of the PC synchronisation field */
	{
	return iPcSyncCount;
	}

inline void TMsvEntry::IncPcSyncCount()
/** Increments the PC synchronisation field. */
	{
	iPcSyncCount++;
	}

inline void TMsvEntry::DecPcSyncCount()
/** Decrements the PC synchronisation field. */
	{
	iPcSyncCount--;
	}

inline TUint TMsvEntry::PersistedFlags() const
	{
	return iData&KMsvEntryPersistedFlags;
	}

inline TUint TMsvEntry::TemporaryFlags() const
	{
	return iData&KMsvEntryTemporaryFlags;
	}

inline TBool TMsvEntry::OffPeak() const
/** Gets the off-peak flag.

@return The value of the off-peak flag */
	{
	if(iData & KMsvOffPeakFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetOffPeak(TBool aOffPeak)
/** Sets the off-peak flag.

@param aOffPeak Value for off-peak flag. */
	{
	iData = (iData & ~KMsvOffPeakFlag) | (aOffPeak ? KMsvOffPeakFlag : KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Scheduled() const
/** Gets the scheduled flag.

@return The value of the scheduled flag */
	{
	if(iData & KMsvScheduledFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetScheduled(TBool aScheduled)
/** Sets the scheduled flag.

@param aScheduled Value for the scheduled flag. */
	{
	iData = (iData & ~KMsvScheduledFlag) | (aScheduled ? KMsvScheduledFlag : KMsvEntryClearFlag);
	}
	
inline TUint TMsvEntry::SendingState() const
/** Gets the sending state.
 
@return The sending state */
	{
	return (iData & KMsvSendingStateFlags) >> KMsvSendingStateShift;
	}
	 
inline void TMsvEntry::SetSendingState(TUint aSendingState)
/** Sets the sending state.
  
@param aSendingState The sending state. This must be a value between 0x00 
and KMsvSendStateMax. */
	{
	__ASSERT_DEBUG(aSendingState <= KMsvSendStateMax, User::Invariant());
	iData = iData & ~KMsvSendingStateFlags | aSendingState << KMsvSendingStateShift;
	}

inline void TMsvEntry::SetPendingDelete(TBool aPendingDelete)
	{
	iData = (iData & ~KMsvEntryPendingDeleteFlag) | (aPendingDelete ? KMsvEntryPendingDeleteFlag : KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::PendingDelete() const
	{
	if(iData & KMsvEntryPendingDeleteFlag)
		{
		return ETrue;
		}
	return EFalse;
	}
	

inline void TMsvEntry::SetDeleteProtected(TBool aDeleteProtected)
	{
	/** temporary variable used to mask the anonymous global enum.		*/
	TInt temp = KMsvEntryDeleteProtected;
/** Sets the delete protected state */
	iMtmData1 = (iMtmData1 & ~KMsvEntryDeleteProtected ) | (aDeleteProtected ? temp : KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::DeleteProtected() const
	{
/** returns the delete protected state */
	if(iMtmData1 & KMsvEntryDeleteProtected)
		{
		return ETrue;
		}
	return EFalse;
	}
	
inline void TMsvEntry::SetForwarded(TBool aForwarded)
	{
	/** temporary variable used to mask the anonymous global enum.		*/
	TInt temp = KMsvEntryForwarded ;
/** Sets the forwarded state */
	iMtmData1 = (iMtmData1 & ~KMsvEntryForwarded) | (aForwarded ? temp : KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::Forwarded() const
	{
/** returns the forwarded state */
	if(iMtmData1 & KMsvEntryForwarded)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvEntry::SetLocallyDeleted(TBool aLocallyDeleted)
	{
	/** temporary variable used to mask the anonymous global enum.		*/
	TInt temp = KMsvEntryLocallyDeleted ;
/** Sets the locally deleted state */
	iMtmData1 = (iMtmData1 & ~KMsvEntryLocallyDeleted) | (aLocallyDeleted ? temp : KMsvEntryClearFlag);
	}

inline TBool TMsvEntry::LocallyDeleted() const
	{
/** returns the locally deleted state  */
	if(iMtmData1 & KMsvEntryLocallyDeleted)
		{
		return ETrue;
		}
	return EFalse;
	}



//**********************************
// TMsvSelectionOrdering
//**********************************

inline TBool TMsvSelectionOrdering::GroupByType() const
/** Gets the group-by-entry-type flag.

@return Group-by-entry-type flag */
	{
	if(iGrouping&KMsvGroupByType)
		{
		return ETrue;
		}
	return EFalse;
	}

inline TBool TMsvSelectionOrdering::GroupStandardFolders() const
/** Gets the group-by-standard-folders flag.

@return Group-by-standard-folders flag */
	{
	if(iGrouping&KMsvStandardFolders)
		{
		return ETrue;
		}
	return EFalse;
	}

inline TBool TMsvSelectionOrdering::GroupByPriority() const
/** Gets the group-by-priority flag.

@return Group-by-priority flag */
	{
	if(iGrouping&KMsvGroupByPriority)
		{
		return ETrue;
		}
	return EFalse;
	}

inline TBool TMsvSelectionOrdering::GroupByMtm() const
/** Gets the group-by-MTM flag.

@return Group-by-MTM flag */
	{
	if(iGrouping&KMsvGroupByMtm)
		{
		return ETrue;
		}
	return EFalse;
	}

inline TBool TMsvSelectionOrdering::ShowInvisibleEntries() const
/** Gets the show-invisible-entries flag. Entries whose entry visible flag index 
field is not set are included in the ordered list only if this is set. 

@return Show-invisible-entries flag */
	{
	if(iGrouping&KMsvInvisibleFlag)
		{
		return ETrue;
		}
	return EFalse;
	}

inline void TMsvSelectionOrdering::SetGroupByType(TBool aFlag)
	{
	iGrouping = (iGrouping & ~KMsvGroupByStandardFolders) | ((aFlag)?KMsvGroupByType:0);
	}

inline void TMsvSelectionOrdering::SetGroupStandardFolders(TBool aFlag)
	{
	iGrouping = (iGrouping & ~KMsvStandardFolders) | ((aFlag)?KMsvGroupByStandardFolders:0);
	}

inline void TMsvSelectionOrdering::SetGroupByPriority(TBool aFlag)
	{
	iGrouping = (iGrouping & ~KMsvGroupByPriority) | ((aFlag)?KMsvGroupByPriority:0);
	}

inline void TMsvSelectionOrdering::SetGroupByMtm(TBool aFlag)
	{
	iGrouping = (iGrouping & ~KMsvGroupByMtm) | ((aFlag)?KMsvGroupByMtm:0);
	}

inline void TMsvSelectionOrdering::SetShowInvisibleEntries(TBool aFlag)
	{
	iGrouping = (iGrouping & ~KMsvInvisibleFlag) | ((aFlag)?KMsvInvisibleFlag:0);
	}

inline TMsvSorting TMsvSelectionOrdering::Sorting() const
/** Gets the sorting order of entries within groups.

@return Sorting order. */
	{
	return iSortType;
	}

inline void TMsvSelectionOrdering::SetSorting(TMsvSorting aSortType)
	{
	iSortType=aSortType;
	}

inline TBool TMsvSelectionOrdering::GroupingOn() const
/** Tests whether any grouping option has been set.

@return ETrue if one or more grouping options have been set, else EFalse */
	{
	if(iGrouping&KMsvAllGroupingFlags)
		{
		return ETrue;
		}
	return EFalse;
	}

//**********************************
// CMsvEntryFilter
//**********************************

inline TMsvId CMsvEntryFilter::Service() const
/** Gets the service ID set for the filter.

@return Service ID set for the filter */
	{
	return iServiceId;
	}

inline void CMsvEntryFilter::SetService(TMsvId aServiceId)
/** Sets the service ID for the filter.

@param aServiceId Service ID for the filter */
	{
	iServiceId = aServiceId;
	}

inline TUid CMsvEntryFilter::Mtm() const
/** Gets the MTM UID set for the filter.

@return MTM UID set for the filter */
	{
	return iMtm;
	}

inline void CMsvEntryFilter::SetMtm(TUid aMtm)
/** Sets the MTM UID for the filter.

@param aMtm MTM UID for the filter */
	{
	iMtm = aMtm;
	}

inline TUid CMsvEntryFilter::Type() const
/** Gets the entry type set for the filter.

@return Entry type set for the filter */
	{
	return iType;
	}

inline void CMsvEntryFilter::SetType(TUid aType)
/** Sets the entry type for the filter.

@param aType Entry type for the filter */
	{
	iType = aType;
	}

inline const TTime& CMsvEntryFilter::LastChangeDate() const
/** Gets the last change date set for the filter.

@return Last change date set for the filter */
	{
	return iLastChange;
	}

inline void CMsvEntryFilter::SetLastChangeDate(const TTime& aLastChange)
/** Sets the last date change for the filter.

This is used to retrieve entries that have changed since a particular date.

@param aLastChange Last date change for the filter, specified in Universal 
Time (GMT) */
	{
	iLastChange = aLastChange;
	}

inline const TMsvSelectionOrdering& CMsvEntryFilter::Order() const
/** Gets the selection ordering set for the filter.

@return Selection ordering set for the filter */
	{
	return iOrdering;
	}

inline void CMsvEntryFilter::SetOrder(const TMsvSelectionOrdering& aOrder)
/** Sets the sort ordering for the filter.

@param aOrder Selection ordering for the filter */
	{
	iOrdering = aOrder;
	}

inline TUid CMsvEntryFilter::SortMtm() const
/** Gets the sorting by MTM for the filter.

@return Sorting by MTM for the filter */
	{
	return iSortMtm;
	}

inline void CMsvEntryFilter::SetSortMtm(TUid aSortMtm)
/** Sets the sorting by MTM for the filter.

@param aSortMtm Sorting by MTM for the filter */
	{
	iSortMtm = aSortMtm;
	}
