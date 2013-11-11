/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPX view plugin manager interface definition.
*
*/



#ifndef MMPXVIEWPLUGINMANAGER_H
#define MMPXVIEWPLUGINMANAGER_H


// INCLUDES
#include <e32std.h>
#include <badesca.h>


// CLASS DECLARATION
class CMPXViewPlugin;

/**
 *  MPX view plugin manager interface.
 *
 *  @lib mpxviewutility.lib
 */
NONSHARABLE_CLASS( MMPXViewPluginManager )
    {
public:

    /**
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since S60 3.2.3
     * @param aUid Specific view plugin uid or plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @return Pointer to a view plugin object. Leave if not found.
     */
    virtual CMPXViewPlugin* ResolvePluginL(
        const TUid& aUid, TUid& aResolvedPluginType ) = 0;

    /**
     * Resolve view plugin based on the criteria passed in parameter.
     *
     * @since S60 3.2.3
     * @param aUids List of Uids, including specific view plugin uid and/or
     *              plugin type uid.
     * @param aResolvedPluginType On return, stores the plugin type of
     *                            the returned pointer
     * @return Pointer to a view plugin object. Leave if not found.
     */
    virtual CMPXViewPlugin* ResolvePluginL(
        const RArray<TUid>& aUids, TUid& aResolvedPluginType ) = 0;

    /**
     * Return view plugin with specific view plugin implementation uid.
     *
     * @since S60 3.2.3
     * @param aUid Specific view plugin uid.
     * @return Pointer to a view plugin object. Leave if not found.
     */
    virtual CMPXViewPlugin* PluginL(
        const TUid& aUid ) = 0;
    };

#endif  // MMPXVIEWPLUGINMANAGER_H

// End of File
