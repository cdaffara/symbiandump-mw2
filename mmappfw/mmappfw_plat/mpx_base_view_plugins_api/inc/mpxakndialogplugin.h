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
* Description:  MPX Avkon dialog plugin definition.
*
*/



#ifndef CMPXAKNDIALOGPLUGIN_H
#define CMPXAKNDIALOGPLUGIN_H


// INCLUDES
#include "mpxviewplugin.h"


// FORWARD DECLARATIONS
class CAknDialog;


// CLASS DECLARATION

/**
 *  MPX Avkon dialog plugin definition.
 *
 *  @lib mpxviewplugin.lib
 */
class CMPXAknDialogPlugin : public CMPXViewPlugin
    {
public:

    /**
     * Destructor.
     *
     * @since S60 3.0
     */
    IMPORT_C virtual ~CMPXAknDialogPlugin();

protected:

    /**
     * Interface method to construct Avkon dialog.
     *
     * @since S60 3.0
     * @return Pointer to a newly created Avkon dialog.
     */
    virtual CAknDialog* ConstructDialogL() = 0;

    /**
     * Return resource id of the Avkon dialog.
     *
     * @since S60 v.0
     * @return Dialog resource id.
     */
    virtual TInt ResourceId() = 0;

// from base class CMPXViewPlugin

    /**
     * Interface method to create view.
     * From CMPXViewPlugin
     *
     * @since S60 3.0
     */
    IMPORT_C virtual void CreateViewL();

    /**
     * Interface method to destroy view.
     * From CMPXViewPlugin
     *
     * @since S60 3.0
     */
    IMPORT_C virtual void DestroyView();

    /**
     * Interface method to activate view.
     * From CMPXViewPlugin    
     *
     * @since S60 3.0
     * @param aParam Parameter required to activate the view.
     */
    IMPORT_C virtual void ActivateViewL( const TDesC* aParam = NULL );
    
    /**
     * Interface method to activate as default view.
     * Will not bring to foreground.
     * From CMPXViewPlugin     
     *
     * @since S60 3.0
     * @leave KErrNotSupported
     */
    IMPORT_C virtual void SetAsDefaultViewL();

    /**
     * Interface method to deactivate view.
     * From CMPXViewPlugin     
     *
     * @since S60 3.0
     */
    IMPORT_C virtual void DeactivateView();

protected:  // data

    /**
     * Avkon dialog that associates with this plugin.
     */
    CAknDialog* iDialog;
    };

#endif  // CMPXAKNDIALOGPLUGIN_H

// End of File
