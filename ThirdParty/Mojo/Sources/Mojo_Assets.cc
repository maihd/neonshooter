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
    namespace Assets
    {
        void AddSearchPath(const char* path)
        {
            FileSystem::AddSearchPath(path);
        }

        void RemoveSearchPath(const char* path)
        {
            FileSystem::RemoveSearchPath(path);
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

            File* file = FileSystem::Open(path, FileOpen::Read);
            if (file)
            {
                int fileSize = file->Size();

                int nbytes;
                WAVHeader header = {};
                if ((nbytes = file->Read(&header, sizeof(header))) != sizeof(header))
                {
                    file->Close();
                    return NULL;
                }

                if (strncmp((char*)header.riff, "RIFF", 4) != 0)
                {
                    file->Close();
                    return NULL;
                }

                if (strncmp((char*)header.wave, "WAVE", 4) != 0)
                {
                    file->Close();
                    return NULL;
                }

                if (strncmp((char*)header.fmt, "fmt ", 4) != 0)
                {
                    file->Close();
                    return NULL;
                }

                header.audioFormat = LE_TO_NATIVE_16(header.audioFormat);
                header.channels = LE_TO_NATIVE_16(header.channels);
                header.blockAlign = LE_TO_NATIVE_16(header.blockAlign);
                header.bitsPerSample = LE_TO_NATIVE_16(header.bitsPerSample);
                header.sampleRate = LE_TO_NATIVE_32(header.sampleRate);
                header.byteRate = LE_TO_NATIVE_32(header.byteRate);

                if (header.audioFormat == 1)
                {
                    uint16_t extraParams = file->ReadUint16LE();
                    file->Skip(extraParams);
                }

                char dataName[4];
                if (file->Read(&dataName, 4) != 4 || strncmp(dataName, "data", 4) != 0)
                {
                    file->Close();
                    return NULL;
                }
                
                uint32_t dataChunkSize = file->ReadUint32LE();
                if (!dataChunkSize)
                {
                    file->Close();
                    return NULL;
                }

                int resultSize = dataChunkSize;
                void* result = ::malloc(resultSize);
                if ((nbytes = file->Read(result, resultSize)) != resultSize)
                {
                    ::free(result);
                    file->Close();
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

                file->Close();
                return result;
            }

            return NULL;
        }

        void  FreeWave(void* wave)
        {
            ::free(wave);
        }

        void* LoadImage(const char* path, int* width, int* height, int* channel)
        {
            path = FileSystem::GetExistsPath(path);
            if (path)
            {
                return ::stbi_load(path, width, height, channel, 0);
            }

            return NULL;
        }

        void  FreeImage(void* pixels)
        {
            ::stbi_image_free(pixels);
        }
    }
}