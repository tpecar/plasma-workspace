add_definitions(-DTRANSLATION_DOMAIN=\"plasma_engine_soliddevice\")

set(soliddevice_engine_SRCS
    soliddeviceengine.cpp
    devicesignalmapper.cpp
    devicesignalmapmanager.cpp
    hddtemp.cpp
    soliddeviceservice.cpp
    soliddevicejob.cpp
)

kcoreaddons_add_plugin(plasma_engine_soliddevice SOURCES ${soliddevice_engine_SRCS} INSTALL_NAMESPACE plasma/dataengine)

target_link_libraries(plasma_engine_soliddevice
  Qt::Network
  KF5::I18n
  KF5::KIOCore
  KF5::Plasma
  KF5::Solid
  KF5::CoreAddons
  KF5::Notifications
)

kcoreaddons_desktop_to_json(plasma_engine_soliddevice plasma-dataengine-soliddevice.desktop)

install(FILES plasma-dataengine-soliddevice.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR} )
install(FILES soliddevice.operations DESTINATION ${PLASMA_DATA_INSTALL_DIR}/services )
