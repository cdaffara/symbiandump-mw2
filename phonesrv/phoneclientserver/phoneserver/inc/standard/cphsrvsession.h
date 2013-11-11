/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Server Session.
*
*/


#ifndef CPHSRVSESSION_H
#define CPHSRVSESSION_H

// INCLUDES
#include <e32base.h>
#include "phcltclientserver.h" 

// FORWARD DECLARATIONS
class CPhSrvServer;
class CPhSrvCallNotify;
class CPhSrvReqManager;
class CPhSrvExtCall;
class CPhSrvSubSessionBase;

// CLASS DECLARATION

/**
*  It is the session class for Phone Server.
*
*  @since 1.0
*/
class CPhSrvSession : public CSession2
    {
    friend class CPhSrvSubSessionBase;


    public: // Constructors and destructor
        
        /**
         * Two-phased constructor.
         *
         * @param aServer It is the main server object.
         * @return Returns a new session.
         */
        static CPhSrvSession* NewL( 
            CPhSrvServer& aServer );

        /**
        * Destructor.
        */
        ~CPhSrvSession();


    public: // Functions from base classes

        /**
        * From CSession2, it is called whenever request is processed.
        * 
        * @param aMessage It is the request.
        */
        void ServiceL( const RMessage2& aMessage );


    public: // Sub-session access

        /**
        * Phone Server reference.
        *
        * @return The phone server.
        */
        CPhSrvServer& PhoneServer() const;

        /**
        * Get information of the count of subsessions.
        *
        * @return The number of subsession objects.
        */
        TInt SubSessionCount() const;

        /**
        * Return a specific subsession.
        *
        * @return The subsession at location aIndex.
        */
        CPhSrvSubSessionBase& SubSessionAt(TInt aIndex) const;

        /**
        * Closes subsession by handle.
        *
        * @param aMessage It contains the handle to the subsession.
        */
        void CloseSubSession( const RMessage2& aMessage );

        /**
        * Panics the client thread.
        *
        * @param aMessage The client to be panicked.
        * @param aPanic It is the reason for panic.
        */
        void PanicClient(
            const RMessage2& aMessage,
            TPhCltServerInitiatedPanic aPanic ) const;
            
        /**
        * Writes to client address space inside and
        * catch bad descriptors.
        *
        * @param aMessage The client.
        * @param aLocation The location where the data is written.
        * @param aDes The data that is written.
        * @param aOffset The data location where to start the writing.
        */
        void Write( 
            const RMessage2& aMessage,
            TInt aLocation,
            const TDesC8& aDes,
            TInt aOffset = 0 );

        /**
        * Reads from client address space and
        * catch bad descriptors.
        *
        * @param aMessage The client.
        * @param aLocation The location where the data is read.
        * @param aDes The read data storage.
        * @param aOffset The data location where to start the reading.
        */
        void Read(
            const RMessage2& aMessage,
            TInt aLocation,
            TDes8& aDes,
            TInt aOffset = 0 );

        /**
        * Reads from client address space and
        * catch bad descriptors (unicode aware)
        *
        * @param aMessage The client.
        * @param aLocation The location where the data is read.
        * @param aDes The read data storage.
        * @param aOffset The data location where to start the reading.
        */
        void Read(
            const RMessage2& aMessage,
            TInt aLocation,
            TDes& aDes,
            TInt aOffset = 0 );

        /**
        * Complete create all request.
        *
        * @param aError The error code that is returned.
        */
        void CompleteCreateAll( TInt aError );


    private:

        /**
        * C++ constructor.
        */
        CPhSrvSession();
        
        /**
        * Symbian OS constructor.
        */
        void ConstructL( CPhSrvServer& aServer );

        /**
        * Return the unique handle associated with this subsession
        *
        * @return The unique handle associated with a subsession.
        */
        TInt SubSessionUniqueHandle( 
            const CPhSrvSubSessionBase& aSubSession ) const;

        /**
        * Service a request - called by ServiceL within a trap
        * harness so that we can catch leaves, and complete messages
        * properly.
        * 
        * @param aMessage The message from the client [sub]session.
        */
        void ProcessRequestL( const RMessage2& aMessage );

        /**
        * Handle function.
        *
        * @param aFunction function.
        * @return ETrue iff handled here.
        */
        TBool HandleCommandL( const RMessage2& aMessage );


    private:    // Data

        // The container for all objects in this session (i.e. the 
        // subsession CObject's from the client-side).
        CObjectCon*     iContainer;

        // The index of open objects associated with this session.
        CObjectIx*      iObjectIx;

        // Server.
        CPhSrvServer*   iServer;

        // Flag, ETrue if message ok.
        TBool           iCreateAll;

        // Message for create all.
        RMessage2       iCreateAllMsg;
    };

#endif      // CPHSRVSESSION_H
            
// End of File
