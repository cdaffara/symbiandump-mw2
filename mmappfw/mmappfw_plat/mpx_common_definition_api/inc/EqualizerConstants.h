/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This file contains declarations for public constants for
*                Equalizer
*
*/

#ifndef EQUALIZERCONSTANTS_H
#define EQUALIZERCONSTANTS_H

//  INCLUDES

#include <e32std.h>

// CONSTANTS
// Used by client application for activating the preset "None"
const TInt KEqualizerPresetNone = -1;

// Used as the maximum length of a ListBox model entry (like "0\tNone")
const TInt KListBoxEntryMaxLength = 64; 

// DATA TYPES
// Used to store a preset name or ListBox model entry
typedef TBuf<KListBoxEntryMaxLength> TPresetName;

#endif      // EQUALIZERCONSTANTS_H

// End of File
