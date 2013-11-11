/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  mpxplaybackutilitytestdefs header for STIF Test Framework TestScripter.
*
*/

#ifndef MPXPLAYBACKUTILITYTESTDEFS_H
#define MPXPLAYBACKUTILITYTESTDEFS_H

const TInt KGeneralPlaybackMsgOffset = 0;
const TInt KVideoPlaybackMsgOffset = 100;
const TInt KStifPlaybackMsgOffset = 200;




const TInt KMPXMessageStif = 0x20011397;

//
//  TInt for Command Id
//
const TMPXAttributeData KMPXStifPlaybackCommand = { KMPXMessageStif, 0x01 };


enum TMPXStifCommand
{
    EPbStifPlayComplete,
    EPbStifSeekForward,
    EPbStifSeekBackward

};


#endif      // MPXPLAYBACKUTILITYTESTDEFS_H

// End of File
