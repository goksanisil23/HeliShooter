#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
    public:
        unsigned int index;
        unsigned int numFrames;
        unsigned int animationSpeed;
        Animation();
        Animation(unsigned int _index, unsigned int _numFrames, unsigned int _animationSpeed);
};

#endif