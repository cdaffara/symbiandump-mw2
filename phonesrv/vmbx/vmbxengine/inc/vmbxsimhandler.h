/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Access class to SIM
*  Interface   : Private, CVmbxSimHandler
*
*/


#ifndef C_VMBXSIMHANDLER_H
#define C_VMBXSIMHANDLER_H

//  INCLUDES
#include <e32base.h>
#include <voicemailboxdefs.h>
#include "voicemailboxdefsinternal.h"


// FORWARD DECLARATION
class CVmbxPbkStore;
class CVoiceMailboxEntry;


// CLASS DECLARATION
/**
*  Creates connection to Sim phonebook and handles data flow from/to it
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxSimHandler ) : public CBase
    {

public:  // Constructors and destructor

    /**
     * Two-phased constructor.
     */
    static CVmbxSimHandler* NewL();

    /**
     * Destructor.
     */
    virtual ~CVmbxSimHandler();

public: // New functions

    /**
     * Retrieves mailbox number from Sim
     * @param in aEntry a data type of CVoiceMailboxEntry
     */
    void GetL( CVoiceMailboxEntry& aEntry );

    /**
     * Saves vmbx number to Sim
     * @param in aEntry a data type of CVoiceMailboxEntry
     * @return Symbian OS error code (KErrNone if successful)
     */
    TInt Save( const CVoiceMailboxEntry& aEntry );

    /**
     * Returns sim write access info
     * @return ETrue if Sim is writable
     **/
    TBool IsWritable();

    /**
     * @see RMobilePhoneStore::NotifyStoreEvent
     *
     */
    void NotifyStoreEvent( TRequestStatus& aStatus,
                                             TUint32& aEvent,
                                             TInt& aIndex ) const;
    /**
     * Retrieves Sim PhoneBook Type
     * @return TVmbxSimPhonebookType
     */
    TVmbxSimPhonebookType PhoneBookType();

    /**
     * CancelAsyncRequest
     * @param aReqToCancel Entry data
     */
    void CancelAsyncRequest( TInt aReqToCancel ) const;

protected:

    /**
     * C++ constructor.
     */
    CVmbxSimHandler();

   /**
    * By default Symbian 2nd phase constructor is private.
    */
   void ConstructL();

private: // data

    /**
     * pbk store
     * Own.
     */
    CVmbxPbkStore* iPbkStore;
    };

#endif      // C_VMBXSIMHANDLER_H
