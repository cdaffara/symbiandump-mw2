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
// Description:
// msvconsts.h
//

/**
 @file
 @internal
 @released
*/
#if !defined(__MSVCONSTS_H__)
#define __MSVCONSTS_H__

#include <e32base.h>
#include <msvstd.hrh>
#include <msvstd.h>

// APIs KMsvTempIndexEntryId and KFirstFreeEntryId moved to msvstd.h

/**
UId for 8-bit/16-bit Character set mechanism.
@internalTechnology
@prototype
*/
const TInt KUIDCharacterSet = 999; 

/**
 @internalComponent
 @released
*/
 enum TMtsrDllIndex    //  Used in CMtmGroupData
 	{
  	EMtsrServerComponentIndex,
  	EMtsrClientComponentIndex,
  	EMtsrUiComponentIndex,
  	EMtsrUiDataComponentIndex
  	};

 /**
UId for the 16-bit chunk storage mechanism.
@internalTechnology
@prototype
*/
const TUid KMsvPlainBodyText16={0x10000F90};

/**
UId for the 8-bit chunk storage mechanism.
@internalTechnology
@prototype
*/
const TUid KMsvPlainBodyText8={0x10000F80};

/**
* @internalAll
* @released
*/
const TInt KMsvNumMtmDllTypes=4;
/**
* @internalAll
* @released
*/
const TUid KUidMsvDataComponent     ={0x10003C69};

// User Interface MTM
/**
* @internalAll
* @released
*/
const TUid KUidMtmUiComponent		={KUidMtmUiComponentVal};
// UI Data MTM 
/**
* @internalAll
* @released
*/
const TUid KUidMtmUiDataComponent	={KUidMtmUiDataComponentVal};
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType1Uid={0x10003C62}; 
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType2Uid={0x10003C63};
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType3Uid={0x10003C64};
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType4Uid={0x10003C65};
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType5Uid={0x10003C66};
/**
* @internalTechnology
* @released
*/
const TUid KMsvDllType6Uid={0x10003C67};
/**
* @internalComponent
* @released
*/
const TUid KUidMtmMultipleComponent	={0x10004845}; 

#endif //__MSVCONSTS_H__
