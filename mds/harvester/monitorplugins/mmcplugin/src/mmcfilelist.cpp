/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/

#include <driveinfo.h>
#include <e32msgqueue.h>

#include "mmcfilelist.h"
#include "mmcmounttaskao.h"
#include "harvesterlog.h"
#include "mdsfileserverpluginclient.h"
#include "mdeharvestersession.h"
#include "mdsutils.h"
#include "fsutil.h"
#include "harvestercenreputil.h"
#include "harvestereventmanager.h"
#include "harvestercommon.h"
#include <placeholderdata.h>
#include <harvesterdata.h>
#include "harvesterpluginfactory.h"

// for CleanupResetAndDestroyPushL
#include <mmf/common/mmfcontrollerpluginresolver.h>

_LIT( KBackslash, "\\");

const TInt KEntryBufferSize = 100;

CMmcFileList::CMmcFileList() : iHem( NULL ), iMediaIdUtil( NULL )
	{
	}

CMmcFileList* CMmcFileList::NewL()
	{
	CMmcFileList* self = new ( ELeave ) CMmcFileList();
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}

void CMmcFileList::ConstructL()
	{
	iHem = CHarvesterEventManager::GetInstanceL();
	iMediaIdUtil = &RMediaIdUtil::GetInstanceL();
	CHarvesterCenRepUtil::GetIgnorePathL( iIgnorePaths );
	}

CMmcFileList::~CMmcFileList()
	{
	if( iHem )
		{
		iHem->ReleaseInstance();
		}
	RMediaIdUtil::ReleaseInstance();
	iIgnorePaths.ResetAndDestroy();
	iIgnorePaths.Close();
	}

TBool CMmcFileList::IsDescInArray(const TPtrC& aSearch, const RPointerArray<HBufC>& aArray)
	{
	const TInt count = aArray.Count();
	
	for( TInt i = 0; i < count; i++ )
		{
		TInt result = MdsUtils::Compare( aSearch, aArray[i]->Des() );
		
		if( result == 0 )
			{
			return ETrue;
			}
		}

	return EFalse;
	}

void CMmcFileList::BuildFileListL( RFs& aFs, const TDesC& aDrivePath, 
		RPointerArray<CPlaceholderData>& aEntryArray )
	{
	WRITELOG( "CMmcFileList::BuildFileListL - start" );
	
	CleanupResetAndDestroyPushL( aEntryArray );
	
	CDesCArrayFlat* path = new(ELeave) CDesCArrayFlat( 30 );
	CleanupStack::PushL( path );
	TFileName firstPath;
	firstPath.Copy( aDrivePath );
	firstPath.Append( KBackslash );
	path->AppendL( firstPath );

	TUint32 mediaId( 0 );
	iMediaIdUtil->GetMediaId( aDrivePath, mediaId );
	
	WRITELOG1( "CMmcFileList::BuildFileListL - mediaId: %u", mediaId );
	
    CDir* directory = NULL;
    TFileName name; 
    TBool rootDir( ETrue );
    
	while ( path->Count() > 0 )
		{
		TInt error( KErrNone );
		if( rootDir )
		    {
		    error = aFs.GetDir( (*path)[0], KEntryAttDir, KHarvesterGetRootDirFlags, directory );
		    rootDir = EFalse;
		    }
		else
		    {
		    error = aFs.GetDir( (*path)[0], KEntryAttDir, KHarvesterGetDirFlags, directory );
		    }
		if ( error == KErrNone )
			{
			for ( TInt i=0; i < directory->Count(); ++i )
				{
				TEntry entry = (*directory)[i];
				name.Zero();
				name.Append( (*path)[0] );
				// If filename is too long, skip it as the file cannot be supported
				if( (name.Length() + entry.iName.Length()) > KMaxFileName )
				    {
				    continue;
				    }
				else
				    {
                    // If thumbnail folder is detected, skip it
				    if( entry.iName.Compare( KExludedThumbPath ) != 0 &&
				        entry.iName.Compare( KExludedMediaArtPath ) != 0 )
				        {
				        name.Append( entry.iName );
				        }
				    else
				        {
				        continue;
				        }
				    }
			
				if ( entry.IsDir() )
					{
					// If path is too long with backslash, skip it as the file cannot be supported
	                if( name.Length() + 1 >= KMaxPath )
	                    {
	                    continue;
	                    }
	                else
	                    {
					    name.Append( KBackslash );
	                    }
					
					// remove drive letter and colon from path
					TPtrC pathOnly = name.Mid( 2 );
					
					if ( IsDescInArray( pathOnly, iIgnorePaths ) )
						{
						continue;
						}

					path->AppendL( name );
					}
				else
					{			
					CPlaceholderData* phData = CPlaceholderData::NewL();
					CleanupStack::PushL( phData );
					phData->SetUri( name );
					phData->SetModified( entry.iModified );
					phData->SetFileSize( entry.iSize );
					
				    phData->SetMediaId( mediaId );
#ifdef _DEBUG					
					TBuf<256> debug;
					debug.Copy( name );
					WRITELOG1("CMmcFileList::BuildFileListL - Uri: %S", &debug );
#endif
					aEntryArray.AppendL( phData );
					CleanupStack::Pop( phData );
					}
				}
			}
		path->Delete( 0 );
		delete directory;
		directory = NULL;
		}
	
	CleanupStack::PopAndDestroy( path );
	
	CleanupStack::Pop( &aEntryArray );
	
	WRITELOG( "CMmcFileList::BuildFileListL - end" );
	}

