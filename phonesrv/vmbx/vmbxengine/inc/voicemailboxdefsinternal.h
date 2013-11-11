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
* Description:  Constant used for vmbx engine
*
*/

#ifndef C_VOICEMAILBOXDEFSINTERNAL_H
#define C_VOICEMAILBOXDEFSINTERNAL_H

//  INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KVmLbxItemsArraySize = 3;
const TInt KVmbxPhoneCharMaxLength = 48;

/**Sim card voice mailbox file type*/
enum TVmbxSimPhonebookType
    {
    /**
     *  Sim file using 6f17 file.
     */
    EVMBXPhoneBook = 0,
    /**
     *  Sim file using 6fc7 file.
     */
    EMBDNPhoneBook
    };

/**CenRep Key which storaged value would be changed*/
enum TVmbxCenRepKey
    {
    /**
     *  Storage Voice number of Line Primary.
     */
    EVmbxVoiceLinePrimaryKey = 0,
    /**
     *  Storage Voice number of Line Alternate.
     */
    EVmbxVoiceLineAlternateKey,
    /**
     *  Storage Video number of Line Primary.
     */
    EVmbxVideoLinePrimaryKey,
    /**
     *  Storage Video number of Line Alternate.
     */
    EVmbxVideoLineAlternateKey,
    /**
     *  the max value.
     */
    EVmbxCenRepMax
    };

/**
 *  TVmbxServiceInfo, contains Service information.
 *
 *  @since S60 5.2
 **/
class TVmbxServiceInfo
    {
    public: // Constructors and destructor

        TVmbxServiceInfo()
            {
            iServiceId = 0;
            iSnapId    = 0;
            }

    public: // Data

        // Service's Id
        TUint       iServiceId;

        // Service's SNAP Id
        TUint32     iSnapId;

        // Service's name.
        TBuf<64>    iName;

        // Service's address.
        TBuf<100> iAddress;
    };

/**Asynchronous operation type of sim card*/  
enum TVmbxSimAsynType
    {
    /**
     *  the default value.
     */
    EVmbxSimDefault,
    /**
     *  Retrieves Phonebook stroe information.
     */
    EVmbxSimGetInfo,
    /**
     *  Retrieves Phonebook entry for "voicemail" mailbox number.
     */
    EVmbxSimMbdnInfo,
    /**
     *  Read "voicemail" mailbox number from phonebook store.
     */
    EVmbxSimEntryRead,
    /**
     *  Write "voicemail" mailbox number to phonebook store.
     */
    EVmbxSimEntryWrite
    };
    
/**Note for vmbx ui utilities*/
enum TVmbxNoteType
    {
    EInvalidNumber,
    ESavedToPhoneMemory,
    ESavedToSimMemory,
    EVideoNumberSaved,
    EDefineVoiceNumber,
    EDefineVideoNumber,
    ENotAllowUserEditing
    };   

#endif // C_VOICEMAILBOXDEFSINTERNAL_H
