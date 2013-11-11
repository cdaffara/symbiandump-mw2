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
* Description: Declaration of CPhoneHandlerCallBase class.
*
*/


#ifndef CPHONEHANDLERCALLBASE
#define CPHONEHANDLERCALLBASE

//  INCLUDES
#include "phonehandlerservice.h" 
#include "phonehandlerobserver.h" 
#include <e32base.h>
#include <rphcltserver.h> 

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CPhoneHandlerActive;
class CPhCltCommandHandler;

/**
*  Base class for services.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerCallBase ) : public CBase,
                                             public MPhoneHandlerService, 
                                             public MPhoneHandlerObserver
    {
    protected:  // Constructors and destructor
        
        /**
        * C++ default constructor.
        */
        CPhoneHandlerCallBase();
        
        /**
        * Destructor.
        */
        virtual ~CPhoneHandlerCallBase();

    public: // New functions
    
    public: // Functions from base classes
    
        /**
        * From MPhoneHandlerService. Starts to process an asynchronous request.
        * @since Series 60 3.1
        * @param 
        * @return 
        */
        virtual void Process() = 0;
    
        /**
        * From MPhoneHandlerService. Deletes service object.
        * @since Series 60 3.1
        * @param 
        * @return 
        */
        virtual void Delete() = 0;
    
        /**
        * From MPhoneHandlerObserver. Notifies when asynchronous request has
        * been completed.
        * @since S60 3.1
        * @param aError Result code of request processing.
        * @return void
        */
        virtual void RequestCompleted( const TInt aError ) = 0;
            
    protected:  // New functions
        
        /**
        * Initializes service that executes call handling key presses.
        * @since S60 3.1
        * @param void
        * @return void
        */
        void LoadServiceL();
        
        /**
        * Increments state parameter of service.
        * @since S60 3.1
        * @param void
        * @return void
        */
        void NextState();
        
        /**
        * Called when service isn't running (e.g. when it's completed).
        * @since S60 3.1
        * @param void
        * @return void
        */
        void IdleState();
        
        /**
        * Returns an active object's TRequestStatus for asynchronous calls.
        * @since S60 3.1
        * @param void
        * @return void
        */
        TRequestStatus& GetStatus();
        
        /**
        * Activates an active object for asynchronous calls.
        * @since S60 3.1
        * @param void
        * @return void
        */
        void Activate();    
        
    protected:  // Functions from base classes
    
    private:

    public:     // Data
           
    protected:  // Data
       
        // call handling service states 
        enum TPhoneHandlerServiceState
        {
        EPhoneHandlerStateIdle      = 0,
        EPhoneHandlerState1         = 1,           
        EPhoneHandlerState2         = 2,
        EPhoneHandlerState3         = 3
        };
        
        // stores service state
        TPhoneHandlerServiceState iState;
        
        // pointer to API providing call handling services
        CPhCltCommandHandler* iCommandHandler; // owned

    private:    // Data
                        
        // Provides a handle to a dynamically loadable DLL.
        RLibrary iLibrary;
         
        // Client side remote class of phone server session 
        RPhCltServer iServer; 
        
        // active object for asynchronous requests
        CPhoneHandlerActive* iActive; // owned
                
    public:     // Friend classes
        
    protected:  // Friend classes
        
    private:    // Friend classes
    
    };

#endif      // CPHONEHANDLERCALLBASE   
            
// End of File
