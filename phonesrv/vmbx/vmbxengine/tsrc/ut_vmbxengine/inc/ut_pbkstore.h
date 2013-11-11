#ifndef UT_PBKSTORE_H
#define UT_PBKSTORE_H

#include <QObject>

class CVmbxPbkStore;

class Ut_PbkStore : public QObject
{
    Q_OBJECT
    
public:
    Ut_PbkStore();
    virtual ~Ut_PbkStore();
    
private slots:
    void testCreateAndDelete();
    void testIsWritable();
    void testPhoneBookType();
    // Can not handle AO in Current UT desing
    void testWrite(); 
    void testPhonebookStore();
    void testGetL();
    
private:
    void createPbk();
    void deletePbk();
    
private:
    CVmbxPbkStore *mPbk;
};

#endif // UT_PBKSTORE_H
