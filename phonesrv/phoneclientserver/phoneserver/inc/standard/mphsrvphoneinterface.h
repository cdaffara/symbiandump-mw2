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
* Description:  Phone Interface.
*
*/


#ifndef MPHSRVPHONEINTERFACE_H
#define MPHSRVPHONEINTERFACE_H


// FORWARD DECLARATIONS
class RTelServer;
class RMobilePhone;
class RMmCustomAPI;


// CLASS DECLARATION

/**
*  Phone interfaces for other classes.
*
*  @since 1.0
*/
class MPhSrvPhoneInterface
    {
    public: // New functions

        /**
        * Return a connection to ETel.
        *
        * @return An ETel connection.
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        virtual RTelServer& PhSrvETelServer() = 0;

        /**
        * Return a connection to the phone.
        *
        * @return Phone connection.
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        virtual RMobilePhone& PhSrvMobilePhone() = 0;

        /**
        * Return a connection to the phone.
        *
        * @return Phone connection.
        */
        /*****************************************************
        *   Series 60 Customer / ETel
        *   Series 60  ETel API
        *****************************************************/
        virtual RMmCustomAPI& PhSrvCustomPhone() = 0;
    };

#endif // MPHSRVPHONEINTERFACE_H


// End of File
