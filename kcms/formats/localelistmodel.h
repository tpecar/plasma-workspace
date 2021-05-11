#ifndef LOCALELISTMODEL_H
#define LOCALELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QLocale>
class KCMFormats;
class LocaleListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LocaleListModel(KCMFormats *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void init(const QList<QLocale> &locales);
    int findIndexFromLocale(const QLocale &locale) const;
    QLocale localeAt(int index) const;
private:
    QString loadFlagIcon(const QString &flagCode) const;

    KCMFormats *m_parent = nullptr;
    QVector<QLocale> m_locales;

};

#endif // LOCALELISTMODEL_H
