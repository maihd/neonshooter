#include <raylib.h>
#include <raymath.h>

#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include <Debug.h>

#include "NeonShooter_World.h"
#include "NeonShooter_Assets.h"
#include "NeonShooter_GameAudio.h"
#include "NeonShooter_ParticleSystem.h"

#if __EMSCRIPTEN__
#include <emscripten.h>
#endif

static float clampf(float value, float min, float max)
{
    const float res = value < min ? min : value;
    return res > max ? max : res;
}

// Calculate linear interpolation between two floats
static float lerpf(float start, float end, float amount)
{
    return start + amount * (end - start);
}

static int frameCount;
int GetFrameCount(void)
{
    return frameCount;
}

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 720;

static World            world;

static Shader           bloomShader;
static RenderTexture    framebuffer;

static void Init(void);
static void Close(void);

static void RunOneFrame(void);

#if defined(RELEASE) && defined(_MSC_VER)
int mainFunction()
#else
int main()
#endif
{
    // Init app
    Init();

    // App main loop
#ifndef __EMSCRIPTEN__
    while (!WindowShouldClose())
    {
        RunOneFrame();
    }
#else
    emscripten_set_main_loop(RunOneFrame, 60, 1);
#endif

    // Close app
#ifndef __EMSCRIPTEN__
    Close();
#endif

    return 0;
}

