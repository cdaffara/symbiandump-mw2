/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Base class for subsessions.
*
*
*/


#ifndef CPOSLMSUBSESSIONBASE_H
#define CPOSLMSUBSESSIONBASE_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CPosLmServer;

/**
* Base class for subsessions.
*/
class CPosLmSubsessionBase : public CObject
    {

    public:  // Data types

        /**
        *  Specifies the type of a subsession class.
        */
        enum TSubsessionType
            {
            EPosLocalAccessSubsession = 0,
            /**< subclass CPosLmLocalAccessSubsession */
            EPosEventListenerSubsession,
            /**< subclass CPosLmEventListenerSubsession */
            EPosLocalDbManProviderSubsession
            /**< subclass CPosLmLocalDbManProviderSubsession */
            };

    public: // Constructors and destructor

        /**
        * Destructor.
        */
        virtual ~CPosLmSubsessionBase();

    public: // New functions

        /**
        * Returns the subsession type.
        *
        * @returns The subsession type.
        */
        TSubsessionType SubsessionType() const;

        /**
        * Handles the servicing of client requests to the server.
        *
        * @param aMessage The message containing the client request.
        */
        virtual void ServiceL(
        /* IN */        const RMessage2& aMessage
        ) = 0;

    protected:

        /**
        * C++ default constructor.
        *
        * @param aSubsessionType Subsession type.
        * @param aLmServer Reference to the landmark server.
        */
        CPosLmSubsessionBase(
        /* IN */ TSubsessionType aSubsessionType,
        /* IN */ CPosLmServer* aLmServer);

        /**
        * Retrieves the landmarks server instance.
        *
        * @return A pointer to the landmarks server object.
        */
        CPosLmServer* Server();



    private:    // Data

        TSubsessionType iSubsessionType;
        CPosLmServer*   iLmServer;


    };

#endif //CPOSLMSUBSESSIONBASE_H

// End of File
