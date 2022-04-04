void MyClass::getUserList()
{
    auto getUserListJob = new GetUserListJob(id);
    connect(getUserListJob, &GetUserListJob::succeeded, this, &MyClass::onGotUser);
    connect(getUserListJob, &GetUserListJob::failed, this, &MyCLass::onGotUserFailed);
    getUserListJob->start(); // start the API request asynchronously
}

void MyClass::onGotUserList(const QJsonDocument &json)
{
    // handle the result, e.g. create a new User object
}

void MyClass::onGotUserListFailed(int errorCode, const QString &errorString)
{
    // handle errors
}
