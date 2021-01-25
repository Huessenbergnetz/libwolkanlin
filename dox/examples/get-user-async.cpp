void MyClass::getUser(const QString &id)
{
    auto getUserJob = new GetUserJob(id);
    connect(getUserJob, &GetUserJob::succeeded, this, &MyClass::onGotUser);
    getUserJob->start(); // start the API request asynchronously
}

void MyClass::onGotUser(const QJsonDocument &json)
{
    // handle the result, e.g. create a new User object
    auto user = new User::fromJson(json);
    if (user) {
        // do something with the User object
    }
}

void MyClass::onGotUserFailed(int errorCode, const QString &errorString)
{
    // handle errors
}
