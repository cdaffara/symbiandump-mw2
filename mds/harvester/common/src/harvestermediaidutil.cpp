/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Utility class to keep a list of mediaid's
*/

#include "harvestermediaidutil.h"
#include "mdsutils.h"


CHarvesterMediaIdUtil* CHarvesterMediaIdUtil::NewL()
	{
	CHarvesterMediaIdUtil* self = new (ELeave) CHarvesterMediaIdUtil;
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop( self );
	return self;
	}


CHarvesterMediaIdUtil::CHarvesterMediaIdUtil()
	{
	// no implementation needed
	}

CHarvesterMediaIdUtil::~CHarvesterMediaIdUtil()
	{
	iFs.Close();
	}

void CHarvesterMediaIdUtil::ConstructL()
	{
	for ( TInt i = 0; i < KMaxDrives; i++ )
		{
		iMediaId[i] = 0;
		}
	User::LeaveIfError( iFs.Connect() );
	}

EXPORT_C TInt CHarvesterMediaIdUtil::GetMediaId( const TDesC& aUri, TUint32& aMediaId )
	{
    TChar ch = aUri[0];
    TInt drive( 0 );
    TInt err = RFs::CharToDrive( ch, drive );
    if ( err == KErrNone )
    	{
    	// return mediaid if already in array
    	if( iMediaId[drive] > 0 )
    		{
    		aMediaId = iMediaId[drive];
    		return KErrNone;
    		}
    	
    	// resolve mediaid and save into array
	    err = MdsUtils::GetVolumeInfo( iFs, aUri, iVolumeInfo );
	    if ( err == KErrNone )
	    	{
	    	iMediaId[drive] = iVolumeInfo.iUniqueID;
	    	aMediaId = iVolumeInfo.iUniqueID;
	    	return KErrNone;
	    	}
    	}
    return err;
	}

EXPORT_C TInt CHarvesterMediaIdUtil::GetDriveLetter( TUint32 aMediaId, TChar& aChar )
	{
	for ( TInt i = 0; i < KMaxDrives; i++ )
		{
		if ( iMediaId[i] == aMediaId )
			{
			TInt err = RFs::DriveToChar( i, aChar );
			return err;
			}
		}
	return KErrNotFound;
	}

EXPORT_C void CHarvesterMediaIdUtil::RemoveMediaId( TUint32 aMediaId )
	{
	for ( TInt i = 0; i < KMaxDrives; i++ )
		{
		if ( iMediaId[i] == aMediaId )
			{
			iMediaId[i] = 0;
			break;
			}
		}
	}

EXPORT_C CHarvesterMediaIdUtil& RMediaIdUtil::GetInstanceL()
	{
    TMediaIdUtilInfo* data =
        static_cast<TMediaIdUtilInfo*>( 
        		UserSvr::DllTls( KHarvesterMediaIdTLSKey ) );

        CHarvesterMediaIdUtil* instance = NULL;

    	if ( data )
            {
            instance = data->iMediaIdUtil;
            data->iRefCount++;
            }
    	else
    		{
            instance = CHarvesterMediaIdUtil::NewL();
            CleanupStack::PushL(instance);
            TMediaIdUtilInfo* tmp = new (ELeave) TMediaIdUtilInfo;
            tmp->iMediaIdUtil = instance;
            tmp->iRefCount = 1;
            UserSvr::DllSetTls( KHarvesterMediaIdTLSKey, tmp );
            	
            CleanupStack::Pop(instance);
            }
    	
    	return *instance;
	}

EXPORT_C void RMediaIdUtil::ReleaseInstance()
	{
	TMediaIdUtilInfo* data = static_cast<TMediaIdUtilInfo*> (UserSvr::DllTls(
			KHarvesterMediaIdTLSKey));
	if (data)
		{
		data->iRefCount--;
		if (data->iRefCount <= 0)
			{
			delete data->iMediaIdUtil;
			data->iMediaIdUtil = NULL;

			delete data;
			data = NULL;
			UserSvr::DllFreeTls(KHarvesterMediaIdTLSKey);
			}
		}
	}
