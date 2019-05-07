#include <Mojo/Input.h>
#include "Mojo_SDL2_Input.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <SDL2/SDL.h>

#define COUNTOF(array) (sizeof(array) / sizeof((array)[0]))

inline namespace Mojo
{
    namespace
    {
        constexpr int   KEY_INPUT_COUNT      = 1024;
        constexpr int   MOUSE_INPUT_COUNT    = 8;
        constexpr int   MAX_GAME_CONTROLLERS = 8;
        constexpr int   MAX_GAMEPAD_BUTTONS  = 16;

        struct GamepadState
        {
            float   axis[8];
            
            bool    buttonPressed[MAX_GAMEPAD_BUTTONS];
            int     buttonUpFrame[MAX_GAMEPAD_BUTTONS];
            int     buttonDownFrame[MAX_GAMEPAD_BUTTONS];
        };

        static int      _currentFrame;

        static int      _inputTextLength;
        static char     _inputText[1024];

        static bool     _keyState[KEY_INPUT_COUNT];
        static int      _keyUpFrame[KEY_INPUT_COUNT];
        static int      _keyDownFrame[KEY_INPUT_COUNT];
                        
        static int      _mouseState;
        static int      _mouseUpFrame[MOUSE_INPUT_COUNT];
        static int      _mouseDownFrame[MOUSE_INPUT_COUNT];
        static float    _mouseX;
        static float    _mouseY;

        static float    _mouseWheelH;
        static float    _mouseWheelV;
        
        static GamepadState        _gamepadStates[MAX_GAME_CONTROLLERS];
        static SDL_GameController* _gameControllers[MAX_GAME_CONTROLLERS];
        
        static float GetJoystickAxis(SDL_GameController* gameController, SDL_GameControllerAxis axis)
        {
            constexpr float FACTOR = 1.0f / ((float)SDL_MAX_SINT16);
            constexpr int   DEAD_ZONE = 4000;

            int value = SDL_GameControllerGetAxis(gameController, axis);

            if (value > DEAD_ZONE)
            {
                return value * FACTOR;
            }
            else if (value < -DEAD_ZONE)
            {
                return value * FACTOR;
            }
            else
            {
                return 0.0f;
            }
        }
    }
    
    namespace Input
    {
        void Setup(void)
        {
            if (SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) == 0)
            {
                int index = 0;
                for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
                {
                    if (SDL_IsGameController(i))
                    {
                        _gameControllers[index++] = SDL_GameControllerOpen(i);
                    }
                }
            }
        }

        void Shutdown(void)
        {
            for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
            {
                if (_gameControllers[i])
                {
                    SDL_GameControllerClose(_gameControllers[i]);
                    _gameControllers[i] = NULL;
                }
            }
        }

        void NewFrame(void)
        {
            _currentFrame++;
            _inputTextLength = 0;

            //for (int i = 0, n = SDL_NumJoysticks(); i < n; i++)
            //{
            //    SDL_Joystick* joystick = SDL_JoystickOpen(i);
            //
            //    if (joystick)
            //    {
            //        GamepadState& gamepadState = _gamepadStates[i];
            //        gamepadState.axis[(int)GamepadAxis::MoveVertical]   = 0.0f;
            //        gamepadState.axis[(int)GamepadAxis::MoveHorizontal] = 0.0f;
            //        
            //        gamepadState.axis[(int)GamepadAxis::LeftVertical]   = 0.0f;
            //        gamepadState.axis[(int)GamepadAxis::LeftHorizontal] = 0.0f;
            //        
            //        gamepadState.axis[(int)GamepadAxis::RightVertical]   = 0.0f;
            //        gamepadState.axis[(int)GamepadAxis::RightHorizontal] = 0.0f;
            //    }
            //}
        }

        void EndFrame(void)
        {
            _inputText[_inputTextLength] = 0;

            SDL_GameControllerUpdate();
            for (int i = 0, n = MAX_GAME_CONTROLLERS; i < n; i++)
            {
                SDL_GameController* gameController = _gameControllers[i];
                if (SDL_GameControllerGetAttached(gameController))
                {
                    float leftVertical    = -GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
                    float leftHorizontal  = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
                    
                    float rightVertical   = -GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY);
                    float rightHorizontal = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX);

                    float leftTrigger     = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
                    float rightTrigger    = GetJoystickAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
                    
                    GamepadState& gamepadState = _gamepadStates[i];
                    gamepadState.axis[(int)GamepadAxis::LeftVertical]   = leftVertical;
                    gamepadState.axis[(int)GamepadAxis::LeftHorizontal] = leftHorizontal;
                    
                    gamepadState.axis[(int)GamepadAxis::RightVertical]   = rightVertical;
                    gamepadState.axis[(int)GamepadAxis::RightHorizontal] = rightHorizontal;

                    gamepadState.axis[(int)GamepadAxis::LeftTrigger]  = leftTrigger;
                    gamepadState.axis[(int)GamepadAxis::RightTrigger] = rightTrigger;

                    for (int button = 0; button < MAX_GAMEPAD_BUTTONS; button++)
                    {
                        bool newState = SDL_GameControllerGetButton(gameController, (SDL_GameControllerButton)button);
                        bool oldState = gamepadState.buttonPressed[button];

                        if (oldState != newState)
                        {
                            gamepadState.buttonPressed[button] = newState;
                            if (newState)
                            {
                                gamepadState.buttonDownFrame[button] = _currentFrame;
                            }
                            else
                            {
                                gamepadState.buttonUpFrame[button] = _currentFrame;
                            }
                        }
                    }
                }
            }
        }

        void UpdateCharInput(int character)
        {
            _inputText[_inputTextLength++] = (char)character;
        }

        void UpdateCharInput(const char* string)
        {
            strcat(_inputText, string);
            _inputTextLength += strlen(string);
        }

        void UpdateKey(KeyCode key, bool down)
        {
            int index = (int)key;
            if (index > -1 && index < KEY_INPUT_COUNT)
            {
                _keyState[index] = down;

                if (down)
                {
                    _keyDownFrame[index] = _currentFrame;
                }
                else
                {
                    _keyUpFrame[index] = _currentFrame;
                }
            }
        }

        void UpdateMouse(MouseButton button, bool down)
        {
            int index = (int)button;
            if (index > -1 && index < MOUSE_INPUT_COUNT)
            {
                if (down)
                {
                    _mouseState |= button;
                    _mouseDownFrame[index] = _currentFrame;
                }
                else
                {
                    _mouseState &= ~button;
                    _mouseUpFrame[index] = _currentFrame;
                }
            }
        }

        void UpdateMouseMove(float x, float y)
        {
            _mouseX = x;
            _mouseY = y;
        }

        void UpdateMouseWheel(float h, float v)
        {
            _mouseWheelV += v;
            _mouseWheelH += h;
        }

        void ShowMouseCursor(void)
        {
            SetMouseCursorVisible(true);
        }

        void HideMouseCursor(void)
        {
            SetMouseCursorVisible(false);
        }

        bool IsMouseCursorVisibled(void)
        {
            return false;
        }

        void SetMouseCursorVisible(bool visible)
        {
            
        }

        int  GetMouseState(float* x, float* y)
        {
            if (x) *x = _mouseX;
            if (y) *y = _mouseY;

            return _mouseState;
        }

        bool GetMouseButton(MouseButton button)
        {
            int index = (int)button;
            if (index > -1 && index < COUNTOF(_mouseDownFrame) && index < COUNTOF(_mouseUpFrame))
            {
                return _mouseDownFrame[index] > _mouseUpFrame[index];
            }

            return false;
        }

        bool GetMouseButtonUp(MouseButton button)
        {
            int index = (int)button;
            if (index > -1 && index < COUNTOF(_mouseUpFrame))
            {
                return _mouseUpFrame[index] == _currentFrame;
            }

            return false;
        }

        bool GetMouseButtonDown(MouseButton button)
        {
            int index = (int)button;
            if (index > -1 && index < COUNTOF(_mouseDownFrame))
            {
                return _mouseDownFrame[index] == _currentFrame;
            }

            return false;
        }

        bool GetKey(KeyCode key)
        {
            int index = (int)key;
            if (index > -1 && index < KEY_INPUT_COUNT)
            {
                return _keyState[index];
            }

            return false;
        }

        bool GetKeyUp(KeyCode key)
        {
            int index = (int)key;
            if (index > -1 && index < KEY_INPUT_COUNT)
            {
                return _keyUpFrame[index] == _currentFrame;
            }

            return false;
        }

        bool GetKeyDown(KeyCode key)
        {
            int index = (int)key;
            if (index > -1 && index < KEY_INPUT_COUNT)
            {
                return _keyDownFrame[index] == _currentFrame;
            }

            return false;
        }

        float GetAxis(int gamepadID, GamepadAxis axis)
        {
            if (gamepadID > -1 && gamepadID < COUNTOF(_gamepadStates))
            {
                GamepadState& state = _gamepadStates[gamepadID];

                int index = (int)axis;
                if (index > -1 && index < COUNTOF(state.axis))
                {
                    return state.axis[index];
                }
            }

            return 0.0f;
        }

        bool  GetButton(int gamepadID, GamepadButton button)
        {
            if (gamepadID > -1 && gamepadID < MAX_GAME_CONTROLLERS)
            {
                GamepadState& state = _gamepadStates[gamepadID];

                int index = (int)button;
                if (index > -1 && index < MAX_GAMEPAD_BUTTONS)
                {
                    return state.buttonPressed[index];
                }
            }

            return false;
        }

        bool  GetButtonUp(int gamepadID, GamepadButton button)
        {
            if (gamepadID > -1 && gamepadID < MAX_GAME_CONTROLLERS)
            {
                GamepadState& state = _gamepadStates[gamepadID];

                int index = (int)button;
                if (index > -1 && index < MAX_GAMEPAD_BUTTONS)
                {
                    return state.buttonUpFrame[index] == _currentFrame;
                }
            }

            return false;
        }

        bool  GetButtonDown(int gamepadID, GamepadButton button)
        {
            if (gamepadID > -1 && gamepadID < MAX_GAME_CONTROLLERS)
            {
                GamepadState& state = _gamepadStates[gamepadID];

                int index = (int)button;
                if (index > -1 && index < MAX_GAMEPAD_BUTTONS)
                {
                    return state.buttonDownFrame[index] == _currentFrame;
                }
            }

            return false;
        }

        const char* GetTextInput(void)
        {
            return _inputText;
        }

        bool IsGamepadAttached(int gamepadID)
        {
            if (gamepadID > -1 && gamepadID < MAX_GAME_CONTROLLERS)
            {
                return SDL_GameControllerGetAttached(_gameControllers[gamepadID]);
            }
            else
            {
                return false;
            }
        }
    }
}