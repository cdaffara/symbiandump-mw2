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


#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
#include <e32svr.h>
#include <hal.h>

const TInt KMtpLogBufferSize = 1000;

CMmMtpDpPerfLog* CMmMtpDpPerfLog::NewL( const TDesC& aTitle )
    {
    CMmMtpDpPerfLog* self = new( ELeave ) CMmMtpDpPerfLog();
    CleanupStack::PushL( self );
    self->ConstructL( aTitle );
    CleanupStack::Pop( self );
    
    return self;
    }

void CMmMtpDpPerfLog::ConstructL( const TDesC& aTitle )
    {
    iOverflowHandler.SetOwner( this );
    HAL::Get( HAL::ENanoTickPeriod, iNTickPeriod );
    
    iTitle = HBufC16::NewL( aTitle.Length() );
    
    TPtr title = iTitle->Des();
    
    title.Copy( aTitle );
    }

CMmMtpDpPerfLog::CMmMtpDpPerfLog() : iDescription( 2 ),
                                    iStartTick( 2 ),
                                    iTotalTime( 2 ),
                                    iTotalUsage( 2 ),
                                    iLastIndex( 0 )
    {
    }

CMmMtpDpPerfLog::~CMmMtpDpPerfLog()
    {
    iDescription.Reset();
    iStartTick.Reset();
    iTotalTime.Reset();
    iTotalUsage.Reset();
    delete iTitle;
    }

void CMmMtpDpPerfLog::Write( const TDesC& aText )
    {
    RDebug::Print( aText );
    }

void CMmMtpDpPerfLog::WriteFormat( TRefByValue<const TDesC> aFmt, ... )
    {
    VA_LIST list;
    VA_START( list, aFmt );

    HBufC* buf = HBufC::New( KMtpLogBufferSize );
    if ( buf != NULL )
        {
        buf->Des().AppendFormatList( aFmt, list, &iOverflowHandler );
        Write( *buf );
        delete buf;
        }
    }

void CMmMtpDpPerfLog::Start( const TDesC& aDescription )
    {
    TInt index = 0;
    TInt err = KErrNone;
    
    // check last remember index first, to minimize looping string search
    if ( ( iLastIndex >= 0 ) && ( iLastIndex < iDescription.Count() ) )
        {
        if ( iDescription[iLastIndex].Compare( aDescription ) == 0 )
            index = iLastIndex;
        else
            {    
            for ( index = 0; index < iDescription.Count(); index++ )
                {
                if ( index == iLastIndex )
                    continue;
                else if ( iDescription[index].Compare( aDescription ) == 0 )
                    break;
                }
            }
        }
    
    if ( index == iDescription.Count() )
        {
        TRAP( err, iDescription.AppendL( aDescription ) );
        
        if ( err == KErrNone )
            TRAP( err, iStartTick.AppendL( 0 ) );
        
        if ( err == KErrNone )
            TRAP( err, iTotalTime.AppendL( 0 ));
        
        if ( err == KErrNone )
            TRAP( err, iTotalUsage.AppendL( 0 ) );
        
        if ( err == KErrNone )
            WriteFormat( _L( "CMmMtpDpPerfLog::Start, '%S' is created..." ), &aDescription );
        else
            {
            WriteFormat( _L( "CMmMtpDpPerfLog::Start, '%S' cannot be created!!!" ), &aDescription );
            return;
            }
        }
    
    iStartTick[index] = User::NTickCount();
    // remember for next time, in case being ::Stop is call immediately afterward
    iLastIndex = index;
    }

