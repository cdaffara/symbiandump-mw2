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
* Description: MSuplEcomEventObserver is an observer
*
*/

#ifndef __EPOS_MSUPLECOMEVENTOBSERVER_H__
#define __EPOS_MSUPLECOMEVENTOBSERVER_H__


// CLASS DECLARATION

/**
*  MSuplEcomEventObserver is an observer
*/
class MSuplEcomEventObserver
	{
	public:
	
		// Interface methods
    
    	/**
        * PlugInStatusChanged.
        * @param aSuplEcomPlugInEvent specifies which event occurred
        * @return void
        */
		virtual void PlugInUninstalled() = 0;
	};
	
#endif