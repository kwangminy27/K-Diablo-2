#pragma once

enum class DISPLAY
{
	MODE = 0 // 0: window, 1: full_screen
};

enum class FONT_FRAME
{
	WIDTH_IDX = 16
};

enum class RESOLUTION
{
	WIDTH = 800,
	HEIGHT = 600,
	GAME_WIDTH = 640,
	GAME_HEIGHT = 480
};

enum class MESSAGE_LOOP
{
	RUN,
	EXIT
};

enum class COLOR
{
	RED,
	GREEN,
	BLUE,
	END
};

enum class ANIMATION_CLIP
{
	ATLAS,
	FRAME
};

enum class ANIMATION_OPTION
{
	LOOP,
	RETURN,
	DESTROY,
	ONCE
};

enum class OBJECT
{
	UI,
	STAGE,
	TILE,
	CHARACTER,
	MISSILE,
	EFFECT,
	TEXT
};

enum class COLLIDER
{
	POINT,
	RECT,
	CIRCLE,
	PIXEL
};

enum class COLLISION_CALLBACK
{
	ENTER,
	STAY,
	LEAVE,
	END
};

enum class BUTTON_STATE
{
	IDLE,
	UNDER_MOUSE,
	PUSH,
	PRESSED
};

enum class LOGO_SCENE
{
	TRADEMARK,
	TITLE,
	CHARACTER_SELECT,
	CHARACTER_CREATE,
	CREDIT
};

enum class STAGE
{
	NORMAL,
	ISOMETRIC
};

enum class TILE_OPTION
{
	NONE,
	BLOCKED,
	IDX_ERROR
};

enum class PLAYER
{
	IDLE,
	CASTING
};

enum class FONT_SIZE
{
	_6,
	_8,
	_16
};

enum class SKILL
{
	ICE_BOLT,
	FROZEN_ORB
};