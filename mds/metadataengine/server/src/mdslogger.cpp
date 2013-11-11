/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata server logger class*
*/

#include "mdslogger.h"
#include "mdsclausebuffer.h"
#include "mdssqliteconnection.h"


#ifdef LOG_MASTER_FLAG
#ifdef _DEBUG

// ========================= MEMBER FUNCTIONS ==================================

// ------------------------------------------------
// NewInstanceL
// ------------------------------------------------
//
CMdSLogger* CMdSLogger::NewInstanceL()
    {
    static CMdSLogger* singleton;
    if ( singleton == NULL )
        {
        singleton = new (ELeave) CMdSLogger();      
        CleanupStack::PushL(singleton);
        singleton->ConstructL();
        CleanupStack::Pop(singleton);
        }
    return singleton;
    }

// ------------------------------------------------
// Default constructor
// ------------------------------------------------
//
CMdSLogger::CMdSLogger()
	: iAltFileFlag(EFalse), iValid(EFalse)
    {
    }

// ------------------------------------------------
// Destructor
// ------------------------------------------------
//
CMdSLogger::~CMdSLogger()
    {
    static CMdSLogger* singleton;
    delete singleton;
    singleton = NULL;
    iValid = EFalse;
    iLog.CloseLog(); 
    iLog.Close();
    }

// ------------------------------------------------
// ConstructL
// ------------------------------------------------
//
void CMdSLogger::ConstructL()
    {
    User::LeaveIfError( iLog.Connect() );
    iLog.CreateLog(KDirectory, KFilename, EFileLoggingModeOverwrite);
    iLog.SetDateAndTime(EFalse, ETrue);
    
    iValid = iLog.LogValid();

    // set up logging categories
    Activate( ELogAlways );
#ifdef LOG_QUERY
    Activate( ELogQuery );
#endif
#ifdef LOG_DB
    Activate( ELogDb );
#endif
#ifdef LOG_MUTEX
    Activate( ELogMutex );
#endif
#ifdef LOG_SERVER
    Activate( ELogServer );
#endif
    }

// ------------------------------------------------
// LogLit
// ------------------------------------------------
//
void CMdSLogger::LogLit( const TDesC8& aText )
    {
    if (!iValid)
    	{
    	return;
    	}

    TInt offset = 0;
    TInt linecount = 0;
    TInt length = aText.Length();
    while( offset < length )
        {
        TInt partLength = Min( length-offset, KLineLength );
#ifdef MDE_FILE_LOGGING
        iLog.Write( aText.Mid( offset, partLength ) );
#else
        TBuf<KLineLength> buffer;
        buffer.Copy( aText.Mid( offset, partLength ) );
        RDebug::Print( buffer );
#endif
        ++linecount;
        offset += partLength;
        }
    CheckSize( linecount );
    }

// ------------------------------------------------
// LogLit
// ------------------------------------------------
//
void CMdSLogger::LogLit( const TDesC16& aText )
    {
    if (!iValid)
    	{
    	return;
    	}

    TInt offset = 0;
    TInt linecount = 0;
    TInt length = aText.Length();
    while( offset < length )
        {
        TInt partLength = Min( length-offset, KLineLength );
#ifdef MDE_FILE_LOGGING
        iLog.Write( aText.Mid( offset, partLength ) );
#else
        RDebug::Print( aText.Mid( offset, partLength ) );
#endif
        ++linecount;
        offset += partLength;
        }
    CheckSize( linecount );
    }

// ------------------------------------------------
// CheckSize
// ------------------------------------------------
//
#ifdef MDE_FILE_LOGGING
void CMdSLogger::CheckSize( TInt aLines )
    {
    iLineCounter += aLines;
    if ( iLineCounter < KLogfileMaxLength ) return; // no worries

    iLineCounter = 0;

    iLog.CloseLog();
    
    if ( !iAltFileFlag )
        {
        iLog.CreateLog(KDirectory, KAltFilename, EFileLoggingModeOverwrite);
        iAltFileFlag = ETrue;       
        }
    else
        {
        iLog.CreateLog(KDirectory, KFilename, EFileLoggingModeOverwrite);
        iAltFileFlag = EFalse;        
        }    
    iLog.SetDateAndTime(EFalse, ETrue);
    }
#else
void CMdSLogger::CheckSize( TInt /*aLines*/ )
    {
    // Nothing to check
    }
#endif

