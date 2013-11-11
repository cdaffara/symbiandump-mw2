/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Server.
*
*/


#ifndef CPHSRVSERVER_H
#define CPHSRVSERVER_H


// INCLUDES
#include "mphsrvphoneinterface.h" 
#include "phcltclientserver.h" 
#include <rphcltserver.h> 
#include <fbs.h>
#include <f32file.h>


// FORWARD DECLARATIONS
class CPhSrvSession;
class CPhSrvUssdManager;
class CPhSrvPhoneController;
class CPhSrvResourceManager;
class CPhSrvCallRequestManager;
class CPhSrvEmergencyRequestManager;
class CPhSrvEmergencyNumberManager;
class MPhSrvNegotiatorCall;
class MPhSrvEmergencyNumberManager;
class MPhSrvComHandNegotiator;
class CPhSrvComHandRequestManager;
class CPhSrvMessengerRequestManager;
class MPhSrvMessengerNegotiator;
class MPhSrvEmergencyNegotiatorCall;


// CLASS DECLARATION

/**
*  It is the server class for Phone Server.
*
*  @since 1.0
*/
class CPhSrvServer : 
    public CServer2
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * Creates a new server and starts it.
        */
        static CPhSrvServer* NewL();
       
        /**
        * Destructor.
        */
        ~CPhSrvServer();
        

    public: // New functions

        /**
        * Connects to Font and bitmap server.
        * @return Symbian error code
        */
        TInt ConnectFBS();

        /**
        * Disconnects Font and bitmap server.
        * @param aForce do we force disconnect
        */
        void DisconnectFBS( TBool aForce = EFalse );

        /**
        * Creates all managers.
        *
        * @param aSession The session for subsessions.
        */ 
        void CreateAllL( CPhSrvSession& aSession );

        /** 
        * Cancels create all.
        *
        * @param aSession The session for subsessions.
        */
        void CancelCreateAll( CPhSrvSession& aSession );

        /**
        * Creates a new object container.
        *
        * @return Returns the created instance.
        */
        CObjectCon* NewContainerL() const;
        
        /**
        * Removes an object container.
        *
        * @param  aCon object container 
        */
        void RemoveContainer( CObjectCon* aCon );

        /**
        * Access to the call negotiator
        *
        * @return The call negotiator
        */
        MPhSrvNegotiatorCall& CallNegotiatorL();
        
        /**
        * Access to the call negotiator
        * NOTE: This is not safe call if IsCallNegotiator() return False
        * @return The call negotiator
        */
        MPhSrvNegotiatorCall& CallNegotiator() const;

        /**
        * Access to the USSD manager
        *
        * @return The USSD manager
        */
        CPhSrvUssdManager& UssdManagerL();
        
        
        /**
        * Access to the call negotiator
        *
        * @return The call negotiator
        */
        MPhSrvEmergencyNegotiatorCall& EmergencyCallNegotiatorL();
        
        /**
        * Access to the call negotiator
        * NOTE: This is not safe call if IsEmergencyCallNegotiator() return
        * false.
        * @return The call negotiator
        */
        MPhSrvEmergencyNegotiatorCall& EmergencyCallNegotiator() const;

        /**
        * Ask if there is a emergency call negotiator.
        *
        * @return ETrue if there is emergency call negotiator.
        */
        TBool IsEmergencyCallNegotiator() const;

        /**
        * Access to the phone
        *
        * @return The phone interface
        */
        MPhSrvPhoneInterface& PhoneInterfaceL();

        /**
        * Access to the emergency number manager
        *
        * @return The emergency number manager
        */
        MPhSrvEmergencyNumberManager& EmergencyNumberManagerL();

        /**
        * Ask if there is a call negotiator.
        *
        * @return ETrue if there is call negotiator.
        */
        TBool IsCallNegotiator() const;

        /**
        * Ask if there is a ussd manager
        *
        * @return ETrue if there is ussd manager.
        */
        TBool IsUssdManager() const;

        /**
        * Access to the command handler negotiator.
        * @since 2.6
        *
        * @return The command handler negotiator
        */
        MPhSrvComHandNegotiator& ComHandNegotiatorL();
        
        /**
         * Access to the command handler negotiator.
         * NOTE: This is not a safe call if
         * IsComHandNegotiator() return false.
         */
        MPhSrvComHandNegotiator& ComHandNegotiator() const;

        /**
        * Ask if there is a command handler negotiator.
        * @since 2.6
        *
        * @return ETrue if there is command handler negotiator.
        */
        TBool IsComHandNegotiator() const;

        /**
        * Access to the messenger negotiator.
        * @since 2.6
        *
        * @return The messenger negotiator
        */
        MPhSrvMessengerNegotiator& MessengerNegotiatorL();
        
        /**
        * Access to the messenger negotiator.
        * NOTE: This is not a safe call if IsMessengerNegotiator()
        * return false.
        * @since 5.0
        *
        * @return The messenger negotiator
        */
        MPhSrvMessengerNegotiator& MessengerNegotiator() const;

        /**
        * Ask if there is a messenger negotiator.
        * @since 2.6
        *
        * @return ETrue if there is messenger negotiator.
        */
        TBool IsMessengerNegotiator() const;


    public: // Functions from base classes
        
        /**
        * From CServer, create a new session.
        *
        * @param aVersion It is the version of the client api.
        * @param aMessage Connect message from Client.
        * @return Returns a new session.
        */          
        CSession2* NewSessionL(
            const TVersion &aVersion,
            const RMessage2& aMessage ) const;
        

    private: // New functions
        
        /**
        * C++ constructor.
        */
        CPhSrvServer( TInt aPriority );

        /**
        * By default Symbian 2nd phase constructor is private.
        * @since 2.8
        */
        void ConstructL();
           
        /**
        * Returns file server session.
        */
        RFs& FsSessionL();


        /**
        * Returns resource manager.
        */
        CPhSrvResourceManager& ResourceManagerL();

        /**
        * Performs step in create all.
        */
        TInt CreateAllStepL();

        /**
        * Performs an step.
        */
        void PerformNextStepL();

        /**
        * Complete create all.
        */
        void CompleteCreateAll( TInt aError );

        /**
        * Create all step.
        */
        static TInt DoCreateAllStepL( TAny* aAny );       

    private:     // Data
    
        // The file server session for the whole server.
        RFs                            iFsSession;


        // The object responsible for controlling the server resource file.
        CPhSrvResourceManager*         iResourceFileManager;

        // The object responsible for controlling the interface to the phone.
        CPhSrvPhoneController*         iPhoneController;

        // The object container indexer which owns and manages the CObjectCon 
        // instances ensuring that each container has a unique handle spanning 
        // all sub-sessions.
        // As well as owning the object containers, it also is responsible 
        // for creating new ones.
        mutable CObjectConIx*          iObjectConIx;

        // The object responsible for acting as the bridge between an external 
        // call client, and the phone app engine notifier client.
        CPhSrvCallRequestManager*      iCallRequestManager;
        
        // The object responsible for acting as the bridge between an external 
        // call client, and the phone app engine notifier client.
        CPhSrvEmergencyRequestManager*      iEmergencyRequestManager;

        // Responsible for all USSD related server functionality.
        CPhSrvUssdManager*             iUssdManager;

        // Responsible for all emergency number related functionality.
        CPhSrvEmergencyNumberManager*  iEmergencyNumberManager;
        
        // The object responsible for acting as the bridge between an external
        // command handler client, and the phone app engine notifier client.
        CPhSrvComHandRequestManager*   iComHandRequestManager;

        // The object responsible for handling messenger requests.
        CPhSrvMessengerRequestManager* iMessengerRequestManager;

        // CREATE ALL VARIABLES

        // Idle object for create all.
        CIdle*                         iCreateAll;

        // Step index.
        TInt                           iCreateAllStep;

        // Error.
        TInt                           iCreateAllError;

        // Array of sessions.
        RPointerArray< CPhSrvSession > iCreateAllArray;

        // Reference count for font and bitmap server connections.
        TInt                           iFBSConnections;
    };

#endif      // CPHSRVSERVER_H


// End of File
