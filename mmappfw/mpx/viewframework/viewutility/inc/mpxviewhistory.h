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
* Description:  MPX view history
*
*/



#ifndef MPXVIEWHISTORY_H
#define MPXVIEWHISTORY_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CMPXViewPlugin;

// CLASS DECLARATION

/**
 *  MPX view history.
 *
 *  @lib mpxviewutility.lib
 *  @since S60 v3.1
 */

NONSHARABLE_CLASS( CMPXViewHistory ) : public CBase
    {
public:
    /**
     * Two-phase constructor
     *
     * @return object created
     */
    static CMPXViewHistory* NewL();

    /**
     * Two-phase constructor
     *
     * @param aHistory pointer to an existing view history
     * @return object created
     */
    static CMPXViewHistory* NewL( CMPXViewHistory* aHistory );

    /**
     * Two-phase constructor
     *
     * @param aUids Uid used to resolve the plugins
     * @param aPluginType type of the plugin resolved
     * @param aPluginImplementationUid Implementation Uid
     * @param aPlugin Pointer to the plugin
     * @return object created
     */
    static CMPXViewHistory* NewL(
        const RArray<TUid>& aUids,
        const TUid aPluginType,
        const TUid aPluginImplementationUid,
        CMPXViewPlugin* aPlugin );

    /**
     * Destructor.
     */
    ~CMPXViewHistory();

private:

    /**
     * default constructor
     */
    CMPXViewHistory();

    /**
     * Second phase constructor
     *
     * @param aUids Uid used to resolve the plugins
     * @param aPluginType type of the plugin resolved
     * @param aPluginImpUid Implementation Uid
     * @param aPlugin Pointer to the plugin
     */
    void ConstructL(
        const RArray<TUid>& aUids,
        const TUid aPluginType,
        const TUid aPluginImpUid,
        CMPXViewPlugin* aPlugin );

public:
    RArray<TUid> iUids;
    TUid iPluginType;
    TUid iPluginImplementationUid;
    CMPXViewPlugin* iPlugin;  // not owned
    };

#endif  // MPXVIEWHISTORY_H

// End of File
