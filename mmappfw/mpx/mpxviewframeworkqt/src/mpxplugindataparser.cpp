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

#include "mpxplugindataparser.h"
#include "mpxplugindata.h"
#include <QString>



const int gParserHexBase( 16 );
const int gParserDecBase( 10 );

static MpxPluginData::TMPXViewPluginPriorities parsePriority( const QStringRef& src );


// ----------------------------------------------------------------------------
// MpxPluginDataParser()
// ----------------------------------------------------------------------------
//
MpxPluginDataParser::MpxPluginDataParser()
:
dataPtr( NULL )
{
    // No implementation required
}

// ----------------------------------------------------------------------------
// ~MpxPluginDataParser()
// ----------------------------------------------------------------------------
//
MpxPluginDataParser::~MpxPluginDataParser()
{
    if( NULL != dataPtr ){
        delete dataPtr;
    }
}

// ----------------------------------------------------------------------------
// data()
// ----------------------------------------------------------------------------
//
const MpxPluginData* MpxPluginDataParser::data() const
{
    return dataPtr;
}

// ----------------------------------------------------------------------------
// takeData()
// ----------------------------------------------------------------------------
//
MpxPluginData* MpxPluginDataParser::takeData()
{
    MpxPluginData* retVal( dataPtr );
    if( NULL != dataPtr ){
        dataPtr  = NULL;
    }
    return retVal;
}

// ----------------------------------------------------------------------------
// parse( const QString& data )
// ----------------------------------------------------------------------------
//
void MpxPluginDataParser::parse( const QString& data )
{
    const QString PluginIdTag( "p" );
    const QString PluginTypeTag( "t" );
    const QString PluginPriorityTag( "i" );
    const QString PluginSupportedTag( "f" );

    QStringRef tagName,
               tagContent;
    int parserOffset( 0 );
    if( NULL != dataPtr ){
        delete dataPtr;
        dataPtr = NULL;
    }
    int integerNodeRepresentation( 0 );
    while( findNextNode( data, parserOffset, tagName, tagContent ) ){
        
        if( NULL == dataPtr ){
            dataPtr = new MpxPluginData();
        }
        if( PluginIdTag == tagName ){
            if( parseInt( integerNodeRepresentation, tagContent.toString() ) )
                dataPtr->setId( integerNodeRepresentation );
        }else if( PluginTypeTag == tagName ){
            if( parseInt( integerNodeRepresentation, tagContent.toString() ) ) {
                dataPtr->setType( integerNodeRepresentation );
            }
        }else if( PluginPriorityTag == tagName ){
            dataPtr->setPriority( parsePriority( tagContent ) );
        }else if( PluginSupportedTag == tagName ){
            parseSupportedId( tagContent.toString() );
        }
    }
}

// ----------------------------------------------------------------------------
// findNextNode( const QString& src, int& offset, QStringRef& tagName, QStringRef& tagContent )
// ----------------------------------------------------------------------------
//
bool MpxPluginDataParser::findNextNode( const QString& src, int& offset, QStringRef& tagName, QStringRef& tagContent ) const
{
    const QChar endOfTag( '>' );
    const QString beginOfStopTag( "</" );
    
    
    int nodeBeginOffset = src.indexOf( endOfTag, offset, Qt::CaseSensitive );
    if( nodeBeginOffset <= offset ){
        //next "start tag" wasn't found. skip parsing
        return false;
    }
    ++nodeBeginOffset;
    int nodeEndOffset = src.indexOf( endOfTag, nodeBeginOffset, Qt::CaseSensitive );
    if( nodeEndOffset <= nodeBeginOffset ){
        //next "stop tag" wasn't found. skip parsing
        return false;
    }
    int nodeStopBegin = src.indexOf( beginOfStopTag, nodeBeginOffset, Qt::CaseSensitive );
    offset = nodeEndOffset + 1;
    if( nodeStopBegin <= nodeBeginOffset ){
        //error invalid document format
        return false;
    }
    const int nameBegin( nodeStopBegin + beginOfStopTag.length() );
    tagName = src.midRef( nameBegin, nodeEndOffset - nameBegin );
    tagContent = src.midRef( nodeBeginOffset, nodeStopBegin - nodeBeginOffset );
    return true;
}

// ----------------------------------------------------------------------------
// parseInt( int& aDst, const QString& src ) const
// ----------------------------------------------------------------------------
//
bool MpxPluginDataParser::parseInt( int& aDst, const QString& src ) const 
{
    const int parserBasesByPriority[] = { gParserHexBase, gParserDecBase },
              parserSupportedBases( 2 );
    bool retVal( false );
    for( int iter( 0 ); !retVal && iter < parserSupportedBases; ++iter ){
        aDst = src.toInt( &retVal, parserBasesByPriority[ iter ] );
    }
    return retVal;
}

// ----------------------------------------------------------------------------
// parseSupportedId( const QString& value )
// ----------------------------------------------------------------------------
//
void MpxPluginDataParser::parseSupportedId( const QString& value )
{
    if( NULL == dataPtr ) {
        return;
    }
    int from( 0 ), to( 0 ), tagValue;
    const QChar coma( ',' );
    do{
        if( from >= value.length() ){
            break;
        }
        to = value.indexOf( coma, from );
        if( to < from ){
            to = value.length();
        }
        if( parseInt( tagValue, value.mid( from, to - from ) ) ){
            dataPtr->addSupportedId( tagValue );
        }
        from = ( to + 1 );
    }while( true );
}

// ----------------------------------------------------------------------------
// parseSupportedId( const QString& value )
// ----------------------------------------------------------------------------
//
MpxPluginData::TMPXViewPluginPriorities parsePriority( const QStringRef& src ) 
{

    const QString MPXViewPluginPriorityLowestTag( "EMPXViewPluginPriorityLowest" );
    const QString MPXViewPluginPriorityLowTag( "EMPXViewPluginPriorityLow" );
    const QString MPXViewPluginPriorityNormalTag( "EMPXViewPluginPriorityNormal" );
    const QString MPXViewPluginPriorityHighTag( "EMPXViewPluginPriorityHigh" );
    const QString MPXViewPluginPriorityHighestTag( "EMPXViewPluginPriorityHighest" );

    MpxPluginData::TMPXViewPluginPriorities 
        retVal( MpxPluginData::EMPXViewPluginPriorityLowest );
    if(  MPXViewPluginPriorityLowTag == src ){
        retVal =  MpxPluginData::EMPXViewPluginPriorityLow;
    }else if( MPXViewPluginPriorityNormalTag == src ){
        retVal =  MpxPluginData::EMPXViewPluginPriorityNormal;
    }else if( MPXViewPluginPriorityHighTag == src ){
        retVal =  MpxPluginData::EMPXViewPluginPriorityHigh;
    }else if( MPXViewPluginPriorityHighestTag == src ){
        retVal =  MpxPluginData::EMPXViewPluginPriorityHighest;
    }
    return retVal;
}


