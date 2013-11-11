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
* Description:  MPX view plugin definition.
*
*/



#ifndef CMPXVIEWPLUGIN_H
#define CMPXVIEWPLUGIN_H


// INCLUDES
#include <ecom/ecom.h>


// CLASS DECLARATION
class MMPXCustomCommandObserver;

/**
 *  MPX view plugin definition.
 *
 *  @lib mpxviewplugin.lib
 */
class CMPXViewPlugin : public CBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 3.0
     * @param aUid Implementation Uid.
     * @param aCommandObserver Observer to handle commands.
     * @return Pointer to newly created object.
     */
    IMPORT_C static CMPXViewPlugin* NewL( 
        const TUid& aUid,
        MMPXCustomCommandObserver* aCommandObserver );

    /**
     * Destructor.
     *
     * @since S60 3.0
     */
    IMPORT_C virtual ~CMPXViewPlugin();

    /**
     * Interface method to create view.
     *
     * @since S60 3.0
     */
    virtual void CreateViewL() = 0;

    /**
     * Interface method to destroy view.
     *
     * @since S60 3.0
     */
    virtual void DestroyView() = 0;

    /**
     * Interface method to activate view.
     *
     * @since S60 3.0
     * @param aParam Parameter required to activate the view.
     */
    virtual void ActivateViewL( const TDesC* aParam = NULL ) = 0;
    
    /**
     * Interface method to activate as default view. 
     * Will not bring to foreground.
     *
     * @since S60 3.0
     */
    virtual void SetAsDefaultViewL() = 0;

    /**
     * Interface method to deactivate view.
     *
     * @since S60 3.0
     */
    virtual void DeactivateView() = 0;
    
protected:
    /**
     * Sets observer for custom commands.
     *
     * @since S60 3.0
     * @param aCommandObserver Observer to handle commands.
     */
    IMPORT_C virtual void SetObserver( 
        MMPXCustomCommandObserver* aCommandObserver );
    
protected: // Data

    MMPXCustomCommandObserver* iCommandObserver; // not owned    

private:    // Data

    // identifies the actual plugin instance
    TUid iDtorKey;
    };

#endif  // CMPXVIEWPLUGIN_H

// End of File
