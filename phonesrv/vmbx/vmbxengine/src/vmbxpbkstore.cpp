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
* Description:  Implementation of the CVmbxPbkStore class
*
*/


// INCLUDE FILES
#include <commonphoneparser.h>
#include <mpbutil.h>
#include <cvoicemailboxentry.h>

#include "vmbxutilities.h"
#include "vmbxetelconnection.h"
#include "vmbxlogger.h"
#include "vmbxpbkstore.h"

#include <e32property.h>
#include <simutils.h>
#include <startupdomainpskeys.h>
//CONSTANTS
// Amount of retries to be performed.
const TInt KVmbxPhonebookBufferSize( 150 );

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVmbxPbkStore::CVmbxPbkStore
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVmbxPbkStore::CVmbxPbkStore(): CActive(EPriorityStandard ),
                                iAsynType( EVmbxSimDefault )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::CVmbxPbkStore =>" );
    CActiveScheduler::Add( this );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::CVmbxPbkStore <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::~CVmbxPbkStore
// destructor
// ---------------------------------------------------------------------------
//
CVmbxPbkStore:: ~CVmbxPbkStore()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::~CVmbxPbkStore =>" );
    Cancel();
    iPhoneBook.Close();
    delete iWait;
    delete iETelConnection;
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::~CVmbxPbkStore <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::NewL
// two phase constructor
// ---------------------------------------------------------------------------
//
CVmbxPbkStore* CVmbxPbkStore::NewL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::NewL =>" );
    CVmbxPbkStore* self = new( ELeave ) CVmbxPbkStore();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::NewL <=" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ConstructL
