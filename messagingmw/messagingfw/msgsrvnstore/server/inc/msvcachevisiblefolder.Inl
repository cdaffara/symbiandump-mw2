// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 
 IsComplete()
 @param None.
 @return Status of the Complete Flag.
 
 The function Checks the Complete Bit whether its Set(Logic 1) or Clear(Logic 0).
 It returns a TBool (ETrue/EFalse) depending on the Complete Flag. 
*/
inline TBool CMsvCacheVisibleFolder::IsComplete() const
	{
	if (iFlags & EMsvCacheVisibleFolderComplete)
		{
		return(ETrue);
		}
	else 
		{
		return (EFalse);
		}
	}

/**
 SetComplete()
@param TBool: Set or Clear.
@return None.
 
The function Sets or Resets the Complete Bit depending on the aStatus variable passed to it.
*/
inline void CMsvCacheVisibleFolder::SetComplete(TBool aStatus)
	{
	if(aStatus)
		{
		iFlags |= EMsvCacheVisibleFolderComplete;
		}
	else
		{
		iFlags &= EMsvCacheVisibleFolderClearComplete;
		}
	}

/**
 GetFolderId()
@param None.
@return TMsvId of the Visible folder.
 
The function Returns TMsvId of the visible folder.
*/
inline TMsvId CMsvCacheVisibleFolder::GetFolderId() const
	{
	return iVisibleFolderId;
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 GetDriveId()
@param None.
@return DriveId of the folder.
 
The function Returns driveId of the visible folder.
*/
inline TUint CMsvCacheVisibleFolder::GetDrive() const
	{
	return iDriveId;
	}
	

/**
 SetDriveId()
 @param aDriveId: Drive id of the folder.
 */	
inline void CMsvCacheVisibleFolder::SetDrive(TUint aDriveId)
	{
	iDriveId = aDriveId;
	}
#endif	
	
inline void CMsvCacheVisibleFolder::SetGetChildrenFromVisibleFolder(TBool aStatus)
	{
	if(aStatus)
		{
		iFlags |= EMsvGetChildrenFromVisibleFolder;
		}
	else
		{
		iFlags ^= EMsvGetChildrenFromVisibleFolder;
		}
	}

inline TBool CMsvCacheVisibleFolder::GetChildrenFromVisibleFolder()
	{
	if (iFlags & EMsvGetChildrenFromVisibleFolder)
		{
		return(ETrue);
		}
	else
		{
		return (EFalse);
		}
	}
