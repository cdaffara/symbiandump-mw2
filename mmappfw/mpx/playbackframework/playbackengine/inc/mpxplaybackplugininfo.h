/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Plugin info class for Playback engine
*
*/


#ifndef C_MPXPLAYBACKPLUGININFO_H
#define C_MPXPLAYBACKPLUGININFO_H

#include <badesca.h>
#include <mpxplugininfo.h>

/**
 *  Playback derived plugin info class
 *
 *  @lib mpxplaybackengine.dll
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CMPXPlaybackPluginInfo ) : public CMPXPluginInfo
    {

public:

    /**
    * Two-Phased constructor
    * @param aData implementation data
    * @return CMPXPlaybackPluginInfo*
    */
    static CMPXPlaybackPluginInfo* NewL( const CImplementationInformation& aData );

    /**
    * Two-Phased constructor
    * @param aData implementation data
    * @return CMPXPlaybackPluginInfo* on cleanupstack
    */
    static CMPXPlaybackPluginInfo* NewLC( const CImplementationInformation& aData );
     
    
    /**
    * Virtual destructor
    */ 
    virtual ~CMPXPlaybackPluginInfo();

protected: // From base class
    
    /**
     * Process a extended tagged data which derived class may support.
     * Default implementation in this class just ignore the data.
     *
     * @since S60 3.2.3
     * @param aTag  xml-styled tag
     * @param aData data in the xml-styled tag
     */
    IMPORT_C virtual void ProcessTaggedDataExL(const TDesC8& aTag, const TDesC8& aData);
    
private: 
    
    /**
    * Two-phased constructor
    */
    void ConstructL( const CImplementationInformation& aData );
    
    /**
    * C++ Constructor
    */
    CMPXPlaybackPluginInfo();
    
public: 

    /**
    * Extract the supported interfaces
    */
    void ExtractSupportedInterfacesL( const TDesC8& aData ); 
    
    /**
    * Returns supported interfaces.
    * 
    * @return supported interfaces
    */
    const CDesCArray& SupportedInterfaces() const;    
    
private: // data

    CDesCArray* iSupportedInterfaces; 
    };

#endif // C_MPXPLAYBACKPLUGININFO_H
