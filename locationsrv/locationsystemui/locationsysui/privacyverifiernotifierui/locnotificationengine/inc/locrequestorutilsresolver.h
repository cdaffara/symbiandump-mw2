/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Requestor utilities, supports Resolving of Requestors.
*
*/


#ifndef CLOCREQUESTORUTILSRESOLVER_H
#define CLOCREQUESTORUTILSRESOLVER_H

//  INCLUDES
#include <e32base.h>

//FORWARD DECLARATION
class CPosRequestor;
class QLocNotificationEngine;

// CLASS DECLARATION

/**
*   
*  
*  
*/
class CLocRequestorUtilsResolver  :	public CBase
    {
    public: // Constructors and destructor

        /**
        * Symbian OS constructor
        * @param None
        * @return new object
        */
        IMPORT_C static CLocRequestorUtilsResolver* NewL();

        /**
        * Destructor.
        */
        ~CLocRequestorUtilsResolver();

	public: // Implement base class virtual methods
       /**
        * Resolve the requestors. 
        * @param[IN,OUT] aRequestors Array of Requestors,ownership is transferred to caller
         
        */
        IMPORT_C void ResolveRequestorsL( 
        				RPointerArray<CPosRequestor>& aRequestors );

      
    private: // Constructors
        /**
        * C++ default constructor.
        */
        CLocRequestorUtilsResolver();

		/**
		* The Symbian 2nd Phase Constructor is always Private
		*/
		void ConstructL();

		
  
    private: // member data
    
		// Qt implementation class for notification engine.Ownership remains with object
        QLocNotificationEngine* iEngine;
    };

#endif // CLOCREQUESTORUTILSRESOLVER_H
            
// End of File
