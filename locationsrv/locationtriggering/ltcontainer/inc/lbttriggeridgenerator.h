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
* Description:  This header file describes the class that handles the storage
*                of triggers in RAM Structures.
*
*/


#ifndef C_LBTCONTAINER_TRIGGERIDGENERATOR_H
#define C_LBTCONTAINER_TRIGGERIDGENERATOR_H

// system includes
#include <e32base.h>
#include <f32file.h>
// user includes






/**
 *  This class provides an abstraction to store and retrieve triggers
 *  from RAM Structures.
 *
 *  It internally utilizes RAM memory and stores the trigger information
 *  in a efficient data structure.
 */
NONSHARABLE_CLASS( CLbtTriggerIdGenerator ) : public CBase
                                        
    {
public:
    /**
     * The Symbian 2 phase constructor.
     * This method creates a new instance of CLbtTriggerIdGenerator
     *
     * @return An instance of this class
     */
    static CLbtTriggerIdGenerator* NewL();

    /**
     * Destructor.
     */
    virtual ~CLbtTriggerIdGenerator();
	
	TInt64 GetTriggerId();
	
	void SetTriggerId(TInt64 aId);

protected: // from base class MLbtTriggerStore
   
private:
    /**
     * C++ constructor
     */
    CLbtTriggerIdGenerator();

    /**
     * Symbian 2nd phase of construction
     */
    void ConstructL();

    /**
     * C++ Copy Constructor
     * The private copy constructor prevents the usage of copy constructors
     * with this class.
     */
    CLbtTriggerIdGenerator(const CLbtTriggerIdGenerator& aRamTriggersManager);
    
    /**
     * Overload equality operator
     * The private overload of the equality operator prevents the usage of
     * equality operator with this class.
     */
    CLbtTriggerIdGenerator& operator =(const CLbtTriggerIdGenerator& aRamTriggersManager);
    
    RFs iFileServer;
    RFile iFileHandle; 
    };


#endif // C_LBTCONTAINER_TRIGGERIDGENERATOR_H
