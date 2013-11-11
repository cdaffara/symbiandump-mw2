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
* Description:  Declaration of CPhoneHandlerActive class.
*
*/


#ifndef CPHONEHANDLERACTIVE_H
#define CPHONEHANDLERACTIVE_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MPhoneHandlerObserver;

// CLASS DECLARATION

/**
*  Active object used to call asynchronous requests.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerActive ) : public CActive
    {
    public: // Constructors and destructor
    
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerActive* NewL( MPhoneHandlerObserver& aObserver );
    
        /**
        * Destructor.
        */
        ~CPhoneHandlerActive();
    
    public: // New functions
    
        /**
        * From CActive. Indicates that the active object has issued a request 
        * and that it is now outstanding.  
        * @since S60 3.1
        * @return 
        */
        void SetActive();
        
    public: // Functions from base classes
        
    protected:  // New functions
    
    protected:  // Functions from base classes
    
        /**
        * From CActive. Handles an active object’s request completion event.
        */
        void RunL();
    
        /**
        * From CActive. Implements cancellation of an outstanding request.
        */
        void DoCancel();
    
        /**
        * From CActive. Handles a leave occurring in the request completion 
        * event handler RunL().
        */
        TInt RunError( TInt aError );

    private:
        
        /**
        * C++ default constructor.
        */
        CPhoneHandlerActive( MPhoneHandlerObserver& aObserver );
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    public:     // Data
    
    protected:  // Data
    
    private:    // Data
        
        // Notifies caller of an asynchronous request when request has been 
        // completed.  
        MPhoneHandlerObserver& iObserver;
        
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes
    };

#endif // CPHONEHANDLERACTIVE_H

// End of File
