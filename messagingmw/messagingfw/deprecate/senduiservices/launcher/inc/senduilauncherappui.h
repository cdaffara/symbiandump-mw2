/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Server application for launching SendUi with low 
*                capability requirements.
*
*/





#ifndef C_CSENDUILAUNCHERAPPUI_H
#define C_CSENDUILAUNCHERAPPUI_H


#include <aknappui.h>

class CMessageData;

/**
 *  Launcher application ui constructs SendUi object
 *
 *  ?more_complete_description
 *
 *  @since S60 v.3.2
 */
class CSendUiLauncherAppUi : public CAknAppUi
    {

public:

    virtual ~CSendUiLauncherAppUi ();

    /**
     * Creates and sends message
     *
     * @since S60 v.3.2
     * @param aServiceId Uid of the requested service
     * @param aMessageData Message data
     * @param aLaunchEmbedded ETrue if editor is to be launched embedded. 
     *      Otherwise EFalse.
     */
   void CreateAndSendMessageL( 
        TUid aServiceUid, 
        CMessageData* aMessageData, 
        TBool aLaunchEmbedded );
        
    /**
     * Used for exiting server application properly
     *
     * @since S60 v.3.2
     * @param aThis Pointer to this AppUi
     */
    static TInt DelayedExit( TAny* aThis );

    /**
     * Does delayed exit earliest after specified delay
     *
     * @since S60 v.3.2
     * @param aDelayTime Requested delay in microseconds
     */
   void DoDelayedExitL( TInt aDelayTime );
   
// from base class CAknAppUi

    /**
     * From CAknAppUi.
     * Handles commands. Only Exit is implemented.
     *
     * @since S60 v.3.2
     * @param aCommand Command code
     */
     void HandleCommandL(TInt aCommand);
    
     CSendUiLauncherAppUi();

private:

 
    void ConstructL();

private: // data

    /**
     * Timer for the delayed exit function
     * Own. 
     */
    CPeriodic* iIdle;

    };


#endif // C_CSENDUILAUNCHERAPPUI_H
