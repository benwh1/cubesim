#ifndef FFMPEGPROCESS_H
#define FFMPEGPROCESS_H

#include <QBuffer>
#include <QDebug>
#include <QImage>
#include <QProcess>

class FFmpegProcess : public QProcess
{
    Q_OBJECT

public:
    explicit FFmpegProcess(QObject *parent = nullptr);

    void flush();
    void writeFrame(QImage image, int numFrames);

};

#endif // FFMPEGPROCESS_H
