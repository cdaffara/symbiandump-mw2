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
* Description:  Harvester Central Repository utility class
*
*/

#ifndef HARVESTERCENREPUTIL_H
#define HARVESTERCENREPUTIL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>
#include "mdsfileserverpluginclient.h"

struct TScanItem
{
	TUint32 iPreinstalled;
	HBufC* iPath;
	~TScanItem() { delete iPath; iPath = NULL;}
};
// CLASS DECLARATION

/**
*  CHarvesterCenRepUtil
* 
*/
NONSHARABLE_CLASS( CHarvesterCenRepUtil ) : public CBase
{
public: // Constructors and destructor
	IMPORT_C ~CHarvesterCenRepUtil();
	IMPORT_C static CHarvesterCenRepUtil* NewL();
	IMPORT_C static CHarvesterCenRepUtil* NewLC();

public: // new functions
    IMPORT_C void AddIgnorePathsToFspL( const TDesC& aDrivePrefix );
    IMPORT_C void AddIgnorePathsToFspL( const TDesC& aDrivePrefix, const TDesC& aFilterPrefix );
    IMPORT_C void RemoveIgnorePathsFromFspL( const TDesC& aDrivePrefix );
    IMPORT_C RMdsFSPEngine& FspEngine();
    
    /**
     * Append ignore paths to given array
     * 
     * @param aIgnorePaths ignore paths
     */
    IMPORT_C static void GetIgnorePathL( RPointerArray<HBufC>& aIgnorePaths );
    
    /**
     * Append first boot scan paths to given array
     * 
     * @param aPaths scan paths
     */
    IMPORT_C static void GetScanItemsL( RPointerArray<TScanItem>& aItems );
    
    /**
     * Append ignored first boot scan paths to given array
     * 
     * @param aIngorePaths ignored scan paths
     */
    IMPORT_C static void GetIgnoredScanPathsL( RPointerArray<HBufC>& aIngorePaths );

    /**
     * Append partial restore scan paths to given array
     * 
     * @param aPaths scan paths
     */
    IMPORT_C static void GetPartialRestorePathsL( RPointerArray<TScanItem>& aPaths );
    
    /**
     * Append ignored partial restore scan paths to given array
     * 
     * @param aIngorePaths ignored scan paths
     */
    IMPORT_C static void GetIgnoredPartialRestorePathsL( RPointerArray<HBufC>& aIngorePaths );
    
    /**
     * Check if ROM scanning is enabled
     * 
     * @param aEnabled is rom scan enabled
     */
    IMPORT_C static void IsRomScanEnabledL( TBool& aEnabled );
    
    /**
     * Check if automatic thumbnail generation is enabled
     * 
     * @param aEnabled is rom scan enabled
     */
    IMPORT_C static void IsThumbnailDaemonEnabledL( TBool& aEnabled );

    /**
     * Check paths for preinstalled content
     * 
     * @param aEnabled is rom scan enabled
     */
    IMPORT_C static void GetAlwaysScanOnBootPathsL( RPointerArray<TScanItem>& aPaths );
    
private:
	CHarvesterCenRepUtil();
	void ConstructL();

	static void GetPathsL( TUint32 aPartialKey, RPointerArray<HBufC>& aPaths );
	
	static void GetItemsL( TUint32 aPartialKey, RPointerArray<TScanItem>& aPaths );
	
	void AddOrRemoveIgnorePathsL( const TDesC& aDrivePrefix, TBool aAdd, const TDesC& aFilterPrefix );
	
private: // member variables
	RFs iFs;
	RMdsFSPEngine iFspEngine;
};

#endif // HARVESTERCENREPUTIL_H

