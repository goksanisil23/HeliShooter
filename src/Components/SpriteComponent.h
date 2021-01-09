#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include "./TransformComponent.h"
#include "../Animation.h"

class SpriteComponent: public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRectangle;
        SDL_Rect dstRectangle;
        bool isAnimated;
        int numFrames;
        int animationSpeed;
        bool isFixed;
        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
        unsigned int animationIndex = 0;
    
    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent(const char* filePath) {
            isAnimated = false;
            isFixed = false;
            SetTexture(filePath);
        }

        SpriteComponent(std::string id, int _numFrames, int _animationSpeed, bool hasDirections, bool _isFixed) {
            isAnimated = true;
            numFrames = _numFrames;
            animationSpeed = _animationSpeed;
            isFixed = _isFixed;

            if(hasDirections) {
                Animation downAnimation = Animation(0, numFrames, animationSpeed);
                Animation rightAnimation = Animation(1, numFrames, animationSpeed);
                Animation leftAnimation = Animation(2, numFrames, animationSpeed);
                Animation upAnimation = Animation(3, numFrames, animationSpeed);

                animations.emplace("DownAnimation", downAnimation);
                animations.emplace("RightAnimation", rightAnimation);
                animations.emplace("LeftAnimation", leftAnimation);
                animations.emplace("UpAnimation", upAnimation);

                animationIndex = 0;
                currentAnimationName = "DownAnimation"; // always start from down at initialization
            }
            else {
                Animation singleAnimation = Animation(0, numFrames, animationSpeed);
                animations.emplace("SingleAnimation", singleAnimation);
                animationIndex = 0;
                currentAnimationName = "SingleAnimation";
            }

            Play(currentAnimationName);

            SetTexture(id);
        }

        void Play(std::string animationName) {
            numFrames = animations[animationName].numFrames;
            animationIndex = animations[animationName].index;
            animationSpeed = animations[animationName].animationSpeed;
            currentAnimationName = animationName;
        }

        void SetTexture(std::string assetTextureId) {
            texture = Game::assetManager->GetTexture(assetTextureId);
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();

            srcRectangle.x = 0;
            srcRectangle.y = 0;
            // this is actually the size of the entire texture, so we load the entire texture to the srcRectangle at the beginning
            // and always copy from this source to that target pixel destination
            srcRectangle.w = transform->width; 
            srcRectangle.h = transform->height;
        }

        void Update(const float& deltaTime) override {

            if(isAnimated) {
                // sprite sheet for chopper has 64 pixels in width, and 2 instances of a chopper animation
                // in the below logic, we shift the starting position of the 32x32 box left and right along the sprite sheet (srcRectangle)
                // to give an animation look. (numFrames = 2 since 2 instances of animation is there in the spritesheet --> column size)
                srcRectangle.x = srcRectangle.w * static_cast<int>((SDL_GetTicks()/animationSpeed) % numFrames);
            }
            // the logic below chooses the rows in the sprite sheet. rows have different directions (left, right, up, down)
            srcRectangle.y = animationIndex * transform->height;

            // we move the texture, which was loaded to srcRectangle, based on how the transform component moves
            dstRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
            dstRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
            dstRectangle.w = transform->width * transform->scale;
            dstRectangle.h = transform->height * transform->scale;
        }

        void Render() override {
            // moves the initially loaded texture (at srcRectangle), to the next position based on the updated info in dstRectangle
            TextureManager::Draw(texture, srcRectangle, dstRectangle, spriteFlip);
        }

};

#endif