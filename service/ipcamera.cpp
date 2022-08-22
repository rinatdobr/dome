#include "ipcamera.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <string>
#include <chrono>

#include <spdlog/spdlog.h>

#include <QProcess>
#include <QString>
#include <QDir>

IpCamera::IpCamera(const std::string &name,
                   const std::string &login,
                   const std::string &pass,
                   const std::string &ip,
                   const std::string &port)
    : m_name(name)
    , m_login(login)
    , m_pass(pass)
    , m_ip(ip)
    , m_port(port)
{
    spdlog::trace("{}:{} {} name={} login={} pass={} ip={} port={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, login, pass, ip, port);
}

std::map<std::string, std::unique_ptr<IpCamera>> IpCamera::Create(const std::vector<dome::config::IpCamera::Raw> &ipCameraConfig)
{
    spdlog::trace("{}:{} {} ipCameraConfig.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, ipCameraConfig.size());

    std::map<std::string, std::unique_ptr<IpCamera>> result;

    for (const auto &config : ipCameraConfig) {
        result.insert(std::make_pair(config.m_name, std::make_unique<IpCamera>(config.m_name, config.m_login, config.m_pass, config.m_ip, config.m_port)));
    }

    spdlog::trace("Total {} IP Cameras", result.size());

    return result;
}

std::string IpCamera::photo()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::string tmpDir("/tmp/dome.ipcameraphoto.XXXXXX");
    char *tmpDirRaw = mkdtemp(&tmpDir[0]);
    chmod(tmpDirRaw, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH);
    spdlog::debug("tmpDir={}", tmpDir);
    auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char buffer[50];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", std::localtime(&time));
    std::string photoPath = tmpDir + "/" + buffer + ".png";
    spdlog::debug("photoPath={}", photoPath);

    QString program = "/usr/bin/ffmpeg";

    QProcess ffmpeg;
    QStringList args;
    args.append(QStringLiteral("-rtsp_transport"));
    args.append(QStringLiteral("tcp"));
    args.append(QStringLiteral("-i"));
    args.append(QDir::toNativeSeparators(QStringLiteral("rtsp://%1:%2@%3:%4/ISAPI/Streaming/Channels/101")
        .arg(QString::fromStdString(m_login))
        .arg(QString::fromStdString(m_pass))
        .arg(QString::fromStdString(m_ip))
        .arg(QString::fromStdString(m_port))));
    args.append(QStringLiteral("-f"));
    args.append(QStringLiteral("image2"));
    args.append(QStringLiteral("-vframes"));
    args.append(QStringLiteral("1"));
    args.append(QStringLiteral("-y"));
    args.append(QStringLiteral("-loglevel"));
    args.append(QStringLiteral("error"));
    args.append(QString::fromStdString(photoPath));
    spdlog::debug("args={}", args.join(' ').toStdString());

    ffmpeg.start(program, args);
    if (ffmpeg.waitForFinished()) {
        int exitCode = ffmpeg.exitCode();
        spdlog::debug("ffmpeg exit code={}", exitCode);
        if (exitCode != 0) {
            spdlog::error("ffmpeg error {}", QString(ffmpeg.readAllStandardError()).toStdString());
            return {};
        }

        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, photoPath);
        return photoPath;
    }
    spdlog::error("ffmpeg was not finished");

    return {};
}
