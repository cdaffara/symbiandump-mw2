/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of testcollectionplugin interface
*
*/

#ifndef TESTCOLLECTIONPLUGIN_H
#define TESTCOLLECTIONPLUGIN_H

// INCLUDES
#include <mpxcommonframeworkdefs.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxcollectionplugin.h>
#include <mpxcollectionpath.h>
#include <mpxmediageneraldefs.h>
#include <mpxmessage2.h>
#include <badesca.h>

// FORWARD DECLARATIONS
class CMPXMedia;
class CMPXMediaArray;

// CONSTANTS

// CLASS DECLARATION

/**
*  CCollectionTestPlugin class 
*
*/
NONSHARABLE_CLASS(CTestCollectionPlugin) : public CMPXCollectionPlugin
    {
public: // Constructors and destructor
    
    /**
    * Two-phased constructor
    * @param aInitParams: initialization parameters
    * @return: object constructed
    */
    static CTestCollectionPlugin* NewL (TAny* aInitParams);

    /**
    * Destructor
    */
    virtual ~CTestCollectionPlugin();
    
public: // from base clase
    /** 
    * From CMPXCollectionPlugin
    * Navigates to the given path
    * @param aPath: a path
    * @param aAttrs, attributes requested
    * @param aFilter, filter to apply or NULL if none
    */
    void OpenL(const CMPXCollectionPath& aPath,
               const TArray<TMPXAttribute>& aAttrs,
               CMPXFilter* aFilter); 
    
    /**
    * From CMPXCollectionPlugin     
    * Get the extended properties of the current file (async)
    * @param aPath: a path   
    * @param aAttrs: array of attributes requested             
    * @param aCaps platsec capabilities of client requesting media; plug-in should also
    *        verify its process capabilities
    * @aParam aSpecs, specifications for attributes
    */
    void MediaL(const CMPXCollectionPath& aPath, 
                const TArray<TMPXAttribute>& aAttrs,
                const TArray<TCapability>& aCaps,
                CMPXAttributeSpecs* aSpecs);

    /**
    * From CMPXCollectionPlugin     
    * Cancel outstanding request
    */
    void CancelRequest ();

    /** 
    * From CMPXCollectionPlugin    
    * Executes the given command on the collection
    * @param aCmd: a command
    * @param aArg: a Argument
    */
    void CommandL (TMPXCollectionCommand aCmd, TInt aArg);

    /** 
    * From CMPXCollectionPlugin      
    * Executes a command
    *
    * @param aCmd a command
    */
    void CommandL(CMPXCommand& aCmd);
            
    /**
    * From CMPXCollectionPlugin        
    * Adds an item (song or playlist) to the collection
    * @param aMedia: Properties of the item
    */
    void AddL (const CMPXMedia& aMedia);

    /**
    * From CMPXCollectionPlugin        
    * Remove an item from the collection database using the given path
    * Note that the selection indicies are hidden within the path
    * @param aPath: path to the item to be removed
    * 
    */
    void RemoveL (const CMPXCollectionPath& aPath);
    
    /**
    * From CMPXCollectionPlugin        
    *  Remove an item from the collection database using the given media properties
    *  @param aMedia: Properties of the item that is to be removed. It may cantain URI only
    *                 or meta data, all of items matched properties will be removed.
    */
    void RemoveL (const CMPXMedia& aMedia);
    
    /**
    * From CMPXCollectionPlugin        
    *  Sets/updates the media for an item in the collection
    *  @param aMedia: new property values for the item
    */
    void SetL (const CMPXMedia& aMedia);

    /**
    * From CMPXCollectionPlugin        
    *  Find the items matching the media specifications 
    *  @param aCriteria: Properties of the item that is to be found
    *  @param aAttrs: Requested attributes to return   
    */
    void FindAllL (const CMPXMedia& aCriteria, const TArray<TMPXAttribute>& aAttrs);

    /**
    * From CMPXCollectionPlugin        
    *  Find the items matching the media specifications 
    *  @param aCriteria: Properties of the item that is to be found
    *  @param aAttrs: Requested attributes to return 
    *  @return results of the search
    */
    CMPXMedia* FindAllSyncL(const CMPXMedia& aCriteria,
                            const TArray<TMPXAttribute>& aAttrs);
                                        
    /**
    * From CMPXCollectionPlugin        
    * Get the list of supported capabilities
    * @return: TCollectionCapability, bitmask of supported capabilities
    */
    TCollectionCapability GetCapabilities ();    
                
private:
    /**
    * Constructor
    */
    CTestCollectionPlugin ();

    /**
    * Symbian 2nd phase constructor.
    */
    void ConstructL ();
    
    /**
    * Append media into aMediaArray base on aIdArray Info
    */
    void AddMediaL(CMPXMediaArray& aMediaArray, 
                   const RArray<TInt> aIdArray, 
                   const TArray<TMPXAttribute>& aAttrs);
    
    /**
    * Add specify attribute to Media
    */
    void AddAttrbuteL(CMPXMedia& aMedia, 
                      const TInt aId,
                      const TDesC& aTitle,
                      const TDesC& aUri,
                      const TArray<TMPXAttribute>& aAttrs);

    /**
    * Add specify attribute to Media specific for Playback framework
    * test cases
    */
    void AddAttributePlaybackFwTestL(CMPXMedia& aMedia, 
                                     const TInt aId,
                                     const TArray<TMPXAttribute>& aAttrs);
    /**
    * Filter out media in aMediaArray which match any one attribute in aFilter
    */                  
    void FilterMediaArray(CMPXMediaArray& aMediaArray, CMPXFilter* aFilter);
    
    /**
    * Fill up media with change message
    */
    void FillItemChangeMessageL(CMPXMessage& aMessage,
                                TMPXItemId aId,
                                TMPXChangeEventType aChangeType,
                                TMPXGeneralCategory aCategory);
                                
    /**
    * Adding specific attribute
    */
    void AddAttributeGeneralTypeL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeGeneralCategoryL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeGeneralDurationL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeGeneralLastPlaybackPositionL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeMusicArtistL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeDrmTypeL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeDrmProtectedL(CMPXMedia& aMedia, const TInt aId);
    void AddAttributeDrmRightsStatusL(CMPXMedia& aMedia, const TInt aId);
    
private:    // data
    };

#endif   // TESTCOLLECTIONPLUGIN_H
            

