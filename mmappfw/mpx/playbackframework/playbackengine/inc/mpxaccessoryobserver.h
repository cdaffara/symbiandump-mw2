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
* Description:  Accessory observer
*
*/


#ifndef CMPXACCESSORYOBSERVER_H
#define CMPXACCESSORYOBSERVER_H

//  INCLUDES
#ifdef __ACCESSORY_FW
#include <AccessoryMode.h>
#include <AccessoryServer.h>
#include <AccPolAccessoryMode.h>
#else
#include <e32base.h>
#include <DosSvrServices.h>
#endif // __ACCESSORY_FW

#include <mpxplaybackframeworkdefs.h>

// FORWARD DECLARATIONS
#ifndef __ACCESSORY_FW
class RDosServer;
#endif // __ACCESSORY_FW

/**
*  Interface for handling accessory event
*/
NONSHARABLE_CLASS(MMPXAccessoryEventObserver)
    {
public:
    /**
    *   Handle accessory event
    *
    *   @param aMode playback accessory mode
    */
    virtual void HandleAccesoryEventL(TMPXPlaybackAccessoryMode aMode)=0;
    };


#ifdef __ACCESSORY_FW

NONSHARABLE_CLASS( CMPXAccessoryObserver ): public CActive
    {
    public: // constructor & destructor

        /** 
        * Two phase constructor
        * @param aObs  Reference to an event observer
        */
        static CMPXAccessoryObserver* NewL(MMPXAccessoryEventObserver& aObs);

        /**
        * Destructor.
        */
        virtual ~CMPXAccessoryObserver();

    public: 

        /**
        * Starts listening to Accessory events.
        * @param None
        * @return None
        */
        void StartL();
        
        /*
        * Gets current accessory mode
        */
        TMPXPlaybackAccessoryMode AccessoryModeL();
        
    private:    // New functions
    
        /**
        * Converts from accessory mode to internal MPX mode enum
        * @param aMode Accessory framework mode
        * @ret MPX Accessory Mode
        */
        TMPXPlaybackAccessoryMode ConvertToMpxMode( 
            TAccPolAccessoryMode& aMode );
        
    private: // from CActive
        /** @see CActive::RunL */
        void RunL();

        /** @see CActive::DoCancel */
        void DoCancel();
        
    private: // Constructor
        
        /**
        * C++ default constructor.
        * @param aObs  Reference to an event observer
        */
        CMPXAccessoryObserver(MMPXAccessoryEventObserver& aObs);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:
        // Event observer
        MMPXAccessoryEventObserver& iObs;
        // Accessory Server session 
        RAccessoryServer     iServer;
        // Accessory mode 
        RAccessoryMode       iMode;
        TAccPolAccessoryMode iAccessory_mode;
        
    };

#else
/**
*  Accessary event observer
*/
NONSHARABLE_CLASS(CMPXAccessoryObserver): public CDosEventListenerBase
    {
public: 

    /**
    *  Two-phased constructor
    *
    *  @param aObs observer
    *  @return object of constructed
    */
    static CMPXAccessoryObserver* NewL(MMPXAccessoryEventObserver& aObs);
    
    /**
    * Destructor.
    */
    virtual ~CMPXAccessoryObserver();
    
public: // from CDosEventListenerBase
    /**
    *  From CDosEventListenerBase
    *  Accessory mode changed
    *
    *  @param aState accessory state
    */
    void AccessoryModeChangedL(TDosAccessoryMode aState);
    
    /**
    *  From CDosEventListenerBase
    *  Start monitoring
    */
    void StartL();

    /*
    * Gets current accessory mode
    */
    TMPXPlaybackAccessoryMode AccessoryModeL();
        
private:    // New functions

    /**
    * Converts from accessory mode to internal MPX mode enum
    * @param aMode Dos Accessory framework mode
    * @ret MPX Accessory Mode
    */
    TMPXPlaybackAccessoryMode ConvertToMpxMode( 
        TDosAccessoryMode& aMode );
        
private: 

    /**
    *  C++ constructor
    *
    *  @param aObs observer
    */
    CMPXAccessoryObserver(MMPXAccessoryEventObserver& aObs);

    /**
    *  2nd phase constructor
    */
    void ConstructL();
private:
    MMPXAccessoryEventObserver& iObs;
    };
#endif // __ACCESSORY_FW
    
#endif      // CMPXACCESSORYOBSERVER_H
