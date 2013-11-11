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
* Description:  Definition of the ClientDataBufferSource class.
*
*/


#ifndef C_CCLIENTDATABUFFERSOURCE_H
#define C_CCLIENTDATABUFFERSOURCE_H

#include <e32base.h>
#include <DataBufferSource.h>
#include <DRMConfigTypes.h>
#include <mmfcontrollerframework.h>

#include "SourceBase.h"
#include "DataBufferQueueItem.h"
#include "BufTypeSupEventAO.h"
#include "ParentChildIntfc.h"

class MCustomCommand;

namespace multimedia
    {
    
    class CEventNotifier;
    class MDataBuffer;

    /**
    *  Implements the ClientSide Source for the DBS.
    *  Provides functionality to the Clients to query attributes
    *  from the ServerSide source
    *  @lib EnhancedMediaClient.lib
    *  @since S60 v3.2
    */    
    class CClientDataBufferSource : public CSourceBase,
                                        public MDataBufferSource,
                                        public MBufferObserver,
                                        public MBufferingTypesSupportedObserver,
                                        public CParentIntfc
        {
        public:
            /**
            * Function to Create the Object.
            * @param aObserver Observer to callback the Event.
            */
            CClientDataBufferSource();
            
            /**
            * Destructor.
            */
            ~CClientDataBufferSource();
            
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
            * Since this is DBS, the size represents the Total 
            * data size  which will be passed by the Client to
            * the Source
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
            
            // From MDataBufferSource begins
            /**
            * From MDataBufferSource.
            * Opens the Source with the mimetype and HeaderData
            *
            * @since S60 v3.2
            * @param aMimeType     MimeType used to Configure the Source.
            * @param aHeaderData   HeaderData to Configure the Source.
            */            
            TInt Open( TDesC8& aMimeType , MDataBuffer& aHeaderData );
            /**
            * From MDataBufferSource.
            * Returns the minimum buffersize the Source needs.
            *
            * @since S60 v3.2
            * @param aBufferSize   Returns the Minimum BufferSize the Source needs.             
            */            
            TInt GetMinimumBufferSize( TUint& aBufferSize );
            /**
            * From MDataBufferSource.
            * Sets the Size of the Data which the Client intends to send
            *
            * @since S60 v3.2
            * @param aBufferSize   Sets the Size of the Data which the Client 
                                intends to Send to the Source             
            */            
            TInt SetSize( TUint aSize );
            /**
            * From MDataBufferSource.
            * Writes the Buffer to the ServerSide Source
            *
            * @since S60 v3.2
            * @param aBuffer   Buffer that needs to be passed to the ServerSide Source                          
            */            
            TInt WriteData( MDataBuffer& aBuffer );
            /**
            * From MDataBufferSource.
            * Tries to Empty the Buffers on the ServerSide Source
            *
            * @since S60 v3.2
            */            
            TInt EmptyBuffers();
            /**
            * From MDataBufferSource.
            * Returns the Buffering Configuration of the ServerSide Source
            *
            * @since S60 v3.2
            * @param aConfig   Returns the Buffering Configuration of the Source.
            */            
            TInt GetBufferingConfig(TBufferingConfig& aConfig);
            /**
            * From MDataBufferSource.
            * Sets the Buffering Configuration of the ServerSide Source
            *
            * @since S60 v3.2
            * @param aConfig   Sets the Buffering Configuration of the Source.
            */            
            TInt SetBufferingConfig(TBufferingConfig aConfig);
            /**
            * From MDataBufferSource.
            * Returns the BitRate of the Source
            * the Source is initialized.
            *
            * @since S60 v3.2
            * @param aRate   Returns the BitRate of the Source.             
            */            
            TInt GetBitRate(TUint& aRate);
            /**
            * From MDataBufferSource.
            * Returns the Mime Type of the Data for which
            * the Source is initialized.
            *
            * @since S60 v3.2
            * @param aArray   Returns an Array of Buffering Types Supported
                            by the Source
            */            
            TInt GetBufferingTypesSupported(TArray<TBufferingConfig::TBufferingType>& aArray);
            /**
            * From MDataBufferSource.
            * Returns the Interface for querying Output Restrictions
            *
            * @since S60 v3.2
            * @param aInterfaceId   Interfacae ID of the interface requested.             
            * @param aVer   Version of the Interface Requested.             
            * @param aInterfaceImpl   Implementation returned.             
            */            
            TInt GetInterface(
                    TUid aInterfaceId,
                    TVersion& aVer,
                    TAny*& aInterfaceImpl );
            // From MDataBufferSource ends
            
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
            
            // From MBufferObserver begins
            /**
            * From MBufferObserver.
            * Callback from the Source when the Buffer sent to the ServerSide
            * is processed
            * @since S60 v3.2
            * @param aItem item sent to the Server.             
            */            
            void BufferProcessed( CDataBufferQueueItem* aItem );
            void HandleCancel( CDataBufferQueueItem& aItem );
            // From MBufferObserver ends
            
            // From MBufferingTypesSupportedObserver begins
            /**
            * From MBufferingTypesSupportedObserver.
            * Callback from the Source when the Buffering Types Support changes
            * @since S60 v3.2
            * @param aItem item sent to the Server.             
            */            
            void BufferingTypesSupportedChanged();
            
            
            // DRM information
            /**
            * Sets the DRM Type of Data which will be sent
            * @since S60 v3.2
            * @param aDRMType DRM Type data sent.             
            */            
            TInt SetDRMType( TDRMType aDRMType );
            /**
            * Gets the DRM Type set by the Client before
            * @since S60 v3.2
            * @param aDRMType DRM Type set by the Client before.             
            */            
            TInt GetDRMType( TDRMType& aDRMType );
            /**
            * Appends the Output Device to the List
            * @since S60 v3.2
            * @param aOutputDevice Output Device to be added to the List             
            */            
            TInt AppendAllowedOutputDevice( TDRMAllowedOutputDevice aOutputDevice );
            /**
            * Removes the Output Device from the List
            * @since S60 v3.2
            * @param aOutputDevice Device to be removed from the List.             
            */            
            TInt RemoveAllowedOutputDevice( TDRMAllowedOutputDevice aOutputDevice );
            /**
            * Gets the Allowed Output Device Count
            * @since S60 v3.2
            * @param aCount OutputDevice Count             
            */            
            TInt GetAllowedOutputDeviceCount( TInt& aCount );
            /**
            * Gets the Output Device in the List from the Index
            * @since S60 v3.2
            * @param aIndex Index in the List             
            * @param aOutputDevice Returned OutputDevice             
            */            
            TInt GetAllowedOutputDevice( TInt aIndex, TDRMAllowedOutputDevice& aOutputDevice );
            /**
            * Reset the DRM Config and empty the List
            * @since S60 v3.2
            */            
            TInt ResetDRMConfig();
            /**
            * Commit the Allowed Output Device List to the Server Side Source
            * @since S60 v3.2
            */            
            TInt CommitDRMConfig();
            
        private:
            void EmptyQueue();
            void DoCommitDRMConfigL();
            
        private:
            // Variable to determine if the ServerSource exists
            TBool iServerSourceExists;
            // Variable to Hold the Mime Type sent by the Client
            HBufC8* iMimeType;
            // Queue of data buffers from client
            TSglQue<CDataBufferQueueItem>* iAudioBufferQueue;
            // Variable to Stote the Source Size set by the Client
            TUint iSourceSize;
            // Handle to send messages to the Server Side Source
            MCustomCommand* iCustomCommand;
            // Handle Identifying the Server Side Source
            TMMFMessageDestination iSourceHandle;
            // Package of the Source Handle
            TMMFMessageDestinationPckg iSourceHandlePckg;
            
            // Event Notifier that notifies the Client
            CEventNotifier* iEventNotifier;
            // Active Object that monitors the Support changed.
            CBufTypeSupEventAO* iBufTypeSupportEvent;
            // if Fixed Duration Support is valid
            TBool iFixedDurationSupport;
            /* Attributes to hold DRM Info */
            // Holds DRM Type set by client
            TDRMType iDRMType;
            // Holds list of allowed output devices set by client
            RArray<TDRMAllowedOutputDevice> iAllowedOutputDeviceList;
            // Holds the next buffer sequence number starting from 0
            TUint iBufferSeqNum;
        };
    } // namespace multimedia

#endif // C_CCLIENTDATABUFFERSOURCE_H

// End of file
