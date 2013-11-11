/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Definitions for plugin update messages
*
*/


#ifndef MPXMESSAGEPLUGINUPDATEDEFS_H
#define MPXMESSAGEPLUGINUPDATEDEFS_H

#include <e32base.h>

/**
* Content ID identifying general plugin attributes
*/
const TInt KMPXMessagePluginGeneral = 0x10282990;

/**
*  TUid - plugin UID
*/
const TMPXAttributeData KMPXAttrPluginId = {KMPXMessagePluginGeneral, 0x01};

/**
*  TBool - ETrue if the plugin is loaded
*/
const TMPXAttributeData KMPXAttrPluginLoaded = {KMPXMessagePluginGeneral, 0x02};

/**
*  TInt - plugin version
*/
const TMPXAttributeData KMPXAttrPluginVersion = {KMPXMessagePluginGeneral, 0x04};

/**
* Content ID identifying plugin addition messages
* Includes the KMPXAttrPluginId, KMPXAttrPluginLoaded and KMPXAttrPluginVersion attributes
*/
const TInt KMPXMessagePluginAdd = 0x10282991;

/**
* Content ID identifying update start messages
* Includes the KMPXAttrPluginId, KMPXAttrPluginLoaded and KMPXAttrPluginVersion attributes
*/
const TInt KMPXMessagePluginUpdateStart = 0x10282992;

/**
* Content ID identifying update end messages
* Includes the KMPXAttrPluginId, KMPXAttrPluginLoaded and KMPXAttrPluginVersion attributes
*/
const TInt KMPXMessagePluginUpdateEnd = 0x10282993;

/**
* Content ID identifying plugin removal messages
* Includes the KMPXAttrPluginId, KMPXAttrPluginLoaded and KMPXAttrPluginVersion attributes
*/
const TInt KMPXMessagePluginRemove = 0x10282994;

#endif // MPXMESSAGEPROGRESSDEFS_H
