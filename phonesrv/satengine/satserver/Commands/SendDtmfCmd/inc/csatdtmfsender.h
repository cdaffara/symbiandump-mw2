/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  DTMF sender for SendDtmf command
*
*/



#ifndef CSATDTMFSENDER_H
#define CSATDTMFSENDER_H

#include <e32base.h>

// CLASS DECLARATION
class CSendDtmfHandler;
class MSatMultiModeApi;

/**
 *  DTMF sender for SendDtmf command.
 *
 *  @lib SendDtmfCmd.lib
 *  @since S60 3.2
 */
class CSatDtmfSender : public CActive
    {

public:  // Constructors and destructor

    /**
     * C++ default constructor.
     * @param aDtmfHandler Observes completion of DTMF sending
     * @param aPhone API for sending DTMF strings.
     */
    CSatDtmfSender( CSendDtmfHandler& aDtmfHandler, 
                    MSatMultiModeApi& aPhone );

    /**
     * Destructor.
     */
    virtual ~CSatDtmfSender();

    /**
     * Transmit DTMF tones across all the current active voice calls.
     * @since S60 3.2
     * @param aTones The string of DTMF characters.
     */
    void SendToneString( const TDesC& aTones );

protected:

    /**
     * From CActive.
     * Called when request status given SetActiveAndWait completes
     */
    void RunL();

    /**
     * From CActive.
     * Called when request is cancelled
     */
    void DoCancel();

private:    // Data

    /**
     * Observer of the DTMF sending.
     */
    CSendDtmfHandler& iDtmfHandler;

    /**
     * API used for sending DTMF string to adaptation.
     */
    MSatMultiModeApi& iPhone;

    };

#endif      // CSATDTMFSENDER_H

