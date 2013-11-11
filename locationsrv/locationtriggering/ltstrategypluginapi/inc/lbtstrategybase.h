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
* Description:  Base class for location acquisition strategy implmentations.
*
*/



#ifndef C_LBTSTRATEGYBASE_H
#define C_LBTSTRATEGYBASE_H


//#include <lbttriggerentry.h>
#include <lbtcommon.h>

#include "lbttriggeringsupervisionobserver.h"


class CLbtBaseStrategyExtension;


const TUid KLocAcquisitionStrategyInterfaceUid = { 0x10283133 };


/**
 * Abstract base class for location acquisition strategy plug-ins implementation.
 *
 * A strategy plug-in is responsible for the trigger supervision process,
 * i.e. acquiring locations with suitable intervals and providing notifications
 * when triggers are fired.
 *
 * A location acquisition strategy plug-in must implement a subclass of 
 * this base class. This base class defines interface between Location
 * Triggering Server and the strategy plug-in. 
 *
 * @lib ltstrategypluginapi.lib
 * @since S60 4.0
 */
class CLbtStrategyBase : public CBase 

    {
    
    public:  // Constructors and destructor

        /**
         * Creates a new instance of a strategy implementation.
         *
         * @param[in] aConstructionParameters Construction parameters for
         * CLbtBaseStrategy. Must by passed to BaseConstructL() by the
         * strategy implementation.
         * @return The strategy implementation.
         */
        IMPORT_C static CLbtStrategyBase* NewL( 
                                    TAny* aConstructionParameters );

        /**
         * Destructor.
         */
        IMPORT_C ~CLbtStrategyBase();

    protected:  // Constructors

        /**
         * C++ default constructor.
         */
        IMPORT_C CLbtStrategyBase();

        /**
         * Creates the internals of the strategy.
         *
         * This function must be called first in the strategy's
         * ConstructL() method.
         *
         * @param[in] aConstructionParameters The construction parameters supplied
         * in the factory call.
         */
        IMPORT_C void BaseConstructL( TAny* aConstructionParameters );

    public:  // New functions

        /**
         * Start supervision process. 
         *
         * This function must be implemented by the strategy plug-in. 
         * Location Triggering Server calls this function to start supervision 
         * process. If the supervision process is already started when this
         * method is called, location acquisition strategy plug-in shall do
         * nothing.
         */
        IMPORT_C virtual void StartSupervision() = 0;

        /**
         * Stop supervision process.
         *
         * This function must be implemented by the strategy plug-in. 
         * Location Triggering Server calls this function to stop
         * supervision process. The strategy plug-in shall release
         * all resources used for supervision process when this function
         * is called.
         *
         * If the supervision process is not started
         * when this method is called, location acquisition strategy plug-in
         * shall do nothing. 
         */
        IMPORT_C virtual void StopSupervision() = 0;

        /**
         * Inform Location Triggering Server that a trigger shall be 
         * fired.
         * 
         * When a trigger is fired, the strategy plug-in calls this 
         * function to inform Location Triggering Server.
         * 
         * When more than one trigger is fired simultaneously, strategy
         * plug-in shall call this function separately  for each the 
         * fired trigger.
         * 
         * @param[in] aId The ID of the firing trigger.
         * @param[in] aPosInfo The position information associated with the firing
         * event. Only basic position information is returned in this parameter.
         */
        IMPORT_C void TriggerFiredL( CLbtGeoAreaBase::TGeoAreaType aAreaType,TLbtTriggerId aId, 
                                    const TPositionInfo& aPosInfo ); 

        /**
         * Inform Location Triggering Server about the location
         * triggering supervision dynamic information.
         *
         * Location acquisition strategy plug-in shall use this 
         * method to update the supervision dynamic information to Location 
         * Triggering Server every time a location request is completed. 
         * 
         * @param[in] aDynInfo Contains the new triggering supervision 
         * dynamic information.
         */
        IMPORT_C void SetTriggeringSupervisionDynamicInfo(
                           const TLbtStrategySupervisionDynamicInfo& aDynInfo);

        /**
         * Inform location acquisition strategy plug-in that system settings 
         * related to trigger supervision are changed.
         * 
         * This function must be implemented by the strategy plug-in. 
         * Location Triggering Server calls this function every time when
         * triggering supervision settings is change. 
         */
        virtual void TriggeringSupervisionSettingsChanged() = 0;

        /**
         * Get system settings related to triggering supervision from 
         * Location Triggering Server.
         *
         * @param[out] aSettings In return, contains system settings
         * related to triggering supervision. 
         */
        IMPORT_C void GetTriggeringSupervisionSettings(
                        TLbtStrategyTriggeringSupervisionSettings& aSettings);

    private:

	    // Reserved for future expansion 
	    IMPORT_C virtual void CLbtStrategy_Reserved1();
	    
	    IMPORT_C virtual void CLbtStrategy_Reserved2();
        
    private:    // Data

        TUid                                    iDtorIdKey;
        
        CLbtBaseStrategyExtension*              iExtension;
        
        MLbtTriggeringSupervisionObserver*		iStrategyObserver;
        
    };  


#endif  // C_LBTSTRATEGYBASE_H
