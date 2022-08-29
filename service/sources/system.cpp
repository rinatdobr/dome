#include "system.h"

#include <QProcess>
#include <QString>
#include <QStringRef>

#include <spdlog/spdlog.h>

double System::cpuTemperature() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    QString program = "/usr/bin/vcgencmd";

    QProcess sensors;
    sensors.start(program, QStringList{"measure_temp"});
    if (sensors.waitForFinished()) {
        int exitCode = sensors.exitCode();
        spdlog::debug("vcgencmd exit code={}", exitCode);
        if (exitCode != 0) {
            spdlog::error("vcgencmd error {}", QString(sensors.readAllStandardError()).toStdString());
            return -1;
        }

        QString output(sensors.readAllStandardOutput());
        if (output.startsWith("temp=")) {
            int position = output.indexOf("'");
            QStringRef tempStr(&output, 5, output.size() - (output.size() - position) - 5);
            spdlog::debug("{}:{}", __PRETTY_FUNCTION__, tempStr.toDouble());
            return tempStr.toDouble();
        }

        spdlog::error("Can't parse temperature from '{}'", output.toStdString());
        return -1;
    }

    spdlog::error("vcgencmd was not finished");
    return -1;
}
