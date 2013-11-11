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
* Description:  Definition of the ClientDescriptorSource class.
*
*/


#ifndef C_CCLIENTDESCRIPTORSOURCE_H
#define C_CCLIENTDESCRIPTORSOURCE_H

#include <e32base.h>
#include <DescriptorSource.h>
#include <mmfcontrollerframework.h>

#include "SourceBase.h"
#include "AsyncAO.h"
#include "DescriptorDataSourceCommon.h"

class MCustomCommand;

namespace multimedia
    {

    class CEventNotifier;

    /**
    *  Implements the ClientSide Source for the Descriptor Source.
    *  Provides functionality to the Clients to query attributes
    *  from the ServerSide source
    *  @lib EnhancedMediaClient.lib
    *  @since S60 v3.2
    */    
    class CClientDescriptorSource : public CSourceBase,
                                    public MDescriptorSource,
                                    public MAsyncAOObserver
        {
        public:
            /**
            * Function to Create the Object.
            */
            CClientDescriptorSource();

            /**
            * Destructor.
            */
            ~CClientDescriptorSource();

            /**
            * Post Contructor. This is to Support the new type of Construction
            * of the Object. Notice there is no Two phased Constructor in this class
            */
            TInt PostConstructor();
            
            // From MControl begins
            /**
            * From MControl.
            * Adds the Observer to this Object
            * @since S60 v3.2
            * @param aObserver Client which set the Observer.             
            */            
            TInt AddObserver( MControlObserver& aObserver );
            /**
            * From MControl.
            * Removes the Observer to this Object
            * @since S60 v3.2
            * @param aObserver Client which set the Observer.             
            */            
            TInt RemoveObserver( MControlObserver& aObserver );
            /**
            * From MControl.
            * Returns the Type of this Object.
            * request and that it is now outstanding 
            * @since S60 v3.2
            */            
            TUid Type();
            /**
            * From MControl.
            * Returns the Control Type of this Object.
            *
            * @since S60 v3.2
            */            
            TControlType ControlType();
            // From MControl ends
            
            // From MSourceControl begins
            /**
            * From MSourceControl.
            * Returns the Size in Bytes of the Source.
            * Since this is Descriptor Source, the size represents
            * the descriptor size which will be passed by the Client
            *  to the Source
            * @since S60 v3.2
            * @param aSize Size returned by the Source.             
            */            
            TInt GetSize( TUint& aSize );
            /**
            * From MSourceControl.
            * Returns the Mime Type of the Data for which
            * the Source is initialized.
            * @since S60 v3.2
            * @param aMimeType MimeType returned by the Source.             
            */            
            TInt GetMimeType( TDes8& aMimeType );
            /**
            * From MSourceControl.
            * Closes the Source.
            *
            * @since S60 v3.2
            */            
            TInt Close();
            // From MSourceControl ends
            
            // From MDescriptorSource begins
            /**
            * From MDescriptorSource.
            * Opens the Source with the mimetype and HeaderData
            *
            * @since S60 v3.2
            * @param aMimeType     MimeType used to Configure the Source.
            * @param aHeaderData   HeaderData to Configure the Source.
            */            
            TInt Open(TDesC8& aMimeType, TDesC8& aData );

            /**
            * From MDescriptorSource.
            * Returns the BitRate of the Source
            *
            * @since S60 v3.2
            * @param aRate   Returns the BitRate of the Source.             
            */            
            TInt GetBitRate( TUint& aRate );
            // From MDescriptorSource ends
            
            // From CSourceBase begins
            /**
            * From CSourceBase.
            * Callback from the StreamControl when the Source is Created
            *
            * @since S60 v3.2
            * @param aCustomCommand    Custom Command handle to send message to the Source             
            * @param aSourceHandle     Handle Identifying the Source             
            */            
            void ServerSourceCreated( MCustomCommand& aCustomCommand,
                                    TMMFMessageDestination& aSourceHandle );
            /**
            * From CSourceBase.
            * Callback from the StreamControl when the ServerSource is Deleted
            *
            * @since S60 v3.2
            */            
            void ServerSourceDeleted();
            /**
            * From CSourceBase.
            * Returns the Content Protection information to the Stream Control
            * Currently returns KErrNotSupported 
            * @since S60 v3.2
            */            
            TBool IsEncrypted();
            /**
            * From CSourceBase.
            * Returns the SourceUID
            *
            * @since S60 v3.2
            */            
            TUid GetSourceUid();
            /**
            * From CSourceBase.
            * Returns the Header Data passed when the Client calls Open
            *  Currently return KErrNotSupported
            * @since S60 v3.2
            */            
            TInt GetHeaderData(TPtr& aPtr);
            // From CSourceBase ends

            // From MAsyncAOObserver begins
            /**
            * From MAsyncAOObserver.
            * Callback from the Source for the Events from the Source Side
            * @since S60 v3.2
            * @param aError Error Code from the ServerSide Source.             
            */            
            void Event( TInt aError );
            // From MAsyncAOObserver ends

        private:
            void DoFillData( TUint aSize );
            void DoSeek( TUint aSeekPos );
            void DoBitRateChanged( TUint aBitRate );
            
        private:
            // Variable to Hold the Mime Type sent by the Client
            HBufC8* iMimeType;
            // Pointer to the Descriptor
            TPtr8 iDataPtr;
            // Current Position in the Descriptor
            TUint iCurrentPosition;
            // BitRate from the Source Side
            TUint iBitRate;
            // Variable to determine if the ServerSource exists
            TBool iServerSourceExists;
            // Handle to send messages to the Server Side Source
            MCustomCommand* iCustomCommand;
            // Handle Identifying the Server Side Source
            TMMFMessageDestination iSourceHandle;
            // Event Notifier that notifies the Client
            CEventNotifier* iEventNotifier;
            // Active Object to get Events from the Server Side Source
            CAsyncAO* iAsyncEventAO;
            // Event Package sent to the Server Side.
            TDescriptorDataSourceEventPckgBuf iEventPckg;
        };
    } // namespace multimedia

#endif // C_CCLIENTDESCRIPTORSOURCE_H

// End of file
