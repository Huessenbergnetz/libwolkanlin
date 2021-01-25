void MyClass::getUser(const QString &id)
{
    auto getUserJob = new GetUserJob(id);
    if (getUserJob->exec()) {
        // handle the result, e.g. create a new User object
        auto user = User::fromJson(getUserJob->result());
        if (!user.isEmpty()) {
            // do something with the User object
        }
    } else {
        // handle errors
        qCritical() << "Failed to get user:" << getUserJob->errorString();
    }
    getUserJob->deleteLater();
}
