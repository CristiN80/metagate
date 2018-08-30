#ifndef DBSTORAGE_H
#define DBSTORAGE_H

#include <QObject>
#include <QSqlDatabase>

class DBStorage : public QObject
{
    Q_OBJECT
public:
    using DbId = qint64;

    explicit DBStorage(const QString &dbpath, const QString &dbname, QObject *parent = nullptr);

    QString dbName() const;

    virtual void init(bool force);

    QString getSettings(const QString &key);
    void setSettings(const QString &key, const QString &value);

protected:
    void setPath(const QString &path);
    void openDB();

    void createTable(const QString &table, const QString &createQuery);
    void createIndex(const QString &createQuery);
    QSqlDatabase database() const;
    bool dbExist() const;

private:
    QSqlDatabase m_db;
    bool m_dbExist;
    QString m_dbPath;
    QString m_dbName;
};

#endif // DBSTORAGE_H
