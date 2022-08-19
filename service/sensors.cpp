#include "sensors.h"

#include <QProcess>
#include <QString>
#include <QStringRef>
#include <QDebug>

#include <pigpiod_if2.h>

Sensors::Sensors()
{
    m_pi = pigpio_start(nullptr, nullptr);
    if (m_pi < 0) {
        qCritical() << "Can't start pigpio";
        return;
    }

    m_dht.reset(DHTXXD(m_pi, 17, 0, nullptr));
}

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
    DHTXXD_manual_read(m_dht.get());
    DHTXXD_data_t data = DHTXXD_data(m_dht.get());
    if (data.status == DHT_GOOD) {
        return data.temperature;
    }
    else {
        qCritical() << "dht22 status:" << data.status;
    }

    return -1;
}

double Sensors::roomHumidity() const
{
    DHTXXD_manual_read(m_dht.get());
    DHTXXD_data_t data = DHTXXD_data(m_dht.get());
    if (data.status == DHT_GOOD) {
        return data.humidity;
    }
    else {
        qCritical() << "dht22 status:" << data.status;
    }

    return -1;
}
