/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  
 *
 */
 
#include <smcmockclassincludes.h>
#include <rsssettings.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RSSSettings::RSSSettings
// -----------------------------------------------------------------------------
//
RSSSettings::RSSSettings(  )
    {
    
    }


// -----------------------------------------------------------------------------
// RSSSettings::Close
// -----------------------------------------------------------------------------
//
void RSSSettings::Close(  )
    {
    SMC_MOCK_METHOD0( void )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Open
// -----------------------------------------------------------------------------
//
TInt RSSSettings::Open(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Open
// -----------------------------------------------------------------------------
//
TInt RSSSettings::Open( 
        RTelServer * aTelServer )
    {
    SMC_MOCK_METHOD1( TInt, RTelServer *, aTelServer )
    }


// -----------------------------------------------------------------------------
// RSSSettings::RegisterAll
// -----------------------------------------------------------------------------
//
TInt RSSSettings::RegisterAll( 
        MSSSettingsObserver & aObserver )
    {
    //SMC_MOCK_METHOD1( TInt, MSSSettingsObserver &, aObserver )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Register
// -----------------------------------------------------------------------------
//
TInt RSSSettings::Register( 
        TSSSettingsSetting aSetting,
        MSSSettingsObserver & aObserver )
    {
    //SMC_MOCK_METHOD2( TInt, TSSSettingsSetting, aSetting, 
    //    MSSSettingsObserver &, aObserver )
    }


// -----------------------------------------------------------------------------
// RSSSettings::CancelAll
// -----------------------------------------------------------------------------
//
void RSSSettings::CancelAll( 
        MSSSettingsObserver & aObserver )
    {
    //SMC_MOCK_METHOD1( void, MSSSettingsObserver &, aObserver )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Cancel
// -----------------------------------------------------------------------------
//
void RSSSettings::Cancel( 
        TSSSettingsSetting aSetting,
        MSSSettingsObserver & aObserver )
    {
    //SMC_MOCK_METHOD2( void, TSSSettingsSetting, aSetting, 
    //    MSSSettingsObserver &, aObserver )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Get
// -----------------------------------------------------------------------------
//
TInt RSSSettings::Get( 
        TSSSettingsSetting aSetting,
        TInt & aValue )
    {
    SMC_MOCK_METHOD2( TInt, TSSSettingsSetting, aSetting, 
        TInt &, aValue )
    }


// -----------------------------------------------------------------------------
// RSSSettings::Set
// -----------------------------------------------------------------------------
//
TInt RSSSettings::Set( 
        TSSSettingsSetting aSetting,
        TInt aValue )
    {
    SMC_MOCK_METHOD2( TInt, TSSSettingsSetting, aSetting, 
        TInt, aValue )
    }


// -----------------------------------------------------------------------------
// RSSSettings::HandleSIMChanged
// -----------------------------------------------------------------------------
//
TInt RSSSettings::HandleSIMChanged(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RSSSettings::PreviousCugValue
// -----------------------------------------------------------------------------
//
TInt RSSSettings::PreviousCugValue( 
        TInt & aPreviousValue )
    {
    SMC_MOCK_METHOD1( TInt, TInt &, aPreviousValue )
    }


// -----------------------------------------------------------------------------
// RSSSettings::ResetPreviousCugValue
// -----------------------------------------------------------------------------
//
TInt RSSSettings::ResetPreviousCugValue(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RSSSettings::HandleRefresh
// -----------------------------------------------------------------------------
//
TInt RSSSettings::HandleRefresh(  )
    {
    SMC_MOCK_METHOD0( TInt )
    }


// -----------------------------------------------------------------------------
// RSSSettings::IsValueValidCugIndex
// -----------------------------------------------------------------------------
//
TBool RSSSettings::IsValueValidCugIndex( 
        const TInt & aValue ) const
    {
    SMC_MOCK_METHOD1( TBool, const TInt &, aValue )
    }


