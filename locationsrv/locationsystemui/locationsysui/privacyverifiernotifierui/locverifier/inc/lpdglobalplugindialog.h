/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  List query dialog suitable for verifier plug-in use.
*
*/


#ifndef CLPDGLOBALPLUGINDIALOG_H
#define CLPDGLOBALPLUGINDIALOG_H

// INCLUDES
#include <hbdevicemessageboxsymbian.h>
#include <hbdevicenotificationdialogsymbian.h>
#include <hbdevicedialogsymbian.h>


const TInt KReqBufferSize = 256;
// FORWARD DECLARATIONS
class MLpdDlgObserver;
class CLpdRequestorProcessor;
class CHbDeviceDialog;
// CLASS DECLARATION

/**
*  A list query dialog suitable for verifier plug-in use.
*  utilizes MLpdDlgObserver
*  for notifying when dialog is dismissed.
*
*  @lib locverifierdlg.lib
*  @since 2.1
*/
class CLpdGlobalPluginDialog : public CBase,
                               public  MHbDeviceDialogObserver ,
                               public  MHbDeviceNotificationDialogObserver 

	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aCallBack is notified when dialog is dismissed
        * @return created object
        */
        static CLpdGlobalPluginDialog* NewL(
                    MLpdDlgObserver& aCallBack);

		
        /**
        * C++ destructor.
        */
        virtual ~CLpdGlobalPluginDialog();

    public: // Functions from base classes

      
        void RunVerificationQueryL();
        
        void RunNotificationL();
        
                        
        void SetRequestInfoL( CLpdRequestorProcessor* aRequestInfo );
              
        
        void Cancel();
        
        void DataReceived(CHbSymbianVariantMap& aData);

        void DeviceDialogClosed(TInt aCompletionCode);
        
        void AddVerDlgHeadingParamL();
        
        void AddQueryTextParamL();
        
        void AddRequestorsParamL();
        
           
        
    public:
        void  NotificationDialogActivated (const CHbDeviceNotificationDialogSymbian *aDialog);
        
        void  NotificationDialogClosed (const CHbDeviceNotificationDialogSymbian *aDialog, TInt aCompletionCode);


          
    private:  // Constructors and destructor

        /**
        * Symbian 2nd Phase Constructor.
        */
		void ConstructL();

        /**
        * C++ constructor.
        * @param aCallBack is notified when dialog is dismissed
        */
        CLpdGlobalPluginDialog(
                    MLpdDlgObserver& aCallBack);

    private:    // Data

        // Object which is notified when query is dismissed
        MLpdDlgObserver& iCallBack;
        CHbDeviceDialogSymbian* iMsgBox;
        CHbDeviceNotificationDialogSymbian* iNotification;
        CLpdRequestorProcessor* iRequestInfo;
        CHbSymbianVariantMap* mSymbianVariantMap;
        TBool mDissmissed;
        TBool isCancelled;

        
	};

#endif      // CLPDGLOBALPLUGINDIALOG_H   
            
// End of File
