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
* Description:  Class used to set & get the trigger attributes
*
*/

#ifndef TRIGGERNODE_H_
#define TRIGGERNODE_H_

#include <e32cmn.h>
#include <lbttriggerconditionarea.h> 
#include <lbttriggerentry.h>


#include "lbtcontextsourceplugincommon.h"

/*
 * This class simply holds the trigger specifications.
 * The input parameters to create a trigger are stored to and 
 * retrieved from this class.
 */
class TTriggerNode       
    {
public: 
    
    /*
     * Store trigger name
     */
     void SetTriggerNameL(const TPtrC& aTriggerName);

     /*
      * Store Latitude
      */
     void SetLatitudeL(const TPtrC& aLatitude);
    
     /*
      * Store Longitude
      */
     void SetLongitudeL(const TPtrC& aLongitude);
    
     /*
      * Store type of monitoring area
      */
     void SetRegionL(const TPtrC& aRegion);
    
     /*
      * Store radius of trigger monitoring area
      */
     void SetRadiusL(const TPtrC& aRadius);
    
     /*
      * Store trigger fire condition.
      * This decided whether the trigger is to be fired when
      * the device enters the triggering are, or whether
      * the trigger is to be fire when the device leaves the 
      * monitoring area, or on both conditions.
      */
     void SetDirectionL(const TPtrC& aDirection);
    
     /*
      * Store Name of Trigger set
      */
     void SetContextSource(const TPtrC& aSetName);
     
     /*
      * Store information as to whether the trigger is
      * to be actively monitored or not
      */
     void SetCurrentStateL(const TPtrC& aCurrentState);

     /*
      * Retrieve trigger name
      */
      TInt GetTriggerName( TPtrC& aTriggerName );
    
     /*
      * Retrieve Latitude
      */
      TInt GetLatitude( TReal& aLatitude );

     /*
      * Retrieve Longitude
      */
      TInt GetLongitude( TReal& aLongitude );
    
     /*
      * Retrieve type of monitoring area
      */
      TInt GetRegion( TPtrC& aRegion );
    
     /*
      * Retrieve radius of trigger monitoring area
      */
      TInt GetRadius( TReal& aRadius );
    
     /*
      * Retrieve trigger fire condition.
      * This decided whether the trigger is to be fired when
      * the device enters the triggering are, or whether
      * the trigger is to be fire when the device leaves the 
      * monitoring area, or on both conditions.
      */
     CLbtTriggerConditionArea::TDirection GetDirection();
    
     /*
      * Retrieve Name of Trigger set
      */
     TPtrC& GetContextSource();

     /*
      * Retrieve information as to whether the trigger is
      * to be actively monitored or not
      */
     CLbtTriggerEntry::TLbtTriggerState GetCurrentState();

private:
    
    /*
     * Latitude
     */
    TReal64 iLatitude;
     
    /*
     * Longitude
     */
    TReal64 iLongitude;
     
    /*
     * Trigger name
     */
     TPtrC iTriggerName;
     
     /*
      * Type of triggering area
      */
     TPtrC iRegion;
     
     /*
      * Radius of monitoring region
      */
     TReal iRadius;
     
     /*
      * Setname to which the trigger belongs
      */
     TPtrC iSetName;
     
     /*
      * Direction of monitoring trigger firing event. (OnEntry or OnExit or both)
      */
     CLbtTriggerConditionArea::TDirection iDirection;
     
     /*
      * Current state of the trigger
      */
     CLbtTriggerEntry::TLbtTriggerState iCurrentState;

    };

#endif /* TRIGGERNODE_H_ */
