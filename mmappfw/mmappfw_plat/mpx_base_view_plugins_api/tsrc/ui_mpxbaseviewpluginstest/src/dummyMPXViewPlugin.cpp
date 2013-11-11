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
* Description:  Implement of the CMPXViewPlugin class for testing the protect function 
*
*/
#include "dummyMPXViewPlugin.h"

// CDummyMPXViewPlugin construct
CDummyMPXViewPlugin::CDummyMPXViewPlugin(const TUid& aUid,MMPXCustomCommandObserver* aCommandObserver)
		{
		iMPXViewPlugin = CMPXViewPlugin::NewL(aUid,aCommandObserver);
		}  

// delete CDummyMPXViewPlugin
CDummyMPXViewPlugin::~CDummyMPXViewPlugin()
    	 {
    	 if(iMPXViewPlugin!=NULL)
    		 {
    		 delete iMPXViewPlugin;
    		 }
    	 }
    
// OverWrite the CreateViewL() function of the CMPXViewPlugin class 
void CDummyMPXViewPlugin::CreateViewL() 
	        {
	        iMPXViewPlugin->CreateViewL();
	        }

// OverWrite the DestroyView() function of the CMPXViewPlugin class
void CDummyMPXViewPlugin::DestroyView() 
        	{
        	iMPXViewPlugin->DestroyView();
        	}

// OverWrite the ActivateViewL() function of the CMPXViewPlugin class
void CDummyMPXViewPlugin:: ActivateViewL( const TDesC* /*aParam*/ ) 
	{
	iMPXViewPlugin->ActivateViewL(/*aParam*/);
	}

// OverWrite the SetAsDefaultViewL() function of the CMPXViewPlugin class
void CDummyMPXViewPlugin:: SetAsDefaultViewL() 
	{
	iMPXViewPlugin->SetAsDefaultViewL();
	}

// OverWrite the DeactivateView() function of the CMPXViewPlugin class
void CDummyMPXViewPlugin:: DeactivateView()
    {
    iMPXViewPlugin->DeactivateView();
    }
