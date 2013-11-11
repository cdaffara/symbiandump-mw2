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
* Description:  Meta data Access Singleton
*
*/


#ifndef CMMMTPDPACCESSSINGLETON_H
#define CMMMTPDPACCESSSINGLETON_H

class CMmMtpDpMetadataAccessWrapper;
class MMTPDataProviderFramework;

class CMmMtpDpAccessSingleton : public CObject
    {
public:
    /**
     * Create Singleton instance
     * need Release(), if no use
     * @param aRFs RFs reference
     * @param aFramework framework reference
     */
    IMPORT_C static void CreateL( MMTPDataProviderFramework& aFramework );
    
    /**
     * release singleton instance
     */
    IMPORT_C static void Release();
    
    /**
     * get metadata access wrapper reference
     */
    IMPORT_C static CMmMtpDpMetadataAccessWrapper& GetAccessWrapperL();
    
    /**
     * do some special process with assess DBs when receives opensession command
     */
    IMPORT_C static void OpenSessionL();
    
    /**
     * do some special process with assess DBs when receives closesession command
     */
    IMPORT_C static void CloseSessionL();
    
    /**
     * Issue ActiveToIdle Status Change (ActiveObject)
     * this function is to be called after an MTP operation is finish to change status from Active to Idle
     * observer of the status might not get the notification if idle->active->idle happens in one synchronous operation 
     */
    IMPORT_C static void ActiveToIdleStatusChange();

    /**
     * Cancel Outstanding ActiveToIdle Status Change
     */
    IMPORT_C static void CancelActiveToIdleStatusChange();

private:
    /*
     * get singleton instance, for internal use
     */
    static CMmMtpDpAccessSingleton* Instance();
    
    /*
     * 2-phase construction
     */
    static CMmMtpDpAccessSingleton* NewL( MMTPDataProviderFramework& aFramework );
    
    /*
     * 2-phase construction
     */
    void ConstructL( MMTPDataProviderFramework& aFramework );
    /*
     * destruction
     */
    ~CMmMtpDpAccessSingleton();
    
    /**
     * get DelayStatusChanger instance, internal use only
     */
    static CIdle* DelayStatusChanger();
    
    /**
     * Perform actural ActiveToIdle Status Change after active object callback
     */
    static TInt DoActiveToIdleStatusChange(TAny* Any);
    
private:
    CMmMtpDpMetadataAccessWrapper* iWrapper;
    
    CIdle* iDelayStatusChanger;

    };

#endif // CMMMTPDPACCESSSINGLETON_H
