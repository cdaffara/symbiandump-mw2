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
* Description:  Class used to set & get the trigger attributes
*
*/

#include "lbttriggernode.h"
#include "lbtcontextsourceplugindebug.h"


// ---------------------------------------------------------------------------
// TTriggerNode::SetTriggerNameL
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetTriggerNameL( const TPtrC& aTriggerName )
    {
    // Check that the name is not NULL,if Yes then 
    // leave with KErrArgument
    if( aTriggerName == KNullDesC )
        {
        User::Leave( KErrArgument );
        }
    
    // Check that the length of the name is <= 256,if not then
    // extract only upto 256 characters before setting
    if( aTriggerName.Length() > KMaxNameLength )
        {
        iTriggerName.Set( aTriggerName.Mid( 0,KMaxNameLength ) );
        }
    else
        {
        iTriggerName.Set( aTriggerName );
        }
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetLatitudeL
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetLatitudeL( const TPtrC& aLatitude )
    {
    LBTCONTEXTSOURCEPLUGINDEBUG ( "TTriggerNode::SetLatitudeL - IN" );
    // Check that the value is not NULL
    if( aLatitude == KNullDesC )
        {
        // set value to NaN & leave with KErrArgument
        TRealX nan;
        nan.SetNaN();
        iLatitude = nan;
        User::Leave( KErrArgument );
        }
    
    // Convert the string to TReal64
    // holds the parsed number in text form
    TBuf<KRealWidth> parsedLatitude;
    TLex lexer( aLatitude );
    User::LeaveIfError( lexer.Val( iLatitude,KRealPoint ) );
    // Format the real number to round off to 6 decimal digits
    // initialize the TRealFormat variable 
    TRealFormat realFormat;
    realFormat.iPoint = KRealPoint;
    realFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    realFormat.iPlaces = KCoordinateDecimalPlaces;
    realFormat.iWidth = KRealWidth;
    User::LeaveIfError( parsedLatitude.Num( iLatitude,realFormat ) );
    // convert the formattted string back to TReal64
    TLex lex( parsedLatitude );
    User::LeaveIfError( lex.Val( iLatitude,KRealPoint ) );
    
    // Check the range of the latitude value,if not within range
    // leave with KErrArgument
    if ( !( iLatitude >= KMinLatitude && iLatitude <= KMaxLatitude ) )
        {
        User::Leave( KErrArgument );
        }
    LBTCONTEXTSOURCEPLUGINDEBUG ( "TTriggerNode::SetLatitudeL - OUT" );
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetLongitudeL
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetLongitudeL( const TPtrC& aLongitude )
    {
    // Check that the value is not NULL
    if( aLongitude == KNullDesC )
        {
        // set value to NaN & leave with KErrArgument
        TRealX nan;
        nan.SetNaN();
        iLatitude = nan;
        User::Leave( KErrArgument );
        }
    
    // Convert the string to TReal64
    // holds the parsed number in text form
    TBuf<KRealWidth> parsedLongitude;
    TLex lexer( aLongitude );
    User::LeaveIfError( lexer.Val( iLongitude,KRealPoint ) );
    // Format the real number to round off to 6 decimal digits
    // initialize the TRealFormat variable 
    TRealFormat realFormat;
    realFormat.iPoint = KRealPoint;
    realFormat.iType = KRealFormatFixed | KDoNotUseTriads;
    realFormat.iPlaces = KCoordinateDecimalPlaces;
    realFormat.iWidth = KRealWidth;
    User::LeaveIfError( parsedLongitude.Num( iLongitude,realFormat ) );
    // convert the formattted string back to TReal64
    TLex lex( parsedLongitude );
    User::LeaveIfError( lex.Val( iLongitude,KRealPoint ) );
    
    // Check the range of the latitude value,if not within range
    // leave with KErrArgument
    if ( !( iLongitude >= KMinLongitude && iLongitude <= KMaxLongitude ) )
        {
        User::Leave( KErrArgument );
        }
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetRegionL
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetRegionL( const TPtrC& aRegion )
    {
    // Check that the value is not NULL
    if( aRegion == KNullDesC )
        {
        User::Leave( KErrArgument );
        }
    // check that the region specified is a circle,since it is the 
    // only permissible value else leave with KErrArgument
    // Convert the value read from file to lower case inorder to compare
    // the values
    TInt length = aRegion.Length();
    HBufC * buffer = HBufC::NewL( length );
    CleanupStack::PushL( buffer );
    TPtr ptr = buffer->Des();
    ptr.Copy( aRegion );
    ptr.LowerCase();

    if ( ! ( ptr.CompareC( KRegionValue ) ) )
        {
        iRegion.Set( aRegion );
        }
    else
        {
        User::Leave( KErrArgument );
        }
    CleanupStack::PopAndDestroy( buffer );
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetRadiusL
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetRadiusL( const TPtrC& aRadius )
    {
    // Check that the value is not NULL
    if( aRadius == KNullDesC )
        {
        // set value to NaN & leave with KErrArgument
        TRealX nan;
        nan.SetNaN();
        iRadius = nan;
        User::Leave( KErrArgument );
        }

    // Check the radius value,
    // Minimum radius must be 100 meters
    TLex lex( aRadius );
    TReal radiusValue = 0;
    User::LeaveIfError( lex.Val( radiusValue ) );
    
    // Check that the radius value is positive else leave with KErrArgument
    if( radiusValue < 0 )
        {
        User::Leave( KErrArgument );
        }
    
    if ( radiusValue < KMinRadius )
        {
        iRadius = KMinRadius;
        }
    else
        {
        iRadius = radiusValue;
        }
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetDirection
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetDirectionL( const TPtrC& aDirection )
    {
    // If the value is null,then set default value to 
    // CLbtTriggerConditionArea::EFireOnEnter
    if( aDirection == KNullDesC )
        {
        iDirection = CLbtTriggerConditionArea::EFireOnEnter;
        }
    
    // Compare the value set in the file with the permissible values
    // If it doesnt match ,then set the default value
    // Convert the value read to lower case to compare the strings
    TInt length = aDirection.Length();
    HBufC * buffer = HBufC::NewL( length );
    TPtr ptr = buffer->Des();
    ptr.Copy( aDirection );
    ptr.LowerCase();

    if( !( ptr.CompareC( KDirectionValueOnEntry ) ) )
        {
        iDirection = CLbtTriggerConditionArea::EFireOnEnter;
        }
    else if( !( ptr.CompareC( KDirectionValueOnExit ) ) )
        {
        iDirection = CLbtTriggerConditionArea::EFireOnExit;
        }
    else
        {
        iDirection = CLbtTriggerConditionArea::EFireOnEnter;
        }
    
    delete buffer;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetContextSource
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetContextSource( const TPtrC& aSetName )
    {
    // If the value is null,then set default context source value to "Location"
    if( aSetName == KNullDesC )
        {
        iSetName.Set( KDefaultContextSourceName );
        }
    
    // Check that the length of the name is <= 256,if not then
    // extract only upto 256 characters before setting
    if( aSetName.Length() > KMaxNameLength )
        {
        iSetName.Set( aSetName.Mid( 0,KMaxNameLength ) );
        }
    else
        {
        iSetName.Set(aSetName);
        }
    }

// ---------------------------------------------------------------------------
// TTriggerNode::SetCurrentState
// ---------------------------------------------------------------------------
//
void TTriggerNode::SetCurrentStateL( const TPtrC& aCurrentState )
    {
    // If the value is null,then set default value to 
    // CLbtTriggerEntry::EStateEnabled
    if( aCurrentState == KNullDesC )
        {
        iCurrentState = CLbtTriggerEntry::EStateEnabled;
        }
    
    // Compare the value set in the file with the permissible values
    // If it doesnt match ,then set the default value
    // Convert the value read to lower case to compare the strings
    TInt length = aCurrentState.Length();
    HBufC * buffer = HBufC::NewL( length );
    TPtr ptr = buffer->Des();
    ptr.Copy( aCurrentState );
    ptr.LowerCase();

    if( !( ptr.CompareC( KCurrentStateOn ) ) )
        {
        iCurrentState = CLbtTriggerEntry::EStateEnabled;
        }
    else if( !( ptr.CompareC( KCurrentStateOff ) ) )
        {
        iCurrentState = CLbtTriggerEntry::EStateDisabled;
        }
    else
        {
        iCurrentState = CLbtTriggerEntry::EStateEnabled;
        }
    
    delete buffer;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetTriggerName
// ---------------------------------------------------------------------------
//
TInt TTriggerNode::GetTriggerName( TPtrC& aTriggerName )
    {
    if ( iTriggerName == KNullDesC )
        {
        return KErrNotFound;
        }

    aTriggerName.Set( iTriggerName );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetLatitude
// ---------------------------------------------------------------------------
//
TInt TTriggerNode::GetLatitude( TReal& aLatitude )
    {
    if ( Math::IsNaN( iLatitude ) ) 
        {
        return KErrNotFound;
        }

    aLatitude = iLatitude;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetLongitude
// ---------------------------------------------------------------------------
//
TInt TTriggerNode::GetLongitude( TReal& aLongitude )
    {
    if ( Math::IsNaN( iLongitude ) ) 
        {
        return KErrNotFound;
        }

    aLongitude = iLongitude;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetDirection
// ---------------------------------------------------------------------------
//
CLbtTriggerConditionArea::TDirection TTriggerNode::GetDirection()
    {
    return iDirection;
    }



// ---------------------------------------------------------------------------
// TTriggerNode::GetRegion
// ---------------------------------------------------------------------------
//
TInt TTriggerNode::GetRegion( TPtrC& aRegion )
    {
    if ( iRegion == KNullDesC )
        {
        return KErrNotFound;
        }

    aRegion.Set( iRegion );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetRadius
// ---------------------------------------------------------------------------
//
TInt TTriggerNode::GetRadius( TReal& aRadius )
    {
    if ( Math::IsNaN( iRadius ) ) 
        {
        return KErrNotFound;
        }

    aRadius = iRadius;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetContextSource
// ---------------------------------------------------------------------------
//
TPtrC& TTriggerNode::GetContextSource()
    {
    return iSetName;
    }

// ---------------------------------------------------------------------------
// TTriggerNode::GetCurrentState
// ---------------------------------------------------------------------------
//
CLbtTriggerEntry::TLbtTriggerState TTriggerNode::GetCurrentState()
    {
    return iCurrentState;
    }

