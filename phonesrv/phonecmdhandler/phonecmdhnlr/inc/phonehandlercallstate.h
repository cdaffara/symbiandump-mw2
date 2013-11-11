/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPhoneHandlerCallState class.
*
*
*/


#ifndef CPHONEHANDLERCALLSTATE_H
#define CPHONEHANDLERCALLSTATE_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CPhoneHandlerControl;

// CLASS DECLARATION

/**
*  Active object that listens to S60 call states.
*
*  @lib RemConAsy
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerCallState ) : public CActive
    {
    public: // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerCallState* NewL( CPhoneHandlerControl& aControl );
    
        /**
        * Destructor.
        */
        ~CPhoneHandlerCallState();
    
    public: // New functions
                
    public: // Functions from base classes
        
    protected:  // New functions
    
    protected:  // Functions from base classes
    
        /**
        * From CActive. Handles S60 start up state change event.
        */
        void RunL();
    
        /**
        * From CActive. Implements cancellation of an outstanding Subscibe() 
        * request.
        */
        void DoCancel();
    
    private:
        
        /**
        * C++ default constructor.
        */
        CPhoneHandlerCallState( CPhoneHandlerControl& aControl );
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Starts to listen change in S60 start up state.
        **/
        void Subscribe();
    
    public:     // Data
    
    protected:  // Data
    
    private:    // Data
    
        // Publish and Subscribe handle used to listen changes in call states 
        RProperty iProperty;
                        
        // reference to control
        CPhoneHandlerControl& iControl;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
    };

#endif // CPHONEHANDLERCALLSTATE_H

// End of File
