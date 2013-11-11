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
* Description:   Clientside class for launching SendUi from SendUiLauncher 
*               (server application)
*
*/




#ifndef R_SENDUISERVICECLIENT_H
#define R_SENDUISERVICECLIENT_H

#include <aknappui.h>
#include <AknServerApp.h>
#include <SendUiConsts.h>

class CMessageData;

const TInt KMessageBufferSize = 4 * sizeof(TUint32);

/**
 *  SendUI Server Application API implentation, client side
 *
 *  @lib sendui.lib
 *  @since S60 S60 v3.2
 */
NONSHARABLE_CLASS (RSendUiServiceClient) : public RAknAppServiceBase
    {

public:

    /**
     * Requests SendUi launch on server application side
     *
     * @since S60 v3.2
     * @param aServiceUid Sending service Uid
     * @param aLaunchEmbedded ETrue, if editor is to be launched embedded
     * @param CMessageData Message to be sent
     */
     void LaunchSendUiL( 
        TUid& aServiceUid,
        TUid& aServiceProviderUid,
        TBool& aLaunchEmbedded, 
        const CMessageData* aMessageData );

// from base class RAknAppServiceBase

    /**
     * From RAknAppServiceBase.
     * Return Service identifier
     *
     * @since S60 v3.2
     * @return Service's identification
     */
    TUid ServiceUid() const;

    virtual ~RSendUiServiceClient();

    };

#endif // R_SENDUISERVICECLIENT_H
