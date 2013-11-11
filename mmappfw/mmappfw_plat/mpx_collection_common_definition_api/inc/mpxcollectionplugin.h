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
* Description:  Collection plugin interface
*
*/


#ifndef CMPXCOLLECTIONPLUGIN_H
#define CMPXCOLLECTIONPLUGIN_H

#include <ecom/ecom.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxattribute.h>
#include <mpxfilter.h>
#include <mpxcommand.h>
#include <mpxsearchcriteria.h>
#include <mpxattributespecs.h>

class MMPXCollectionPluginObserver;
class MMPXCollectionEngineObserver;
class CMPXCollectionPath;
class CMPXActiveTaskQueue;
class MMPXTaskQueueObserver;

const TUid KMPXCollectionInterfaceUid={0x101FFC36};

/********************************************************************************
*
*  Note:
*
*  ** remove void CommandL(TMPXCollectionCommand aCmd, TInt aArg = 0)
*
*  ** remove MediaL() overload
*
*  ** remove null implementations
*
*  ** rename GetCapabilities() to Capabilities()
*
*********************************************************************************/

/**
 *  CMPXCollectionPlugin class
 *
 *  Interface to access collection
 */
NONSHARABLE_CLASS(CMPXCollectionPlugin): public CBase
    {

public: // Constructors and destructor

    /**
     * Two-phased constructor.
     *
     * @since S60 3.2.3
     * @param aUid uid of interface implementation
     * @param aObs Reference to the callback interface
     * @return object of constructed
     */
    inline static CMPXCollectionPlugin* NewL(const TUid& aUid,
                                             MMPXCollectionPluginObserver& aObs);

    /**
     * Destructor.
     *
     * @since S60 3.2.3
     */
    inline virtual ~CMPXCollectionPlugin();

public:

    /**
     * Callback setter.
     *
     * @since S60 3.2.3
     * @param aObs Reference to the callback interface
     */
    inline void SetObserver(MMPXCollectionPluginObserver& aObs);

    /**
     * 
     *
     * @param aCmd a command
     * @param aArg optional argument
     * @deprecated executes a command on the selected collection, use CommandL(CMPXCommand& aCmd)
     */
    virtual void CommandL(TMPXCollectionCommand aCmd, TInt aArg = 0)=0;

    /**
     * Executes a command.
     *
     * @since S60 3.2.3
     * @param aCmd a command
     */
    virtual void CommandL(CMPXCommand& /*aCmd*/)
        {/*Note: need to implement this */}

    /**
     * Navigates to the given path.
     *
     * @since S60 3.2.3
     * @param aPath a path
     * @param aAttrs attributes requested
     * @param aFilter filter to apply or NULL if none
     */
    virtual void OpenL(const CMPXCollectionPath& /*aPath*/,
                   const TArray<TMPXAttribute>& /*aAttrs*/,
                   CMPXFilter* /*aFilter*/) = 0;

    /**
     *
     *  Media properties of the current file (async).
     *  Note: if selection is set in aPath, HandleMedia will return an array of
     *        media properties of current selected items.
     *        if no selection is set in aPath, HandleMedia will return media
     *        properities of current item.
     *
     * @since S60 3.2.3
     * @param aPath path for the media file
     * @param aAttr attributes requested
     * @param aCaps platsec capabilities of client requesting media; plug-in should also
     *        verify its process capabilities
     * @param aSpecs, specifications for attributes
     * @deprecated use the following version of MediaL
     */
    virtual void MediaL(const CMPXCollectionPath& /*aPath*/,
                        const TArray<TMPXAttribute>& /*aAttrs*/,
                        const TArray<TCapability>& /*aCaps*/,
                        CMPXAttributeSpecs* /*aSpecs*/) = 0;
    /**
     * Cancel outstanding request.
     *
     * @since S60 3.2.3.
     */
    virtual void CancelRequest()=0;

    /**
     *  Adds an item or items to the collection.
     *
     *  @since S60 3.2.3
     *  @param aNewProperties Properties of the item
     */
    virtual void AddL(const CMPXMedia& aNewMedia) = 0;

    /**
     * Remove a collection path.
     * Note that the selection indicies are hidden within the path
     *
     * @since S60 3.2.3
     * @param aPath path to remove
     *
     */
    virtual void RemoveL(const CMPXCollectionPath& aPath ) = 0;

    /**
     *  Remove an item or items from the collection.
     *
     *  @since S60 3.2.3
     *  @param aProperties Properties of the item. It may cantain URI only
     *                      or meta data, all of items matched properties
     *                      will be removed.
     */
    virtual void RemoveL(const CMPXMedia& aMedia) = 0;

    /**
     *  Sets/updates the media for the item
     *  specified in the path.
     *
     *  @since S60 3.2.3
     *  @param aMedia new value
     */
    virtual void SetL(const CMPXMedia& aMedia) = 0;

    /**
     *  Find a list of items matched (async).
     *
     *  @since S60 3.2.3
     *  @param aCriteria properties to be searched
     *  @param aAttrs attributes to return
     */
    virtual void FindAllL(const CMPXSearchCriteria& aCriteria,
                          const TArray<TMPXAttribute>& aAttrs) = 0;

    /**
     * Find a list of items matched (sync).
     *
     *  @since S60 3.2.3
     *  @param aCriteria properties to be searched
     *  @param aAttrs attributes to return
     *  @return results of the search
     */
    virtual CMPXMedia* FindAllSyncL(const CMPXSearchCriteria& aCriteria,
                                    const TArray<TMPXAttribute>& aAttrs) = 0;

    /**
     * Get the list of supported capabilities.
     *
     * @since S60 3.2.3
     * @return TCollectionCapability bitmask of supported capabilities
     */
    virtual TCollectionCapability GetCapabilities() = 0;

    /** Note change it to pure virtual when all of plugins implement this interface
     *
     *  Media properties of the current file (async).
     *  Note: if selection is set in aPath, HandleMedia will return an array of
     *        media properties of current selected items.
     *        if no selection is set in aPath, HandleMedia will return media
     *        properities of current item.
     *
     * @since S60 3.2.3
     * @param aPath path for the media file
     * @param aAttr attributes requested
     * @param aCaps platsec capabilities of client requesting media; plug-in should also
     *        verify its process capabilities
     * @param aSpecs, specifications for attributes
     * @param aFilter filter to apply or NULL if none
     */
    virtual void MediaL(const CMPXCollectionPath& aPath,
                        const TArray<TMPXAttribute>& aAttrs,
                        const TCapabilitySet& aCaps,
                        CMPXAttributeSpecs* aSpecs,
                        CMPXFilter* /*aFilter*/)
            {
            RArray<TCapability> caps;
            CleanupClosePushL(caps);
            if (aCaps.HasCapability(ECapabilityDRM))
                {
                caps.AppendL(ECapabilityDRM);
                }
            // else add further "relevant" capabilities here

            // Add "none" capability, if no caps listed
            if (caps.Count() == 0)
                {
                caps.AppendL(ECapability_None);
                }
            MediaL(aPath, aAttrs, caps.Array(), aSpecs);
            CleanupStack::PopAndDestroy(&caps);
            };

public:

    /**
     * Implementation uid of plugin.
     *
     * @since S60 3.2.3
     */
    inline const TUid& Uid();

    /**
     * Current outstanding async task in the task queue.
     *
     * @since S60 3.2.3
     */
    inline TInt Task();

    /**
     *  Add a task and automatically schedule task.
     *
     *  @since S60 3.2.3
     *  @param aTask task id
     *  @param aCallback callback
     *  @param aHandler task handler
     *  @param aParamData data
     *  @param aBuf pointer to the buffer containing
     *               externalized parameter data.
     *               Client passes ownership of the buffer
     *               to TaskQueue.
     *  @param aPtrData object pointer
     *  @param aCObject1 pointer to CBased object, ownership transferred
     *  @param aCObject2 pointer to CBased object, ownership transferred
     */
     inline void AddTaskL(TInt aTask,
                          TAny* aCallback,
                          MMPXTaskQueueObserver* aHandler,
                          TInt aParamData=0,
                          CBufBase* aBuf=NULL,
                          TAny* aPtrData=NULL,
                          CBase* aCObject1=NULL,
                          CBase* aCObject2=NULL);

    /**
     *  Complete current task
     *
     *  @since S60 3.2.3
     */
    inline void CompleteTask();

    /**
     *  Complete all tasks wih the specified error code.
     *
     *  @since S60 3.2.3
     *  @param aError completion error code
     */
    inline void CompleteAllTasks(TInt aError);

    /**
     *  Remove task for one client.
     *
     *  @since S60 3.2.3
     *  @param aCallback callback when task completed
     */
    inline void RemoveTask(TAny* aCallback);

    /**
     * Callback of current active task.
     *
     * @since S60 3.2.3
     * @return pointer to call back object or NULL if no active task
     */
    inline MMPXCollectionEngineObserver* Callback();

    /**
     *  Set call back for current active task. 
     *  NULL if there is no active task for this plugin.
     *
     *  @since S60 3.2.3
     *  @param aCallback callback when task completed
     */
    inline void SetCallback(MMPXCollectionEngineObserver* aCallback);

private:
    /**
     * This is internal and not intended for use in plugin side.
     *
     * @since S60 3.2.3
     */
    inline void InitializeL();

protected:

    MMPXCollectionPluginObserver*  iObs; // Not owned
private: // Data

    // identifies the actual plugin instance
    TUid iDtorKey;
    // Task queue for the plugin
    CMPXActiveTaskQueue* iTaskQueue;
    TUid iUid; //Implementation uid of the plugin
    // Not owned, callback when current active task completed
    MMPXCollectionEngineObserver* iCallback;
    };

#include "mpxcollectionplugin.inl"
#endif      // CMPXCOLLECTIONPLUGIN_H


