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

#include "mpxviewpluginresolver_p.h"    // MpxViewPluginResolverPrivate
#include "mpxplugindata.h"
#include "mpxplugindataparser.h"

#include <mpxviewpluginqt.h>
#include <QHash>

const char gRequestedPluginInterfaceName [] = "org.nokia.mmdt.MpxViewPlugin/1.0";

const int gNoneScore( 0 );
const int gSupportedScore( 5 );
const int gImplementationScore( 100 ); 
const int gTypeScore( 1 );

// ----------------------------------------------------------------------------
// score
// ----------------------------------------------------------------------------
//
static int score( int , const MpxPluginData& );

// -----------------------------------------------------------------------------
//MpxViewPluginResolverPrivate
// -----------------------------------------------------------------------------
//
MpxViewPluginResolverPrivate::MpxViewPluginResolverPrivate( 
        const QList<int>& request )
:
requestedPlugins( request )
{
    //No implementation needed
}

// -----------------------------------------------------------------------------
// ~MpxViewPluginResolverPrivate
// -----------------------------------------------------------------------------
//
MpxViewPluginResolverPrivate::~MpxViewPluginResolverPrivate()
{
    //No implementation needed
}

// -----------------------------------------------------------------------------
// resolvePlugin()
// -----------------------------------------------------------------------------
//
MpxViewPlugin* MpxViewPluginResolverPrivate::resolvePlugin()
{
    MpxPluginViewInterface* retVal = 0;
    if( 0 < requestedPlugins.count() ){
        XQPluginLoader  loader;
        QList< XQPluginInfo > impls;
        if (loader.listImplementations(gRequestedPluginInterfaceName, impls)) {
            retVal = resolveAndLoadPlugins( impls );
        }
    }
    return (retVal!=0) ? retVal->viewPlugin() : 0;
}

// ----------------------------------------------------------------------------
// resolveAndLoadPlugins( const QList< XQPluginInfo >& impls );
// ----------------------------------------------------------------------------
//
MpxPluginViewInterface* MpxViewPluginResolverPrivate::resolveAndLoadPlugins( 
        const QList< XQPluginInfo >& impls )
{
    MpxPluginViewInterface* retVal = 0;
    
    //parse plugin info data 
    QHash<const XQPluginInfo*, MpxPluginData* > parsedInfo;
    QList< XQPluginInfo >::const_iterator iter( impls.begin() );
    MpxPluginDataParser parser;
    for( ; iter != impls.end(); ++iter ){
        if( 0 < ( *iter ).opaqueData().length() ){
            parser.parse( ( *iter ).opaqueData() );
            if( parser.data() ){
                parsedInfo.insert( &( *iter ), parser.takeData() );
            }
        }
    }
    
    //look for optimal plugin
    QList<int> uids( requestedPlugins );
    QList<int>::iterator currentUidIter,
                         optimalUidIter;
    QHash<const XQPluginInfo*, MpxPluginData* >::iterator 
        currentInfoIter,
        optimalInfoIter;
    int currentPluginScore( 0 ),
        optimalPluginScore( 0 );
    
    do{
        //initialize iterators with invalid objects
        optimalInfoIter = parsedInfo.end();
        
        for( currentUidIter = uids.begin();
             uids.end() != currentUidIter;
             ++currentUidIter )
        {
            for( currentInfoIter = parsedInfo.begin();
                 parsedInfo.end() != currentInfoIter;
                 ++currentInfoIter )
            {
                currentPluginScore = score( (*currentUidIter ), 
                                            (*(*currentInfoIter) ) );
                if( gNoneScore < currentPluginScore ){
                    if( parsedInfo.end() == optimalInfoIter ){
                        //first plugin with no zero score. set as optimal
                        optimalPluginScore = currentPluginScore;
                        optimalInfoIter = currentInfoIter;
                        optimalUidIter = currentUidIter;
                    }
                    if( optimalPluginScore < currentPluginScore ||
                        ( ( currentPluginScore == optimalPluginScore ) &&
                          ( ( *currentInfoIter )->priority() > ( *optimalInfoIter )->priority() ) ) )
                    {
                        //current plugin fits better then "optimal" one
                        optimalPluginScore = currentPluginScore;
                        optimalInfoIter = currentInfoIter;
                        optimalUidIter = currentUidIter;
                    }
                }
            }
        }
        if( parsedInfo.end() == optimalInfoIter ){
             //plugin was not found
             break;
         }
         XQPluginLoader loader( optimalInfoIter.key()->uid() );
         if( loader.load() ) {
             retVal = qobject_cast<MpxPluginViewInterface*>(loader.instance());
         }
         //remove already loaded plugin description
         delete ( *optimalInfoIter );
         parsedInfo.erase( optimalInfoIter );
         uids.erase( optimalUidIter );
    
    }while( NULL == retVal );
    uids.clear();
    
    //clear parsed data structures
    for( currentInfoIter = parsedInfo.begin();
         parsedInfo.end() != currentInfoIter;
         currentInfoIter = parsedInfo.erase( currentInfoIter ) ){
         delete ( *currentInfoIter );
    }
    return retVal;
}
// ----------------------------------------------------------------------------
// score
// ----------------------------------------------------------------------------
//
int score( int uid, const MpxPluginData& pluginData )
{
    int retVal( gNoneScore );
    QList< int >::const_iterator iter( pluginData.supportedId().begin() );
    for( ; iter != pluginData.supportedId().end(); ++iter ){
        if( ( *iter ) == uid ){
            retVal += gSupportedScore;
        }
    }
    if( pluginData.id() == uid ){
        retVal += gImplementationScore; 
    }
    if( pluginData.type() == uid ){
        retVal += gTypeScore;
    }
    
    qDebug(QString("Score 0x%1: %2").arg(uid, 0, 16).arg(retVal).toLatin1().data());
    
    return retVal;
}
