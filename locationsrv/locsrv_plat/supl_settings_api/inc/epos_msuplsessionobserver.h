/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer class to receive change in SUPL settings 
*
*/


#ifndef __EPOS_MSUPLSESSIONOBSERVER_H__
#define __EPOS_MSUPLSESSIONOBSERVER_H__

//  INCLUDES
#include <e32base.h>

// DATA TYPES
// CLASS DECLARATION

/**
*  Abstract class for SUPL session observers.  This class is used to 
*  notify clients about type of change happened in the SUPL trigger session db.
*
*  This class must be implemented by clients who want to listen for changes in the
*  active SUPL sessions. @p CSuplSettings::SetSessionObserverL() is used to register
*
*  This is part of @lib epos_suplsettings.lib
*  @since S60 5.2
*/
class MSuplSessionObserver
    {
    public:	    
	    
    /**
     * Event types for SUPL session changes.  
     */
    enum TSuplSessionEventType
	{
    /** Unspecified event */            
	ESuplSessionEventUnspecified  = 0x00, 
    /** SUPL session DB Add Event */
	ESuplSessionDBAddEvent = 0x01,
	/** SUPL session DB Delete Event*/
	ESuplSessionDBDeleteEvent = 0x03,
	/** SUPL session DB Update Event*/
	ESuplSessionDBUpdateEvent = 0x05,
	};

    public:  // New functions
        
        /**
        * Invoked by SUPL settings API when a change has been detected in the SUPL session db.
        *
        * To be able to receive this notification, the observer object must
        * first be registered by invoking @p CSuplSettings::SetSessionObserverL().
        *
        *  @since S60 5.2
        * @param[OUT] aEvent Information about the SUPL session event.
        * @param[OUT] aSessionId Identifies the session which has undergone a db change event
        */
        virtual void HandleSuplSessionChangeL(
                TSuplSessionEventType aEvent,TInt64 aSessionId ) = 0;
    };

#endif      // __EPOS_MSUPLSESSIONOBSERVER_H__  
            
// End of File
