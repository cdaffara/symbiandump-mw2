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
* Description:    Refreshtimer.
*
*/




#ifndef simplesettings_H
#define simplesettings_H

#include <e32std.h>
#include <s32file.h>

// FORWARD DECLARATION
class RFs;
class RFileReadStream;
class CRepository;


//**********************************
// TSimpleSettings
//**********************************
class TSimpleSettings
    {

public:

    /**
     * Constructor
     */
    TSimpleSettings( );

    /**
     * Publish-expiry accessor
     * @return expiry in seconds
     */
    TUint ExpiryPuhlish();

    /**
     * Watcher-expiry accessor
     * @return expiry in seconds
     */
    TUint ExpiryWatcher();

    /**
     * Api request expiry accessor
     * @return expiry in seconds
     */
    TUint ExpiryApi();

    /**
     * Winfo-expiry accessor
     * @return expiry in seconds
     */
    TUint ExpiryWinfo();
    
    /**
     * CLIENT-OBJ-DATA-LIMIT accessor
     * @return CLIENT-OBJ-DATA-LIMIT
     */    
    TInt ObjLimit();
       
    /**
     * SOURCE-THROTTLE-PUBLISH accessor
     * @return SOURCE-THROTTLE-PUBLISH
     */        
    TInt Throttle();
       
    /**
     * MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS accessor
     * @return MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS
     */        
    TInt MaxSubscriptions();
    
    /**
     * SIP Profile Id
     * @return SIP Profile Id, 0 means default
     */        
    TInt SipProfileId();    
       
    /**
     * Read static settings from central repository
     */
    void ReadCentRepSettings( );
    
    /**
     * Read OTA settings from central repository
     * @param aSetId SIMPLE settings id
     */
    void ReadOTASettingsL( TInt aSetId );    
    

#ifdef _DEBUG
    /**
     * Read optional ini file
     * @param aFs file system handle
     * @leave if file not found
     */
    void ReadIniFileL( RFs&  aFs );
#endif

private:

    /**
     * Get integer value from Central Repository.
     * @param aRepository repository
     * @param aId key id
     * @return value of the key      
     */
    TInt GetCenRepIntValueL( 
        CRepository* aRepository,
        TUint32 aId );
                
private: //data

    /**
     * Publish expiry time
     */
    TUint iExpiryPuhlish;
    
    /**
     * Watcher expiry time
     */    
    TUint iExpiryWatcher;
    
    /**
     * Winfo watcher expiry time
     */    
    TUint iExpiryWinfo;
    
    /**
     * API request expiry time
     */    
    TUint iExpiryApi; 
    
    /**
     * CLIENT-OBJ-DATA-LIMIT
     */    
    TInt iObjLimit;
    
    /**
     * SOURCE-THROTTLE-PUBLISH
     */    
    TInt iThrottle;
    
    /**
     * MAX-NUMBER-OF-PRESENCE-SUBSCRIPTIONS
     */    
    TInt iMaxSubscriptions;
    
    /**
     * Sip Profile Id
     */
    TInt iSipProfileId; 
  
#ifdef _DEBUG
    friend class T_TSimpleSettings;
    friend class T_CSimpleSipConnection;
#endif
    };

#endif      // simplesettings_H

// End of File
