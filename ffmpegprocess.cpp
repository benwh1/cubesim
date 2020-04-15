#include "ffmpegprocess.h"

FFmpegProcess::FFmpegProcess(QObject *parent) :
    QProcess(parent)
{
    setProgram("ffmpeg");

    connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
}

void FFmpegProcess::writeFrame(QImage image, int numFrames){
    //use QBuffer to convert the image to PNG data
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    //send the data to ffmpeg
    for(int i=0; i<numFrames; i++){
        write(data);
    }
}

void FFmpegProcess::onFinished(int r, QProcess::ExitStatus s){
    qDebug() << "finished with return code" << r << "and exit status" << s;
}