// two phase constructor
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ConstructL =>" );
    TInt value( 0 );
    TInt res = RProperty::Get( KPSUidStartup, KPSSimStatus, value );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ConstructL res = %d", res );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ConstructL value = %d", value );
    if ( (ESimUsable != value && ESimReadable != value ) || KErrNone != res )
        {
        // Sim card not ready to use
        User::Leave( KErrNotReady );
        }
    // Open tel server and phone
    iETelConnection = CVmbxETelConnection::NewL();

    iWait = new( ELeave ) CActiveSchedulerWait; 
    TVmbxAlsLineType alsLine = VmbxUtilities::AlsLine();

    // Supported ALS line,ALS line on
    if ( EVmbxAlsLineDefault != alsLine )
        {
        // open 6f17 file ,if not found the file, leave
        User::LeaveIfError( OpenVmbxPhonebook() );
        }
    // Not supported ALS line,ALS line off
    else
        {
        // Open 6fc7 file, if not found, open 6f17 file
        TInt result = OpenMbdnPhonebook();
        if ( KErrPathNotFound == result )
            {
            //close 6fc7 and open 6f17 file
            iPhoneBook.Close();
            // open 6f17 file ,if not found the file, leave
            User::LeaveIfError( OpenVmbxPhonebook() );
            }
        else
            {
            User::LeaveIfError( result );
            }
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetVmbxInfo
// Activates phonebook info query
// ---------------------------------------------------------------------------
TInt CVmbxPbkStore::GetVmbxInfo( 
        RMobilePhoneBookStore::TMobilePhoneBookInfoV1& aInfo )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetVmbxInfo =>" );
    TInt result( KErrInUse );
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1Pckg
                                            InfoPckg( aInfo );
    // get Info from phonebook
    if( !IsActive() && !iWait->IsStarted() )
        {
        iPhoneBook.GetInfo( iStatus, InfoPckg );
        iAsynType = EVmbxSimGetInfo;
        SetActive();
        iWait->Start();

        result = iStatus.Int();
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::PhonebookInfo: \
                SIM Phonebook info read, status: %I", result );
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetVmbxInfo <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::IsWritable
// Sim write support
// ---------------------------------------------------------------------------
//
TBool CVmbxPbkStore::IsWritable()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::IsWritable =>" );
    TBool result( EFalse );
    if ( IsWriteAccess() )
        {
        // Get current sim entry, then write the same entry 
        // to sim if read successufully
        CVoiceMailboxEntry* simEntry(NULL);
        TRAPD( newErr, simEntry = CVoiceMailboxEntry::NewL() );
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::IsWritable newErr = %d ",
            newErr );
        if ( KErrNone == newErr )
            {
            // get als line info
            TVmbxAlsLineType alsLine = VmbxUtilities::AlsLine();
            simEntry->SetVmbxAlsLineType( alsLine );
            // ALS line off
            if ( IsSimFileExisting(EMBDNPhoneBook) && (EVmbxAlsLineDefault == alsLine) )
                {
                // ALS line on and 6f17 exist 
                // just check 6f17 file write access
                iPhoneBookType = EMBDNPhoneBook;
                }
            else
                {
                // ALS line on, only should write to 6fc7; ALS off, 6f17 file inexist
                //  check 6fc7 file write access
                iPhoneBookType = EVMBXPhoneBook;
                }
            simEntry->SetVoiceMailboxType( EVmbxVoice );
            simEntry->SetServiceId( KVmbxServiceVoice );
            TRAPD( err, SimReadL( *simEntry ) );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWritable read %I <=", err );
            if ( KErrNotFound == err )
                {
                simEntry->SetVmbxNumber( KNullDesC );
                }
            if ( KErrNone == err || KErrNotFound == err )
                {
                err = Write( *simEntry );
                VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWritable write %I <=",
                    err );
                // If write successfully, means writable
                if ( KErrNone == err )
                    {
                    result = ETrue;
                    }
                }
            }
        delete simEntry;
        simEntry = NULL;
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWritable result %I <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::PhoneBookType
// PhoneBookType
// ---------------------------------------------------------------------------
//
TVmbxSimPhonebookType CVmbxPbkStore::PhoneBookType()
    {
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::PhoneBookType type %I", 
                         iPhoneBookType );
    return iPhoneBookType;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::Write
// Write to SIM
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::Write( const CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::Write =>" );
    TInt result( KErrInUse );
    TBuf8<KVmbxPhonebookBufferSize> pbData;

    CPhoneBookBuffer* pbkBuffer = new CPhoneBookBuffer();
    
    if ( !pbkBuffer )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::Write: \
        Phonebook creation error" );
        result = KErrNoMemory;
        }
    else
        {
        pbkBuffer->Set( &pbData );
        TInt activeAlsLine = aEntry.VmbxAlsLineType();
        // Add index, const value for vmbx write.
        int entryIndex = 1;
        // New entry
        result = pbkBuffer->AddNewEntryTag();
        if ( KErrNone == result )
            {
            // Type of index is TUint16 in Multimode ETel and TInt in old ETel.
            result = pbkBuffer->PutTagAndValue( 
            RMobilePhoneBookStore::ETagPBAdnIndex, (TUint16)entryIndex );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::Write: ETagPBAdnIndex \
                    result=%I",  result );
            // Add name, Type of ETagPBText is TDes16
            TPtrC vmbxName( KNullDesC );
            aEntry.GetVmbxName( vmbxName );
            result = pbkBuffer->PutTagAndValue( 
            RMobilePhoneBookStore::ETagPBText, vmbxName );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::Write: ETagPBText\
                    result=%I",  result );
            // Add number, Type of ETagPBNumber is TDes16
            TPtrC vmbxNumber( KNullDesC );
            aEntry.GetVmbxNumber( vmbxNumber );
            result = pbkBuffer->PutTagAndValue( 
            RMobilePhoneBookStore::ETagPBNumber, vmbxNumber );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::Write: ETagPBNumber\
                    result=%I",  result );
            }

        if ( KErrNone == result )
            {
            if ( iPhoneBookType == EMBDNPhoneBook )
                {
                RMobilePhone::TMobilePhoneVoicemailIdsV3 mbdnInfo;
                result = GetMbdnInfo( mbdnInfo );

                if ( KErrNone == result )
                    {
                    TInt index = mbdnInfo.iVoice;
                    if( !IsActive() && !iWait->IsStarted() )
                        {
                        // write vmbx number to 6fc7 file
                        iPhoneBook.Write( iStatus, pbData, index );
                        // Wait for asynchronous call to finish
                        iAsynType = EVmbxSimEntryWrite;
                        SetActive();
                        iWait->Start();
                        result = iStatus.Int();
                        }
                    }
                VMBLOGSTRING( "Mbdn writing" );
                }
            else
                {
                if( !IsActive() && !iWait->IsStarted() )
                    {
                    // write vmbx number to 6f17 file
                    iPhoneBook.Write( iStatus, pbData, entryIndex );
                    iAsynType = EVmbxSimEntryWrite;
                    // Wait for asynchronous call to finish
                    SetActive();
                    iWait->Start();
                    result = iStatus.Int();
                    }
                VMBLOGSTRING( "CVmbxPbkStore::Write 6f17 writing" );
                }
            }
        }
    delete pbkBuffer;
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::Write: result=%I<=",  result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::PhonebookStore
// Return RMobilePhoneBookStore
// ---------------------------------------------------------------------------
RMobilePhoneBookStore& CVmbxPbkStore::PhonebookStore()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::PhonebookStore <=>" );
    return iPhoneBook;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetMbdnInfo
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::GetMbdnInfo( RMobilePhone::TMobilePhoneVoicemailIdsV3& aInfo )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo =>" );
    TInt result( KErrInUse );
    // Get identifiers
    if ( !IsActive() && !iWait->IsStarted() )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo: GetMailboxNumbers" );
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: iStatus %I",
                                                             iStatus.Int() );
        RMobilePhone::TMobilePhoneVoicemailIdsV3Pckg infoPckg( aInfo );
        iETelConnection->Phone().GetMailboxNumbers( iStatus, infoPckg );
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo :iStatus %I",
                                                             iStatus.Int() );
        iAsynType = EVmbxSimMbdnInfo;
        // Wait for asynchronous call to finish
        SetActive();
        iWait->Start();

        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: iVoice original value %I",
                                                             aInfo.iVoice );
        if ( iStatus.Int() == KErrNotFound )
            {
            result = KErrNone;
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetMbdnInfo: KErrNotFound,\
             but igorned and change to KErrNone" );
            }
        else
            {
            result = iStatus.Int();
            }
        TVmbxAlsLineType alsLine = VmbxUtilities::AlsLine();
        if ( EVmbxAlsLineDefault == alsLine )
            {
            aInfo.iVoice =  EVmbxAlsLine1;
            }
        VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: iVoice last value %I",
            aInfo.iVoice );
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetMbdnInfo: result %I<=",
         result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::OpenMbdnPhonebook
