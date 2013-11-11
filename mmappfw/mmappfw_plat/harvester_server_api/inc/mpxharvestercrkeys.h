/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPX Harvester CenRep keys.
*
*/


#ifndef MPXHARVESTERCRKEYS_H
#define MPXHARVESTERCRKEYS_H


// MPX Harvester Features Uid
const TUid KCRUIDMpxHarvesterFeatures = { 0x101FFCD2 };

// List of scan paths for the harvester
const TInt KMpxHarvesterScanPathKey = 1;

// List of blocked paths for the harvester
const TInt KMpxHarvesterBlockPathKey = 2;

// List of container types
const TInt KMpxHarvesterContainerKey = 3;

// List of folders to monitor for file changes
const TInt KMpxHarvesterAutoScanDirectoryKey = 4;

// Disable the podcasting feature in harvester 1=disabled 0=enabled
const TInt KMpxHarvesterDisablePodcasting = 5;

// Enable the Ram Drive feature.
const TInt KMpxHarvesterEnableRamDisk = 6;

// Minimum disk space in Mb on Ram Drive to allow copying of Harvester databases to the Ram Disk.
const TInt KMpxHarvesterMaxAllowedRamDiskSpace = 7;

#endif  // MPXHARVESTERCRKEYS_H

// End of file
