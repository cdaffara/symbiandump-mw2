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
* Description: Declaration of MPhoneHandlerObserver interface.
*
*/


#ifndef MPHONEHANDLEROBSERVER_H
#define MPHONEHANDLEROBSERVER_H

// INCLUDES
#include <e32def.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Interface used to notify PhoneCmdHandler objects about completed 
*  asynchronous request. 
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
class MPhoneHandlerObserver
    {
    public:  // Constructors and destructor
        

    public: // New functions
        
        /**
        * Asynchronous request has been completed.
        * @since S60 3.1
        * @param aError Result code of request processing.
        * @return void
        */
        virtual void RequestCompleted( const TInt aError ) = 0;
    
    public: // Functions from base classes

        
    protected:  // New functions
        

    protected:  // Functions from base classes
        

    private:


    public:     // Data
    
    protected:  // Data

    private:    // Data
         
    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };

#endif  //MPHONEHANDLEROBSERVER_H   
            
// End of File