// opening mbdn-type phonebook
// ---------------------------------------------------------------------------
//   
TInt CVmbxPbkStore::OpenMbdnPhonebook()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook =>" );
    //Open mbdn-type phonebook , Currently the file not exist, thr return
    // value also KErrNone
    TInt result = iPhoneBook.Open( iETelConnection->Phone(),
                                         KETelIccMbdnPhoneBook );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook :\
        MBDN opening result = %d", result );

    TBool res = IsSimFileExisting( EMBDNPhoneBook );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook :\
        MBDN reading res = %d", res );
    if ( !res )
        {
        result = KErrPathNotFound;
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenMbdnPhonebook res = %d<=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::OpenVmbxPhonebook
// opening vmbx-type phonebook
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::OpenVmbxPhonebook()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook =>" );
    //Open vmbx-type phonebook , Currently the file not exist, thr return
    // value also KErrNone
    TInt result = iPhoneBook.Open( iETelConnection->Phone(),
                                                 KETelIccVoiceMailBox );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook :\
        Vmbx opening result = %d", result );
    TBool res = IsSimFileExisting( EVMBXPhoneBook );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook :\
        Vmbx reading res = %d", res );
    if ( !res )
        {
        result = KErrPathNotFound;
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::OpenVmbxPhonebook result=%d <=", result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::GetL
// Fetches mailbox number from Sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::GetL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetL: iPhoneBookType %d=>",
                         iPhoneBookType );

    if ( EMBDNPhoneBook == iPhoneBookType )
        {
        TRAPD( err, SimReadL( aEntry ) );

        TPtrC vmbxNumber( KNullDesC );
        if ( KErrNone == err )
            {
            err = aEntry.GetVmbxNumber( vmbxNumber );
            }

        // 6fc7 file empty
        if ( ( KErrNone != err ) || ( !vmbxNumber.Length() ) )
            {
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::GetL: \
            no vmbx number in 6fc7 file then read from 6f17 file err%I", err );
            // close 6fc7 and open 6f17 file
            iPhoneBook.Close();
            User::LeaveIfError( OpenVmbxPhonebook() );
            // read vmbx number from 6f17 file
            SimReadL( aEntry );
            }
        }
    else
        {
        SimReadL( aEntry );
        }

    VMBLOGSTRING( "VMBX: CVmbxPbkStore::GetL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::SimReadL
// read vmbx number from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::SimReadL( CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::SimReadL =>" );

    TInt numEntries( 1 );
    TBuf8<KVmbxPhonebookBufferSize> pbData;

    TInt result( KErrInUse );
    if ( iPhoneBookType == EMBDNPhoneBook )
        {
        RMobilePhone::TMobilePhoneVoicemailIdsV3 mbdnInfo;
        result = GetMbdnInfo( mbdnInfo );
        if ( KErrNone == result )
            {
            VMBLOGSTRING( "start MBDN PhoneBook read" );
            if( !IsActive() && !iWait->IsStarted() )
                {
                // read vmbx number from 6fc7 file
                iPhoneBook.Read( 
                iStatus, mbdnInfo.iVoice, numEntries, pbData );
                iAsynType = EVmbxSimEntryRead;
                // Wait for asynchronous call to finish
                SetActive();
                iWait->Start();
                }
            }
        }
    else
        {
        // Record#1 in sim is for line1 number and Record#2 in sim is for
        // line2 number so line is used to fetch
        VMBLOGSTRING( "start VMBX PhoneBook read" );
        TInt activeAlsLine = aEntry.VmbxAlsLineType();
        if ( EVmbxAlsLineDefault == activeAlsLine )
            {
            activeAlsLine = EVmbxAlsLine1;
            }
        if( !IsActive() && !iWait->IsStarted() )
            {
            result = KErrNone;
            // read vmbx number from 6f17 file
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::SimReadLactiveAlsLine = %I",
                 activeAlsLine );
            iPhoneBook.Read( iStatus, activeAlsLine, numEntries, pbData );
            iAsynType = EVmbxSimEntryRead;
            // Wait for asynchronous call to finish
            SetActive();
            iWait->Start();
            }
        }

    User::LeaveIfError( result );
    // asynchronous call finished
    VMBLOGSTRING2( "PhoneBook read iStatus = %I", iStatus.Int() );

    if( iStatus.Int() == KErrNone )
        {
        ParseDataL( aEntry, pbData );
        }
    else
        {
        User::Leave( iStatus.Int() );
        }

    VMBLOGSTRING( "VMBX: CVmbxPbkStore::SimReadL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ParseDataL
// 
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ParseDataL( CVoiceMailboxEntry& aEntry, TDes8& aPbData )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL =>" );
    TInt result( KErrNotFound );

    VMBLOGSTRING2( "CVmbxPbkStore::ParseDataL Lengh = %d", aPbData.Length() );

    if ( aPbData.Length() )
        {
        // -> Search Tags "ETagPBNumber" and "ETagPBText"
        // and read (decode) them
        // create buffer
        CPhoneBookBuffer* pbkBuffer = new( ELeave ) CPhoneBookBuffer();
        CleanupStack::PushL( pbkBuffer );

        // start read
        pbkBuffer->Set( &aPbData );
        pbkBuffer->StartRead();

        result = ReadNewEntryTag( pbkBuffer );
        // Read first "new-entry-tag"
        if ( KErrNone == result )
            {
            ReadPbkDataL( pbkBuffer, aEntry );
            }
        CleanupStack::PopAndDestroy( pbkBuffer );
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ParseDataL: result %I", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL <=" );
    }


// ---------------------------------------------------------------------------
// CVmbxPbkStore::ReadNewEntryTag
// Reads "new-entry-tag" from phonebook data from Sim
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::ReadNewEntryTag( CPhoneBookBuffer* aPbkBuffer )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadNewEntryTag =>" );
    TInt result( KErrNone );
    TUint8 tagValue( 0 );
    CPhoneBookBuffer::TPhBkTagType dataType(
                               CPhoneBookBuffer::EPhBkTypeNoData );

    result = aPbkBuffer->GetTagAndType( tagValue, dataType );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadNewEntryTag result = %d",
     result );

    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadNewEntryTag tagValue = %X",
     tagValue );
    if ( tagValue != RMobilePhoneBookStore::ETagPBNewEntry )
        {
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::ParseDataL: Unknown result" );
        result = KErrArgument;  // Something wrong in TLV
        }
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadNewEntryTag <=" );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::ReadPbkData
// Reads tags from buffer retrieved from sim
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::ReadPbkDataL( CPhoneBookBuffer* aPbkBuffer,
                                         CVoiceMailboxEntry& aEntry )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL =>" );

    TInt result( KErrNone );
    TInt next( KErrNone );
    TBool found( EFalse );
    TUint8 tagValue( 0 );
    CPhoneBookBuffer::TPhBkTagType dataType(
                                          CPhoneBookBuffer::EPhBkTypeNoData );

    result = aPbkBuffer->GetTagAndType( tagValue, dataType );

    // loop through data to find a number and an possible alpha string
    while ( next == KErrNone && result == KErrNone )
        {
        VMBLOGSTRING2("VMBX: CVmbxPbkStore::ReadPbkDataL: tagValue: %d",
         tagValue );
        VMBLOGSTRING2("VMBX: CVmbxPbkStore::ReadPbkDataL: dataType: %d",
         dataType );

        // Check for text field
        if ( tagValue == RMobilePhoneBookStore::ETagPBText )
            {
            VMBLOGSTRING("VMBX: CVmbxPbkStore::ReadPbkDataL: \
                          ETagPBText found Alpha ID" );
            found = ETrue;
            // Alpha string field found from TLV entry,
            // assuming 16bit data
            TPtrC16 alphaPtrC;
            result = aPbkBuffer->GetValue( alphaPtrC );
            
            if ( KErrNone == result )
                {
                // set name to vmbx entry
                result = aEntry.SetVmbxName( alphaPtrC );
                }
            }

        // Check for number field
        else if ( tagValue == RMobilePhoneBookStore::ETagPBNumber )
            {
            VMBLOGSTRING("VMBX: CVmbxPbkStore::ReadPbkDataL: \
                          ETagPBNumber found Number" );
            found = ETrue;
            // Number field found from TLV entry, assuming 16bit data
            TPtrC16 numberPtrC;
            result = aPbkBuffer->GetValue( numberPtrC );

            if ( KErrNone == result )
                {
                // set number to vmbx entry
                result = aEntry.SetVmbxNumber( numberPtrC );
                }
            }
        else
            {
            // skip field
            aPbkBuffer->SkipValue( dataType );
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL: SkipValue" );
            }
        // read next if no errors
        if ( KErrNone == result )
            {
            // Read next field type
            next = aPbkBuffer->GetTagAndType( tagValue, dataType );
            VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadPbkDataL: \
                                    next GetTagAndType = %I", result );
            }
        }

    // Neither alpha string Nor number is found
    if( KErrNone == result && !found )
        {
        result = KErrNotFound;
        }
    
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::ReadPbkDataL result=%I", result );
    User::LeaveIfError( result );
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::ReadPbkDataL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::RunL
// 
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::RunL()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::RunL =>" );
    if ( iWait->IsStarted() )
        {
        // stop blocking
        iWait->AsyncStop();
        VMBLOGSTRING( "VMBX: CVmbxSimHandler::RunL: AsyncStop" );
        }
    iAsynType = EVmbxSimDefault;
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::RunL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::DoCancel
// 
// ---------------------------------------------------------------------------
//
void CVmbxPbkStore::DoCancel()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel =>" );
    if ( iWait->IsStarted() )
        {
        // stop blocking
        iWait->AsyncStop();
        VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel: AsyncStop" );
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::DoCancel: iAsynType %I",iAsynType );
    switch ( iAsynType )    
        {
        // only for USIM
        case EVmbxSimMbdnInfo:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : \
                                        EVmbxSimMbdnInfo" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                              EMobilePhoneGetMailboxNumbers );
            break;
            }
        case EVmbxSimGetInfo:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : \
                                              EVmbxSimGetInfo" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreGetInfo );
            break;
            }            
        case EVmbxSimEntryRead:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : EVmbxSimEntryRead" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreRead );
            break;
            }
        case EVmbxSimEntryWrite:
            {
            VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel : EVmbxSimEntryWrite" );
            iETelConnection->Phone().CancelAsyncRequest( 
                                               EMobilePhoneStoreWrite );
            break;
            }
        default:
            break;
        }
    
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::DoCancel <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::RunError
// 
// ---------------------------------------------------------------------------
//
TInt CVmbxPbkStore::RunError(TInt aError)
    {
    // Avoid warning
    aError = aError;
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::RunError: %I", aError );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::IsSimFileExisting
// check sim file existing or not
// ---------------------------------------------------------------------------
//
TBool CVmbxPbkStore::IsSimFileExisting( const TVmbxSimPhonebookType aType )
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::IsSimFileExisting =>" );
    iPhoneBookType = aType;
    TBool result( ETrue );
    CVoiceMailboxEntry* entry(NULL);
    TRAPD( err, entry = CVoiceMailboxEntry::NewL() );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::IsSimFileExisting err = %d ",
     err );
    if ( KErrNone != err )
        {
        result = EFalse;
        }
    else
        {
        entry->SetVoiceMailboxType( EVmbxVoice );
        TRAPD( err, SimReadL( *entry ) );
        // KErrPathNotFound means when current file path not found.
        if ( KErrPathNotFound == err )
            {
            result = EFalse;
            }
        }
    delete entry;
    entry = NULL;
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore::IsSimFileExisting result = %d <= ",
         result );
    return result;
    }

// ---------------------------------------------------------------------------
// CVmbxPbkStore::IsWriteAccess
// Sim write access support
// ---------------------------------------------------------------------------
//
TBool CVmbxPbkStore::IsWriteAccess()
    {
    VMBLOGSTRING( "VMBX: CVmbxPbkStore::IsWriteAccess =>" );
    TBool result( EFalse );
    RMobilePhoneBookStore::TMobilePhoneBookInfoV1 info;
    TInt temp = GetVmbxInfo( info );
    if( KErrNone == temp )
        {
        result = ( info.iCaps &
                RMobilePhoneBookStore::KCapsWriteAccess ? ETrue : EFalse );
        
        }
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWriteAccess: info.iCaps %X",
                     info.iCaps );
    VMBLOGSTRING2( "VMBX: CVmbxPbkStore:: IsWriteAccess result %I <=", result );
    return result;
    }

//End of file
