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
* Description: Declaration of CPhoneHandlerDialCall class.
*
*/



#ifndef CPHONEHANDLERDIALCALL_H
#define CPHONEHANDLERDIALCALL_H

//  INCLUDES
#include "phonehandlercallbase.h" 
#include <RemConExtensionApi.h> 

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CPhoneHandlerControl;

// CLASS DECLARATION

/**
*  Handles dial and speed dial key presses. 
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerDialCall ) : public CPhoneHandlerCallBase
                                             
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerDialCall* NewL(
                CPhoneHandlerControl& aControl, 
                TRemConExtCallHandlingApiOperationId aOperation
                    = ERemConExtDialCall );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneHandlerDialCall();

    public: // New functions
    
    public: // Functions from base classes

         /**
        * From MPhoneHandlerService. Starts processing service.
        * @since S60 3.1
        * @return void
        */
        void Process();
        
        /**
        * From MPhoneHandlerService. Deletes service.
        * @since S60 3.1
        * @return void
        */
        void Delete();
        
        /**
        * From MPhoneHandlerObserver. Notifies when asynchronous request has 
        * been completed.
        * @since S60 3.1
        * @param aError request error code
        * @return void
        */
        void RequestCompleted( const TInt aError );
            
    protected:  // New functions
    
    protected:  // Functions from base classes
          
    private:

        /**
        * C++ default constructor.
        */
        CPhoneHandlerDialCall(      
            CPhoneHandlerControl& aControl, 
            TRemConExtCallHandlingApiOperationId aOperation );
       
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    public:     // Data
    
    protected:  // Data
    
    private:    // Data
        
        // reference to control
        CPhoneHandlerControl& iControl;
        
        // RemConExtensionApi commands which can initiate dial call are 
        // ERemConExtDialCall and ERemConExtSpeedDial.
        // iOperation is used to give response to exact RemConExtensionApi
        // command.
        TRemConExtCallHandlingApiOperationId iOperation;
        
    public:     // Friend classes
    
    protected:  // Friend classes
    
    private:    // Friend classes
    
    };

#endif      // CPHONEHANDLERDIALCALL_H   
            
// End of File
