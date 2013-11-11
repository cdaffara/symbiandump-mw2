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
* Description: Declaration of CPhoneHandlerLastNumberRedial class.
*
*/



#ifndef CPHONEHANDLERLASTNUMBERREDIAL_H
#define CPHONEHANDLERLASTNUMBERREDIAL_H

//  INCLUDES
#include "phonehandlercallbase.h" 
#include <RemConExtensionApi.h> 
#include <f32file.h>        
#include <logcli.h>
#include <logview.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CPhoneHandlerControl;

/**
*  Handles last number redial key press.
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
NONSHARABLE_CLASS( CPhoneHandlerLastNumberRedial ) : public CPhoneHandlerCallBase
                                                     
    {
    public: // Constructors and destructor         
    
        /**
        * Two-phased constructor.
        */
        static CPhoneHandlerLastNumberRedial* 
            NewL( CPhoneHandlerControl& aControl,
                  TRemConExtCallHandlingApiOperationId aOperation 
                    = ERemConExtLastNumberRedial );
        
        /**
        * Destructor.
        */
        virtual ~CPhoneHandlerLastNumberRedial();
        
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
        CPhoneHandlerLastNumberRedial( 
            CPhoneHandlerControl& aControl,
            TRemConExtCallHandlingApiOperationId aOperation );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
    
    public:     // Data
    
    protected:  // Data
    
    private:    // Data
            
        // A handle to a file server session.
        RFs iFsSession;

        // Log Engine implementation
        CLogClient* iLogClientPtr; // owned

        // A view on a recent event list.
        CLogViewRecent* iLogViewRecentPtr; // owned
                
        // reference to control
        CPhoneHandlerControl& iControl;
        
        // iOperation is used to give response to exact RemConExtensionApi
        // command
        TRemConExtCallHandlingApiOperationId iOperation;
        
    public:     // Friend classes
    
    protected:  // Friend classes
    
    private:    // Friend classes
         
    };


#endif  // CPHONEHANDLERLASTNUMBERREDIAL_H
            
// End of File
