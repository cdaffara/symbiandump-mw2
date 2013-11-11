/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Harvester Central Repository utility class*
*/


#include "harvestercenreputil.h"
#include "centralrepository.h"
#include "mdsutils.h"
#include "mdeconstants.h"

const TUid KHarvesterRepoUid = { 0x200009FE };
const TUint32 KSearchMask                     = 0xFFFF0000;
const TUint32 KIgnorePathPartialKey           = 0x00010000;
const TUint32 KBootScanPartialKey             = 0x00020000;
const TUint32 KBootIgnorePartialKey           = 0x00030000;
const TUint32 KPartialRestorePartialKey       = 0x00040000;
const TUint32 KPartialRestoreIgnorePartialKey = 0x00050000;
const TUint32 KAlwaysScanPartialKey           = 0x00060000;

const TUint32 KRomScanEnabledKey              = 0x00090000;

const TInt32 KThumbnailManagerCenrepUID = 0x102830B0; // TNM cenrep uid
const TUint32 KEnableDaemon = 0x300; // from TNM

CHarvesterCenRepUtil::CHarvesterCenRepUtil()
	{
	// No implementation required
	}


EXPORT_C CHarvesterCenRepUtil::~CHarvesterCenRepUtil()
	{
	iFspEngine.Close();
	iFs.Close();
	}

