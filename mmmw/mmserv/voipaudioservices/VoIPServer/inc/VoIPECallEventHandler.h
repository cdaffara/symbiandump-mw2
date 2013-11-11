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
 * Description: VoIP Auddio Services
 *
 */

#ifndef VOIPECALLENTHANDLER_H
#define VOIPECALLENTHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include "VoIPServer.h"

// CLASS DECLARATION
class CVoIPECallEventHandler : public CActive
    {
public:
    /**
     * Two-phased constructor.
     */
    static CVoIPECallEventHandler* NewL(CVoIPAudioServer* aServer);

    /**
     * Destructor.
     */
    virtual ~CVoIPECallEventHandler();

private:
    /**
     * From CActive
     * Cancel outstanding request
     */
    void DoCancel();

    /**
     * From CActive
     * Implementation of CActive::RunL.
     * Called when server request has completed.
     **/
    void RunL();

private:

    /**
     * C++ default constructor.
     */
    CVoIPECallEventHandler(CVoIPAudioServer* aServer);

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    CVoIPAudioServer* iVasServer;
    RProperty iProperty;
    };

#endif  // VOIPECALLENTHANDLER_H

// End of File
