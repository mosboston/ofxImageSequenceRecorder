#ifndef _OFXIMAGESEQUENCERECORDER
#define _OFXIMAGESEQUENCERECORDER

/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"

typedef struct {
    string fileName;
    ofPixels image;
} QueuedImage;

class ofxImageSequenceRecorder : public ofThread {
public:

    int counter;
    queue<QueuedImage> q;
    string prefix;
    string format;

    ofxImageSequenceRecorder(){
        counter=0;

    }

    void setPrefix(string pre){
        prefix = pre;
        counter = 0; // keith
    }

    void setFormat(string fmt){
        format = fmt;
    }

    void threadedFunction() {
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
                if (i.image.getImageType() == OF_IMAGE_GRAYSCALE)
                    i.image.setImageType(OF_IMAGE_COLOR);
                ofSaveImage(i.image, i.fileName);
                q.pop();
            }
            sleep(1);
        }
    }

    void addFrame(ofImage &img){
        addFrame(img.getPixelsRef());
    }

    void addFrame(ofVideoGrabber &cam){
        addFrame(cam.getPixelsRef());
    }

    void addFrame(ofVideoPlayer &player){
        addFrame(player.getPixelsRef());
    }

    void addFrame(ofPixels imageToSave) {
        char fileName[100];
        sprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());
        counter++;

        QueuedImage qImage;

        qImage.fileName = fileName;
        qImage.image = imageToSave;

        q.push(qImage);
    }
};

#endif
