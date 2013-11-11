/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/

#ifndef XARADIOITFEXT_H
#define XARADIOITFEXT_H

// Currently defined in OpenMAXAL.h for RADIO:
//#define XA_RADIO_EVENT_ANTENNA_STATUS_CHANGED       ((XAuint32) 0x00000001)
//#define XA_RADIO_EVENT_FREQUENCY_CHANGED            ((XAuint32) 0x00000002)
//#define XA_RADIO_EVENT_FREQUENCY_RANGE_CHANGED      ((XAuint32) 0x00000003)
//#define XA_RADIO_EVENT_PRESET_CHANGED               ((XAuint32) 0x00000004)
//#define XA_RADIO_EVENT_SEEK_COMPLETED               ((XAuint32) 0x00000005)

// ADDED:
#define XA_RADIO_EVENT_STEREO_STATUS_CHANGED          ((XAuint32) 0x00000006)
#define XA_RADIO_EVENT_SIGNAL_STRENGTH_CHANGED 				((XAuint32) 0x00000007)

#endif /* XARADIOITFEXT_H */
