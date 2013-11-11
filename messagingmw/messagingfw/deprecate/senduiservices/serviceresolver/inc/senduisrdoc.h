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
* Description:   Document class for SendUi service resolver
*
*/





#ifndef C_CSENDUISRDOC_H
#define C_CSENDUISRDOC_H


#include <akndoc.h>
#include <ConeResLoader.h>

// forward declarations
class CSendUiSrAppUi;
class CEikApplication;

/**
 *  Document class for SendUiServiceResolver application
 *
 *  An instance of class CSendUiSrDocument is the Document part of the AVKON
 *  application framework for the SendUiServiceResolver application
 *  @since S60 v3.2
 */
class  CSendUiSrDocument : public CAknDocument
    {

public:

    static CSendUiSrDocument* NewL(CEikApplication& aApp);
    
    static CSendUiSrDocument* NewLC(CEikApplication& aApp);

    virtual ~CSendUiSrDocument();

// from base class CAknDocument

    /**
     * From CAknDocument.
     * Create new AppUi
     *
     * @since S60 v3.2
     * @return Reference to new AppUi.
     */
    CEikAppUi* CreateAppUiL();

private:

    CSendUiSrDocument(CEikApplication& aApp);

    void ConstructL();
 
    /**
     * Create new AppUi
     *
     * @since S60 v3.2
     * @return Reference to new AppUi.
     */
    void HandleUpdateSendList();
    
    /**
     * Hides task name from fast swap window
     *
     * @since S60 v3.2
     * @param aWgName Reference to a new window group name
     */
    void UpdateTaskNameL(CApaWindowGroupName* aWgName);

    };


#endif // C_CSENDUIDOC_H
