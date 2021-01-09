#include "./Animation.h"

Animation::Animation() {
    
}

Animation::Animation(unsigned int _index, unsigned int _numFrames, unsigned int _animationSpeed)
    : index(_index), numFrames(_numFrames), animationSpeed(_animationSpeed) {}