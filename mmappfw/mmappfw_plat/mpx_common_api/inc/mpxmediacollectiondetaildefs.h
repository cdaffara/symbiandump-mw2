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
* Description:  Content description for Collection details
*
*/


#ifndef MPX_MEDIA_COLLECTION_DETAILS_H
#define MPX_MEDIA_COLLECTION_DETAILS_H

#include <mpxattribute.h>

/**
* Content ID for getting collection details
*/

const TInt KMPXMediaIdCollectionDetails=0x101FFC47;

/**
*  Attributes supported for KMPXMediaCollectionDetails
*/    
const TMPXAttributeData KMPXMediaColDetailNumberOfItems = {KMPXMediaIdCollectionDetails, 0x01}; // TInt
const TMPXAttributeData KMPXMediaColDetailDuration = {KMPXMediaIdCollectionDetails, 0x02}; // TInt
const TMPXAttributeData KMPXMediaColDetailTotalSize = {KMPXMediaIdCollectionDetails, 0x04}; // TInt
const TMPXAttributeData KMPXMediaColDetailLastRefreshed = {KMPXMediaIdCollectionDetails, 0x08}; // TInt64
const TMPXAttributeData KMPXMediaColDetailDBCreated = {KMPXMediaIdCollectionDetails, 0x10}; // TBool
const TMPXAttributeData KMPXMediaColDetailDBCorrupted = {KMPXMediaIdCollectionDetails, 0x20}; // TInt
const TMPXAttributeData KMPXMediaColDetailSpaceId = {KMPXMediaIdCollectionDetails, 0x40}; 
// TBool. Can this plugin supply MediaL() metadata?
const TMPXAttributeData KMPXMediaColDetailMediaNotAvailable= {KMPXMediaIdCollectionDetails, 0x80};
const TMPXAttributeData KMPXMediaColDetailAll = {KMPXMediaIdCollectionDetails, 0xffffffff};

/** 
*  Attributes supported for KMPXMediaCollectionDetails.
*
*  @deprecated Use the above consts
*/        
enum 
    {
    EMPXMediaColDetailNumberOfItems=0x01, // TInt
    EMPXMediaColDetailDuration=0x02, // TInt
    EMPXMediaColTotalSize=0x04,  // TInt
    EMPXMediaLastRefreshed=0x08, // TInt64
    EMPXMediaColDetailDBCreated=0x10, // TBool
    EMPXMediaColDetailDBCorrupted=0x20, // TBool
    EMPXMediaColDetailIdSpaceId=0x40,
    EMPXMediaColDetailIdMediaNotAvailable=0x80, // TBool. Can this plugin supply MediaL() metadata?
    EMPXMediaColDetailsAll=0xffffffff
    };   
    
#endif // MPX_MEDIA_COLLECTION_DETAILS_H
