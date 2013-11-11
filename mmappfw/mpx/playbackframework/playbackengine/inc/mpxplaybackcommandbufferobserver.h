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
* Description:  Command buffer bserver
*
*/


#ifndef MMPXPLAYBAKCMDBUFFEROBSERVER_H
#define MMPXPLAYBAKCMDBUFFEROBSERVER_H

#include <mpxplaybackframeworkdefs.h>


enum TMPXPlaybackSkipEvent
    {
    EPbsSkipEventEnd,
    EPbsSkipEventNext,
    EPbsSkipEventPrevious
    };

/**
*  Media key event
*/
NONSHARABLE_CLASS(TMPXPlaybackCmdInfo)
    {
public:
    TInt iTrackOffset;
    TMPXPlaybackState iState;
    TInt iPos;
    TInt iNavKeyBuffered;
    };


/**
*  Media key observer
*/
NONSHARABLE_CLASS(MMPXPlaybackCmdBufferObserver)
    {
public:

    /**
    *  Handle command from buffering
    *
    *  @param aEvent comand info
    */
    virtual void HandleCmdBufferEventL(const TMPXPlaybackCmdInfo& aEvent)=0;

    /**
    * Handle command skip event from key buffering
    *
    * @param aSkipEvent Skip event
    */
    virtual void HandleCmdBufferSkipEventL(const TMPXPlaybackSkipEvent aSkipEvent)=0;
    };


#endif // MMPXPLAYBAKCMDBUFFEROBSERVER_H
