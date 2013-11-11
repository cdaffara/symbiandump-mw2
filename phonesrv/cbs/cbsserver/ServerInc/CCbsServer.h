/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the CCbsServer class
*    
*                Represents CBS Server. Derives from EPOC client-server framework base
*                class CServer.
*
*/




#ifndef     CCBSSERVER_H
#define     CCBSSERVER_H

//  INCLUDES
#include <e32base.h>
#include "CbsCommon.h"
#include "MCbsDbTopicListObserver.H"

//  CONSTANTS  
// This is the priority of the server active object.
const TInt KCbsServerPriority = CActive::EPriorityStandard;
  
//  FORWARD DECLARATIONS
#ifdef __WINS__
// Required to satify EPOCEXE target type build criteria
IMPORT_C TInt WinsMain( TAny * );
#endif

class CObjectConIx;
class CCbsDbImp;
class CCbsRecEtel;
class CCbsReceiverHelper;
class CCbsObject;
class CCbsSatRefresh;
class CCbsShutdownMonitor;

//  CLASS DECLARATION 

/**
*   CCbsServer is the core part of CBS Server.
*    
*   CCbsServer allocates all server-side resources on startup and
*   creates CCbsSession objects in response to client requests.
*/
class CCbsServer : public CServer2
    {
    public:     // New functions
        /**
        *   Creates an instance of the class.
        *
        *   The name of the server will be KCbsServerName as specified 
        *   in CbsServerConstants.h.
        *
        *   After the call is made, the server is responsible of releasing 
        *   both database and receiver during destruction. 
        *
        *   @return                     The method returns a pointer to the 
        *                               new CCbsServer object.
        */
        static CCbsServer* NewL();
        
        /**
        *   Destructor.
        */
        ~CCbsServer();

        /**
        *   Creates a new session object container.
        *
        *   It is the responsibility of the caller to delete the object 
        *    container, when it is no longer used.
        *
        *   @return                     A pointer to the new object container.
        */
        CObjectCon* NewContainerL();

        /**
        *   Returns a reference to the database.
        *            
        *   @return                     Database object
        */
        CCbsDbImp& Database();

        /**
        *   Returns a reference to the receiver.
        *        
        *   @return                     Receiver object
        */
        CCbsRecEtel& Receiver();

        /**
        *   Returns the total number of detected (via topic detection feature)
        *   topics and then clears the counter.
        *
        *   @return                     Number of detected topics.
        */
        TInt TotalTopicsDetected();

        /**
        *   Panics the client.
        *
        *   @param                      Reason of panic
        */
        void PanicClient( TInt aPanic ) const;

        /**
        *   Shuts the server down.
        */  
        void Shutdown();

        /**
        *   Returns the object container index reference
        *
        *   @return                     Container index reference
        */  
        CObjectConIx& ContainerIndex();

        /**
        *   Returns the current message
        *
        *   @return                     Current IPC message
        */
        const RMessage2 ServerMessage() const;

        /**
        *   Returns an object by handle.
        *
        *   @param  aHandle         Handle to the object.
        *   @return                 Pointer to the object or, if there is no
        *                           such object, NULL.
        */
        CCbsObject* ReturnObjectByHandle( TUint aHandle );

        /**
        *   Deletes a subsession
        *
        *   @param  aHandle         Handle to the object.
        */
        void DeleteObjectByHandle( TUint aHandle );

        /**
        *   Returns the object container
        *
        *   @@return                The object container
        */
        CObjectCon& Container();

        /**
        *   Returns the object index
        *
        *   @@return                The object index
        */
        CObjectIx& ObjectIx();
        
        /**
        *   Reloads the SIM topics in case of SAT/SIM refresh
        *        
        */
        void ReLoadSimTopicsL();

    public:     // From CServer2      
        
        /**
        *   Creates a new session (an instance of CCbsSession).
        *
        *   The method is meant to be called by the Symbian OS server framework and 
        *   so it should not be used in any other case.
        *
        *   The method leaves if the version given as parameter and the server
        *   version differ.
        *
        *   @param  aVersion            Expected version of CBS Server.
        *   @return                     New session object.
        */
        CSession2* NewSessionL( const TVersion& aVersion, const RMessage2& aMessage ) const;
        
    private:

        /**
        *   Constructor.
        *
        *   @param  aPriority           Priority of the server. 
        */
        CCbsServer( TInt aPriority );

        /**
        *   Finalizes the construction by creating subcomponents
        *   and synchronizing DB and receiver states.
        */
        void ConstructL();

        /**
        *   Initializes the receiver
        */
        void InitReceiverL();

        /**
        *   Initializes topic detection status.
        */
        void InitTopicDetectionStatusL();

    private:    // Prohibited operators and functions

        // Copy constructor
        CCbsServer( const CCbsServer& );

        // Assignment operator
        CCbsServer& operator=( const CCbsServer& );

    private:    // Data

        // Own: The container index, that is required to create 
        // object containers.
        CObjectConIx* iContainerIx;
        
        // Own: database object.
        CCbsDbImp* iDatabase;
        
        // Own: receiver object.
        CCbsRecEtel* iReceiver;

        // Own: gateway to the receiver.
        CCbsReceiverHelper* iHelper;

        // Own: PubSub shutdown monitor
        CCbsShutdownMonitor* iShutdownMonitor;

        // Own: the object container that contains the subsessions.
        CObjectCon* iContainer;

        // Own: object index that is used to find a subsession object 
        // by it's handle.
        CObjectIx* iIndex;
        
        // Sat refresh listener
        CCbsSatRefresh* iSatRefresh;
    };

#endif      //  CCBSSERVER_H
            
// End of File


