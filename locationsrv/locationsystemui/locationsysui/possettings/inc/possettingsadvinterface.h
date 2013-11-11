/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Positioning settings advanced interface class.
*
*/

#ifndef POSSETTINGSENGADVANCEDINTERFACE_H
#define POSSETTINGSENGADVANCEDINTERFACE_H

#include "possettingsadvop.h"
#include <QtPlugin>



//Provider id and version
static const char* KAdvancedInterfaceId = 
           "com.nokia.PosSettingsAdvInterface/1.0";

 
/**
 * Settings UI ECom plug-in interface
 *
 * PosSettingsAdvInterface class tdefines interface for Advanced 
 * positioning settings UI. Any Settings UI component should derive from this 
 * class and implement the abstract functions.
 *
 */
class PosSettingsAdvInterface 
    {
    
public:
    /**
     * Destructor
     */
    virtual ~PosSettingsAdvInterface(){}
	
  
    /**
     *	This method performs operations required to display advanced settings UI 
     *	The operations would involve the following :
     *	- add data model items
     *	- add connections for the data model items
     *	- add actions for menu items ( if any )
     *	- add connection to the view using the slot updateAdvancedView
     *	  to update the view when the form is updated.
     */
    virtual int initialise( PosSettingsAdvOp* operation ) = 0;
    
    };


// MACROS
Q_DECLARE_INTERFACE( PosSettingsAdvInterface,KAdvancedInterfaceId );

#endif /* POSSETTINGSENGADVANCEDINTERFACE_H */
