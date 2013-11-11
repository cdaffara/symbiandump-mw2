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



#ifndef MPXINCREMENTALOPENDEFS_H
#define MPXINCREMENTALOPENDEFS_H

#include <mpxattribute.h>

/**
 * Content ID for opening incrementally
 */
const TInt KMPXCommandIdIncrementalOpenL = 0x1028297B;

/**
 * Enumeration for ascending or decending
 */
enum TReadDirection
    {
    EReadUnknown = -1,
    EReadDescending = 0,
    EReadAscending = 1
    };

/**
 *  Attributes supported for KMPXCommandIdIncrementalOpenL
 */    

/**
 * The collection path to open (CMPXCollectionPath)
 */    
const TMPXAttributeData KMPXCollectionCommandIdIncOpenLPath = {
                        KMPXCommandIdIncrementalOpenL, 0x01};

/**
 * Offset to start reading from (TInt)
 */ 
const TMPXAttributeData KMPXCollectionCommandIdIncOpenLOffset = {
                        KMPXCommandIdIncrementalOpenL, 0x02};

/**
 * Number of items to read (TInt)
 */                         
const TMPXAttributeData KMPXCollectionCommandIdIncOpenLNumItems = {
                        KMPXCommandIdIncrementalOpenL, 0x04};

/**
 * Read ascending or decending (EReadDescending for dsc, 
 *                              EReadAscending for asc)
 * 
 * Can use ascending read if a key item can be provided for faster access
 */                         
const TMPXAttributeData KMPXCollectionCommandIdIncOpenLAscDsc = {
                        KMPXCommandIdIncrementalOpenL, 0x08};

/**
 * Value of some key (TDesC) to accelerate open db
 *
 * For example, KMPXMediaGeneralTitle can be used
 * as a reference for plugins to quickly jump to the 
 * last retrieved item
 */                         
const TMPXAttributeData KMPXCollectionCommandIdIncOpenLKeyItem = {
                        KMPXCommandIdIncrementalOpenL, 0x10};
                        
#endif // MPXINCREMENTALOPENDEFS_H
