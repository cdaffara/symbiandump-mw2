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
* Description:  Some generic utilities.*
*/


#include "mdscommoninternal.h"
#include "mdeharvestersession.h"
#include <e32base.h>

inline TUint32 FSUtil::MediaID( const RFs& aFs, TInt aDrive )
    {
    TVolumeInfo vi;
    
    TInt err = aFs.Volume(vi, aDrive);
    if(err == KErrNone)
        {
        return vi.iUniqueID;
        }
        
    return 0;
    }

inline TUint32 FSUtil::MediaID( const RFs& aFs, const TDesC& aUri )
    {
    TInt drive = DriveNumber(aUri);
    if(drive == -1)
        {
        return 0;
        }
        
    return MediaID(aFs, drive);
    }

inline TUint32 FSUtil::GetPreviousMediaIDL( const CMdEHarvesterSession* aMdeSession,
											TChar aDrive )
	{
	RArray<TMdEMediaInfo> presentMedias;
	CleanupClosePushL( presentMedias );
	CMdEHarvesterSession* sess = (CMdEHarvesterSession*) aMdeSession;
	sess->GetPresentMediasL( presentMedias );

    TUint32 mediaid = 0;
    for( TInt i = presentMedias.Count() - 1; i >=0; i-- )
    	{
    	TMdEMediaInfo info = presentMedias[i];
    	if( info.iDrive == aDrive )
    		{
    		mediaid = info.iMediaId;
    		break;
    		}
    	}
    CleanupStack::PopAndDestroy( &presentMedias );
    
    return mediaid;
	}

inline TInt FSUtil::DriveNumber( const TDesC& aUri )
    {
    TChar ch = aUri[0];
    TInt drive = 0;
    TInt err = RFs::CharToDrive(ch, drive);
    if(err == KErrNone)
        {
        return drive;
        }
        
    return -1;
    }