// ---------------------------------------------------------------------------
// DescribeL                Returns sql clause with variables
// ---------------------------------------------------------------------------
//
CMdsClauseBuffer* CMdSLogger::DescribeL( const CMdsClauseBuffer& aBuffer, const RRowData& aRowData )
    {
    if (!iValid)
    	{
    	return NULL;
    	}

    CMdsClauseBuffer* buf = CMdsClauseBuffer::NewLC( aBuffer.ConstBufferL().Length() + 1024 );
    buf->BufferL().Copy( aBuffer.ConstBufferL() );
    buf->AppendL( _L(" variables=") );
    for( TInt i=0; i<aRowData.Size(); ++i )
        {
        if ( i>0 ) buf->AppendL( _L(", ") );
        LogVariableL( *buf, aRowData.Column(i) );
        }
    CleanupStack::Pop( buf );
    return buf;
    }

// ---------------------------------------------------------------------------
// DescribeL                Returns sql clause with variables
// ---------------------------------------------------------------------------
//
CMdsClauseBuffer* CMdSLogger::DescribeFullL( const TDesC& aBuffer, const RRowData& aRowData )
    {
    if (!iValid)
    	{
    	return NULL;
    	}

    const TChar KQuestionMark = '?';
    TPtrC buffer( aBuffer );
    const TInt aBufferLength = aBuffer.Length();
    CMdsClauseBuffer* buf = CMdsClauseBuffer::NewLC( aBufferLength *2 );
    TInt columnNumber = 0;
    TInt qpos = buffer.Locate( KQuestionMark );
    while( qpos >= 0 )
    	{
		buf->ReserveSpaceL( buf->ConstBufferL().Length() + qpos );
		buf->BufferL().Append( buffer.Left( qpos ) );
		LogVariableL( *buf, aRowData.Column(columnNumber++) );
    	buffer.Set( buffer.Mid( qpos + 1 ) );
    	qpos = buffer.Locate( KQuestionMark );
    	}
	buf->ReserveSpaceL( buf->ConstBufferL().Length() + buffer.Length() );
	buf->BufferL().Append( buffer );

	CleanupStack::Pop( buf );
    return buf;
    }

// ---------------------------------------------------------------------------
// LogVariableL         Logging method for query variables
// ---------------------------------------------------------------------------
//
void CMdSLogger::LogVariableL( CMdsClauseBuffer& aBuf, const TColumn& aColumn )
    {
    _LIT( KApostrophe, "\'" );
    switch( aColumn.Type() )
        {
        case EColumnNotUsed:
            {
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 8 );
            _LIT( KMessageUnused, "<unused>" );
            aBuf.BufferL().Append( KMessageUnused );
            break;
            }
        case EColumnBool:
            {
            TBool val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 4 );
            aBuf.BufferL().AppendNum( val );
            }
            break;
        case EColumnInt32:
            {
            TInt32 val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 16 );
            aBuf.BufferL().AppendNum( static_cast<TInt>(val) );
            }
            break;
        case EColumnUint32:
            {
            TUint32 val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 16 );
            aBuf.BufferL().AppendNum( val, EDecimal );
            }
            break;
        case EColumnTime:
            {
            TTime val(0);
            TBuf<128> tmp;
            aColumn.Get( val );
            _LIT(KTimeFormat, "%D%1.%M%2.%Y%3 %H:%T:%S");
            val.FormatL(tmp, KTimeFormat);
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 20 );
            aBuf.BufferL().Append( tmp );
            }
            break;
        case EColumnInt64:
            {
            TInt64 val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 32 );
            aBuf.BufferL().AppendNum( val );
            }
            break;
        case EColumnReal32:
            {
            TReal32 val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 32 );
            aBuf.BufferL().AppendNum( val, TRealFormat() );
            }
            break;
        case EColumnReal64:
            {
            TReal64 val=0;
            aColumn.Get( val );
            aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 32 );
            aBuf.BufferL().AppendNum( val, TRealFormat() );
            }
            break;
        case EColumnDes16:
            {
            TPtrC16 val= TPtr16((TUint16*)0, 0); //KNullPtr16;
            aColumn.Get( val );
            if( val.Ptr() )
            	{
            	aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 
            			val.Length() + 2 * KApostrophe().Length() ); // for ''
            	aBuf.BufferL().Append( KApostrophe );
            	aBuf.BufferL().Append( val );
            	aBuf.BufferL().Append( KApostrophe );
            	}
            else
            	{
            	_LIT( KNull, "NULL" );
            	aBuf.ReserveSpaceL( aBuf.ConstBufferL().Length() + 
            			KNull().Length() );
            	aBuf.BufferL().Append( KNull );
            	}
            }
            break;
        default:
            User::Leave( KErrCorrupt );
        }
    }

#endif // _DEBUG
#endif // LOG_MASTER_FLAG

