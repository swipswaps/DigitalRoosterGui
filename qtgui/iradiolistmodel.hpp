/*************************************************************************************
 * \filename
 * \brief	Internet Radiostation Listmodel
 *
 * \details needed to bind a list of episodes to QML
 *
 * \author Thomas Ruschival
 * \license {This file is licensed under GNU PUBLIC LICENSE Version 3 or later
 *
 * 			 SPDX-License-Identifier: GPL-3.0-or-later}
 *************************************************************************************/
#ifndef QTGUI_IRADIOLISTMODEL_HPP_
#define QTGUI_IRADIOLISTMODEL_HPP_

#include <QUrl>
#include <QAbstractListModel>
#include <QObject>
#include <memory>

namespace DigitalRooster {
class PlayableItem;
class MediaPlayerProxy;
class ConfigurationManager;

/**
 * Simple Model for displaying Internet Radio stations in QML List
 */
class IRadioListModel : public QAbstractListModel {
    Q_OBJECT
public:
    IRadioListModel(QObject* parent = nullptr);

    IRadioListModel(
        std::shared_ptr<DigitalRooster::ConfigurationManager> confman,
        std::shared_ptr<DigitalRooster::MediaPlayerProxy> pp,
        QObject* parent = nullptr);

    enum IRadioStationRoles { StationNameRole = Qt::UserRole + 1, UriRole };

    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QUrl get_station_url(int index);
    Q_INVOKABLE void send_to_player(int index);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    std::shared_ptr<DigitalRooster::ConfigurationManager> cm;
    std::shared_ptr<DigitalRooster::MediaPlayerProxy> mpp;
};
} // namespace DigitalRooster

#endif /* QTGUI_IRADIOLISTMODEL_HPP_*/
