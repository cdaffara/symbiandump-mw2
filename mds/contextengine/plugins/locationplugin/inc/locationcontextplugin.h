/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  A Context Engine Plug-in to get location context information.
*
*/


#ifndef CLOCATIONCONTEXTPLUGIN_H
#define CLOCATIONCONTEXTPLUGIN_H

#include <e32def.h>

#include "contextplugin.h"
#include "rlocationtrail.h"
#include "rlocationobjectmanipulator.h"
#include "locationdatatype.h"

// forward declarations
class TLocality;
class CMdESession;
class CHarvesterData;
class CMdEProperty;

/**
* This class implements the context plugin interface for GPS
* location information.
*
*/
class CLocationContextPlugin : public CContextPlugin
    {
public:

    /**
     * Creates and constructs a new instance of CLocationContextPlugin.
     *
     * @return A pointer to the created instance.
     */
    static CLocationContextPlugin* NewL();

    /**
     * Destructor.
     */
    virtual ~CLocationContextPlugin();

// from base class CContextPlugin

    /**
     * From CContextPlugin.
     * Initialization of Location Context Plugin.
     *
     * @param An observer to notify the completion of initialization to.
     */
    void Init( MContextPluginObserver& aObserver );

    /**
     * Set a pointer to MdESession.
     * Session is used to store harvested context data.
     *
     * @param aSession Session pointer to set.
     */
    void SetMdeSession( CMdESession& aSession );

    /**
     * From CContextPlugin.
     * This method takes a location context snapshot for one metadata object
     * and writes the information to the object received as parameter.
     * The observer will be informed about the status of the operation.
     *
     * @param aObserver  An interface class to inform about finishing context snapshot.
     * @param aMetadataObject An object to take the snapshot for.
     */
    void ContextSnapshot( MContextPluginObserver& aObserver, CHarvesterData& aHD );

    /**
     * From CContextPlugin.
     * This method takes a location context snapshot for several metadata objects
     * and writes the information to the all the objects received as parameter.
     * The observer will be informed about the status of the operation.
     *
     * @param aObserver  An interface class to inform about finishing context snapshot.
     * @param aMetadataObjectArray Array of objects to take the snapshot for.
     */
    void ContextSnapshot( MContextPluginObserver& aObserver,
                          RPointerArray<CHarvesterData>& aHDArray );

private:

    /**
     * Constructor, called by NewL() only.
     */
    CLocationContextPlugin();

    /**
     * Second phase constructor called by NewL.
     */
    void ConstructL();

    /**
     * A simple method to get creation date/time from a mde object.
     *
     * @param aObject        A metadata object to get the date/time from.
     * @param aTimeProperty  A property to store the time information to.
     */    
    void GetMdeObjectCreationTimeL( CMdEObject& aObject, CMdEProperty*& aProperty );

private: // data

    /** @var Observer to notify about finishing status of the context snapshot. */
    MContextPluginObserver* iObserver;
    
    /** @var Service to get GPS location information. */
    RLocationTrail iLocationTrail;
    
    /** @var Pointer to Mde session which is used to store context data. */
    CMdESession* iMdeSession;
    
    /** @var Location trail status flag to signal if Connect() was successful. */
    TBool iLocationTrailConnected;
    
    /** @var Service to modify location context objects. */
    RLocationObjectManipulator iManipulator;
    };

#endif // CLOCATIONCONTEXTPLUGIN_H
