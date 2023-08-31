#include "../../include/utils/ProgressBar.h"

void ProgressBar::setProgress(float progress) {
    if (this->_progress < progress && progress <= 1) {
        float deltaProg = progress - this->_progress;
        this->_progress = progress;

        if ((deltaProg + this->_updateHelper) >= this->_updateInterval) {
            this->_updateHelper = std::abs(this->_updateInterval - (deltaProg+this->_updateHelper));
            this->_draw();
        } else {
            this->_updateHelper += deltaProg;
        }
    }
}

float ProgressBar::getProgress() {
    return this->_progress;
}

void ProgressBar::init() {
    this->_draw();
}

void ProgressBar::_draw() {
    int pos = this->_barWidth * this->_progress;
    for (int i = 0; i < this->_barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(this->_progress * 100.0) << " %\r";
	std::cout.flush();

    if(this->_progress >= 1) {
        std::cout << std::endl;
    }
}
