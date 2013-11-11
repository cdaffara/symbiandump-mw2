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
* Description:  Positioning settings plugin helper private class. 
*
*/

#ifndef POSITIONINGSETTINGSPLUGIN_HELPERPRIVATE_H
#define POSITIONINGSETTINGSPLUGIN_HELPERPRIVATE_H

class PosSettingsPluginHelper;
/**
 * PosSettingsPluginHelperPrivate is a class that provides helper functions:
 * To check if feature flag is On/Of
 * To check if any positioning PSY is visible
 */
class PosSettingsPluginHelperPrivate
    {
private:
    
    /**
     * Returns whether any position psy is available.
     * @return true if any position psy is available,
     *         false if it is not available.
     */
    bool isPositionPsyAvailable();
    
     /**
     * Returns whether feature flag is enabled.
     * @return true if feature flag is enabled,
     *         false if it is not enabled.
     */
    bool isFeatureFlagEnabled();
    
    friend class PosSettingsPluginHelper;
    };


#endif /* POSITIONINGSETTINGSPLUGIN_HELPERPRIVATE_H_ */
