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
* Description:  A session class for LocationManagerServer.
*
*/

#ifndef C_CLOCATIONMANAGERSESSION_H
#define C_CLOCATIONMANAGERSESSION_H

#include <e32base.h>
#include <etel3rdparty.h>

class CLocationManagerServer;

/**
*  A session class for LocationManagerServer.
*
*  @since S60 3.1
*/
class CLocationManagerSession : public CSession2
    {
public:
    /**
     * C++ constructor.
     */
    CLocationManagerSession();
    
    /**
     * From CSession2, passes the request forward to DispatchMessageL.
     * @since S60 3.1
     * @param aMessage, a message from client.
     * @return None.
     */
    void ServiceL( const RMessage2& aMessage );
    
    /**
     * Handles the request from client.
     * @since S60 3.1
     * @param aMessage, a message from client.
     * @return None.
     */
    void DispatchMessageL( const RMessage2& aMessage );
    
    /**
     * From CSession2 to add session to server's counter.
     * @since S60 3.1
     * @param None.
     * @return None.
     */
    void CreateL();    

private:
    /**
     * Start location recording.
     */  
    void StartLocationTrailL( const RMessage2& aMessage );

    /**
     * Stop location recordind.
     */  
    void StopLocationTrailL( const RMessage2& aMessage );

    /**
     * Get the current state of the location trail.
     */
    void GetLocationTrailStateL( const RMessage2& aMessage );

    /**
     * Add notification request for location trail's state changes.
     */    
    void NotifyLocationTrailStateChangeL( const RMessage2& aMessage );
    
    /**
     * Cancel notification request.
     */    
    void CancelNotificationRequest( const RMessage2& aMessage );    

    /**
     * Get location by time stamp.
     */
    void RetrieveLocationL( const RMessage2& aMessage );

    /**
     * Get current location.
     */
    void CurrentLocationL( const RMessage2& aMessage );
    
    /**
     * Cancel request for current location.
     */
    void CancelLocationRequest( const RMessage2& aMessage );
    
    /**
     * Cancel request for tracklog notifications.
     */
    void CancelTrackLogNotificationRequest( const RMessage2& aMessage );
    
    /**
     * Create new location object with given coordinates.
     */ 
    void CreateLocationObjectL( const RMessage2& aMessage );
    
    /**
     * Create new location object from location trail.
     */ 
    void LocationSnapshotL( const RMessage2& aMessage );
    
    /**
     * Remove location object by object ID
     */ 
    void RemoveLocationObjectL( const RMessage2& aMessage );
    
    /**
     * Copy location data from one meadia object to 
     * defined media objects by media object Id.
     */ 
    void CopyLocationDataByIdL( const RMessage2& aMessage );
    
    /**
     * Copy location data from one media object to 
     * defined media objects by media object Uri.
     */ 
    void CopyLocationDataByURIL( const RMessage2& aMessage );
    
    /**
     * Start Track Log recording.
     */
    void StartTrackLogL( const RMessage2& aMessage );
    
    /**
     * Stop Track Log recording.
     */
    void StopTrackLogL( const RMessage2& aMessage );
    
    /**
     * Get Track Log recording status and GPS signal quality.
     */
    void GetTrackLogStatusL( const RMessage2& aMessage );
    
    /**
     * Delete Track Log file.
     */
    void DeleteTrackLogL( const RMessage2& aMessage );

    /**
     * Get track-log recording status.
     */
    void IsTrackLogRecordingL( const RMessage2& aMessage );
    
    /**
     * Get currently track log file name
     */
    void TrackLogNameL( const RMessage2& aMessage );
    
    /**
     * Get changed gps quality info when it is really changed
     */
    void RegisterTrackLogObserver( const RMessage2& aMessage );
    
    void GetCaptureSettingL( const RMessage2& aMessage );

private:
    /**
    * C++ destructor.
    */
    virtual ~CLocationManagerSession(); 

    /*
    * Reference to Location Manager Server.
    */
    CLocationManagerServer& Server();
        
private:
    CBufFlat* iIdBuffer;
    RMessage2 iMessage;

    TInt iNotificationHandle;
    TInt iLocationHandle;
    TInt iTrackLogNotificationHandle;
    };

#endif // C_CLOCATIONMANAGERSESSION_H

// End of file.
