#include "sensors.h"

#include <QProcess>
#include <QString>
#include <QStringRef>

double Sensors::cpuTemperature() const
{
    QString program = "/usr/bin/vcgencmd";

    QProcess sensors;
    sensors.start(program, QStringList{"measure_temp"});
    if (sensors.waitForFinished()) {
        int exitCode = sensors.exitCode();
        if (exitCode != 0) {
            return -1;
        }
        QString output(sensors.readAllStandardOutput());
        if (output.startsWith("temp=")) {
            int position = output.indexOf("'");
            QStringRef tempStr(&output, 5, output.size() - (output.size() - position) - 5);
            return tempStr.toDouble();
        }

        return -1;
    }

    return -1;
}

double Sensors::roomTemperature() const
{
    QString program = "/usr/bin/vcgencmd";

    QProcess sensors;
    sensors.start(program, QStringList{"measure_temp"});
    if (sensors.waitForFinished()) {
        int exitCode = sensors.exitCode();
        if (exitCode != 0) {
            return -1;
        }
        QString output(sensors.readAllStandardOutput());
        if (output.startsWith("temp=")) {
            int position = output.indexOf("'");
            QStringRef tempStr(&output, 5, output.size() - (output.size() - position) - 5);
            return tempStr.toDouble();
        }

        return -1;
    }

    return -1;
}
