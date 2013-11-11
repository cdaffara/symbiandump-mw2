/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class definition of Status Manager
*
*/



#ifndef C_CLBTSTATUSMANAGER_H
#define C_CLBTSTATUSMANAGER_H


#include <e32property.h>


typedef TInt16 TAcquisitionStatus;


/**
 * This class defines the methods used by CLbtStrategyEngine to report 
 * location acquisition status. This class uses Location Triggering Status 
 * Information API. 
 */
NONSHARABLE_CLASS( CLbtStatusManager ) : public CBase

	{
	
public:

    /**
     * Factory function that instantiates an object of CLbtStatusManager
     */
	static CLbtStatusManager* NewL();
	
	/**
	 * Destructor
	 */
	~CLbtStatusManager();
	
	/**
	 * This method is invoked by an instance of CLbtStrategyEngine to report
	 * location acquisition status.
	 */
	void ReportLocationAcquisitionStatus( TAcquisitionStatus aStatus );
	
	/**
	 * This method checks the status of last location acquisition operation.
	 * TRUE if successful, otherwise FALSE
	 */
	TBool LocationAcqSuccess();

private:

	/**
	 * Default constructor
	 */
	CLbtStatusManager();
	
	/**
	 * 2nd phase constuctor for instantiating member variables
	 */
	void ConstructL();
	
private:

	RProperty	            iStatusInfo;
	
	TAcquisitionStatus      iLastLocAcqStatus;
	
	};

#endif	// C_CLBTSTATUSMANAGER_H
