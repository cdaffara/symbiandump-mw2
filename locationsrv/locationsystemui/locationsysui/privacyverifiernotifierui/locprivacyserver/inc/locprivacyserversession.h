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
* Description:  Server session class definition.
*
*/


#ifndef CLOCPRIVACYSERVERSESSION_H
#define CLOCPRIVACYSERVERSESSION_H

//  INCLUDES
#include <e32base.h>
#include <lbs/epos_cposrequestor.h>

// FORWARD DECLARATIONS
class CPosPrivacyServer;
class RPosRequestorStack;

// CLASS DECLARATION

/**
 *  The EPos privacy server session class.
 *
 */
class CLocPrivacyServerSession : public CSession2
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * @param aServer Reference to the server Active Object
         * @return Pointer to a new instance of CPosPrivacyServerSession
         */
        static CLocPrivacyServerSession* NewL(
        /* IN  */   CLocPrivacyServer&  aServer
        );

        /**
         * C++ destructor
         */
        ~CLocPrivacyServerSession();

    private: // Private Constructing

        /**
         * C++ private constructor
         */
        CLocPrivacyServerSession();

        /**
         * EPOC default constructor.
         * @param aServer Reference to the server Active Object
         */
        void ConstructL(
        /* IN   */  CLocPrivacyServer& aServer
        );

    public:    // Functions from base classes

        /**
        * From CSession2.
        * This function services all requests from clients.
        *
        * @param aMessage the message that should be serviced
        */
        void ServiceL(
        /* IN  */   const RMessage2& aMessage
        );

    private: // New functions

        /**
        * Closes the session.
        */
        void CloseSession();

        // Contains the actual ServiceL code.
        //
        void DispatchL(const RMessage2& aMessage);

      
            
    private:

        // By default, prohibit copy constructor
        CLocPrivacyServerSession( const CLocPrivacyServerSession& );
        // Prohibit assigment operator
        CLocPrivacyServerSession& operator= ( const CLocPrivacyServerSession& );

    private:    // Data
        // Buffer for hoding resolved requestors
        CBufFlat* iBufFlat;
      };

#endif      // CLOCPRIVACYSERVERSESSION_H

// End of File
