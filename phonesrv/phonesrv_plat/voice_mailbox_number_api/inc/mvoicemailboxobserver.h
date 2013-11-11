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
* Description:  Notifies when mailbox numbers are changed
*  Interface   : External, MVoiceMailboxObserver
*
*/

#ifndef M_VOICEMAILBOXOBSERVER_H
#define M_VOICEMAILBOXOBSERVER_H

class CVoiceMailboxEntry;

// INCLUDES
/**
 *  Notifier for vmbx number/address changes
 *
 *  @lib vmbxengine.lib
 *
 */
class MVoiceMailboxObserver
    {
public:

   /**
    * Observer callback function which is called when
    * changes to voice mailbox entries occur.
    *
    * @param aVmbxEntry The new vmbx entry.
    */
    virtual void HandleNotifyL( const CVoiceMailboxEntry& aVmbxEntry ) = 0;

    };

#endif // M_VOICEMAILBOXOBSERVER_H
