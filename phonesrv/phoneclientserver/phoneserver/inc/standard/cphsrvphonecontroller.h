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
* Description:  Phone Controller.
*
*/


#ifndef CPHSRVPHONECONTROLLER_H
#define CPHSRVPHONECONTROLLER_H


// INCLUDES
#include    <etelmm.h>            // ETel
#include <rmmcustomapi.h>         // CustomAPI
#include "mphsrvphoneinterface.h" // The interface class 


// CLASS DECLARATION

/**
*  Interface to the server-wide phone.
*
*  @since 1.0
*/
class CPhSrvPhoneController : 
    public CBase, 
    public MPhSrvPhoneInterface
    {
    public:  // Constructors and destructor

        /**
        * Destructor.
        */
        ~CPhSrvPhoneController();

        /**
        * Symbian OS second phase constructor
        */
        void ConstructL();


    public: // Functions from base classes

        /**
        * @see MPhSrvPhoneInterface
        */
        RTelServer& PhSrvETelServer();

        /**
        * @see MPhSrvPhoneInterface
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobilePhone& PhSrvMobilePhone();

        /**
        * @see MPhSrvPhoneInterface
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMmCustomAPI& PhSrvCustomPhone();


    private:     // Data

        // The RTelServer instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RTelServer iETelServer;

        // The CustomAPI instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMmCustomAPI iCustomPhone;

        // The RMobilePhone instance.
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        RMobilePhone iMobilePhone;
    };

#endif // CPHSRVPHONECONTROLLER_H


// End of File
