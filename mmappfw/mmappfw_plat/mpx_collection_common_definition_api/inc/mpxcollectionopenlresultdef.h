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
* Description:  Definitions for incremental open command type
*
*/



#ifndef MPXCOLLECTIONOPENLRESULTDEFS_H
#define MPXCOLLECTIONOPENLRESULTDEFS_H

#include <e32base.h>
#include <mpxattribute.h>

/**
 * Structure to hold the offset and size of each data block
 */
NONSHARABLE_STRUCT( TMPXOpenDataBlock )
    {
    TInt iOffset;
    TInt iSize;
    };

/**
 * Content ID for opening incrementally
 */
const TInt KMPXCollectionOpenLResults = 0x1028297E;

/**
 *  Attributes supported for KMPXCollectionOpenLResults
 */    

/**
 * TMPXOpenDataBlock structure to hold what block was fetched
 * contains the offset and the size of the datablock
 *
 * Used for the collection generic cache to merge in datasets
 * 
 */
const TMPXAttributeData KMPXCollectionOpenLResultRange = {
                        KMPXCollectionOpenLResults, 0x01};

/**
 * RArray<TMPXOpenDataBlock> serialized to a CBufBase* descriptor 
 * Migrate to RMPXArray<> in the future
 * 
 * This information will be provided by the generic cache
 *
 * Contains a cumulative list of all fetched blocks
 * Client should merge blocks together if overlapping results are fetched
 */
const TMPXAttributeData KMPXCollectionOpenLAllResultRange = { 
                        KMPXCollectionOpenLResults, 0x02};

/**
 * TBool supports KMPXCommandIdIncrementalOpenL commands
 * The collection plugin will set this to ETrue if a path supports
 * Incremental Opening. 
 *
 * The collection open utility will use this flag to determine
 * if incremental open is supported
 * ETrue if supported
 * EFalse or not set is not supported
 */
const TMPXAttributeData KMPXCollectionOpenLSupportsIncremental = { 
                        KMPXCollectionOpenLResults, 0x04};
                        
#endif // MPXCOLLECTIONOPENLRESULTDEFS_H
