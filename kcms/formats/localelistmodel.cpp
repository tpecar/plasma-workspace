#include "localelistmodel.h"
#include "kcmformats.h"
#include <QTextCodec>
#include <KLocalizedString>
LocaleListModel::LocaleListModel(KCMFormats *parent)
    : m_parent(parent)
{
}
int LocaleListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_locales.size();
}
QString LocaleListModel::getFlag(int index) const
{
    if (index < 0 || index >= (int)m_locales.size())
        return {};

    auto locale = m_locales.at(index);
    QString flagcode;
    const QStringList split = locale.name().split(QLatin1Char('_'));
    if (split.count() > 1) {
        flagcode = split[1].toLower();
    }
    return m_parent->loadFlagIcon(flagcode);
}
QString LocaleListModel::getText(int index) const
{
    if (index < 0 || index >= (int)m_locales.size())
        return {};

    auto locale = m_locales.at(index);
    const QString clabel = !locale.nativeCountryName().isEmpty() ? locale.nativeCountryName() : QLocale::countryToString(locale.country());
    const QString nativeLangName = locale.nativeLanguageName();
    if (!nativeLangName.isEmpty()) {
        return QString(i18n("%1 - %2 (%3)", clabel, nativeLangName, locale.name()));
    } else {
        return QString(i18n("%1 (%2)", clabel, locale.name()));
    }
}
QVariant LocaleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= (int)m_locales.size())
        return {};

    auto locale = m_locales.at(index.row());
    if (role == Qt::DecorationRole) {
        QString flagcode;
        const QStringList split = locale.name().split(QLatin1Char('_'));
        if (split.count() > 1) {
            flagcode = split[1].toLower();
        }
        return m_parent->loadFlagIcon(flagcode);
    }

    const QString clabel = !locale.nativeCountryName().isEmpty() ? locale.nativeCountryName() : QLocale::countryToString(locale.country());
        // This needs to use name() rather than bcp47name() or later on the export will generate a non-sense locale (e.g. "it" instead of
        // "it_IT")
    // TODO: Properly handle scripts (@foo)
//    QString cvalue = locale.name();
//    if (!cvalue.contains(QLatin1Char('.')) && cvalue != QLatin1Char('C')) {
//        // explicitly add the encoding,
//        // otherwise Qt doesn't accept dead keys and garbles the output as well
//        cvalue.append(QLatin1Char('.'));
//        //cvalue.append(QString::fromUtf8(QTextCodec::codecForLocale()->name()));
//    }

    const QString nativeLangName = locale.nativeLanguageName();
    if (!nativeLangName.isEmpty()) {
        return QString(i18n("%1 - %2 (%3)", clabel, nativeLangName, locale.name()));
    } else {
        return QString(i18n("%1 (%2)", clabel, locale.name()));
    }
}

void LocaleListModel::init(const QList<QLocale> &locales)
{
    beginInsertRows(QModelIndex(), 0, locales.size() - 1);
    m_locales = locales.toVector();
    endInsertRows();
}
int LocaleListModel::findIndexFromLocale(const QLocale &locale) const
{
    auto index = m_locales.indexOf(locale);

    // no result
    if (index == -1 && !m_locales.isEmpty())
        return 0;
    else
        return index;
}
QLocale LocaleListModel::localeAt(int index) const
{
    if (index > 0 && index < m_locales.size())
        return m_locales.at(index);
    else
        return {};
}
