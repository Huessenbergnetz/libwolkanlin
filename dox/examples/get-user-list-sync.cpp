void MyClass::getUserList()
{
    auto getUserListJob = new GetUserListJob();
    if (getUserListJob->exec()) {
        // handle the result, e.g. load it into a model
    } else {
        // handle errors
        qCritical() << "Failed to get users:" << getUserListJob->errorString();
    }
    getUserListJob->deleteLater();
}
