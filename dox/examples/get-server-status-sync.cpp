void MyClass::getServerStatus()
{
    auto job = new GetServerStatusJob();
    if (job->exec()) {
        // handle the result, e.g. create a new ServerStatus object
        auto status = ServerStatus::fromJson(job->result());
        if (!status.isEmpty()) {
            // do something with the ServerStatus object
        }
    } else {
        // handle errors
        qCritical() << "Failed to get server status:" << job->errorString();
    }
    job->deleteLater();
}
