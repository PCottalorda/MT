#pragma once
#include "D:\Programmation\Librairies\SFML\sources\include\SFML\Window\Joystick.hpp"


class XBoxJoystick :
	public sf::Joystick
{
public:
	XBoxJoystick();
	virtual ~XBoxJoystick();

	/**
	 * @brief Associate the name of a button to an integer value. The integer value corresponds to the one used in the class <b>Joystick</b> of the SFML 
	 */
	enum Button
	{
		A		= 0,
		B		= 1,
		X		= 2,
		Y		= 3,
		LB		= 4,
		RB		= 5,
		START	= 6,
		SELECT	= 7,	// SELECT and BACK are the same button
		BACK	= 7,
		LEFT	= 8,
		RIGHT	= 9
	};


	struct JoystickPosition
	{
		float x;
		float y;
	};

	const JoystickPosition getLeftJoystickPosition();
	const JoystickPosition getRJoystickPosition();
	const JoystickPosition getDPadPosition();
	

private:
	JoystickPosition RightJoystick;
	JoystickPosition LeftJoystick;
	JoystickPosition Dpad;
};

