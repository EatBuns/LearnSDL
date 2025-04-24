#pragma once
#include "XNode.h"
#include <SDL.h>
#include <unordered_map>

class inputAbstract : public XNode
{
public:
	inputAbstract() = default;
	virtual ~inputAbstract() = default;
	virtual void inputEvent(SDL_Event& e) = 0;
	virtual bool isRight() = 0;
	virtual bool isLeft() = 0;
	virtual bool isUp() = 0;
	virtual bool isDown() = 0;
	virtual bool isSpace() = 0;
	virtual bool isAttack() = 0;
	virtual void changeKeyMap(const std::string& name, Sint32 key) = 0;

	void on_update(float delta) final {};
	void on_render() final {};

protected:
	std::unordered_map<std::string, Sint32> key_map;
};


class keyBoradBridge :public inputAbstract
{
public:
	void inputEvent(SDL_Event& e) override;

	keyBoradBridge();
	~keyBoradBridge() {}

	void changeKeyMap(const std::string& name, Sint32 key)
	{
		key_map[name] = key;
	}

	bool isRight() override;
	bool isLeft() override;
	bool isUp() override;
	bool isDown() override;
	bool isSpace() override;
	bool isAttack() override;

private:
	bool is_right, is_left, is_up, is_down, is_space,is_attack;
};



class GameControlBridge : public inputAbstract
{
public:
    GameControlBridge()
        : is_right(false), is_left(false), is_up(false), is_down(false), is_space(false) {}

    void inputEvent(SDL_Event& e) override
    {
        if (e.type == SDL_CONTROLLERBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONUP)
        {
            bool is_pressed = (e.type == SDL_CONTROLLERBUTTONDOWN);
            switch (e.cbutton.button)
            {
            case SDL_CONTROLLER_BUTTON_DPAD_UP:
                is_up = is_pressed;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                is_down = is_pressed;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                is_left = is_pressed;
                break;
            case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                is_right = is_pressed;
                break;
            case SDL_CONTROLLER_BUTTON_A:
                is_space = is_pressed;
                break;
            default:
                break;
            }
        }
    }

	void changeKeyMap(const std::string& name, Sint32 key)
	{
		key_map[name] = key;
	}

    bool isRight() override { return is_right; }
    bool isLeft() override { return is_left; }
    bool isUp() override { return is_up; }
    bool isDown() override { return is_down; }
    bool isSpace() override { return is_space; }
	bool isAttack() override { return false; } // Game controller doesn't have attack button in this example

private:
    bool is_right, is_left, is_up, is_down, is_space;
};

class inputControl
{
public:
	enum class InputMode
	{
		keyboard = 0,
		gamecontrol
	};
	inputControl();
	~inputControl();

	void InputEvent(SDL_Event& e);
	bool isRight();
	bool isLeft();
	bool isUp();
	bool isDown();
	bool isSpace();
	bool isAttack();
	void changeInputMode(InputMode i);
	void changeKeyMap(const std::string& name, Sint32 key){inputs[(int)idx]->changeKeyMap(name, key);}

private:
	inputAbstract* inputs[8];
	InputMode idx;		//当前使用的输入模式
};
