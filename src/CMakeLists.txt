set(quotesee_SRCS quotesee.cpp
    quote.cpp
    qsconfig.cpp
    qsgeneralconfig.cpp
    plotter.cpp
    ../plotting/kplotaxis.cpp
    ../plotting/kplotpoint.cpp
    ../plotting/kplotobject.cpp
    ../plotting/kplotwidget.cpp)

kde4_add_ui_files(qsconfig_SRCS qsconfig.ui qsgeneralconfig.ui)
kde4_add_plugin(plasma_applet_quotesee ${quotesee_SRCS})

target_link_libraries(plasma_applet_quotesee
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTXMLPATTERNS_LIBRARY}
                      ${KDE4_KIO_LIBS})

install(TARGETS plasma_applet_quotesee
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-applet-quotesee.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES
        itemBackground.svgz
        extIcon.svgz
        DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/quotesee/)
