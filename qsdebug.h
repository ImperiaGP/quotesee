#ifndef QSDEBUG_H
#define QSDEBUG_H

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDir>

namespace qsDebug
{
    /**
     * message handler, will output debug, warning, critical and fatal to file
     *
     * @param message type
     * @param message
     */
    static void debugOutput(QtMsgType type, const char *msg)
    {
        QFile file;
        file.setFileName( "quotesee_debug.log" );
        QDir::setCurrent(QDir::homePath());

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
            return;

        QTextStream out(&file);

         switch (type)
         {
             case QtDebugMsg:
                 out << "Debug:\t";
                 out << msg << endl;
                 break;

             case QtWarningMsg:
                 out << "Warning:\t";
                 out << msg << endl;
                 break;

             case QtCriticalMsg:
                 out << "Critical:\t";
                 out << msg << endl;
                 break;

             case QtFatalMsg:
                 out << "Fatal:\t";
                 out << msg << endl;
                 abort();
         }
         file.close();
     }
};

#endif // QSDEBUG_H
