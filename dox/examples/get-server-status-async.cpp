void MyClass::getServerStatus()
{
    auto job = new GetServerStatusJob();
    connect(job, &GetServerStatusJob::succeeded, this, &MyClass::onGotStatus);
    connect(job, &GetServerStatusJob::failed, this, &MyClass::onGotStatusFailed);
    job->start();
}

void MyClass::onGotStatus(const QJsonDodcument &json)
{
    // handle the result
    auto status = ServerStatus::fromJson(json);
    if (!status.isEmpty()) {
        // do something with the ServerStatus object
    }
}

void MyClass::onGotStatusFailed(int errorCode, const QString &errorString)
{
    // handle errors
}
