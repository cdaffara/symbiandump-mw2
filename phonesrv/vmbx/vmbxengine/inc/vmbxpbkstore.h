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
*  Interface   : Private, CVmbxPbkStore
*
*/


#ifndef C_VMBXPHBSTORE_H
#define C_VMBXPHBSTORE_H

//  INCLUDES
#include <e32base.h>
#include <etelmm.h>

#include <voicemailboxdefs.h>
#include "voicemailboxdefsinternal.h"

// FORWARD DECLARATION
class CVmbxETelConnection;
class CPhoneBookBuffer;
class CVoiceMailboxEntry;


// CLASS DECLARATION
/**
*  Creates connection to Sim phonebook and handles data flow from/to it
*
*  @lib vmbxengine.lib
*/
NONSHARABLE_CLASS( CVmbxPbkStore ) : public CActive
    {

public:  // Constructors and destructor

    /**
     * C++ 2 constructor.
     */
    static CVmbxPbkStore* NewL();

    /**
     * Destructor.
     */
    virtual ~CVmbxPbkStore();

public: // New functions

    /**
     * Sim write support
     *
     * @return ETrue if writable
     */
    TBool IsWritable();

    /**
     * sim PhoneBook Type
     * @return TVmbxSimPhonebookType
     */
    TVmbxSimPhonebookType PhoneBookType();

    /**
     * Writes an entry into ICC-based phonebook.
     *
     * @param in aEntry Reference of CVoiceMailboxEntry
     * @return KErrNone if succesful,
     *         Other system wide errors.
     **/
    TInt Write( const CVoiceMailboxEntry& aEntry );

    /**
     * Retrieves hanlder of RMobilePhoneBookStore
     *
     * @retuns hanlder of RMobilePhoneBookStore
     */
    RMobilePhoneBookStore& PhonebookStore();

    /**
    * Retrieves mailbox number from Sim
    * @param out aEntry a data type CVoiceMailboxEntry
    */
    void GetL( CVoiceMailboxEntry& aEntry );

private: // from base class CActive

     /**
      * @see CActive::RunL
      */
     void RunL();

     /**
      * @see CActive::DoCancel
      */
     void DoCancel();

    /**
     * @see CActive::RunError
     */
    TInt RunError(TInt aError);

private: // New functions

    /**
     * Reads an entry from the ICC-based phonebook.
     *
     * @param out aEntry Reference of CVoiceMailboxEntry
     **/
    void SimReadL( CVoiceMailboxEntry& aEntry );

    /**
     * Opens VMBX phonebook
     * @retuns KErrNone if succesful
     **/
    TInt OpenVmbxPhonebook();

    /**
     * Opens MBDN phonebook
     * @retuns KErrNone if succesful
     **/
    TInt OpenMbdnPhonebook();

    /**
     * Get Phonebook Info
     * @param out aInfo information from RPhonebookStore
     *
     **/
    TInt GetVmbxInfo( RMobilePhoneBookStore::TMobilePhoneBookInfoV1& aInfo );

    /**
     * Retrieves the Mailbox numbers identifier information
     * @param out aInfo information from RPhonebookStore
     * @retuns KErrNone if succesful
     **/
    TInt GetMbdnInfo( RMobilePhone::TMobilePhoneVoicemailIdsV3& aInfo );


    /**
     * Reads "ParseDataL" from phonebook data from Sim
     *
     * @param out aEntry entry of CVoiceMailboxEntry
     * @param in aPbData
     */
    void ParseDataL( CVoiceMailboxEntry& aEntry, TDes8& aPbData );

    /**
     * Reads "new-entry-tag" from phonebook data from Sim
     *
     * @param in aPbkBuffer Phonebook data buffer
     * @retuns KErrNone if succesful
     */
    TInt ReadNewEntryTag( CPhoneBookBuffer* aPbkBuffer );

    /**
     * Reads tags from buffer retrieved from sim
     *
     * @param in aPbkBuffer Phonebook data buffer
     * @param out aEntry Reference of CVoiceMailboxEntry
     */
    void ReadPbkDataL( CPhoneBookBuffer* aPbkBuffer, 
                        CVoiceMailboxEntry& aEntry );

    /**
     * Checks sim file existing
     *
     * @retuns ETrue if file exists
     **/
    TBool IsSimFileExisting( const TVmbxSimPhonebookType aType );

    /**
     * Sim write acces support
     *
     * @return ETrue if writable access
     */
    TBool IsWriteAccess();

protected:

    /**
     * C++ constructor.
     *
     */
    CVmbxPbkStore();

    /**
     * By default Symbian 2nd phase constructor is protected.
     */
    void ConstructL();

protected: // data

    /**
     *Pointer to Telephony server and phone connection
     *Own
     */
     CVmbxETelConnection* iETelConnection;

    /**
     * Phonebook store client
     */
    RMobilePhoneBookStore iPhoneBook;

    /**
     * Sim Phonebook type
     */
    TVmbxSimPhonebookType iPhoneBookType;

    /**
     * CActiveSchedulerWait
     * Own.
     */
    CActiveSchedulerWait* iWait;


    /**
     * Asynchronous operation type of sim card
     */
    TVmbxSimAsynType iAsynType;

    };

#endif      // C_VMBXPHBSTORE_H
