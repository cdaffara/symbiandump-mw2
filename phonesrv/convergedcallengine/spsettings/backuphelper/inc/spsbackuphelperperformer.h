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
* Description:  Service provider settings backup helper performer
*
*/

#ifndef SPSBACKUPHELPERPERFORMER_H
#define SPSBACKUPHELPERPERFORMER_H

class CSPSettings;
class CSPEntry;

class CSpsBackupHelperPerformer :
    public CBase
    {
public:
    static CSpsBackupHelperPerformer* NewL();
    
    virtual ~CSpsBackupHelperPerformer ();
    
    /**
     * Performs restore operation finalization
     */
    void PerformL();

private: // New functions
    
    /**
     * Reads settings from service provider settings
     */
    void ReadSettingsL();
    
    /**
     * Finalizes restore
     */
    void FinalizeRestoreL();

    /**
     * Resets service provider settings to factory values
     */
    void ResetToFactorySettingsL();

    /**
     * Resolves next free serviceid in spsettings
     * @return result
     */
    TInt NextFreeServiceIdL();
    
    /**
     * Set's next free serviceid to spsettings
     * @param wanted service id
     */
    void SetNextFreeServiceIdL( TInt aServiceId );
    
private:
    CSpsBackupHelperPerformer();
    
    void ConstructL();

private:
    /**
     * Handle to spsettings
     */
    CSPSettings* iServiceProviderSettings;
    
    /**
     * Spsettings before factory reset
     */
    RPointerArray<CSPEntry> iEntries;
    };



#endif // SPSBACKUPHELPERPERFORMER_H
