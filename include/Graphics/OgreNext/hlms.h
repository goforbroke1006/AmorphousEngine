//
// Created by goforbroke on 1/2/24.
//

#ifndef AMORPHOUS_ENGINE_HLMS_H
#define AMORPHOUS_ENGINE_HLMS_H

#include <OGRE/Ogre.h>
#include <OGRE/OgreConfigFile.h>
#include <OGRE/OgreArchiveManager.h>
#include <OGRE/OgreHlmsManager.h>

#include <OGRE/Hlms/Pbs/OgreHlmsPbs.h>
#include <OGRE/Hlms/Unlit/OgreHlmsUnlit.h>

template<class T>
T *CreateHlms(const Ogre::String &rootFolder) {
    using namespace Ogre;

    String mainFolderPath;
    StringVector libraryFoldersPaths;
    StringVector::const_iterator libraryFolderPathIt;
    StringVector::const_iterator libraryFolderPathEn;
    ArchiveManager &archiveManager = ArchiveManager::getSingleton();
    T::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
    ArchiveVec archiveLibraryFolders;
    Archive *archive = archiveManager.load(rootFolder + mainFolderPath,
                                           "FileSystem", true);
    libraryFolderPathIt = libraryFoldersPaths.begin();
    libraryFolderPathEn = libraryFoldersPaths.end();
    while (libraryFolderPathIt != libraryFolderPathEn) {
        Archive *archiveLibrary =
                archiveManager.load(rootFolder + *libraryFolderPathIt,
                                    "FileSystem", true);
        archiveLibraryFolders.push_back(archiveLibrary);
        ++libraryFolderPathIt;
    }
    T *hlms = OGRE_NEW T(archive, &archiveLibraryFolders);

    RenderSystem *renderSystem = Root::getSingletonPtr()->getRenderSystem();
    if (renderSystem->getName() == "Direct3D11 Rendering Subsystem") {
        bool supportsNoOverwriteOnTextureBuffers;
        renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
                                         &supportsNoOverwriteOnTextureBuffers);
        if (!supportsNoOverwriteOnTextureBuffers)
            hlms->setTextureBufferDefaultSize(512 * 1024);
    }
    return hlms;
}

void registerHlms(const std::string &filepath) {
    using namespace Ogre;

    ConfigFile cf;
    cf.load(filepath);
    String rootHlmsFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");
    if (rootHlmsFolder.empty())
        rootHlmsFolder = "./";
    else if (*(rootHlmsFolder.end() - 1) != '/')
        rootHlmsFolder += "/";

    auto mng = Root::getSingleton().getHlmsManager();
    mng->registerHlms(CreateHlms<HlmsUnlit>(rootHlmsFolder));
    mng->registerHlms(CreateHlms<HlmsPbs>(rootHlmsFolder));
}

void registerHlmsForEngine() {
    std::string path = "./Media/";

    auto mng = Ogre::Root::getSingleton().getHlmsManager();
    mng->registerHlms(CreateHlms<Ogre::HlmsUnlit>(path));
    mng->registerHlms(CreateHlms<Ogre::HlmsPbs>(path));
}

#endif //AMORPHOUS_ENGINE_HLMS_H
