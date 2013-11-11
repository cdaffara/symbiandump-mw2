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

#ifndef MPXPLUGINDATA_H
#define MPXPLUGINDATA_H

// INCLUDES
#include <QList>
// CLASS DECLARATION

/**
 *  QMpxPluginData
 * 
 */
class MpxPluginData
    {
public:
    enum TMPXViewPluginPriorities{
        EMPXViewPluginPriorityLowest = -20000,
        EMPXViewPluginPriorityLow = -10000,
        EMPXViewPluginPriorityNormal = 0,
        EMPXViewPluginPriorityHigh = 10000,
        EMPXViewPluginPriorityHighest = 20000
    };

    /**
     * Constructor
     */
    MpxPluginData( );

    /**
     * Destructor.
     */
    ~MpxPluginData();

    /**
     * Plugin id geter
     * @return plugin id 
     */
    int id()  const;
    
    /**
     * Plugin interface id geter
     * @return interface id implemented by plugin 
     */
    int type() const;
    
    /**
     * Plugin priority geter
     * @return priority of plugin 
     */
    TMPXViewPluginPriorities priority() const;
    
    /**
     * Plugin supported functionality geter
     * @return list of functionality delivered by plugin
     */
    const QList< int >& supportedId() const;
    
    /**
     * Plugin id seter
     * @param value - new value of plugin id
     */
    void setId( int value );
    
    /**
     * Plugin interface id seter
     * @param value - new value of plugin interface id
     */
    void setType( int value );
    
    /**
     * Plugin priority seter
     * @param value - new plugin priority
     */
    void setPriority( TMPXViewPluginPriorities value );
    
    /**
     * Plugin supporterd functionality seter
     * @param value - new functionality supported by plugin
     */
    void addSupportedId( int value );

private:
    int pluginId;
    int pluginType;
    TMPXViewPluginPriorities pluginPriority;
    QList< int > pluginSupportedId;
    };

#endif // MPXPLUGINDATA_H
