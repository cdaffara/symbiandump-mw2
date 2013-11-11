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

#include "ut_spsbufferedpublisher.h"
#include <EUnitMacros.h>
#include <EUnitDecorators.h>

#include <e32property.h>

#include "spsbufferedpublisher.h"
#include "spdefaultvalues.h"

// - Construction -----------------------------------------------------------

Ut_CSpsBufferedPublisher* Ut_CSpsBufferedPublisher::NewL()
    {
    
    Ut_CSpsBufferedPublisher* self = new (ELeave) Ut_CSpsBufferedPublisher;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

Ut_CSpsBufferedPublisher::~Ut_CSpsBufferedPublisher()
    {
    }

Ut_CSpsBufferedPublisher::Ut_CSpsBufferedPublisher()
    {
    }

void Ut_CSpsBufferedPublisher::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// - Test methods -----------------------------------------------------------



void Ut_CSpsBufferedPublisher::SetupL(  )
    {
    iPublisher = CSpsBufferedPublisher::NewL( KUidSystemCategory, KSPNotifyChangeKey );
    iPublisher->Start();
    }
    

void Ut_CSpsBufferedPublisher::Teardown(  )
    {
    delete iPublisher;
    iPublisher = NULL;
    }

void Ut_CSpsBufferedPublisher::Ut_ConstructL()
    {
    // Do nothing
    }

void Ut_CSpsBufferedPublisher::Ut_CSpsBufferedPublisher_DataSizeL()
    {
    const TUint32 data( 0xFFFFFFFF );
    CSpsBufferedPublisher::SetL( KUidSystemCategory, KSPNotifyChangeKey, data );
    RArray<TUint32> array;
    iPublisher->GetL( array );
    EUNIT_ASSERT_EQUALS( 1, array.Count() );
    EUNIT_ASSERT_EQUALS( data, array[0] );
    array.Close();
    }
    
void Ut_CSpsBufferedPublisher::Ut_CSpsBufferedPublisher_BufferSizeL()
    {
    TInt testedBuffer = 100000;
    
    for( TInt i(0); i < testedBuffer; i++ )
        {
        CSpsBufferedPublisher::SetL( KUidSystemCategory, KSPNotifyChangeKey, i );
        }
    
    RArray<TUint32> array;
    iPublisher->GetL( array );
    EUNIT_ASSERT_EQUALS( 127, array.Count() );

    for( TInt i(127); i > 0; i-- )
        {
        EUNIT_ASSERT_EQUALS( testedBuffer - i, array[array.Count()-i] );
        }
        
        
    array.Close();
    }

void Ut_CSpsBufferedPublisher::Ut_CSpsBufferedPublisher_DataValidyL()
    {
    
    for( TInt i(0); i < 100000; i++ )
        {
        CSpsBufferedPublisher::SetL( KUidSystemCategory, KSPNotifyChangeKey, i );
        
        RArray<TUint32> array;
        iPublisher->GetL( array );
        EUNIT_ASSERT_EQUALS( 1, array.Count() );
        EUNIT_ASSERT_EQUALS( i, array[0] );
        array.Close();
        }
    
    RArray<TUint32> array;
    iPublisher->GetL( array );
    EUNIT_ASSERT_EQUALS( 0, array.Count() );
    array.Close();
    }

// - EUnit test table -------------------------------------------------------

EUNIT_BEGIN_TEST_TABLE(
    Ut_CSpsBufferedPublisher,
    "Add test suite description here.",
    "UNIT" )
    
EUNIT_TEST(
    "Construct - destruct",
    "CSpsBufferedPublisher",
    "Construct - destruct",
    "FUNCTIONALITY",
    SetupL, Ut_ConstructL, Teardown)

EUNIT_TEST(
    "Test DataSize",
    "CSpsBufferedPublisher",
    "Test DataSize",
    "FUNCTIONALITY",
    SetupL, Ut_CSpsBufferedPublisher_DataSizeL, Teardown)
    
EUNIT_TEST(
    "Test BufferSize",
    "CSpsBufferedPublisher",
    "Test BufferSize",
    "FUNCTIONALITY",
    SetupL, Ut_CSpsBufferedPublisher_BufferSizeL, Teardown)
    
EUNIT_TEST(
    "Test DataValidy",
    "CSpsBufferedPublisher",
    "Test DataValidy",
    "FUNCTIONALITY",
    SetupL, Ut_CSpsBufferedPublisher_DataValidyL, Teardown)

EUNIT_END_TEST_TABLE
