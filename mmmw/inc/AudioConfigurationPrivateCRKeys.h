/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Audio Configuration internal Central Repository keys for
*                specifying buffering mode during progressive download.
*
*/


#ifndef AUDIOCONFIGURATIONINTERNALCRKEYS_H
#define AUDIOCONFIGURATIONINTERNALCRKEYS_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

/**
* Audio Configuration CR UID.
*/
const TUid KCRUidAudioConfiguration = {0x10207B74};

/**
* This key is required for the Enhanced Media Client to determine the "buffering mode"
* when the source runs out of data. When it happens, the source will start sending
* buffer to underlying framework when buffering condition is met.
*
* Possible integer values:
* 0 = "None". No buffering is done, as and when data is downloaded, it will be given
*     to underlying framework.
* 1 = "Fixed Size". Fixed size buffering. The source waits until the data accumulated
*     reaches the limit (in bytes) specified by KPDLPrimaryBufferingAmount.
* 2 = "Fixed duration". Fixed duration buffering. The source waits until the data
*     accumulated reaches the limit (calculated dynamically). When playback begins,
*     it is guarenteed to continue playback for KPDLPrimaryBufferingAmount duration
*     before it may underflow again. (Note: The playback may stop due to other reasons
*     but insufficient data).
* 3 = "Dynamic duration". Dynamic duration buffering. The source waits until the data
*     accumulated reaches the limit (in bytes) specified by KPDLPrimaryBufferingAmount
*     plus a constant. The limit determined is the best possible approximation.
*     When the playback begins it is may never underflow until the clip is played
*     back completely. This assuming that data transfer rate and the bit rate of the
*     clip remains constant until the download is complete.
      (Note: The playback may stop due to other reasons but insufficient data).
*/
const TUint32 KPDLPrimaryBufferingType  = 0x00000001;

/**
* Size/Duration value associated with primary buffering type.
*/
const TUint32 KPDLPrimaryBufferingAmount = 0x00000002;

/**
* This key is used if primary configuration cannot be used.
* This may happen when the component does not know all the variables needed to
* to determine the buffering limit.
*
* If the buffering type specified is still invalid at runtime(for e.g. the customer may have
* configured both primary and secondary to fixed duration and the bit rate may
* not be known in the begining of playback). The component will default to no
* buffering and will send data to framework as and when it is received. The playback
* may include breaks in this case. To avoid this, this should be configured to fixed
* size. 
*/
const TUint32 KPDLSecondaryBufferingType = 0x00000003;

/**
* Size/Duration value associated with secondary buffering type.
*/
const TUint32 KPDLSecondaryBufferingAmount = 0x00000004;

/**
* Initial Percentage of File download before starting playback.
*/
const TUint32 KPDLInitialBufferingPercentage = 0x00000005;


#endif      // AUDIOCONFIGURATIONINTERNALCRKEYS_H

// End of file
