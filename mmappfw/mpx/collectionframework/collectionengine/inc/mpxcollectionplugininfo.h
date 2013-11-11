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
* Description:  Plugin info class for collection engine
*
*/


#ifndef C_MPXCOLLECTIONPLUGININFO_H
#define C_MPXCOLLECTIONPLUGININFO_H

#include <badesca.h>
#include <mpxplugininfo.h>

/**
 *  Collection derived plugin info class
 *
 *  @lib mpxcollectionengine.dll
 *  @since S60 3.2
 */
NONSHARABLE_CLASS( CMPXCollectionPluginInfo ) : public CMPXPluginInfo
    {

public:

    /**
    * Two-Phased constructor
    * @param aData implemnetation data
    * @return CMPXCollectionPluginInfo*
    */
    static CMPXCollectionPluginInfo* NewL( const CImplementationInformation& aData );

    /**
    * Two-Phased constructor
    * @param aData implemnetation data
    * @return CMPXCollectionPluginInfo* on cleanupstack
    */
    static CMPXCollectionPluginInfo* NewLC( const CImplementationInformation& aData );
     
    
    /**
    * Virtual destructor
    */ 
    virtual ~CMPXCollectionPluginInfo();

public: 
   
    /**
    * From CMPXPluginInfo  
    */
    void ProcessTaggedDataExL(const TDesC8& aTag, const TDesC8& aData);

    /**
    * Return the list of non-cacheable attributes
    * @return TArray<TUid> containing the list of attributes not cacheable
    */
    const TArray<TUid> NonCacheableAttrs();
    
private: 

    /**
    * Extract the non-cacheable attributes
    */
    void ExtractNonCacheableAttrsL( const TDesC8& aData );
   
private: 
    
    /**
    * Two-phased constructor
    */
    void ConstructL( const CImplementationInformation& aData );
    
    /**
    * C++ Constructor
    */
    CMPXCollectionPluginInfo();
    
private: // data

    RArray<TUid> iNonCachableAttrs;
    };

#endif // C_MPXCOLLECTIONPLUGININFO_H
