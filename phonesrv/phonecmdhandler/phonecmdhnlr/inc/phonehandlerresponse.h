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
* Description: Declaration of CPhoneHandlerResponse class.
*
*/


#ifndef CPHONEHANDLERRESPONSE_H
#define CPHONEHANDLERRESPONSE_H

//  INCLUDES
#include "phonehandlerservice.h" 
#include "phonehandlerobserver.h" 
#include <RemConExtensionApi.h> 

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CPhoneHandlerControl;
class CPhoneHandlerActive;

// CLASS DECLARATION

/**
*  Handles answer call key press.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerResponse ) : public CBase,
                                             public MPhoneHandlerService,
                                             public MPhoneHandlerObserver
                                               
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerResponse* NewL( CPhoneHandlerControl& aControl );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneHandlerResponse();

    public: // New functions
    
        /**
        * Sets response message. This method has to be called before Process() 
        * is called.
        * @since S60 3.1
        * @param aOperation operation to which response is sent
        * @param aError Symbian OS error code
        * @return void
        */
        void SetResponse( TRemConExtCallHandlingApiOperationId aOperation, 
                          TInt aError );
                
    public: // Functions from base classes
        
        /**
        * From MPhoneHandlerService. Sends response to RemCon FW.
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
        CPhoneHandlerResponse( CPhoneHandlerControl& aControl );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    public:     // Data
    
    protected:  // Data
    
    private:    // Data
    
        // active object for asynchronous requests
        CPhoneHandlerActive* iActive;
        
        // reference to control
        CPhoneHandlerControl& iControl;
        
        // iOperation is used to give response to exact RemConExtensionApi
        // command
        TRemConExtCallHandlingApiOperationId iOperation;
        
        // Response code
        TInt iError;
    
    public:     // Friend classes
   
    protected:  // Friend classes
   
    private:    // Friend classes
   
    };

#endif      // CPHONEHANDLERRESPONSE_H   
            
// End of File
