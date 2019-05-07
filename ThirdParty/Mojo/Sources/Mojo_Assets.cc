#include <Mojo/Array.h>
#include <Mojo/Assets.h>
#include <Mojo/FileSystem.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

inline namespace Mojo
{
    static Array<const char*> _searchPaths;

    namespace Assets
    {
        void  AddSearchPath(const char* path)
        {
            _searchPaths.Push(path);
        }

        void  RemoveSearchPath(const char* path)
        {
            for (int i = 0, n = _searchPaths.count; i < n; i++)
            {
                if (_stricmp(_searchPaths[i], path) == 0)
                {
                    _searchPaths.Erase(i);
                    break;
                }
            }
        }

        void* LoadWave(const char* path, int* size, int* freq, int* format)
        {
#pragma pack(push, 1)
            struct WAVHeader
            {
                uint8_t  riff[4];
                uint32_t size;

                uint8_t  wave[4];   // "WAVE"
                uint8_t  fmt[4];    // "fmt\0"
                uint32_t fmtLength;

                uint16_t audioFormat;
                uint16_t channels;

                uint32_t sampleRate;
                uint32_t byteRate;

                uint16_t blockAlign;
                uint16_t bitsPerSample;
            };
#pragma pack(pop, 1)

            File file;
            if (!file.Open(path, FileOpen::Read))
            {
                for (int i = 0, n = _searchPaths.count; i < n; i++)
                {
                    const char* searchPath = _searchPaths[i];

                    char targetPath[1024];
                    ::sprintf(targetPath, "%s/%s", searchPath, path);
                    if (file.Open(targetPath, FileOpen::Read))
                    {
                        break;
                    }
                }
            }

            if (file._handle)
            {
                int fileSize = file.Size();

                int nbytes;
                WAVHeader header = {};
                if ((nbytes = file.Read(&header, sizeof(header))) != sizeof(header))
                {
                    file.Close();
                    return NULL;
                }

                if (strncmp((char*)header.riff, "RIFF", 4) != 0)
                {
                    file.Close();
                    return NULL;
                }

                if (strncmp((char*)header.wave, "WAVE", 4) != 0)
                {
                    file.Close();
                    return NULL;
                }

                if (strncmp((char*)header.fmt, "fmt ", 4) != 0)
                {
                    file.Close();
                    return NULL;
                }

                if (header.audioFormat == 1)
                {
                    uint16_t extraParams = 0;
                    file.Read(&extraParams, sizeof(extraParams));
                    file.Seek(extraParams, FileSeek::Current);
                }

                char dataName[4];
                if (file.Read(&dataName, 4) != 4 || strncmp(dataName, "data", 4) != 0)
                {
                    file.Close();
                    return NULL;
                }
                
                uint32_t dataChunkSize;
                if (file.Read(&dataChunkSize, 4) != 4)
                {
                    file.Close();
                    return NULL;
                }

                int resultSize = dataChunkSize;
                void* result = ::malloc(resultSize);
                if ((nbytes = file.Read(result, resultSize)) != resultSize)
                {
                    ::free(result);
                    file.Close();
                    return NULL;
                }

                *size = resultSize;
                *freq = header.sampleRate;

                if (header.channels == 1)
                {
                    if (header.bitsPerSample == 8)
                    {
                        *format = 0;
                    }
                    else
                    {
                        *format = 1;
                    }
                }
                else
                {
                    if (header.bitsPerSample == 8)
                    {
                        *format = 2;
                    }
                    else
                    {
                        *format = 3;
                    }
                }

                file.Close();
                return result;
            }

            return NULL;
        }

#if 0
        void* LoadWave(const char* path, int* length, int* frequency, int* format)
        {
            Uint8* wave;
            Uint32 readLength;
            SDL_AudioSpec spec;
            if (SDL_LoadWAV_RW(path, &spec, &wave, &readLength))
            {
                switch (spec.format)
                {
                case AUDIO_U8:
                case AUDIO_S8:
                    if (format) *format = spec.channels == 2 ? 2 : 0;
                    break;
                
                case AUDIO_U16:
                case AUDIO_S16:
                    if (format) *format = spec.channels == 2 ? 3 : 1;
                    break;
                }

                if (frequency)
                {
                    *frequency = spec.freq;
                }

                if (length)
                {
                    *length = (int)readLength;
                }

                return wave;
            }
            else
            {
                return NULL;
            }
        }
#endif

        void  FreeWave(void* wave)
        {
            ::free(wave);
        }

        void* LoadImage(const char* path, int* width, int* height, int* channel)
        {
            if (FileSystem::Exists(path))
            {
                return ::stbi_load(path, width, height, channel, 0);
            }

            for (int i = 0, n = _searchPaths.count; i < n; i++)
            {
                const char* searchPath = _searchPaths[i];

                char targetPath[1024];
                ::sprintf(targetPath, "%s/%s", searchPath, path);
                if (FileSystem::Exists(targetPath))
                {
                    return ::stbi_load(targetPath, width, height, channel, 0);
                }
            }
            return NULL;
        }

        void  FreeImage(void* pixels)
        {
            ::stbi_image_free(pixels);
        }
    }
}