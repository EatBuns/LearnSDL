#include "inputAbstract.h"

inputControl::inputControl():idx(InputMode::keyboard)
{
	for (int i = 0; i < 8; ++i)
	{
		inputs[i] = nullptr;
	}
	inputs[0] = new keyBoradBridge();
    inputs[1] = new GameControlBridge();
}

inputControl::~inputControl()
{
	for (int i = 0; i < 8; ++i)
	{
		if(inputs[i])
			delete inputs[i];
	}
}

void inputControl::InputEvent(SDL_Event& e)
{
	inputs[(int)idx]->inputEvent(e);
}

bool inputControl::isRight()
{
	return inputs[(int)idx]->isRight();
}

bool inputControl::isLeft()
{
	return inputs[(int)idx]->isLeft();
}

bool inputControl::isUp()
{
	return inputs[(int)idx]->isUp();
}

bool inputControl::isDown()
{
	return inputs[(int)idx]->isDown();
}

bool inputControl::isSpace()
{
	return inputs[(int)idx]->isSpace();
}

bool inputControl::isAttack()
{
    return inputs[(int)idx]->isAttack();
}

bool inputControl::is_Skill_1()
{
    return inputs[(int)idx]->isSkill_1();;
}

void inputControl::changeInputMode(InputMode i)
{
	idx = i;
}

void keyBoradBridge::inputEvent(SDL_Event& e)  
{  
    /*if ((keysym.mod & KMOD_CTRL) && (keysym.sym == SDLK_c)) {
        printf("Ctrl + C 组合键被按下，模拟复制操作...\n");
    }*/

    if (e.type == SDL_KEYDOWN)  
    {  
        if (e.key.keysym.sym == key_map["UP"])  
        {  
            printf("UP\n");  
            is_up = true;  
        }  
        else if (e.key.keysym.sym == key_map["DOWN"])  
        {  
            printf("DOWN\n");  
            is_down = true;  
        }  
        else if (e.key.keysym.sym == key_map["LEFT"])  
        {  
            printf("LEFT\n");  
            is_left = true;  
        }  
        else if (e.key.keysym.sym == key_map["RIGHT"])  
        {  
            printf("RIGHT\n");  
            is_right = true;  
        }  
        else if (e.key.keysym.sym == key_map["SPACE"])  
        {  
            printf("SPACE\n");  
            is_space = true;  
        }  
        else if (e.key.keysym.sym == key_map["ATTACK"])
        {
            printf("ATTACK\n");
            is_attack = true;
        }
        else if (e.key.keysym.sym == key_map["SKILL_1"] && !e.key.repeat)
        {
            printf("SKILL_1,e.key.repeat:%d\n", e.key.repeat);
            is_skill_1 = true;
        }
    }  
    else if (e.type == SDL_KEYUP)  
    {  
        e.key.repeat;
        if (e.key.keysym.sym == key_map["UP"])  
        {  
            printf("NO UP\n");  
            is_up = false;  
        }  
        else if (e.key.keysym.sym == key_map["DOWN"])  
        {  
            printf("NO DOWN\n");  
            is_down = false;  
        }  
        else if (e.key.keysym.sym == key_map["LEFT"])  
        {  
            printf("NO LEFT\n");  
            is_left = false;  
        }  
        else if (e.key.keysym.sym == key_map["RIGHT"])  
        {  
            printf("NO RIGHT\n");  
            is_right = false;  
        }  
        else if (e.key.keysym.sym == key_map["SPACE"])  
        {  
            printf("NO SPACE\n");  
            is_space = false;  
        }  
        else if (e.key.keysym.sym == key_map["ATTACK"])
        {
            printf("NO ATTACK\n");
            is_attack = false;
        }
        else if (e.key.keysym.sym == key_map["SKILL_1"])
        {
            printf("NO SKILL_1\n");
            is_skill_1 = false;
        }
    }  
}
			

keyBoradBridge::keyBoradBridge()
{
	key_map["UP"] = SDL_KeyCode::SDLK_UP;
	key_map["DOWN"] = SDL_KeyCode::SDLK_DOWN;
	key_map["LEFT"] = SDL_KeyCode::SDLK_LEFT;
	key_map["RIGHT"] = SDL_KeyCode::SDLK_RIGHT;
	key_map["SPACE"] = SDL_KeyCode::SDLK_SPACE;
    key_map["ATTACK"] = SDL_KeyCode::SDLK_j;
	key_map["SKILL_1"] = SDL_KeyCode::SDLK_1;

	is_right = false;
	is_left = false;
	is_up = false;
	is_down = false;
	is_space = false;
    is_attack = false;
}

bool keyBoradBridge::isRight()
{
	return is_right;
}

bool keyBoradBridge::isLeft()
{
	return is_left;
}

bool keyBoradBridge::isUp()
{
	return is_up;
}

bool keyBoradBridge::isDown()
{
	return is_down;
}

bool keyBoradBridge::isSpace()
{
	return is_space;
}

bool keyBoradBridge::isAttack()
{
    return is_attack;
}

bool keyBoradBridge::isSkill_1()
{
    return is_skill_1;
}
