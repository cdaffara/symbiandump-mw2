/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Active object wrapper for launching Settings UI
*
*/


#ifndef C_COMASUPLSETTINGSUIOBSERVER_H
#define C_COMASUPLSETTINGSUIOBSERVER_H

/**
* This class provides a callback interface for handling the
* completion of settings.  The Client derives a
* class from this interface and implements the SettingsUICompleted-methods
* that interest it.
*/
class MOMASuplUICompletionObserver
    {

    public:

    /** 
    * This callback method is used to notify the client about
    * completion of settingsUi Launch
    * @param aError error encountered while launching change
    * @return None
    */
    virtual void SettingsUICompletedL(TInt aError) = 0;

    /** 
    * This callback method is used to notify the client about
    * completion of settingsUi Launch for SUPL usage
    * @param aError error encountered while launching change
    * @return None
    */
   
    virtual void SettingsUsageUICompletedL(TInt aError) = 0;

    /** 
    * This callback method is used to notify the client about
    * completion of Timeout UI
    * @param aError error encountered while launching change
    * @return None
    */
   
    virtual void SettingsTimeOutUICompletedL(TInt aError) = 0;
    };
#endif // C_COMASUPLSETTINGSUIOBSERVER_H

