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
* Description:  Concrete class representing start-up trigger entries.
*
*/


#ifndef LBTSTARTUPTRIGGER_H
#define LBTSTARTUPTRIGGER_H

#include <lbttriggerentry.h>
#include <lbs.h>

/**
 *  Concrete class representing start-up trigger entries.
 *
 *  When a start-up trigger is fired, Location Triggering Server tries to 
 *  launch a new instance of the trigger handling process. Trigger handling
 *  process is specified by the client application when the trigger is 
 *  created. The command line arguments, if specified, are passed to the 
 *  thread function of the new process's main thread. If the specified 
 *  process can't be launched when the trigger is fired, Location Triggering 
 *  Server will try to launch the process next time when the trigger is 
 *  fired again. 
 *
 *  Start-up trigger has following additional attributes
 *
 *  -  <B>Trigger handling process identity</B> Trigger handling process 
 *  identity consists of the executable name of the trigger handling process.
 *  The process is launched when the start-up trigger is fired. This attribute 
 *  can't be modified after the start-up trigger is created.
 *
 *  -  <B> SID of the trigger handling process</B> When SID of the trigger 
 *  handling process is specified when the trigger is created, the trigger 
 *  handling process is able to access the trigger. If the SID is not specified,
 *  the trigger handling process can't access the trigger. This attribute 
 *  cannot be modified after the start-up trigger is created.
 *
 *  -  <B>Command-line argument string</B> Command-line argument string is passed
 *  as an argument to the main thread of the launched process, when it's 
 *  first scheduled. Command-line argument string is passed to the launched 
 *  process as it is specified. Location Triggering Server doesn't interpret any
 *  special character. This attribute can be modified after the trigger is
 *  created.
 *
 *  Trigger handling process identity must be specified when the trigger 
 *  is created in Location Triggering Server. SID of the trigger handling 
 *  process and Command-line argument string is optional.
 *
 *  If the trigger handling process is not the owner process of the trigger and
 *  the SID of the trigger handling process is not specified when the trigger 
 *  is created, it can't access the firing trigger
 *  information. In this case, if the trigger handling process needs to 
 *  get information of the firing trigger, it can get it either from 
 *  command-line argument string which is set when the trigger is created 
 *  or request a service from the owner process.
 *
 *  If a UIKON based UI application is specified as trigger handling process,
 *  the UIKON framework will prevent starting a new instance of the application if
 *  it's already running. In this case, the trigger handling process (UI application)
 *  shall issue trigger firing event notification request to Location Triggering 
 *  Server when it's started.
 *
 *  If the owner process, Manager UI or trigger handling process 
 *  of a trigger is removed from the system, the trigger becomes 
 *  invalid. The process can be removed for example when the process's 
 *  executable resides in removable media (like MMC), and that media is 
 *  removed from the terminal. An invalid trigger is not supervised by 
 *  the system. If a trigger has been invalid longer than the predefined system 
 *  clean up time, the trigger is removed from the system. System clean up time 
 *  is defined by system and cant' be accessed by client application.
 * 
 *  @see RLbt
 *
 *  @lib lbt.lib
 *  @since S60 5.1
 */
