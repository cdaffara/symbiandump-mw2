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
* Description:  Implementation of the CVoiceMailboxEntry class
*
*/

// System includes
#include <QtTest/QtTest>
#include <cvoicemailboxentry.h>
// For global value
#include "ut_vmbxuiengine.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::NewL
// 
// ---------------------------------------------------------------------------
//

EXPORT_C CVoiceMailboxEntry* CVoiceMailboxEntry::NewL()
    {
    qDebug( "DummyVoiceMailboxEntry::NewL >" );
    CVoiceMailboxEntry* self = CVoiceMailboxEntry::NewLC();
    CleanupStack::Pop( self );
    qDebug( "DummyVoiceMailboxEntry::NewL <" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::NewLC
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CVoiceMailboxEntry* CVoiceMailboxEntry::NewLC()
    {
    qDebug( "DummyVoiceMailboxEntry::NewLC >" );
    CVoiceMailboxEntry* self = new( ELeave ) CVoiceMailboxEntry;
    CleanupStack::PushL( self );
    self->ConstructL();
    qDebug( "DummyVoiceMailboxEntry::NewLC <" );
    return self;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::~CVoiceMailboxEntry
// Destructor
// ---------------------------------------------------------------------------
//
CVoiceMailboxEntry::~CVoiceMailboxEntry()
    {
    qDebug( "DummyVoiceMailboxEntry::~CVoiceMailboxEntry >" );
    delete ivmbxNumber;
    delete iVmbxBrandId;
    delete ivmbxName;
    qDebug( "DummyVoiceMailboxEntry::~CVoiceMailboxEntry <" );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::ServiceId
// Get ServiceId
// ---------------------------------------------------------------------------
//
EXPORT_C TServiceId CVoiceMailboxEntry::ServiceId() const
    {
    qDebug( "DummyVoiceMailboxEntry::ServiceId %d", iVmbxServiceId );
    return iVmbxServiceId;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetServiceId
// Set entry's service id
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetServiceId( 
                        const TServiceId& aVmbxServiceId )
    {
    qDebug( "DummyVoiceMailboxEntry::SetServiceId %d", aVmbxServiceId);
    iVmbxServiceId = aVmbxServiceId;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::VoiceMailboxType
// Returns type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxType CVoiceMailboxEntry::VoiceMailboxType( ) const
    {
    qDebug( "DummyVoiceMailboxEntry::VoiceMailboxType %d", iVmbxType );
    return iVmbxType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVoiceMailboxType
// Set Type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailboxEntry::SetVoiceMailboxType( const TVmbxType& aType )
    {
    qDebug( "DummyVoiceMailboxEntry::SetVoiceMailboxType %d", aType );
    iVmbxType = aType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::VmbxAlsLineType
// Get ALS line Type of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxAlsLineType CVoiceMailboxEntry::VmbxAlsLineType() const
    {
    qDebug( "DummyVoiceMailboxEntry::VmbxAlsLineType >" );
    return iVmbxLineType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxAlsLineType
// Set ALS line Type of the entry instance
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetVmbxAlsLineType( 
                                const TVmbxAlsLineType& aLine )
    {
    qDebug( "DummyVoiceMailboxEntry::SetVmbxAlsLineType %d >", aLine );
    if ( EVmbxAlsLineDefault == aLine  )
        {
        iVmbxLineType = EVmbxAlsLine1;
        }
    else
        {
        iVmbxLineType = aLine;
        }
    qDebug( "DummyVoiceMailboxEntry::SetVmbxAlsLineType %d <", iVmbxLineType );
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxNumber
// Get number or address of the entry instance
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::GetVmbxNumber( TPtrC& aVmbxNumber ) const
    {
    qDebug("DummyVoiceMailboxEntry::GetVmbxNumber >");
    aVmbxNumber.Set(globalNumber.utf16());
    qDebug("DummyVoiceMailboxEntry::GetVmbxNumber %d", globalExpRet);
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxNumber
// Set number or address of the entry instance
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetVmbxNumber( const TDesC& aVmbxNumber )
    {
    qDebug("DummyVoiceMailboxEntry::SetVmbxNumber >");
    globalNumber = QString::fromUtf16(aVmbxNumber.Ptr(), aVmbxNumber.Length());
    qDebug("DummyVoiceMailboxEntry::SetVmbxNumber %d", globalExpRet);
    return globalExpRet;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxAlsLineType
// Get Brand Id of the entry instance
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::GetBrandId( TPtrC8& aBrandId ) const
    {
    qDebug("DummyVoiceMailboxEntry::GetBrandId >");
    TInt result( KErrNotFound ); 
    if ( iVmbxBrandId )
        {
        aBrandId.Set( iVmbxBrandId->Des() );
        result = KErrNone;
        }
    else
        {
        aBrandId.Set( KNullDesC8 );
        }
    qDebug("DummyVoiceMailboxEntry::GetBrandId <");
    return result;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetBrandId
// Set Brand Id of the entry instance
// ---------------------------------------------------------------------------
//
TInt CVoiceMailboxEntry::SetBrandId( const TDesC8& aBrandId )
{
    qDebug("DummyVoiceMailboxEntry::SetBrandId >");
    TInt result( KErrNoMemory );
    if ( KVmbxMaxNumberLength < aBrandId.Length() ){
        result = KErrArgument;
    }else{
        delete iVmbxBrandId;
        iVmbxBrandId = aBrandId.Alloc();
        if ( iVmbxBrandId ){
            result = KErrNone;
        }
    }
    qDebug("DummyVoiceMailboxEntry::SetBrandId <");
    return result;
}

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::GetVmbxName
// Get Name of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::GetVmbxName( TPtrC& aVmbxName ) const
{
    qDebug("DummyVoiceMailboxEntry::GetVmbxName >");
    TInt result( KErrNotFound );
    if ( ivmbxName ){
        aVmbxName.Set( ivmbxName->Des() );
        result = KErrNone;
    }else{
        qDebug( "DummyVoiceMailboxEntry::GetVmbxName:KNullDesC" );
        aVmbxName.Set( KNullDesC );
    }      
    qDebug("DummyVoiceMailboxEntry::GetVmbxName <");
    return result;
}

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetVmbxName
// Set Name of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CVoiceMailboxEntry::SetVmbxName( const TDesC& aVmbxName )
{
    qDebug("DummyVoiceMailboxEntry::SetVmbxName >");
    TInt result( KErrNoMemory );
    if (KVmbxMaxNumberLength < aVmbxName.Length()){
        result = KErrArgument;
    }else{
        delete ivmbxName;
        ivmbxName = aVmbxName.Alloc(); // Returns NULL if fails.
        if ( ivmbxName ){
            result = KErrNone;
        }       
    }
    qDebug("DummyVoiceMailboxEntry::SetVmbxName <");
    return result;
}

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::UsingMemoryLocation
// Returns memory location of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C TVmbxMemoryLocation CVoiceMailboxEntry::UsingMemoryLocation( ) const
    {
    qDebug("DummyVoiceMailboxEntry::UsingMemoryLocation <>");
    return iUsingMemory;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::Reset
// Reset the data members of the entry instance
// ---------------------------------------------------------------------------
//
EXPORT_C void CVoiceMailboxEntry::Reset()
{
    qDebug("DummyVoiceMailboxEntry::Reset >");
    iVmbxServiceId = KVmbxServiceIdNone;
    iVmbxType = EVmbxNone;
    iVmbxLineType = EVmbxAlsLineDefault;
    if (ivmbxNumber){
    	ivmbxNumber->Des().Zero();
    	qDebug("DummyVoiceMailboxEntry::Reset vmbx number");
    }
    if (ivmbxName){
    	ivmbxName->Des().Zero();
    	qDebug("DummyVoiceMailboxEntry::Reset vmbx name");
    }
    if (iVmbxBrandId){
    	iVmbxBrandId->Des().Zero();
    	qDebug("DummyVoiceMailboxEntry::Reset vmbx brandId");
    }
    iUsingMemory = EVmbxSimMemory;
    qDebug("DummyVoiceMailboxEntry::Reset <");
}

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::SetUsingMemoryLocation
// Set using memory of the entry instance
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::SetUsingMemoryLocation( 
                                        const TVmbxMemoryLocation& aType  )
    {
    qDebug("DummyVoiceMailboxEntry::SetUsingMemoryLocation %d", aType);
    iUsingMemory = aType;
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::CVoiceMailboxEntry
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CVoiceMailboxEntry::CVoiceMailboxEntry()
    {
    qDebug("DummyVoiceMailboxEntry::CVoiceMailboxEntry <>");
    }

// ---------------------------------------------------------------------------
// CVoiceMailboxEntry::ConstructL
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CVoiceMailboxEntry::ConstructL()
    {
    qDebug("DummyVoiceMailboxEntry::ConstructL <>");
    }

// End of file
