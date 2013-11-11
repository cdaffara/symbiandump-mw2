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
* Description:  CCP CS call class
*
*/


#ifndef MCCPCSOBSERVER_H
#define MCCPCSOBSERVER_H

// INCLUDES
#include <e32base.h>
#include "mccpobserver.h"

// FORWARD DECLARATIONS
class MCCPCall;
class MCCPConferenceCall;

/**
* CCP CS specific operations observer class.
* Observer for CCP events, errors and statuses etc. 
*
*  @since S60 3.2
*/
class MCCPCSObserver : public MCCPObserver
    {
protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPCSObserver() {};

public:
    /**
    * Notify of MO call that is not requested
    * @since S60 3.2
    * @param aCall Pointer to the created MO call in plug-in.
    * @return none
    */
    virtual void MoCallCreated( MCCPCall& aCall ) = 0;

    /**
    * Notify of conference call that is not requested.
    * @since S60 3.2
    * @param aConferenceCall Created conference
    * @return none
    */
    virtual void ConferenceCallCreated( MCCPConferenceCall& aConferenceCall ) = 0;

    /**
    * Notify data port information for video telephony.
    * @since S60 3.2
    * @param aPortName Dataport information.
    * @return none
    */
    virtual void DataPortName( TName& aPortName ) = 0;
    };

#endif // MCCPCSOBSERVER_H

// End of File
