/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitors Usb insertions and removals.*
*/


#ifndef __MMCUSBAO_H__
#define __MMCUSBAO_H__

#include <e32base.h>
#include <f32file.h>
#include <e32property.h>

#include "mmcmonitorao.h"

class CMMCUsbAO : public CActive
    {
    public:
        
        static CMMCUsbAO* NewL();
        virtual ~CMMCUsbAO();
        
        TBool StartMonitoring( MMMCMonitorObserver& aObserver );
        TBool StopMonitoring();
        TBool Resume();
        TBool Pause();
        
        void RunL();
        TInt RunError( TInt aError );
        void DoCancel();
        
    private:
        
        CMMCUsbAO();
        void ConstructL();

        void StartNotify();
        TUint32 MediaID( TInt aDrive );
        
#ifdef _DEBUG
        void PrintDriveStatus( TInt aStatus );
#endif
        
    private:

        MMMCMonitorObserver* iObserver;
        RFs iFs;
        RProperty iDriveState;
        TInt iDrive;
        TBool iMassStorageMode;
    };

#endif // __MMCUSBAO_H__
