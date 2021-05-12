
set(QT_MIN_VERSION "5.14.0")
set(KF5_MIN_VERSION "5.73.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Quick
    Svg
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    I18n
    KCMUtils
    Declarative
    Config
)


# KI18N Translation Domain for this library
#add_definitions(-DTRANSLATION_DOMAIN=\"kcmformats\")

set(kcm_formats_PART_SRCS
    kcmformats.cpp
    localelistmodel.cpp
)

add_library(kcm_formats MODULE ${kcm_formats_PART_SRCS})

kcoreaddons_desktop_to_json(kcm_formats "package/metadata.desktop")

target_link_libraries(kcm_formats
    Qt5::Core
    KF5::ConfigCore
    KF5::CoreAddons
    KF5::I18n
    KF5::QuickAddons)


########### install files ###############
install(TARGETS kcm_formats  DESTINATION ${KDE_INSTALL_PLUGINDIR}/kcms )
install(FILES package/metadata.desktop RENAME kcm_formats.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
kpackage_install_package(package kcm_formats kcms)
