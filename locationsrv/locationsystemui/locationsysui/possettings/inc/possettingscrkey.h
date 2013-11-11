/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Central Repository Key definition for ordering of the
*								positioning settings plugins displayed in the advaced
*								settings view
*
*/

#ifndef POSSETTINGSCRKEY_H
#define POSSETTINGSCRKEY_H

#include <e32base.h>

/*
 * UID of the Positioning Settings Central Repository Key. 
 * This key defines the position of the positioning settings plugins that are shown 
 * in the advanced settings view of Positioning.
 */
const TUid KCRUidPositioningSettings = {0x2002E685};

/*
 * Key for listing the positioning settings plugins in the advacned view of Positioning. 
 * This key is a Read only key and the default values for the Keys is configured by the device 
 * manufacturer while flashing the device. They cannot be dynamically configured. 
 * The keys contains a string which contains a list of Unique Identifers for the Positioning settings 
 * plugins in the order in which the device manufacturer wants these plugins to be displayed.
 * The maximum length of this string is 1024 characters
 * The format of this Key is defined as
 * | Count | Length(1) | Dllname(1) | Length(2) | Dllname(2) | ... | Length( Count ) | Dllname( Count ) |
 * where,
 * Count ( 4 digits ) - The number of positioning Settings plugins which are predefined to be displayed in 
 * the Advanced Settings view, in the Decimal format.
 * Length(n) ( 4 digits ) - Length of the plugin's dll name in the string format. This field contains the length
 * in the in the Decimal format. 
 * String(n) - Dll name of the positioning settings plugins encoded as a string.
 */
const TUint32 KPositioningSettingsPlugins = 0x00000001;

/*
 * Key for storing old state of background positioning in main view.
 * This key is a Read/Write key and the default value for the key is one [i.e. enabled]
 * The key contains integer which can be either 0 [disabled] or 1 [enabled]
 * The key value is modified only when user marks/unmarks background positioning checkbox in the main view
 * The key value is NOT modified when background positioning checkbox is removed/inserted 
 */
const TUint32 KPositioningSettingsBackgroundServicesStatus = 0x00000002;

#endif //POSSETTINGSCRKEY_H
