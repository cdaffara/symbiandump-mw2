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
* Description:  MPX view plugin info definition
*
*/



#ifndef CMPXVIEWPLUGININFO_H
#define CMPXVIEWPLUGININFO_H


// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>


// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
 *  MPX view plugin info.
 *
 *  @lib mpxviewutility.lib
 *  @since S60 v3.1
 */
NONSHARABLE_CLASS( CMPXViewPluginInfo ) : public CBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since 3.0
     * @param aData Plugin's implementation information.
     * @return Pointer to newly created object.
     */
    static CMPXViewPluginInfo* NewL( 
        const CImplementationInformation& aData );

    /**
     * Two-phased constructor.
     *
     * @since 3.0
     * @param aData Plugin's implementation information.
     * @return Pointer to newly created object.
     */
    static CMPXViewPluginInfo* NewLC( 
        const CImplementationInformation& aData );

    /**
     * Destructor.
     */
    virtual ~CMPXViewPluginInfo();

    /**
     * Returns plugin's display name.
     *
     * @since 3.0
     * @return Plugin's display name.
     */
    const TDesC& DisplayName() const;

    /**
     * Returns plugin's implementation uid.
     *
     * @since 3.0
     * @return Plugin's implementation uid.
     */
    const TUid& ImplementationUid() const;

    /**
     * Returns plugin's supported uids.
     *
     * @since 3.0
     * @return Plugin's supported uids.
     */
    const RArray<TUid> SupportedUids() const;

    /**
     * Returns plugin's plugin type.
     *
     * @since 3.0
     * @return Plugin's plugin type.
     */
    const TUid& PluginType() const;

    /**
     * Returns plugin's view priority.
     *
     * @since 3.0
     * @return Plugin's view priority.
     */
    TInt Priority() const;

    /**
     * Test whether the plugin is user selectable.
     *
     * @since 3.0
     * @return ETrue if plugin is user selectable, EFalse otherwise.
     */
    TBool IsUserSelectable() const;

    /**
     * Test whether the plugin should be preloaded.
     *
     * @since 3.0
     * @return ETrue if plugin needs preloaded, EFalse otherwise.
     */
    TBool IsPreLoad() const;

private:

    /**
     * C++ default constructor.
     */
    CMPXViewPluginInfo();

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @param aData Plugin's implementation information.
     */
    void ConstructL( 
        const CImplementationInformation& aData );

    /**
     * Extracts plugin priority from data.
     *
     * @param aData Plugin's opaque data.
     * @return Plugin priority.
     */
    TInt ExtractPluginPriority( 
        const TDesC8& aData );

    /**
     * Extracts plugin supported Uids from data.
     *
     * @param aData Plugin's opaque data.
     * @param aArray On return, contains list of plugin uids.
     */
    void ExtractPluginUidsL( 
        const TDesC8& aData,
        RArray<TUid>& aArray );

    /**
     * Extracts plugin type from data.
     *
     * @param aData Plugin's opaque data.
     * @return Plugin type.
     */
    TUid ExtractPluginType( 
        const TDesC8& aData );

    /**
     * Extracts plugin flags from data.
     *
     * @param aData Plugin's opaque data.
     * @return Plugin feature flags.
     */
    TInt ExtractPluginFlags( 
        const TDesC8& aData );

private:    // data

    /**
     * List of supported Uids.
     */
    RArray<TUid> iSupportedUids;

    /**
     * Plugin's display name.
     * Own.
     */
    HBufC* iPluginName;

    /**
     * Plugin's implementation Uid.
     */
    TUid iPluginUid;

    /**
     * Plugin's plugin type Uid.
     */
    TUid iPluginTypeUid;

    /**
     * Plugin feature flags.
     */
    TInt iFlags;

    /**
     * Plugin priority, TMPXViewPluginPriorities defined in mpxviewplugin.hrh.
     */
    TInt iPriority;
    };

#endif  // CMPXVIEWPLUGININFO_H

// End of File
