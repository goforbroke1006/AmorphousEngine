//
// Created by goforbroke on 1/5/24.
//

#ifndef AMORPHOUS_ENGINE_IRRLICHT_H
#define AMORPHOUS_ENGINE_IRRLICHT_H

#include "../GraphicsEngine.h"

#include <irrlicht.h>

#define TO_COLOR 255.0

namespace AmE {
    class Irrlicht : public GraphicsEngine {
    public:
        Irrlicht();

        ~Irrlicht() override;

        [[nodiscard]] size_t getWindowHnd() const override;

        void initialize(const std::map<std::string, GameObject *> &gameObjects) override;

        bool update(const std::map<std::string, GameObject *> &gameObjects) override;

        void stop() override;

    private:
        bool mQuit = false;

        irr::IrrlichtDevice *mDevice;
        irr::video::IVideoDriver *mVideoDriver;
        irr::scene::ISceneManager *mSceneManager;
        irr::gui::IGUIEnvironment *mGuiEnv;

        Color mBackgroundColor{};
    };
}

#endif //AMORPHOUS_ENGINE_IRRLICHT_H
