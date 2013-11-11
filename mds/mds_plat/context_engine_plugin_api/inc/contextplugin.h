/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the Context plug-in ECom interface.
*
*/

#ifndef CCONTEXTPLUGIN_H
#define CCONTEXTPLUGIN_H

#include <e32base.h>
#include <implementationinformation.h>

// forward declarations
class CMdEObject;
class CMdESession;
class CHarvesterData;

/** Uid for this interface */
const TUid KCContextPluginInterfaceUid = { 0x200009F7 };

class MContextPluginObserver
	{
public:
    /**
     * A pure virtual method intended to inform about completion of context
     * plug-in initialization. Symbian error codes are used.
     *
     * @param aErrorCode  Error code for error occurred during initialization.
     */
    virtual void PluginInitializationStatus( TInt aErrorCode ) = 0;

    /**
     * A pure virtual method intended to inform about context snapshot taking completion.
     * Symbian error codes are used.
     *
     * @param aErrorCode  Error code for error occurred while taking the snapshot.
     */
    virtual void PluginSnapshotStatus( CHarvesterData* aHD ) = 0;
    };


/**
* This class implements the context plugin interface.
*/
class CContextPlugin : public CBase
    {
public:

    /**
     * Creates an implementation of a desired implementation defined by the parameter.
     *
     * @param aUid 
     * @return A pointer to the created plug-in.
     */
    IMPORT_C static CContextPlugin* NewL( const TUid& aUid );
    
    /**
     * Destroys the implementation. This is virtual method
     * and this class is not intended for derivation, so not exported.
     */
    IMPORT_C virtual ~CContextPlugin();

    /**
     * Lists all available implementations which implement this interface.
     *
     * @param[out] aImplInfoArray A list of existing implementations of the interface.
     */
    IMPORT_C static void ListImplementationsL( RImplInfoPtrArray& aImplInfoArray );

    /**
     * A pure virtual interface method to initialize the plug-in.
     *
     * @param aObserver An interface class to inform about initialization completion.
     */
    virtual void Init( MContextPluginObserver& aObserver ) = 0;

    /**
     * Set a pointer to MdESession.
     * Session is used to store harvested context data.
     *
     * @param aSession Session pointer to set.
     */
    virtual void SetMdeSession( CMdESession& aSession ) = 0;

    /**
     * This pure virtual method takes the context snapshot for one metadata object
     * and writes the information to the object received as parameter.
     * The observer will be informed about the status of the operation.
     *
     * @param aObserver  An interface class to inform about finishing context snapshot.
     * @param aMetadataObject An object to take the snapshot for.
     */
    virtual void ContextSnapshot( MContextPluginObserver& aObserver, CHarvesterData& aHD ) = 0;

    /**
     * This pure virtual method takes the context snapshot for several
     * metadata object and writes the information to the all the objects received as parameter.
     * The observer will be informed about the status of the operation.
     *
     * @param aObserver  An interface class to inform about finishing context snapshot.
     * @param aMetadataObjectArray Array of objects to take the snapshot for.
     */
    virtual void ContextSnapshot( MContextPluginObserver& aObserver,
                          RPointerArray<CHarvesterData>& aHDArray ) = 0;

private: // data

    /** 
     * Instance identifier key. When instance of an implementation is created
     * by ECOM framework, the framework will assign UID for it.
     * The UID is used in destructor to notify framework that this instance is
     * being destroyed and resources can be released.
     */
    TUid iDtor_ID_Key;
    };

#endif // CCONTEXTPLUGIN_H