EXPORT_C CHarvesterCenRepUtil* CHarvesterCenRepUtil::NewLC()
	{
	CHarvesterCenRepUtil* self = new (ELeave)CHarvesterCenRepUtil();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

EXPORT_C CHarvesterCenRepUtil* CHarvesterCenRepUtil::NewL()
	{
	CHarvesterCenRepUtil* self=CHarvesterCenRepUtil::NewLC();
	CleanupStack::Pop(); // self;
	return self;
	}

void CHarvesterCenRepUtil::ConstructL()
	{
	User::LeaveIfError( iFs.Connect() );
	User::LeaveIfError( iFspEngine.Open( iFs, KMdsFSPluginPosition ) );
	}

EXPORT_C RMdsFSPEngine& CHarvesterCenRepUtil::FspEngine()
	{
	return iFspEngine;
	}

EXPORT_C void CHarvesterCenRepUtil::AddIgnorePathsToFspL( const TDesC& aDrivePrefix )
	{
	AddOrRemoveIgnorePathsL( aDrivePrefix, ETrue, KNullDesC );
	}

EXPORT_C void CHarvesterCenRepUtil::AddIgnorePathsToFspL( const TDesC& aDrivePrefix,
														  const TDesC& aFilterPrefix )
	{
	AddOrRemoveIgnorePathsL( aDrivePrefix, ETrue, aFilterPrefix);
	}

EXPORT_C void CHarvesterCenRepUtil::RemoveIgnorePathsFromFspL( const TDesC& aDrivePrefix )
	{
	AddOrRemoveIgnorePathsL( aDrivePrefix, EFalse, KNullDesC );
	}

void CHarvesterCenRepUtil::AddOrRemoveIgnorePathsL( const TDesC& aDrivePrefix,
													TBool aAdd,
													const TDesC& aFilterPrefix )
	{
	CRepository* repo = CRepository::NewLC( KHarvesterRepoUid ); 
	RArray<TUint32> ignoredKeys;
	CleanupClosePushL( ignoredKeys );
	TInt ret = repo->FindL( KIgnorePathPartialKey, KSearchMask, ignoredKeys );
	
	TBuf<KMaxFileName> path;
	for( TInt i = ignoredKeys.Count() - 1; i >=0; i-- )
		{
		const TInt error( repo->Get( ignoredKeys[i], path ) );
		if( error != KErrNone )
		    {
		    continue;
		    }
		if( aFilterPrefix != KNullDesC )
			{
			TPtrC pathStart = path.Left( aFilterPrefix.Length() );
			if( MdsUtils::Compare( pathStart, aFilterPrefix ) != 0 )
				{
				continue;
				}
			}
		HBufC* ignorePath = HBufC::NewL( aDrivePrefix.Length() + path.Length() );
		TPtr ptr = ignorePath->Des();
		ptr.Append( aDrivePrefix );
		ptr.Append( path );
		if( aAdd )
			{
			iFspEngine.AddIgnorePath( *ignorePath );
			}
		else
			{
			iFspEngine.RemoveIgnorePath( *ignorePath );
			}
		delete ignorePath;
		ignorePath = NULL;
		}
	CleanupStack::PopAndDestroy( &ignoredKeys );
	CleanupStack::PopAndDestroy( repo );	
	}

EXPORT_C void CHarvesterCenRepUtil::GetIgnorePathL( 
		RPointerArray<HBufC>& aIgnorePaths )
	{
	GetPathsL( KIgnorePathPartialKey, aIgnorePaths );
	}

EXPORT_C void CHarvesterCenRepUtil::GetScanItemsL(
		RPointerArray<TScanItem>& aItems )
	{
	GetItemsL( KBootScanPartialKey, aItems );
	}

EXPORT_C void CHarvesterCenRepUtil::GetIgnoredScanPathsL( 
		RPointerArray<HBufC>& aIngorePaths )
	{
	GetPathsL( KBootIgnorePartialKey, aIngorePaths );
	}

EXPORT_C void CHarvesterCenRepUtil::GetPartialRestorePathsL( 
		RPointerArray<TScanItem>& aPaths )
	{
	GetItemsL( KPartialRestorePartialKey, aPaths );
	}

EXPORT_C void CHarvesterCenRepUtil::GetIgnoredPartialRestorePathsL( 
		RPointerArray<HBufC>& aIngorePaths )
	{
	GetPathsL( KPartialRestoreIgnorePartialKey, aIngorePaths );
	}
	
EXPORT_C void CHarvesterCenRepUtil::IsRomScanEnabledL( TBool& aEnabled )
    {
    CRepository* rep = CRepository::NewLC( KHarvesterRepoUid );
    
    rep->Get( KRomScanEnabledKey, aEnabled );
    
    CleanupStack::PopAndDestroy( rep );    
    }

EXPORT_C void CHarvesterCenRepUtil::IsThumbnailDaemonEnabledL( TBool& aEnabled )
    {
    CRepository* rep = CRepository::NewLC( TUid::Uid( KThumbnailManagerCenrepUID ) );
    
    rep->Get( KEnableDaemon, aEnabled );
    
    CleanupStack::PopAndDestroy( rep );    
    }

EXPORT_C void CHarvesterCenRepUtil::GetAlwaysScanOnBootPathsL( 
        RPointerArray<TScanItem>& aPaths )
    {
    GetItemsL( KAlwaysScanPartialKey, aPaths );
    }

void CHarvesterCenRepUtil::GetPathsL( TUint32 aPartialKey, RPointerArray<HBufC>& aPaths )
	{
	CRepository* repo = CRepository::NewLC( KHarvesterRepoUid ); 
	RArray<TUint32> scanPathKeys;
	CleanupClosePushL( scanPathKeys );

	repo->FindL( aPartialKey, KSearchMask, scanPathKeys );

	TBuf<KMaxFileName> path;
	const TInt count = scanPathKeys.Count();
	aPaths.ReserveL( count );
	for( TInt i = count - 1; i >=0; i-- )
		{
	    const TInt error( repo->Get( scanPathKeys[i], path ) );
	    if( error != KErrNone )
	        {
	        continue;
	        }
		HBufC* pathBuf = path.AllocL();
		CleanupStack::PushL( pathBuf );
		aPaths.AppendL( pathBuf );
		CleanupStack::Pop( pathBuf );
		}

	CleanupStack::PopAndDestroy( &scanPathKeys );
	CleanupStack::PopAndDestroy( repo );
	}

void CHarvesterCenRepUtil::GetItemsL( TUint32 aPartialKey, RPointerArray<TScanItem>& aItems )
	{
	CRepository* repo = CRepository::NewLC( KHarvesterRepoUid ); 
	RArray<TUint32> scanPathKeys;
	CleanupClosePushL( scanPathKeys );

	repo->FindL( aPartialKey, KSearchMask, scanPathKeys );

	TBuf<KMaxFileName> path;
	
	for( TInt i = scanPathKeys.Count() - 1; i >=0; i-- )
		{
	    const TInt error( repo->Get( scanPathKeys[i], path ) );
	    
	    if( error != KErrNone )
	        {
	        continue;
	        }
		TUint32 preinstalled = MdeConstants::MediaObject::ENotPreinstalled;
		
		TScanItem* item = new (ELeave) TScanItem();
		CleanupStack::PushL( item );
		item->iPath = path.AllocL();
		item->iPreinstalled = preinstalled;
		CleanupStack::Pop( item );
		aItems.AppendL( item ); // ownership is transferred
		}

	CleanupStack::PopAndDestroy( &scanPathKeys );
	CleanupStack::PopAndDestroy( repo );	
	}

// End of file

