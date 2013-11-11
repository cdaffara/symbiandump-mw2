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
* Description: Implement of the CMPXViewPlugin class for testing the protect function 
*
*/
#ifndef DUMMYMPXVIEWPLUGIN_H_
#define DUMMYMPXVIEWPLUGIN_H_

#include <mpxviewplugin.h>
#include <mpxaknviewplugin.h>
#include <mpxakndialogplugin.h>


// ============================ MEMBER FUNCTIONS ===============================
class CDummyMPXViewPlugin : public CMPXViewPlugin
    {

public: 
    // Construct CDummyMPXViewPlugin
	CDummyMPXViewPlugin(const TUid& aUid,MMPXCustomCommandObserver* aCommandObserver);  
	
	// Delete CDummyMPXViewPlugin
	virtual ~CDummyMPXViewPlugin();
    
	virtual void CreateViewL();

    virtual void DestroyView();

    virtual void ActivateViewL( const TDesC* aParam = NULL );

    virtual void SetAsDefaultViewL();

    virtual void DeactivateView();
        
private : 
        
        CMPXViewPlugin* iMPXViewPlugin;
       
    };

#endif /*DUMMYMPXVIEWPLUGIN_H_*/
