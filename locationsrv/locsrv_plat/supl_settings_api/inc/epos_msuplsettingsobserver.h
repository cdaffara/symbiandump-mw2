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

/*! \file epos_msuplsettingsobserver.h
    \brief SUPL settings observer interface 
*/

#ifndef __EPOS_MSUPLSETTINGSOBSERVER_H__
#define __EPOS_MSUPLSETTINGSOBSERVER_H__

//  INCLUDES
#include <e32base.h>
#include <epos_csuplsettings.h>
// DATA TYPES
// CLASS DECLARATION

/**
*  Abstract class for SUPL settings observers.  This class is used to 
*  notify clients about type of change happened in the SUPL settings.
*
*  This class must be implemented by clients who want to listen for changes in the
*  SUPL Settings. @p CSuplSettings::SetObserverL() is used to register
*  SUPL Settings observers.
*
*  This is part of @lib epos_suplsettings.lib
*  @since S60 3.1
*/
class MSuplSettingsObserver
    {
    public:	    
	    
    /**
     * Event types for SUPL Settings changes.  Client should be prepare to situation 
     * that also other events might come in future when more functionality is added
     */
    enum TSuplSettingsEventType
	{
    /** Unspecified event */            
	ESuplSettingsEventUnspecified  = 0x00, 
    /** Communication parameters change event */ 
	ESuplSettingsEventCommParameterChange = 0x01, 
    /** SUPL usage change event */ 
	ESuplSettingsEventSuplUsageChange = 0x02, 
	/** SUPL Settings DB Add Event */
	ESuplSettingsDBAddEvent = 0x03,
	/** SUPL Settings DB Delete Event*/
	ESuplSettingsDBDeleteEvent = 0x05,
	/** SUPL Settings DB Update Event*/
	ESuplSettingsDBUpdateEvent = 0x07,
	};

    public:  // New functions
        
        /**
        * Invoked by SUPL settings API when a change has been detected in the SUPL Settings.
        *
        * To be able to receive this notification, the observer object must
        * first be registered by invoking @p CSuplSettings::SetObserverL().
        *
        * @param[OUT] aEvent Information about the SUPL Settings event.
        * @param[OUT] aSlpId The SLP Id that was changed in case if DB Change events
        */
        virtual void HandleSuplSettingsChangeL(
                TSuplSettingsEventType aEvent,TInt aSlpId = -1                        
        ) = 0;
        
        /**
        * Invoked by SUPL settings API when a any client of SUPL Settings API calls CSuplSettings::ChangeSuplTriggerServiceStatus().
        *
        * To be able to receive this notification, the observer object must
        * first be registered by invoking @p CSuplSettings::SetObserverL().
        *
        * @param[OUT] TSuplTriggerStatus, Information saying status of Supl trigger service .
        */
        virtual void HandleSuplTriggerStatusChangeL(
                CSuplSettings::TSuplTriggerStatus aSuplTriggerStatus ) = 0;
        
        
    };

#endif      // __EPOS_MSUPLSETTINGSOBSERVER_H__  
            
// End of File
