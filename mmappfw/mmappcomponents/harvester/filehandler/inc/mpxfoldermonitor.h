/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitors for new or deleted files
*
*/


#ifndef CMPXFOLDERMONITOR_H
#define CMPXFOLDERMONITOR_H

#include <e32base.h>
#include <f32file.h>

class MMPXFileAdditionObserver;
class MMPXFolderMonitorObserver;

/**
 *  Folder monitor class
 *
 *  @lib mpxfilehandler
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXFolderMonitor ) : public CActive
    {

public:

    /**
    * Two-phased constructor
    */
    static CMPXFolderMonitor* NewL( MMPXFolderMonitorObserver& aObserver, 
                                    RFs& aFs );

    /**
    * Virtual destructor
    */
    virtual ~CMPXFolderMonitor();

    /**
    * Start monitoring a particular drive
    * @param aDrive drive to monitor
    */
    void StartL( TDriveNumber aDrive );

    /**
    * Start monitoring a particular folder
    * @param aFolder folder to monitor
    */
    void StartL( const TDesC& aFolder );
    
protected:
        
    /**
    * From CActive
    */
    void RunL();
    
    /**
    * From CActive
    */
    void DoCancel();

    /**
    *  From CActive
    */
    TInt RunError(TInt aError);    
            
private:
    
    /**
    * Private constructor
    */
    CMPXFolderMonitor( MMPXFolderMonitorObserver& aObserver, 
                       RFs& aFs );

    /**
    * 2nd phased constructor
    */
    void ConstructL();

private: // data
    MMPXFolderMonitorObserver&  iObserver;
    RFs& iFs; 
    HBufC* iFolderName;   // drive name, always "?:\\"
    };

#endif // CMPXFOLDERMONITOR_H
