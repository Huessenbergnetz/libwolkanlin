void MyClass::getUser(const QString &id)
{
    m_user = new User();
    m_user->get(id);
}
