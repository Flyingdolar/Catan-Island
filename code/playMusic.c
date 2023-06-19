#include "catan.h"
#define MUSICPATH "./resources/bgMusic.wav"


int playMusic() {
    // 初始化音频
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Can not init audio, %s", SDL_GetError());
        return -1;
    }

    // 加载音乐文件
    Mix_Music* music = Mix_LoadMUS(MUSICPATH);
    if (music == NULL) {
        SDL_Log("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    // 播放音乐
    if (Mix_PlayMusic(music, -1) == -1) {
        SDL_Log("Failed to play music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    while (game->state != END) {
        SDL_Delay(100);
    }

    // 清理音频资源
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    return 0;
}