class CLbtStartupTrigger : public CLbtTriggerEntry
    {
public:
    /**
     * Allocates and constructs a new start-up trigger entry.
     * Default values are assigned to the attributes. 
     * The default values are
     * 
     *  - Trigger Id is KLbtNullTriggerId.
     *
     *  - Trigger Name is an empty string.
     *
     *  - Trigger State CLbtTriggerEntry::EStateEnabled.
     *  
     *  - Requestors are not set. 
     *
     *  - Manager UI is not set(KNullUid). 
     *
     *  - Trigger condition is not set.
     *
     *  - Trigger handling process file name is empty string.
     * 
     *  - SID of the trigger handling process is KNullUid.
     *
     *  - Command-line argument string is an empty string
     *
     * @return Pointer to the new start-up trigger entry.
     */
    IMPORT_C static CLbtStartupTrigger* NewL();

    /**
     * Allocates and constructs a new start-up trigger entry. The
     * constructed object is pushed onto cleanup stack.
     * Default values are assigned to the attributes. 
     * The default values are
     * 
     *  - Trigger Id is KLbtNullTriggerId.
     *
     *  - Trigger Name is an empty string.
     *
     *  - Trigger State CLbtTriggerEntry::EStateEnabled.
     *  
     *  - Requestors are not set. 
     *
     *  - Manager UI is not set(KNullUid). 
     *
     *  - Trigger condition is not set.
     *
     *  - Trigger handling process file name is empty string.
     * 
     *  - SID of the trigger handling process is KNullUid.
     *
     *  - Command-line argument string is an empty string
     *
     * @return Pointer to the new start-up trigger entry.
     */
    IMPORT_C static CLbtStartupTrigger* NewLC();

    /**
     * Allocates and constructs a new start-up trigger entry.
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestorType Identifies the type of requestor, a
     * service or a contact.
     * @param[in] aRequestorFormat Determines the type of data held 
     * in aRequestorData
     * @param[in] aRequestorData Requestor data. Can be a telephone 
     * number, a URL etc.
     * @param[in] aManagerUi The UID of manager UI. 
     * @param[in] aCondition Pointer of the new trigger condition object.
     * This object takes the ownership of aCondition.
     * @param[in] aFileName A descriptor containing the full path name of
     * the executable to be loaded when the start-up trigger is fired. If 
     * this name has no file extension, an extension of .exe is appended. 
     * The length of the resulting full path name must not be greater 
     * than KMaxFileName. The length of the file name itself must not be 
     * greater than KMaxOsName. If no path is specified, the system will 
     * look in system executable directories on all local drives, in the  
     * same search order as specified in TFindFile::FindByPath().
     * @param[in] aSecureId The SID of the trigger handling process.
     * If the specified value is KNullUid or a wrong value is specified,
     * the trigger handling process would not be able to access the 
     * trigger.
     * @param[in] aCommandLine Command-line argument string.
     * @return Pointer to the new start-up trigger entry.
     * @leave KErrArgument If the name of the trigger is longer than
     *   @p KLbtMaxNameLength.
     * @leave Other standard symbian error code, such as KErrNoMemory,
     * KErrGeneral, etc.
     * @panic ELbtErrArgument The length of aFileName is longer than
     * @p KMaxFileName.
     */
    IMPORT_C static CLbtStartupTrigger* NewL( 
        const TDesC& aName,
        TLbtTriggerState aState,
        CRequestor::TRequestorType aRequestorType,
        CRequestor::TRequestorFormat aRequestorFormat,
        const TDesC& aRequestorData,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition,
        const TDesC& aFileName, 
        const TSecureId& aSecureId = KNullUid,
        const TDesC& aCommandLine = KNullDesC() );
    
    /**
     * Allocates and construct a new start-up trigger entry.
     *
     * @panic ELbtErrArgument The length of aFileName is greater than
     * @p KMaxFileName.
     *
     * @param[in] aName The name of the trigger entry.
     * @param[in] aState The state of trigger entry.
     * @param[in] aRequestors The requestor for the service. This object
     * does not take the ownership of aRequestor.
     * @param[in] aManagerUi The UID of manager UI.
     * @param[in] aCondition Pointer of the new trigger condition object.
     * This object takes the ownership of aCondition.
     * @param[in] aFileName A descriptor containing the full path name of
     * the executable to be loaded when the start-up trigger is fired. If 
     * this name has no file extension, an extension of .exe is appended. 
     * The length of the resulting full path name must not be greater 
     * than KMaxFileName. The length of the file name itself must not be 
     * greater than KMaxOsName. If no path is specified, the system will 
     * look in system executable directories on all local drives, in the  
     * same search order as specified in TFindFile::FindByPath().
     * @param[in] aSecureId The SID of the trigger handling process.
     * If the specified value is KNullUid or a wrong value is specified,
     * the trigger handling process would not be able to access the 
     * trigger.
     * @param[in] aCommandLine Command-line argument string.
     * @return Pointer to the new start-up trigger entry.
     * @leave KErrArgument If the name of the trigger is longer than
     * @p KLbtMaxNameLength.
     * @leave Other standard symbian error code, such as KErrNoMemory,
     * KErrGeneral, etc.
     */
    IMPORT_C static CLbtStartupTrigger* NewL( 
        const TDesC& aName,
        TLbtTriggerState aState,
        const RRequestorStack& aRequestors,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition,
        const TDesC& aFileName, 
        const TSecureId& aSecureId = KNullUid,
        const TDesC& aCommandLine = KNullDesC() );

    /**
     * Destructor
     */
    IMPORT_C ~CLbtStartupTrigger();

    /**
     * Gets the type of trigger entry, CLbtTriggerEntry::EStartup.
     *
     * @return CLbtTriggerEntry::EStartup.
     */
    IMPORT_C TType Type() const;
    
    /**
     * Gets identity of the trigger handling process.
     * 
     * @param[out] aFileName On return contains the full path name
     * of the executable of trigger handling process. Maximum length 
     * of the resulting full path name is KMaxFileName. Client gets 
     * panic USER 11 if the length of executable full path name is 
     * greater than the maximum length of aFileName. Empty string 
     * is returned if the attribute has not been set.
     * @param[out] aSecureId On return contains the SID of
     * the triggering handling process. KNullUid is returned if
     * the SID has not been previously set. 
     */
    IMPORT_C void GetProcessId( 
        TDes& aFileName, 
        TSecureId& aSecureId ) const;
    
    /**
     * Sets the identity of the trigger handling process.
     *
     * @param[in] aFileName A descriptor containing the full path name of
     * the executable to be loaded when the start-up trigger is fired. If 
     * this name has no file extension, an extension of .exe is appended. 
     * The length of the resulting full path name must not be greater 
     * than @p KMaxFileName. The length of the file name itself must not be 
     * greater than KMaxOsName. If no path is specified, the system will 
     * look in system executable directories on all local drives, in the  
     * same search order as specified in TFindFile::FindByPath().
     * @param[in] aSecureId The SID of the trigger handling process.
     * If the specified value is KNullUid or a wrong value is specified,
     * the trigger handling process would not be able to access the 
     * trigger.
     * @panic ELbtErrArgument The length of aFileName is greater than
     * @p KMaxFileName.
     */
    IMPORT_C void SetProcessId( 
        const TDesC& aFileName, 
        const TSecureId& aSecureId );
    
    /**
     * Gets the command-line argument string.
     *
     * @return The command-line argument string. Empty string is returned
     * if the command-line argument string has not been set
     */
    IMPORT_C const TDesC& CommandLine() const;
    
    /**
     * Sets the command-line argument string.
     * 
     * @param[in] aCommandLine The command-line argument string. It can
     * be an empty string.
     */
    IMPORT_C void SetCommandLineL( const TDesC& aCommandLine );
    
    
protected:
    
    /**
     * Internalize method that subclass must implement.
     * @param[in] aStream Stream from which the object should be internalized.
     */
    virtual void DoInternalizeL(RReadStream& aStream);
    
    /**
     * Externalize method that subclass must implement.
     * @param[in] aStream Stream to which the object should be externalized.
     */
    virtual void DoExternalizeL(RWriteStream& aStream) const;  

         

private:
    /**
     * Default constructor
     */
    CLbtStartupTrigger(); 
        
    /**
     * By default, prohibit copy constructor
     */
    CLbtStartupTrigger( const CLbtStartupTrigger& );
    
    /**
     * Prohibit assigment operator
     */
    CLbtStartupTrigger& operator= ( const CLbtStartupTrigger& );
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();
    
    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL(const TDesC& aName,
        TLbtTriggerState aState,
        CRequestor::TRequestorType aRequestorType,
        CRequestor::TRequestorFormat aRequestorFormat,
        const TDesC& aRequestorData,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition,
        const TDesC& aFileName, 
        const TSecureId& aSecureId ,
        const TDesC& aCommandLine);
    
    /**
     * Symbian 2nd phase constructor
     */    
    void CLbtStartupTrigger::ConstructL( const TDesC& aName,
        CLbtTriggerEntry::TLbtTriggerState aState,
        const RRequestorStack& aRequestors,
        TUid aManagerUi,
        CLbtTriggerConditionBase* aCondition,
        const TDesC& aFileName, 
        const TSecureId& aSecureId ,
        const TDesC& aCommandLine);
        
private:
    /**
     * Process file name 
     */
    TFileName iProcessFileName;
    
    /**
     * Secure Id
     */
    TSecureId iSecureId;
    
    /**
     * Command line arguement
     */
    HBufC* iCommandLine;
    };


#endif // LBTSTARTUPTRIGGER_H
