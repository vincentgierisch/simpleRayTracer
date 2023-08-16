#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>

class ProgressBar {
    private:
        float _progress;
        float _updateInterval;
        int _barWidth = 70;
        float _updateHelper = 0.f;

        void _draw();
    public:
        ProgressBar(): _progress(0.f), _updateInterval(0.01) {};
        ProgressBar(float updateInterval): _progress(0), _updateInterval(updateInterval) {};
        void setProgress(float progress);
        float getProgress();
        void init();
};
#endif