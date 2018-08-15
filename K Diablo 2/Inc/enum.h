#pragma once

enum class RESOLUTION
{
	WIDTH = 800,
	HEIGHT = 600
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
	DESTROY
};

enum class OBJECT
{
	UI
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
	TITLE_SCREEN
};