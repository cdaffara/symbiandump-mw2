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
* Description: This is the first view for Positioning Settings plugin  
*
*/

#ifndef POSSETTINGSVIEW_H
#define POSSETTINGSVIEW_H

#include "possettingsglobal.h"

#include<QObject>
#include <cpbasesettingview.h>

// Forward declaration
class PosSettingsEngine;
class PosSettingsForm;
class HbGroupBox;
class QGraphicsLinearLayout;

class POSSETTINGS_EXPORT PosSettingsView: public CpBaseSettingView
    {    
public:
    /**
     * Constructor 
     */
     explicit PosSettingsView( QGraphicsItem *parent = 0 );
    
    /**
     * Destructor
     */ 
    ~PosSettingsView();
     
private: // Data members
    /**
     * Pointer to settings engine object
     * Own
     */ 
    PosSettingsEngine* mSettingsEngine;
    
    /**
     * Pointer to settings form object.
     */
    PosSettingsForm* mSettingsForm;    
    };

#endif // POSSETTINGSVIEW_H
