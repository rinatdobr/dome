#include "sensors.h"

#include <QProcess>
#include <QString>

double Sensors::cpuTemperature() const
{
    QString program = "/usr/bin/sensors";

    QProcess sensors;
    sensors.start(program, QStringList{});
    if (sensors.waitForFinished()) {
        int exitCode = sensors.exitCode();
        if (exitCode != 0) {
            return -1;
        }
        QByteArray output = sensors.readAllStandardOutput();
        return output.size();
    }

    return -1;
}
