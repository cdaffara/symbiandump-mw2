/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#include "commontestclass.h"
#include <mpxsubscription.h>

// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXSubscriptionNewL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXSubscriptionNewL(CStifItemParser& /*aItem*/)
    {
    FTRACE( FPrint( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::NewL begin") ) );
    iLog->Log( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::NewL begin") );
    TInt result = KErrNone;
    if( !iSubscription )
    	{
    	TRAP( result,iSubscription = CMPXSubscription::NewL() );
    	//iSubscription = CMPXSubscription::NewL();
    	if( result == KErrNone )
    		{
    		iLog->Log( _L("Successfully create instance of CMPXSubscription, none error returns.\r\n") );
    		}
    	else
    		{
    		iLog->Log( _L("Failed to create instance of CMPXSubscription,error %d returns.\r\n"),result );
    		}
    	}
    else
    	{
		iLog->Log( _L("instance of CMPXSubscription has already existed.\r\n") );
    	}
    return result;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXSubscriptionNewLWithediaArrayL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXSubscriptionNewLWithMediaArrayL(CStifItemParser& /*aItem*/)
    {
    FTRACE( FPrint( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::NewL(CMPXMediaArray& aMediaArray) begin") ) );
    iLog->Log( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::NewL(CMPXMediaArray& aMediaArray) begin") );
    TInt result = KErrNone;
    if( !iSubscription )
    	{
    	TRAP( result,iSubscription = CMPXSubscription::NewL( *iMPXMediaArray ) );
    	//iSubscription = CMPXSubscription::NewL( *iMPXMediaArray );
    	if( result == KErrNone )
    		{
    		iLog->Log( _L("Successfully create instance of CMPXSubscription, none error returns.\r\n") );
    		}
    	else
    		{
    		iLog->Log( _L("Failed to create instance of CMPXSubscription,error %d returns.\r\n"),result );
    		}
    	}
    else
    	{
		iLog->Log( _L("instance of CMPXSubscription has already existed.\r\n") );
    	}
    return result;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXSubscriptionAddItemL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXSubscriptionAddItemL(CStifItemParser& /*aItem*/)
    {
    FTRACE( FPrint( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::AddItemL() begin") ) );
    iLog->Log( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::AddItemL() begin") );
    TInt result = KErrNone;
    CMPXSubscriptionItem* subscriptionItem = CMPXSubscriptionItem::NewL();
    CleanupStack::PushL( subscriptionItem );
    TRAP( result,iSubscription->AddItemL( *subscriptionItem ) );
    //iSubscription->AddItemL( *subscriptionItem );
    CleanupStack::Pop();
    if( result == KErrNone )
    	{
    	iLog->Log( _L("Successfully add item to CMPXSubscription instance.No error returns.\r\n") );
    	}
    else
    	{
    	iLog->Log( _L("Failed to add item to CMPXSubscription instance.Error %d returns.\r\n"),result );
    	}
    return result;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXSubscriptionItemsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXSubscriptionItemsL(CStifItemParser& /*aItem*/)
    {
    FTRACE( FPrint( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::ItemsL() begin") ) );
    iLog->Log( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::ItemsL() begin") );
    TInt result = KErrNone;
    
    const CMPXMediaArray* mediaArray;
    TRAP( result,mediaArray = iSubscription->ItemsL() );
    //mediaArray = iSubscription->ItemsL(); 
    //TInt count = mediaArray->Count();
    
    if( result == KErrNone )
    	{
    	iLog->Log( _L("Successfully get CMPXMediaArray pionter.No error returns.\r\n") );
    	}
    else
    	{
    	iLog->Log( _L("Failed to get CMPXMediaArray pionter.Error %d returns.\r\n"),result );
    	}
    return result;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CMPXSubscriptionIsEqualOperator()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CMPXSubscriptionIsEqualOperator(CStifItemParser& aItem)
    {
    FTRACE( FPrint( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::operator==() begin") ) );
    iLog->Log( _L("CCommonTestClass::CMPXSubscription testing CMPXSubscription::operator==() begin") );
    TInt result = KErrNone;
    TBool IsEqual;
    
    TInt EqualTag;
    if( aItem.GetNextInt( EqualTag ) == KErrNone )
    	{
    	if( EqualTag )
    		{
    	    CMPXSubscription* subscription = CMPXSubscription::NewL( *iMPXMediaArray );
    	    IsEqual = iSubscription->operator==( *subscription );
    		}
    	else
    		{
    		CMPXMediaArray* mediaArray = CMPXMediaArray::NewL();
    		CMPXSubscription* subscription = CMPXSubscription::NewL( *mediaArray );
    		
    	    CMPXSubscriptionItem* subscriptionItem = CMPXSubscriptionItem::NewL();
    	    CleanupStack::PushL( subscriptionItem );
    	    TRAP( result,iSubscription->AddItemL( *subscriptionItem ) );
    	    //iSubscription->AddItemL( *subscriptionItem );
    	    
    	    CMPXSubscriptionItem* subscriptionItem2 = CMPXSubscriptionItem::NewL();
    	    CleanupStack::PushL( subscriptionItem2 );
    	    TRAP( result,iSubscription->AddItemL( *subscriptionItem2 ) );
    	    //iSubscription->AddItemL( *subscriptionItem2 );
    	    CleanupStack::Pop( 2 );
    	    
    		IsEqual = iSubscription->operator==( *subscription );
    		}
    	
        if( IsEqual )
        	{
        	iLog->Log( _L("The 2 CMPXSubscription instances are equal.\r\n") );
        	}
        else
        	{
        	iLog->Log( _L("The 2 CMPXSubscription instances are not equal.\r\n"),result );
        	}
    	}
    else
    	{
    	iLog->Log( _L("Bad test parameter is passed in, leave here.\r\n"),result );
    	result = KErrBadTestParameter;
    	User::Leave( result );
    	}
    return result;
    }


