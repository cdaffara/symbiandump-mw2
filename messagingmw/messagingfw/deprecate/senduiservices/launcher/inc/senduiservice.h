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
* Description:   Communication class between SendUi and SendUiLauncher
*
*/




#ifndef C_CSENDUISERVICE_H
#define C_CSENDUISERVICE_H


#include <AknServerApp.h>

#include "senduilauncher.hrh"
// forward declarations
class RDesReadStream;
class CSendUiLauncherAppUi;
class CMessageData;


/**
 *  SendUi launch service implementation class
 *
 *  ?more_complete_description
 *
 *  @since S60 v.3.2
 */
NONSHARABLE_CLASS (CSendUiService) : public CAknAppServiceBase
    {

public:

    /**  IPC message slots */
    enum TIpcSlots
            {
            ESlot1 = 0,
            ESlot2,
            ESlot3
            };

    static CSendUiService* NewL();

    virtual ~CSendUiService();

// from base class CAknAppServiceBase

    /**
     * From CAknAppServiceBase.
     * Server side service implementation
     *
     * @since S60 v.3.2
     * @param RMessage2& IPC message
     */
    void ServiceL( const RMessage2& aMessage );
        
private:

    void ConstructL();

private: // data

    /**
     * Temporary storage for the incoming message data
     * Own.
     */
     TInt iTransferredBytes;
    
    /**
     * Temporary storage for the incoming message data
     * Own.
     */
    HBufC8* iTransferBuffer;
    
    /**
     * Message data container
     * Own.
     */
    CMessageData* iMessageData;

    /**
     * Pointer to application ui.
     * Not own.
     */
    CSendUiLauncherAppUi* iAppUi;
    
    /**
     * Readstream
     * Own.
     */
     RDesReadStream* iReadStream;
    };

#endif // C_CSENDUISERVICE_H
