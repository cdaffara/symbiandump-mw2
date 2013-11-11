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
 * Description:  Mcc Internal definitions
 *
 */

#ifndef INTERNALDEF_H
#define INTERNALDEF_H

// CONSTANTS
// When the remote end sends us SID/CN frames, then we can update the SID/CN
// parameters programmatically. Otherwise, we should not try to "emulate"
// SID/CN frames as this can lead to degradation of audio quality. Instead,
// we will send NULL/NO_DATA into the decoder.

// AMR-NB SID FT value is 8 and is located in the upper 4 bits
// See 3GPP TS 26.101
const TUint8 KAmrSidMode = 8;

// AMR-NB mode mask
const TUint8 KAmrModeMask = 0x78;

// AMR-NB SID Update frame interval
const TInt KAmrNbSidUpdateInterval = 8;

// Minimum required data length for DTX decision
const TInt KMinDataLenForDtx = 1;

// AMR NO_DATA frame needs one byte
// See 3GPP TS 26.101 and RFC 2367
//  ________________
// |0|1|1|1|1|1|0|0|
// |P|  FT   |Q|P|P|
//
//  P = PADDING
//  FT = 15 = NO_DATA
//  Q = QUALITY
const TUint8 KAmrNoDataFrame = 0x7C;

// Length of the header and AMR NO_DATA frames
const TInt KNoDataLength = 1;

// AMR mode solving needs 3 bit shift
const TInt KModeShiftBits = 3;

// VoIP codec header bytes can be either audio, comfort noise or unvoiced
// non-sid (null/no data) frame. Unvoiced non-sid frame is detected when
// the header byte does not indicate audio or comfort noise.
//const TUint8 KVoIPAudioFrame = 1;
const TUint8 KVoIPCNFrame = 2;

const TUint KJBAdaptUpdateIval = 250;
const TUint KJBMillisecondsToMicroseconds = 1000;
const TUint KJBEmptyBufferDelay = 2;

const TInt KDefaultSampleRateInkHz = 8;
const TInt KLowBufferLimit = 2;
const TInt KOverflowAlarmLimit = 20;

// Codec frame sizes
const TInt KAMRNBFrameSize = 32;
const TInt KILBCFrameSize = 52;
const TInt KG729FrameSize = 24; // 10 octets + 2 for header

// Max possible sequence number of RTP packet with standard RTP header.
const TUint KMaxSeqNumber = 0xFFFF; // 16-bit value

// Codec frame times
const TInt KAMRNBFrameTime = 20;

// VoIP codecs have a two byte header
const TInt KVoIPHeaderLength = 2;

#endif //INTERNALDEF_H

// End of file
