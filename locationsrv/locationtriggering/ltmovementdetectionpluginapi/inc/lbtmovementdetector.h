/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declaration of Movement Detection API
*
*/


#ifndef C_LBTMOVEMENTDETECTOR_H
#define C_LBTMOVEMENTDETECTOR_H

#include <e32base.h>
#include "lbtmovementdetector.hrh"

const TUid KMovementDetectorPlugInInterfaceUid = {KMovementDetectionPlugInInterfaceId};
  
  
 /**
 *  Observer interface for getting notifications from movement detector @ref CMovementDetector.
 *
 *  Classes interested in getting notifications when a terminal is moving
 *  must implement this interface. 
 *
 *  @lib movementdetector.lib
 *  @since  S60 v4.0
 */
class MLbtMovementObserver
	{
public:

    /**
    *  Enumeration for different observer events that are notified through
    *  @ref MMovementObserver.
    */
	enum TMovementObserverEvent
		{
		EEventMovement = 1,     /** Movement detector senses movement */
		EEventDetectorOffline,  /** Movement detector has gone offline */
		EEventDetectorFailed,   /** Movement detector has failed */
		EEventDetectorOnline,   /** Movement detector has gone online */
		EEventAccuracyChanged   /** Change in accuracy that movement detector promises */
		};
		
    /**
     * An observer callback to handle notification event. This is called when movement
     * detector senses movement.
     *
     * @since S60 TACO
     * @param aEvent Eventcode for an event that took place. Refer to @p TMovementObserverEvent
     */
	void virtual HandleDetectorNotification(const TMovementObserverEvent aEvent) = 0;
	
	};


/**
 *  ECom interface definition for movement detectors.
 *
 *  All movement detector implementations must derive from and implement this interface.
 *  The interface defines basic operations that all movement detectors must support. Movement
 *  detector report back movement observers thourgh an interface @ref MMovementObserver.
 *
 *  Supported use cases:
 *  - Load movement detector.
 *  - Unload movement detector.
 *  - Request notification about movement detector events.
 *  - Remove notification request.
 *
 *  @lib movementdetector.lib
 *  @since S60 TACO
 */
class CLbtMovementDetector : public CBase
    {

public:

    /**
     * Loads a movement detector and initalises it. Detector does not start running until
     * a call to @p RequestNotificationL().
     *
     * @since S60 TACO
     */
    IMPORT_C static CLbtMovementDetector* NewL();

    /**
     * Unloads this movement detector.
     *
     * @since S60 TACO
     */
    IMPORT_C virtual ~CLbtMovementDetector();

    /**
     * Request notification for movement. This registers an object as an observer to
     * movement notifications as well as .
     *
     * @since S60 TACO
     * @param aObserver An observer to be registered. Object must implement @ref MMovementObserver
     *
     * @param aSleepDuration Time duration during which movement detection should listen for movement
     */
    IMPORT_C virtual void RequestNotificationL( MLbtMovementObserver* aObserver,
                                                    TInt aSleepDuration) = 0;

    /**
     * Request removal of movement notification request.
     *
     * @since S60 TACO
     */
    IMPORT_C virtual void StopNotification() = 0;

        

private: // data


    /**
     * Instance identifier key
     */
     TUid iDtor_ID_Key;
    };


#endif // C_MOVEMENTDETECTOR_H
