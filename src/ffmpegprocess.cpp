#include "ffmpegprocess.h"

FFmpegProcess::FFmpegProcess(QObject *parent) :
    QProcess(parent)
{
    setProgram("ffmpeg");
}

void FFmpegProcess::flush(){
    while(bytesToWrite() > 0){
        waitForBytesWritten();
    }
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
