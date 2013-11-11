class XQAiwRequest
{
public:
    void send();	
};

class XQApplicationManager
{
public:
    XQAiwRequest* create(const QString& service, const QString& interface, const QString& operation, bool embedded = true);
};

