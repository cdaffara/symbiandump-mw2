/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  An interface to Track Log.
*
*/

#ifndef RTRACKLOG_H_
#define RTRACKLOG_H_

#include <lbssatellite.h> 
#include <rlocationmanager.h>
#include <locationeventdef.h>

// forward declaration
class MTrackLogObserver;
class CTrackLogObserverAO;

/*
 *  TrackLog API is used to start, stop and delete track-logs, 
 *  to get track-log status and to get track-log in GPX format. 
 * 
 *  @lib LocationManager.lib
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( RTrackLog ) : public RLocationManager
{

public:
	IMPORT_C RTrackLog();
	
	/**
	 * Starts track-log recording. Also starts location trail recording 
	 * if it is not already running.
	 * @since S60 3.2
	 * @param None.
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */ 
	IMPORT_C TInt StartTrackLog();
	
	/**
	 * Stops track-log recording.
	 * @since S60 3.2
	 * @param None.
	 * @return None.
	 */ 
	IMPORT_C void StopTrackLog();
	
	/**
	 * Gets the status of track-log recording and GPS signal quality.
	 * @since S60 3.2
	 * @param aRecording, indicating track log recording status
	 * @param aFixQuality, GPS signal quality
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */ 
	IMPORT_C TInt GetStatus(TBool& aRecording, TPositionSatelliteInfo& aFixQuality);

	/**
	 * Deletes a track-log file
	 * @since S60 3.2
	 * @param aUri, Track log URI
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */ 
	IMPORT_C TInt DeleteTrackLog(TDesC& aUri);
	
	/**
	 * Returns a boolean indicating track-log recording status.
	 * @since S60 3.2
	 * @param None.
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */ 
	IMPORT_C TBool IsRecording();
	
	/**
	 * Registers a new track-log observer.
	 * @since S60 3.2
	 * @param aObserver, observer which is added.
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */
	IMPORT_C TInt AddObserver(MTrackLogObserver* aObserver );
	
	/**
	 * Removes a registered track-log observer.
	 * @since S60 3.2
	 * @param aObserver, observer which is removed.
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */
	IMPORT_C TInt RemoveObserver(MTrackLogObserver* aObserver);
	
	/**
	 * Returns the name of the current track-log file.
	 * @since S60 3.2
	 * @param aTrackLogName, track log file name
	 * @return KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 */
	IMPORT_C TInt TrackLogName(TDes& aTrackLogName);
	
	/**
	 * Overrides inherited class close.
	 * Close observers array and calls main class close.
	 * @since S60 3.2
	 * @param None.
	 * @return None.
	 */ 
	IMPORT_C void Close();
	
public:
	
	/**
	 * @param aStatus, the request status of the active object
	 * @return None.
	 */	
	void SendNotifyRequest( TRequestStatus& aStatus );
	
	/**
	 * Notifies of an event in tracklog. The event can be of type EStarted/EStopped/ESignalChanged.
	 * @param aError error code
	 * @return None.
	 */
	void TrackLogEvent( TInt aError );

	
private:
	/**
	 * Array of observers
	 */ 
	RPointerArray<MTrackLogObserver> iObservers;
	
	TBool iSubscribed;
	/**
	 * An active object that observes tracklog events (starting/stopping/GPS fix quality changes).
	 */
	CTrackLogObserverAO* iTrackLogObserverAO;
	
	TPositionSatelliteInfo iFixQuality;
	TBool iRecording;
	TEventTypes iEventType;
	TPckg<TPositionSatelliteInfo> iWrapQuality;
	TPckg<TBool> iWrapRec;
	TPckg<TEventTypes> iWrapEventType;
};

/**
 * Track log Observer interface class for receiving track-log events.
 */ 
class MTrackLogObserver
{

public:
	/**
	 * Called to notify the observer that the 
	 * Track-log recording has started.
	 * 
	 * @param aError, KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 * @return None.
	 */ 
	virtual void TrackLogStarted(TInt aError) = 0;
	
	/**
	 * Called to notify the observer that the 
	 * Track-log recording was stopped.
	 * 
	 * @param aError, KErrNone if successful, otherwise one of the other 
     *       system-wide error codes.
	 * @return None.
	 */ 
	virtual void TrackLogStopped(TInt aError) = 0;
	
	/**
	 * Called to notify the observer that the 
	 * GPS fix quality has changed.
	 * 
	 * @param aFixQuality, GPS signal quality
	 * @param aTrackingActive, Is Tracking active
	 * @return None.
	 */ 
	virtual void GPSFixQualityChanged(TPositionSatelliteInfo& aFixQuality, TBool& aTrackingActive) = 0;
	
private:
};
#endif /*RTRACKLOG_H_*/
