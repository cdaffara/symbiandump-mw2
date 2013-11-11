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
* Description:  MPX plugin implementation info
*
*/



#ifndef C_CMPXPLUGININFO_H
#define C_CMPXPLUGININFO_H


// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>
#include <mpxplugin.hrh>

// CONSTANTS
const TInt KMPXArrayGranularity = 4;

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
* InternalComponent.
* Interface to be implemented by customers of the class TaggedDataParser.
*
* @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(MTaggedDataParserClient)
    {
public:
    /**
     * Process a tagged data.
     *
     * @since S60 3.2.3
     * @param aTag  xml-styled tag
     * @param aData data in the xml-styled tag
     */
    virtual void ProcessTaggedDataL(const TDesC8& aTag, const TDesC8& aData)=0;
    };

/**
 *  MPX plugin info.
 *
 *  @lib mpxcommon.lib
 */
class CMPXPluginInfo : public CBase,
                                    public MTaggedDataParserClient
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 3.2.3
     * @param aData Plugin's implementation information.
     * @return Pointer to newly created object.
     */
    static CMPXPluginInfo* NewL(const CImplementationInformation& aData );

    /**
     * Two-phased constructor.
     *
     * @since S60 3.2.3
     * @param aData Plugin's implementation information.
     * @return Pointer to newly created object.
     */
    static CMPXPluginInfo* NewLC(const CImplementationInformation& aData );

    /**
     * Destructor.
     *
     * @since S60 3.2.3
     */
    IMPORT_C virtual ~CMPXPluginInfo();

protected: // From base class

    /**
     * Process a tagged data.
     *
     * @since S60 3.2.3
     * @param aTag  xml-styled tag
     * @param aData data in the xml-styled tag
     */
    IMPORT_C void ProcessTaggedDataL(const TDesC8& aTag, const TDesC8& aData);

    /**
     * Process a extended tagged data which derived class may support.
     * Default implementation in this class just ignore the data.
     *
     * @since S60 3.2.3
     * @param aTag  xml-styled tag
     * @param aData data in the xml-styled tag
     */
    IMPORT_C virtual void ProcessTaggedDataExL(const TDesC8& aTag, const TDesC8& aData);

public: // new functions
    /**
     * Returns plugin's display name.
     *
     * @since S60 3.2.3
     * @return Plugin's display name.
     */
    const TDesC& DisplayName() const;

    /**
     * Returns plugin's implementation uid.
     *
     * @since S60 3.2.3
     * @return Plugin's implementation uid.
     */
    IMPORT_C const TUid& ImplementationUid() const;

    /**
     * Returns plugin's supported uids.
     *
     * @since S60 3.2.3
     * @return Plugin's supported uids.
     */
    const TArray<TUid> SupportedUids() const;

    /**
     * Returns plugin's plugin type.
     *
     * @since S60 3.2.3
     * @return Plugin's plugin type.
     */
    const TUid& PluginType() const;

    /**
     * Returns plugin's supported App Uid.
     *
     * @return Plugin's supported App Uid..
     */
    const TUid& SupportedAppUid() const;
    
    /**
     * Returns plugin's priority.
     *
     * @since S60 3.2.3
     * @return Plugin's priority.
     */
    TInt Priority() const;

    /**
     * Returns supported mime types.
     *
     * @since S60 3.2.3
     * @return supported mime types
     */
    const CDesCArray& SupportedMimeTypes() const;

    /**
     * Returns supported extensions.
     *
     * @since S60 3.2.3
     * @return supported extensions
     */
    const CDesCArray& SupportedExtensions() const;

    /**
     * Returns supported schemas.
     *
     * @since 3.0
     * @return supported schemas
     */
    const CDesCArray& SupportedSchemas() const;

    /**
    *  Check if the plugin supports the array of uids.
    *
    *  @since S60 3.2.3
    *  @param aUids contains plugin uids
    *  @return ETrue if plugin supports all uids
    */
    TBool SupportUids(const TArray<TUid>& aUids) const;

    /**
     * Returns the flags bit array.
     *
     * @since S60 3.2.3
     * @return flags value
     */
    IMPORT_C TUint Flags() const;

    /**
     * Returns a reference to the reference count.
     *
     * @since S60 3.2.3
     * @return reference count
     */
    TInt& ReferenceCount();

    /**
     * Returns the plugin version.
     *
     * @since S60 3.2.3
     * @return version as integer.
     */
    TInt Version() const;

    /**
     *  Compare two plugin info by prioritiy.
     *
     *  @since S60 3.2.3
     *  @param aPluginInfo1 the first plugin info object
     *  @param aPluginInfo2 the second plugin info object
     *  @return zero if two plugins have same priority
     *          a negative value(-1), if the priority of the first is higher than
     *          the second's. That is to say, plugins with higher priority is
     *          listed before plugins with lower priority
     *          a positive value(1), if the priority of the first is lower than
     *          the second's.
     */
    static TInt ComparePluginInfoByPriority(const CMPXPluginInfo& aPluginInfo1,
                                            const CMPXPluginInfo& aPluginInfo2);

protected:

    /**
     * C++ default constructor.
     *
     * @since S60 3.2.3
     */
    IMPORT_C CMPXPluginInfo();

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 3.2.3
     * @param aData Plugin's implementation information.
     * @leave The function leaves with one of the system wide error codes,
     */
    IMPORT_C void ConstructL(const CImplementationInformation& aData );

    /**
     * Extracts plugin supported Uids from data.
     *
     * @since S60 3.2.3
     * @param aData Supported uids descriptor separated by comma.
     */
    void ExtractSupportedUidsL(const TDesC8& aData);

    /**
     * Extracts a list of UIDs from a text string.
     *
     * @since S60 3.2.3
     * @param aData Supported uids descriptor separated by comma.
     * @param aArray array to insert the data into
     * @leave The function leaves with one of the system wide error codes,
     */
    IMPORT_C void ExtractUidsFromTextL( const TDesC8& aData,
                                        RArray<TUid>& aArray );

    /**
    * Extracts elemements separated by semicolon into array descriptor
    *
    * @param aText original content to extract elements from
    * @param aArray the array to add the extracted elements to
    */
    IMPORT_C void ExtractIntoArrayL(const TDesC8& aText, CDesCArray*& aArray);

protected:    // data
    /**
     * List of supported Uids. Sorted by TUid.iUid
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
     * Plugin's supported App Uid.
     */
    TUid iSupportedAppUid;
    /**
     * Plugin feature flags.
     */
    TUint iFlags;

    /**
     * Plugin priority.
     */
    TInt iPriority;

    /**
    The version number of this plugin.
    */
    TInt iVersion;

    /**
     * Loaded plugin reference count.
     */
    TInt iReferenceCount;

    // Note: move the following staff into derived plugin info class,
    // so that the plugin info class can be base class for all plugins in mpx
    CDesCArray* iSupportedSchemas;
    CDesCArray* iSupportedMimeTypes;
    CDesCArray* iSupportedExtensions;
    };

#endif  // C_CMPXPLUGININFO_H

// End of File