void Init(void)
{
    srand((uint32_t)(time(0)));
    
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Neon shooter");

    DebugPrint("Hello world");

    SetConfigFlags(FLAG_VSYNC_HINT);
    SetTargetFPS(60);

    GameAudioInit();
    InitCacheTextures();
    InitParticles();

    world = WorldNew();

#ifdef __EMSCRIPTEN__
    const char* bloomShaderSource = 
        "#version 100\n"

        "precision mediump int;\n"
        "precision mediump float;\n"

        "varying vec2 fragTexCoord;\n"
        "varying vec4 fragColor;\n"

        "uniform sampler2D texture0;\n"
        "uniform vec2 texSize;\n"

        "void main() {\n"
        "float weight[5];\n"
        "weight[0] = 0.227027;\n"
        "weight[1] = 0.1945946;\n"
        "weight[2] = 0.1216216;\n"
        "weight[3] = 0.054054;\n"
        "weight[4] = 0.016216;\n"

        "float size = max(texSize.x, texSize.y);\n"

        "vec2 texOffset = 1.0 / vec2(size, size);\n"
        "vec3 result = texture2D(texture0, fragTexCoord).rgb * weight[0];\n"

        "for(int i = 1; i < 5; ++i) {\n"
        "float f = float(i);"
        "result += texture2D(texture0, fragTexCoord + vec2(texOffset.x * f, 0.0)).rgb * weight[i];\n"
        "result += texture2D(texture0, fragTexCoord - vec2(texOffset.x * f, 0.0)).rgb * weight[i];\n"
        "result += texture2D(texture0, fragTexCoord + vec2(0.0, texOffset.y * f)).rgb * weight[i];\n"
        "result += texture2D(texture0, fragTexCoord - vec2(0.0, texOffset.y * f)).rgb * weight[i];\n"
        "}\n"

        "result = texture2D(texture0, fragTexCoord).rgb + result;\n"
        "result = vec3(1.0) - exp(-result * 0.5);\n"
        "result = pow(result, vec3(1.0 / 2.2));\n"
        "gl_FragColor = vec4(result, 1.0);\n"
        "}";
#elif 1
    const char* bloomShaderSource =
        "#version 330 core\n"

        "in vec2 fragTexCoord;"
        "in vec4 fragColor;"

        "out vec4 finalColor;"

        "uniform sampler2D image;"

        "void main() {"
        "float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);"

        "vec2 texSize = textureSize(image, 0);"
        "float size = max(texSize.x, texSize.y);"

        "vec2 texOffset = 1.0 / vec2(size, size);"
        "vec3 result = texture(image, fragTexCoord).rgb * weight[0];"

        "for(int i = 1; i < 5; ++i) {"
        "result += texture(image, fragTexCoord + vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
        "result += texture(image, fragTexCoord - vec2(texOffset.x * i, 0.0)).rgb * weight[i];"
        "result += texture(image, fragTexCoord + vec2(0.0, texOffset.y * i)).rgb * weight[i];"
        "result += texture(image, fragTexCoord - vec2(0.0, texOffset.y * i)).rgb * weight[i];"
        "}"

        "result = texture(image, fragTexCoord).rgb + result;"
        "result = vec3(1.0) - exp(-result * 0.5f);"
        "result = pow(result, vec3(1.0 / 2.2));"
        "finalColor = vec4(result, 1.0);"
        "}";
#else
    const char* bloomShaderSource =
        "#version 330\n"

        // Input vertex attributes (from vertex shader)
        "in vec2 fragTexCoord;"
        "in vec4 fragColor;"

        // Output fragment color
        "out vec4 finalColor;"

        // Input uniform values
        "uniform sampler2D image;"
        "uniform vec4 colDiffuse;"

        "void main()"
        "{"
            "float samples = 5.0;"        // pixels per axis; higher = bigger glow, worse performance
            "float quality = 1.7;" 	      // lower = smaller glow, better quality

            "vec2 size = textureSize(image, 0);"
            "vec2 sizeFactor = vec2(1.0) / size * quality;"

            "vec4 sum = vec4(0);"

            // Texel color fetching from texture sampler
            "vec4 source = texture(image, fragTexCoord);"

            "int range = 2;"

            "for (int x = -range; x <= range; x++)"
            "{"
                "for (int y = -range; y <= range; y++)"
                "{"
                    "sum += texture(image, fragTexCoord + vec2(x, y) * sizeFactor);"
                "}"
            "}"

            // Calculate final fragment color
            "finalColor = ((sum / (samples * samples)) + source) * colDiffuse;"
        "}";
#endif

    bloomShader = LoadShaderFromMemory(NULL, bloomShaderSource);
    framebuffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Close(void)
{
    WorldFree(&world);
    ReleaseParticles();

    GameAudioRelease();
    ClearCacheTextures();
    CloseWindow();
}

void RunOneFrame(void)
{
    static float    timer           = 0.0f;
    static float    timeStep        = 1.0f / 60.0f;

    static int      fpsCount        = 0;
    static int      fpsValue        = 0;
    static float    fpsTimer        = 0.0f;
    static float    fpsInterval     = 1.0f;

    static float    axisVertical    = 0.0f;
    static float    axisHorizontal  = 0.0f;

    static Vector2  aim             = { 0.0f, 0.0f };
    static bool     fire            = false;

    //while (!WindowShouldClose())
    {
        frameCount++;

        GameAudioUpdate();

        float deltaTime = GetFrameTime();

        fpsTimer += deltaTime;
        if (fpsTimer >= fpsInterval)
        {
            fpsTimer -= fpsInterval;

            fpsValue = (int)(fpsCount / fpsInterval);
            fpsCount = 0;
        }

        timer += deltaTime;
        while (timer >= timeStep)
        {
            const float LERP_RATE = 0.5f;

            // Move by keyboard

            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) || IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
            {
                if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
                {
                    axisVertical = lerpf(axisVertical, -1.0f, LERP_RATE);
                }

                if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
                {
                    axisVertical = lerpf(axisVertical, 1.0f, LERP_RATE);
                }
            }
            else
            {
                axisVertical = lerpf(axisVertical, 0.0f, LERP_RATE);
            }

            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
            {
                if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
                {
                    axisHorizontal = lerpf(axisHorizontal, -1.0f, LERP_RATE);
                }

                if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
                {
                    axisHorizontal = lerpf(axisHorizontal, 1.0f, LERP_RATE);
                }
            }
            else
            {
                axisHorizontal = lerpf(axisHorizontal, 0.0f, LERP_RATE);
            }

            // Shooting by mouse

            float mx = GetMouseX();
            float my = GetMouseY();
            bool fire = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
            {
                Vector2 clip = { 2.0f * mx / GetScreenWidth() - 1.0f, 2.0f * my / GetScreenHeight() - 1.0f };

                Vector2 mpos = { clip.x * GetScreenWidth(), clip.y * GetScreenHeight() };

                Vector2 taim = Vector2Normalize(Vector2Subtract(mpos, world.player.position));

                aim = Vector2Lerp(aim, taim, 0.8f);
            }

            // Moving & shooting by controller

            if (IsGamepadAvailable(0))
            {
                float axis_left_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
                float axis_left_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
                axisVertical = lerpf(axisVertical, fabsf(axis_left_y) > 0.05f ? axis_left_y : 0.0f, LERP_RATE);
                axisHorizontal = lerpf(axisHorizontal, fabsf(axis_left_x) > 0.05f ? axis_left_x : 0.0f, LERP_RATE);

                float axis_right_x = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
                float axis_right_y = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_Y);
                float x = fabsf(axis_right_x) > 0.1f ? axis_right_x : 0.0f;
                float y = fabsf(axis_right_y) > 0.1f ? axis_right_y : 0.0f;
                if (Vector2Length((Vector2){ x, y }) < 0.01f)
                {
                    //aim = (Vector2){0, 0};
                }
                else
                {
                    fire = true;

                    float cur_angle = atan2f(aim.y, aim.x);
                    float aim_angle = atan2f(y, x);

                    cur_angle = lerpf(cur_angle, aim_angle, 0.8f);
                    aim = (Vector2){ cosf(cur_angle), sinf(cur_angle) };

                    aim.x = lerpf(aim.x, x, 0.6f);
                    aim.y = lerpf(aim.y, y, 0.6f);
                }
            }

            Vector2 axes = { axisHorizontal, axisVertical };
            if (Vector2Length(axes) < 0.01f)
            {
                axes = (Vector2){ 0, 0 };
            }
            else
            {
                float magnitude = clampf(Vector2Length(axes), 0, 1);
                float angle = atan2f(axes.y, axes.x);

                axes = (Vector2){ cosf(angle) * magnitude, sinf(angle) * magnitude };
            }

            fpsCount++;

            timer -= timeStep;

            WorldUpdate(&world, axes.x, axes.y, aim, fire, timeStep);

            UpdateParticles(&world, timeStep);
        }

        BeginDrawing();
        {
            ClearBackground(BLACK);
            
            BeginTextureMode(framebuffer);
            ClearBackground(BLACK);

            Camera2D camera = {
                (Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f },
                (Vector2){ 0, 0 },
                0,
                0.5f,
            };
            BeginMode2D(camera);
            {
                WorldRender(world);
                DrawParticles();
            }
            EndMode2D();

            EndTextureMode();

            BeginShaderMode(bloomShader);

#ifdef __EMSCRIPTEN__
            int texSizeLocation = GetShaderLocation(bloomShader, "texSize");
            Vector2 texSize = { (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT }; 
            SetShaderValue(bloomShader, texSizeLocation, &texSize, SHADER_UNIFORM_VEC2);
#endif

            DrawTextureRec(framebuffer.texture, (Rectangle){ 0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT }, (Vector2){ 0, 0 }, WHITE);
            EndShaderMode();

            DrawText(TextFormat("CPU FPS: %d", fpsValue), 0, 0, 18, RAYWHITE);
            DrawText(TextFormat("GPU FPS: %d", GetFPS()), 0, 24, 18, RAYWHITE);
        }
        EndDrawing();
    }
}
