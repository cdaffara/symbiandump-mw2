/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class declaration of CLbtDiskSpaceListner
*
*/


#ifndef C_LBTDISKSPACELISTNER_H
#define C_LBTDISKSPACELISTNER_H

#include <e32base.h>
#include <f32file.h>


class MLbtDiskSpaceObserver;

/**
 *  This class listens to disk space change
 *  
 *  CLbtDiskSpaceListner registers for change in disk space
 *
 *  @since S60 v5.1
 */
class CLbtDiskSpaceListner : public CActive
    {
public:
    /**
     * Factory function that instantiates an object of CLbtDiskSpaceListner
     */
    static CLbtDiskSpaceListner* NewL( MLbtDiskSpaceObserver& aObserver );
    
    /**
     * Destructor.
     */
    virtual ~CLbtDiskSpaceListner();

    /**
     * Starts listening to disk space change
     */
    void StartToListen();

         
protected: // From CActive
    void RunL();
    void DoCancel();

private: 
    /**
     * Default constructor
     */
    CLbtDiskSpaceListner( MLbtDiskSpaceObserver& aObserver );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL(); 

private://Data
    // Handle to the observer
    MLbtDiskSpaceObserver&      iObserver; 
    
    // Handle to file server
    RFs      iFs;
    };


#endif // C_LBTDISKSPACELISTNER_H
