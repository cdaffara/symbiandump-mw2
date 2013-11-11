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
* Description:  Monitors MMC insertions and removals.*
*/


#ifndef __MMCMONITORAO_H__
#define __MMCMONITORAO_H__

#include <e32base.h>
#include <f32file.h>
#include "mdscommoninternal.h"

enum TMMCEventType
    {
    EMounted,
    EDismounted,
    EFormatted
    };

class MMMCMonitorObserver
    {
    public:
        /**
        * A pure virtual method to notify about any MMC insertions and removals.
        *
        * @param aDriveChar  A drive char of MMC which was inserted or removed.
        * @param aEventType  EMounted if MMC is inserted, EDismounted if it is removed.
        */
        virtual void MountEvent( TChar aDriveChar,
        	TUint32 aMediaID, TMMCEventType aEventType ) = 0;
    };

class CMMCMonitorAO : public CActive
    {
    public:
        
        /**
        * Creates and constructs a new instance of CMMCMonitorAO.
        *
        * @return A pointer to the new instance of CMMCMonitorAO
        */
        static CMMCMonitorAO* NewL();
        
        /**
        * Destructor
        */
        virtual ~CMMCMonitorAO();
        
        /**
        * Starts monitoring the MMC insertions and removals.
        *
        * @param aObserver  Both events are notified via the aObserver.
        * @param aMedias Array of present medias
        * @return ETrue if success, EFalse if not
        */
        TBool StartMonitoring( MMMCMonitorObserver& aObserver, RArray<TMdEMediaInfo>& aMedias );
        
        /**
        * Stops monitoring.
        *
        * @return ETrue if success, EFalse if not
        */
        TBool StopMonitoring();
        
        /**
        * Inherited from CActive. This method will be called on file server notifying.
        */
        void RunL();
        
        /**
        * Handles a leave occurring in the request completion event handler RunL().
        *
        * @param aError  An error code.
        * @return An error code.
        */
        TInt RunError( TInt aError );
        
        /**
        * Cancels file server notifying. Inherited from CActive.
        */
        void DoCancel();
        
    private:
        
        /**
        * C++ constructor - not exported;
        * implicitly called from NewL()
        */
        CMMCMonitorAO();
        
        /**
        * 2nd phase construction, called by NewL()
        */
        void ConstructL();
        
        /**
        * Starts file server to notify us about MMC insertions and removals.
        */
        void StartNotify();
        
        /**
        * Makes a list of all removable medias which is present.
        */
        void BuildDriveList();
        
        /**
        * Compares previous and present drive list. Notify an observer about any differences in lists.
        */
        void CompareDriveLists();
        
#ifdef _DEBUG
    	void PrintAllLists();
#endif
    
        
    private:
        /**
        * An observer.
        */
        MMMCMonitorObserver* iObserver;
        
        /**
        * A file server.
        */
        RFs iFs;
        
        /**
        * A present drive list.
        */
        TDriveList iDriveList;
        
        /**
        * A previous drive list.
        */
        TDriveList iPreviousDriveList;
        
        TUint32 iMediaIdList[KMaxDrives];
    };

#endif // __MMCMONITORAO_H__