void CMmcFileList::HandleFileEntryL( CMdEHarvesterSession& aMdeSession,
		RPointerArray<CPlaceholderData>& aEntryArray, 
		RPointerArray<CPlaceholderData>& aHarvestEntryArray,
		TUint32 aMediaID, CHarvesterPluginFactory* aPluginFactory )
	{
	WRITELOG( "CMmcFileList::HandleFileEntryL()" );
	
	// calculate batch size
	TInt batchSize( KEntryBufferSize );
	const TInt count( aEntryArray.Count() );
	if ( count < KEntryBufferSize )
		{
		batchSize = count;
		}

#ifdef _DEBUG
	WRITELOG2( "CMmcFileList::HandleFileEntryL() batchSize = %d, aEntryArray.Count() = %d", batchSize, aEntryArray.Count() );
#endif
	
	RArray<TPtrC> uris;
	RArray<TMdSFileInfo> fileInfos;
	CleanupClosePushL( uris );
	CleanupClosePushL( fileInfos );
	
	uris.ReserveL( batchSize );
	fileInfos.ReserveL( batchSize );

	for ( TInt i = 0; i < batchSize; i++ )
		{
		CPlaceholderData* e = aEntryArray[i];
		
		TPtr uriPtr = e->Uri();
		TPtrC uriPtrC;
		uriPtrC.Set( uriPtr.Ptr(), uriPtr.Length() );
		uris.Append( uriPtr );

		TMdSFileInfo fileInfo;
		fileInfo.iModifiedTime = e->Modified().Int64();
		fileInfo.iSize = e->FileSize();
		fileInfos.Append( fileInfo );
		}
	
	RArray<TFilePresentStates> results;
	CleanupClosePushL( results );

	WRITELOG( "CMmcFileList::HandleFileEntryL -- SetFilesToPresent start" );
	aMdeSession.SetFilesToPresentL( aMediaID, uris, fileInfos, results );
	WRITELOG( "CMmcFileList::HandleFileEntryL -- SetFilesToPresent end" );

	for ( TInt i = 0; i < batchSize; i++ )
		{
		CPlaceholderData* e = aEntryArray[ 0 ];
		aEntryArray.Remove( 0 );
		CleanupStack::PushL( e );

		TFilePresentStates found = results[ i ];

		if ( found == EMdsNotFound )
			{
			if ( aPluginFactory && !aPluginFactory->IsSupportedFileExtension( e->Uri() ) )
				{
				iHem->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
				CleanupStack::PopAndDestroy( e );
				continue;
				}
			e->SetPresentState( found );
			aHarvestEntryArray.AppendL( e );
			CleanupStack::Pop( e );
			}
		else if( found == EMdsPlaceholder )
			{
			e->SetPresentState( found );
			aHarvestEntryArray.AppendL( e );
			CleanupStack::Pop( e );
			}
		else if( found == EMdsModified )
			{
			e->SetPresentState( found );
			aHarvestEntryArray.AppendL( e );
			CleanupStack::Pop( e );
			}
		else
			{
			iHem->DecreaseItemCountL( EHEObserverTypeMMC, 1 );
			CleanupStack::PopAndDestroy( e );
			}
		}
	
	if( aEntryArray.Count() == 0 )
		{
		aEntryArray.Compress();
		}
	
	CleanupStack::PopAndDestroy( &results );
	CleanupStack::PopAndDestroy( &fileInfos );
	CleanupStack::PopAndDestroy( &uris );
	WRITELOG( "CMmcFileList::HandleFileEntryL -- end" );
	}
