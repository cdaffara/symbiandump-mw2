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
* Description:  Service provider settings backup helper monitor
*
*/

#ifndef SPSBACKUPHELPERMONITOR_H
#define SPSBACKUPHELPERMONITOR_H

#include <e32property.h>
#include <e32base.h>

#include <abclient.h>
using namespace conn;

class CSpsBackupHelperPerformer;

class CSpsBackupHelperMonitor : 
    public CActive
    {
public:
    static CSpsBackupHelperMonitor* NewL();
    
    virtual ~CSpsBackupHelperMonitor();
    
private: // From CActive
    
    void RunL();

    TInt RunError( TInt aError );
    
    void DoCancel();
    
private: // New functions

    /**
     * Process backup state
     * 
     * @param aBackupStateValue P&S-key from abdefs.h 
     */
    void ProcessBackupStateL( TInt aBackupStateValue );

    /**
     * Determines if there is restore ongoing
     * 
     * @param aBackupStateValue P&S-key from abdefs.h
     * @return result
     */
    TBool RestoreOngoing( TInt aBackupStateValue );

    /**
     * Determines if there is no backup/restore ongoing
     * 
     * @param aBackupStateValue P&S-key from abdefs.h
     * @return result
     */
    TBool NoBackupRestore( TInt aBackupStateValue );
    
    /**
     * Check the sbs status
     */
    void CheckStatusL();
    
    /**
     * Subscribe P&S key
     */
    void Subscribe();
    
    /**
     * Implements logic to inform secure backup engine that
     * we are ready for backup or restore
     * @return If we are participating to backup or restore
     */
    TBool ParticipateBackupAndRestoreL( TInt aBackupStateValue );
    
private:
    CSpsBackupHelperMonitor();
    
    void ConstructL();

private:
    /**
     * Active backup client
     */
    CActiveBackupClient* iABClient;
    
    /**
     * Performs restore finalization
     */
    CSpsBackupHelperPerformer* iPerformer;
    
    /**
     * Property to listened
     */
    RProperty iProperty; /*<! SBE’s P&S Flag */

    /**
     * 
     */
    TBool iParticipateRestore;
    };

#endif // SPSBACKUPHELPERMONITOR_H
