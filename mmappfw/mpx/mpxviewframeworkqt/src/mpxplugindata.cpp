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

#include "mpxplugindata.h"

// -----------------------------------------------------------------------------
// MpxPluginData
// -----------------------------------------------------------------------------
//
MpxPluginData::MpxPluginData()
:
pluginId( 0 ),
pluginType( 0 ),
pluginPriority( EMPXViewPluginPriorityLowest ),
pluginSupportedId()
{
    // No implementation required
}

// -----------------------------------------------------------------------------
// ~MpxPluginData
// -----------------------------------------------------------------------------
//
MpxPluginData::~MpxPluginData()
{
    pluginSupportedId.clear();
}

// -----------------------------------------------------------------------------
// id
// -----------------------------------------------------------------------------
//
int MpxPluginData::id()const
{
    return pluginId;
}

// -----------------------------------------------------------------------------
// type
// -----------------------------------------------------------------------------
//
int MpxPluginData::type()const
{
    return pluginType;
}

// -----------------------------------------------------------------------------
// priority
// -----------------------------------------------------------------------------
//
MpxPluginData::TMPXViewPluginPriorities MpxPluginData::priority() const
{
    return pluginPriority;
}

// -----------------------------------------------------------------------------
// supportedId
// -----------------------------------------------------------------------------
//
const QList<int>& MpxPluginData::supportedId() const
{
    return pluginSupportedId;
}

// -----------------------------------------------------------------------------
// setId
// -----------------------------------------------------------------------------
//
void MpxPluginData::setId( int value )
{
    pluginId = value;
}

// -----------------------------------------------------------------------------
// setId
// -----------------------------------------------------------------------------
//
void MpxPluginData::setType( int value )
{
    pluginType = value;
}

// -----------------------------------------------------------------------------
// setType
// -----------------------------------------------------------------------------
//
void MpxPluginData::setPriority( TMPXViewPluginPriorities value )
{
    pluginPriority = value;
}

// -----------------------------------------------------------------------------
// addSupportedId
// -----------------------------------------------------------------------------
//
void MpxPluginData::addSupportedId( int value )
{
    pluginSupportedId.append( value );
}
