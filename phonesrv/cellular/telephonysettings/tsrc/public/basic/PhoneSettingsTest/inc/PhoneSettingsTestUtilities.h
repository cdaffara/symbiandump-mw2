/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Declaration of CPhoneSettingsTestUtilities class.
*
*/



#ifndef PHONESETTINGSTESTUTILITIES_H
#define PHONESETTINGSTESTUTILITIES_H

#include <e32base.h>

// CLASS DECLARATION

NONSHARABLE_CLASS(CPhoneSettingsTestUtilities) : public CBase     
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CPhoneSettingsTestUtilities* NewL(); 

        /**
        * Destructor.
        */
        virtual ~CPhoneSettingsTestUtilities();

    private:

        /**
        * C++ default constructor.
        */
        CPhoneSettingsTestUtilities(); 

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();        

    public: // Functions
    
        /**
        * OpenPhoneL
        */
        static void OpenPhoneL(
            RTelServer& aTelServer, RMobilePhone& aPhone, RMobileLine& aLine); 	    

        /**
        * ClosePhoneL
        */
        static void ClosePhoneL(
            RTelServer& aTelServer, RMobilePhone& aPhone, RMobileLine& aLine); 	    
    };

#endif      // PHONESETTINGSTESTUTILITIES_H

// End of File