/*
* Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the CVoiceMailbox class
*
*/

// System includes
#include <QtTest/QtTest>
#include <cvoicemailbox.h>
#include <cvoicemailboxentry.h>
// For global export return value
#include "ut_vmbxuiengine.h"

// ============================ MEMBER FUNCTIONS =============================

 TInt TVoiceMailboxParams::Version()
    {
    qDebug("TVoiceMailboxParams::Version <>");
    return iVersion;
    }
    

 TVoiceMailboxParams::TVoiceMailboxParams():
                                        iType( EVmbxNone ),
                                        iServiceId( KVmbxServiceIdNone ),
                                        iLineType( EVmbxAlsLineDefault )
    {
    qDebug("TVoiceMailboxParams default constructor <>");
    }
    
 TVoiceMailboxParams::TVoiceMailboxParams( 
                        const CVoiceMailboxEntry& aEntry )
    {
    qDebug("TVoiceMailboxParams Entry cnstructor >");
    iType = aEntry.VoiceMailboxType();
    iServiceId = aEntry.ServiceId();
    iLineType = aEntry.VmbxAlsLineType();
    qDebug("TVoiceMailboxParams Entry cnstructor <");
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::CVoiceMailboxImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVoiceMailbox::CVoiceMailbox()
    {
    qDebug("DummyVoiceMailbox::CVoiceMailbox <>");
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVoiceMailbox::ConstructL()
    {
    qDebug("DummyVoiceMailbox::ConstructL <>");
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NewL
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVoiceMailbox* CVoiceMailbox::NewL()
{
    qDebug("DummyVoiceMailbox::NewL" );

    CVoiceMailbox * self = NULL;
    self = CVoiceMailbox::NewLC();
    CleanupStack::Pop( self );
    qDebug("DummyVoiceMailbox::NewL <");
    return self;   
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NewLC
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CVoiceMailbox* CVoiceMailbox::NewLC()
    {
    qDebug("DummyVoiceMailbox::NewLC >");
    CVoiceMailbox* self = new( ELeave ) CVoiceMailbox;
    CleanupStack::PushL( self );
    self->ConstructL();
    qDebug("DummyVoiceMailbox::NewLC <");
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::~CVoiceMailbox
// Destructor
// ---------------------------------------------------------------------------
//
CVoiceMailbox::~CVoiceMailbox()
    {
    qDebug("DummyVoiceMailbox::~CVoiceMailbox > <");
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::GetStoredEntry
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::GetStoredEntry( 
    const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry) const
    {
    qDebug("DummyVoiceMailbox::GetStoredEntry >");
    Q_UNUSED(aParams);
    TRAPD (err, aEntry = CVoiceMailboxEntry::NewL());    
    qDebug("DummyVoiceMailbox::GetStoredEntry err %d<", err);
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::SaveEntry
// Destructor
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::SaveEntry(const CVoiceMailboxEntry& aEntry)
    {
    qDebug("DummyVoiceMailbox::SaveEntry <>");
    Q_UNUSED(aEntry);
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryNewEntry
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryNewEntry(
        const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry)
    {
    qDebug("DummyVoiceMailbox::QueryNewEntry <>");
    Q_UNUSED(aParams);
    TRAPD (err, aEntry = CVoiceMailboxEntry::NewL());
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryChangeEntry
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryChangeEntry( 
        const TVoiceMailboxParams& aParams, CVoiceMailboxEntry*& aEntry)
    {
    qDebug("DummyVoiceMailbox::QueryChangeEntry <>");
    Q_UNUSED(aParams);
    TRAPD (err, aEntry = CVoiceMailboxEntry::NewL());
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::QueryVmbxType
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::QueryVmbxType(
                            TVoiceMailboxParams& aParams )
    {
    qDebug("DummyVoiceMailbox::QueryVmbxType <>");
    aParams.iType = EVmbxVoice;
    aParams.iServiceId = KVmbxServiceVoice;
    aParams.iLineType = EVmbxAlsLine1;
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeL
//
// ---------------------------------------------------------------------------
//
void CVoiceMailbox::NotifyVmbxNumberChangeL(
    MVoiceMailboxObserver& aObserver, const TBool aNotifyOnActiveLineOnly )
    {
    qDebug("DummyVoiceMailbox::NotifyVmbxNumberChangeL <>");
    Q_UNUSED(aObserver);
    Q_UNUSED(aNotifyOnActiveLineOnly);
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::NotifyVmbxNumberChangeCancel
//
// ---------------------------------------------------------------------------
//
void CVoiceMailbox::NotifyVmbxNumberChangeCancel()
    {
    qDebug("DummyVoiceMailbox::NotifyVmbxNumberChangeCancel <>");
    }

// ---------------------------------------------------------------------------
//  CVoiceMailbox::CheckConfiguration
//
// ---------------------------------------------------------------------------
//
TBool CVoiceMailbox::CheckConfiguration(
    const TVoiceMailboxParams& aParams, const TInt aFlags )
    {
    qDebug("DummyVoiceMailbox::CheckConfiguration <>");
    Q_UNUSED(aParams);
    Q_UNUSED(aFlags);
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailbox::GetServiceIds
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::GetServiceIds( RIdArray& aProfileIds ) const
    {
    qDebug("DummyVoiceMailbox::GetServiceIds <>");
    Q_UNUSED(aProfileIds);
    return 0;
    }


// ---------------------------------------------------------------------------
// CVoiceMailbox::SaveProvisionedEntry
//
// ---------------------------------------------------------------------------
//
TInt CVoiceMailbox::SaveProvisionedEntry( 
                                const CVoiceMailboxEntry& aEntry)
    {
    qDebug("DummyVoiceMailbox::SaveProvisionedEntry <>");
    Q_UNUSED(aEntry);
    return globalExpRet;
    }

// End of file
