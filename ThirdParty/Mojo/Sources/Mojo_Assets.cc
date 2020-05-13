#include <Mojo/Assets.h>
#include <Mojo/FileSystem.h>
#include <Mojo/Core/Array.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace Mojo
{
    //inline U32 Stream::ReadUint32LE(void)
    //{
    //    U32 result;
    //    return this->Read(&result, sizeof(result)) == sizeof(result) ? LE_TO_NATIVE_32(result) : 0;
    //}

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
                U8  riff[4];
                U32 size;

                U8  wave[4];   // "WAVE"
                U8  fmt[4];    // "fmt\0"
                U32 fmtLength;

                U16 audioFormat;
                U16 channels;

                U32 sampleRate;
                U32 byteRate;

                U16 blockAlign;
                U16 bitsPerSample;
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
                    U16 extraParams = file->ReadUint16LE();
                    file->Skip(extraParams);
                }

                char dataName[4];
                if (file->Read(&dataName, 4) != 4 || strncmp(dataName, "data", 4) != 0)
                {
                    file->Close();
                    return NULL;
                }
                
                U32 dataChunkSize = file->ReadUint32LE();
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