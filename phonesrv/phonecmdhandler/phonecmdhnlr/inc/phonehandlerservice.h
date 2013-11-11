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
* Description: Declaration of MPhoneHandlerService interface.
*
*/


#ifndef MPHONEHANDLERSERVICE_H
#define MPHONEHANDLERSERVICE_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Interface to control objects that perform call handling key presses. 
*
*  @lib PhoneCmdHandler
*  @since S60 3.1
*/
class MPhoneHandlerService 
    {
    public:  // Constructors and destructor
    
    public: // New functions
        
        /**
        * Starts to process an asynchronous request.
        * @since Series 60 3.1
        * @param 
        * @return 
        */
        virtual void Process() = 0;
    
        /**
        * Deletes service object.
        * @since Series 60 3.1
        * @param 
        * @return 
        */
        virtual void Delete() = 0;
        
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

#endif      // MPHONEHANDLERSERVICE_H   
            
// End of File