void CMmMtpDpPerfLog::Stop( const TDesC& aDescription )
    {
    TUint32 endtick = User::NTickCount();
    TInt index = 0;

    // check last remember index first, to minimize looping string search
    if ( ( iLastIndex >= 0 ) && ( iLastIndex < iDescription.Count() ) )
        {
        if ( iDescription[iLastIndex].Compare( aDescription ) == 0 )
            index = iLastIndex;
        else
            {    
            for ( index = 0; index < iDescription.Count(); index++ )
                {
                if ( index == iLastIndex )
                    continue;
                else if ( iDescription[index].Compare( aDescription ) == 0 )
                    break;
                }
            }
        }
    
    if ( index == iDescription.Count() )
        {
        WriteFormat( _L( "CMmMtpDpPerfLog::Stop, '%S' cannot be found!!!" ), &aDescription );
        return;
        }
    
    TUint microsecTaken = ( endtick-iStartTick[index] ) * iNTickPeriod;
    
    iTotalTime[index] += microsecTaken;
    iTotalUsage[index]++;
    
    TBuf<3> lastTimeDecimal;
    TBuf<3> totalTimeDecimal;
    TBuf<3> averageTimeDecimal;
    
    TUint32 lastTimeValue = microsecTaken / 1000;
    TUint32 lastTimeDecimalValue = microsecTaken % 1000;
    TUint64 totalTimeValue = iTotalTime[index] / 1000;
    TUint32 totalTimeDecimalValue = iTotalTime[index] % 1000;
    TUint32 averageTimeValue = iTotalTime[index] / iTotalUsage[index] / 1000;
    TUint32 averageTimeDecimalValue = iTotalTime[index] / iTotalUsage[index] % 1000;
    
    if ( ( lastTimeDecimalValue < 100 ) && ( lastTimeDecimalValue > 0 ) )
        lastTimeDecimal.AppendNum( 0 );
    
    lastTimeDecimal.AppendNum( lastTimeDecimalValue );
    
    if ( ( totalTimeDecimalValue < 100 ) && ( totalTimeDecimalValue > 0 ) )
        totalTimeDecimal.AppendNum( 0 );
    
    totalTimeDecimal.AppendNum( totalTimeDecimalValue );
    
    if ( ( averageTimeDecimalValue < 100 ) && ( averageTimeDecimalValue > 0 ) )
        averageTimeDecimal.AppendNum( 0 );    
    
    averageTimeDecimal.AppendNum( averageTimeDecimalValue );
    
    if (totalTimeValue <= 0xFFFFFFFF)
        {
        //Define the ptr on stack to avoid compiling warnning for wiscw udeb
        TPtrC ptr( iDescription[index] );
        WriteFormat( _L( "<PERFLOG>%S-%S, usage = %u, last time = %u.%S ms, total time = %u.%S ms, average time = %u.%S ms</PERFLOG>" ), 
            iTitle, 
            &ptr,
            iTotalUsage[index], 
            lastTimeValue, 
            &lastTimeDecimal, 
            I64LOW(totalTimeValue), 
            &totalTimeDecimal, 
            averageTimeValue, 
            &averageTimeDecimal );
        }
    else
        {
        //Define the ptr on stack to avoid compiling warnning for wiscw udeb
        TPtrC ptr( iDescription[index] );
        WriteFormat( _L( "<PERFLOG>%S-%S, usage = %u, last time = %u.%S ms, total time = %u%u.%S ms, average time = %u.%S ms</PERFLOG>" ), 
            iTitle, 
            &ptr,
            iTotalUsage[index], 
            lastTimeValue, 
            &lastTimeDecimal, 
            I64HIGH(totalTimeValue), 
            I64LOW(totalTimeValue),
            &totalTimeDecimal, 
            averageTimeValue, 
            &averageTimeDecimal );
        }
    }
     
// Maintain a references for writing...
void TMmMtpDpOverflowHandler::SetOwner( CMmMtpDpPerfLog* aOwner )
    {
    iOwner = aOwner;
    }
    
void TMmMtpDpOverflowHandler::Overflow( TDes& aDes )
    {
    iOwner->Write( _L( "Logging Overflow!" ) );
    iOwner->Write( aDes );
    }

#endif // defined(_DEBUG) || defined(MMMTPDP_PERFLOG)

