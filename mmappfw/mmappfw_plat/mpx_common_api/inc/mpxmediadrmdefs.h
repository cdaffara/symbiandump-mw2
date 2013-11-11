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
* Description:  Definitions for DRM media type
*
*/



#ifndef CMPXMEDIADRMDEFS_H
#define CMPXMEDIADRMDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
*  Content ID identifying DRM category of content provided
*  in the media object and associated attributes. 
*/
const TInt KMPXMediaIdDrm=0x101FFC45;

enum TMPXMediaDrmType
    {
    EMPXDrmTypeOMA,
    EMPXDrmTypeWMA
    };
    
enum TMPXMediaDrmRightsStatus
    {
    EMPXDrmRightsFull,
    EMPXDrmRightsMissing,
    EMPXDrmRightsRestricted,
    EMPXDrmRightsExpired,
    EMPXDrmRightsPreview
    };
   
enum TMPXMediaDrmRightsType
    {
    EMPXDrmRightsTypeNoRestriction,
    EMPXDrmRightsTypeCount,
    EMPXDrmRightsTypeTime,
    EMPXDrmRightsTypeInterval,
    EMPXDrmRightsTypeTimeCount,
    EMPXDrmRightsTypeAccTime
    };

/**
*  Attributes supported for KMPXMediaIdDrm
*/    
const TMPXAttributeData KMPXMediaDrmType = {KMPXMediaIdDrm, 0x0001};   // TInt
const TMPXAttributeData KMPXMediaDrmProtected = {KMPXMediaIdDrm, 0x0002};   // TBool
const TMPXAttributeData KMPXMediaDrmRightsStatus = {KMPXMediaIdDrm, 0x0004};   // TInt
const TMPXAttributeData KMPXMediaDrmRightsType = {KMPXMediaIdDrm, 0x0008};   // TInt
const TMPXAttributeData KMPXMediaDrmCount = {KMPXMediaIdDrm, 0x0010};   // TInt
const TMPXAttributeData KMPXMediaDrmStartTime = {KMPXMediaIdDrm, 0x0020};   // TInt64
const TMPXAttributeData KMPXMediaDrmEndTime = {KMPXMediaIdDrm, 0x0040};   // TInt64
const TMPXAttributeData KMPXMediaDrmInterval = {KMPXMediaIdDrm, 0x0080};   // TTimeIntervalSeconds
const TMPXAttributeData KMPXMediaDrmIntervalStartTime = {KMPXMediaIdDrm, 0x0100};   // TInt64
const TMPXAttributeData KMPXMediaDrmAccumulatedTime = {KMPXMediaIdDrm, 0x0200};   // TTimeIntervalSeconds
const TMPXAttributeData KMPXMediaDrmSendingAllowed = {KMPXMediaIdDrm, 0x0400};   // TBool
const TMPXAttributeData KMPXMediaDrmCanSetAutomated = {KMPXMediaIdDrm, 0x0800};   // TBool
const TMPXAttributeData KMPXMediaDrmHasInfoUrl = {KMPXMediaIdDrm, 0x1000};   // TBool
const TMPXAttributeData KMPXMediaDrmHasPreviewUrl = {KMPXMediaIdDrm, 0x2000};   // TBool
const TMPXAttributeData KMPXMediaDrmAboutToExpire = {KMPXMediaIdDrm, 0x4000};   // TBool
const TMPXAttributeData KMPXMediaDrmAll = {KMPXMediaIdDrm, 0xffffffff};

/** 
*  Attributes supported for KMPXMediaIdDrm.
*
*  @deprecated Use the above consts
*/
enum 
    {
    EMPXMediaDrmType                = 0x0001,   // TInt
    EMPXMediaDrmProtected           = 0x0002,   // TBool
    EMPXMediaDrmRightsStatus        = 0x0004,   // TInt
    EMPXMediaDrmRightsType          = 0x0008,   // TInt
    EMPXMediaDrmCount               = 0x0010,   // TInt
    EMPXMediaDrmStartTime           = 0x0020,   // TInt64
    EMPXMediaDrmEndTime             = 0x0040,   // TInt64
    EMPXMediaDrmInterval            = 0x0080,   // TTimeIntervalSeconds
    EMPXMediaDrmIntervalStartTime   = 0x0100,   // TInt64
    EMPXMediaDrmAccumulatedTime     = 0x0200,   // TTimeIntervalSeconds
    EMPXMediaDrmSendingAllowed      = 0x0400,   // TBool
    EMPXMediaDrmCanSetAutomated     = 0x0800,   // TBool
    EMPXMediaDrmHasInfoUrl          = 0x1000,   // TBool
    EMPXMediaDrmHasPreviewUrl       = 0x2000,   // TBool
    EMPXMediaDrmAboutToExpire       = 0x4000,   // TBool
    EMPXMediaDrmAll                 = 0xffffffff
    };


#endif // CMPXMEDIADRMDEFS_H
