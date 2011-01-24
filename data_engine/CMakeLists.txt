set(quotesee_engine_SRCS quoteseeengine.cpp)

kde4_add_plugin(plasma_engine_quotesee ${quotesee_engine_SRCS})
target_link_libraries(plasma_engine_quotesee
                      ${KDE4_PLASMA_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTXMLPATTERNS_LIBRARY})

install(TARGETS plasma_engine_quotesee
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES plasma-dataengine-quotesee.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
