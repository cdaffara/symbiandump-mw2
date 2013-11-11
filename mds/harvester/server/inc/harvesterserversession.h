/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester servers session*
*/


#ifndef __CHARVESTERSERVERSESSION_H__
#define __CHARVESTERSERVERSESSION_H__

// INCLUDE FILES
#include <e32base.h>

// LOCAL INCLUDES
#include "harvesterserver.h"

// CLASS DECLARATION
/**
* CServerSession.
*  An instance of class CHarvesterServerSession is created for each client.
*/
class CHarvesterServerSession : public CSession2
    {
    public: // Constructors and destructors

        /**
        * NewL.
        * Two-phased constructor.
        * @param aServer  Harvester server implementation
        * @return Pointer to created CHarvesterServerSession object.
        */
        static CHarvesterServerSession* NewL( CHarvesterServer& aServer );

        /**
        * NewLC.
        * Two-phased constructor.
        * @param aServer  Harvester server implementation
        * @return Pointer to created CHarvesterServerSession object.
        */
        static CHarvesterServerSession* NewLC( CHarvesterServer& aServer );

        /**
        * ~CHarvesterServerSession 
        * Destructor.
        */
        virtual ~CHarvesterServerSession(); 


    protected:
    
       /**
        * ~ServiceL 
        * Protected ServiceL method
        * @param aMessage  Message receved from the Harvester Client
        */
        void ServiceL( const RMessage2& aMessage );
        
        virtual void Disconnect(const RMessage2 &aMessage);

    private: // Constructors and destructors

        /**
        * CMdSServerSession.
        * C++ default constructor.
        * @param aServer  Harvester server implementation
        */
        CHarvesterServerSession( CHarvesterServer& aServer );

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

        /**
        * Reference to Harvester Server implementation
        */
        CHarvesterServer& iServer;

    };

#endif //  __CHARVESTERSERVERSESSION_H__




