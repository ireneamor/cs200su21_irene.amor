// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AEInput.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	declaration for input stuff
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_INPUT_H
#define AE_INPUT_H
// ---------------------------------------------------------------------------
// Defines/Enums

#define AE_INPUT_KEY_NUM	256
#define AE_INPUT_MOUSE_NUM 3

#define AE_MOUSE_LEFT	0
#define AE_MOUSE_RIGHT	1
#define AE_MOUSE_MIDDLE	2
#define AE_KEY_CTRL		17
#define AE_KEY_SHIFT	16
#define AE_KEY_TAB		9
#define AE_KEY_ALT		10

// Gamepad buttons
#define AE_GAMEPAD_DPAD_UP          0x0001
#define AE_GAMEPAD_DPAD_DOWN        0x0002
#define AE_GAMEPAD_DPAD_LEFT        0x0004
#define AE_GAMEPAD_DPAD_RIGHT       0x0008
#define AE_GAMEPAD_START            0x0010
#define AE_GAMEPAD_BACK             0x0020
#define AE_GAMEPAD_LEFT_THUMB       0x0040
#define AE_GAMEPAD_RIGHT_THUMB      0x0080
#define AE_GAMEPAD_LEFT_SHOULDER    0x0100
#define AE_GAMEPAD_RIGHT_SHOULDER   0x0200
#define AE_GAMEPAD_A                0x1000
#define AE_GAMEPAD_B                0x2000
#define AE_GAMEPAD_X                0x4000
#define AE_GAMEPAD_Y                0x8000

// ---------------------------------------------------------------------------
// Externs

extern AEVector2 gAEMousePosition;		// Mouse Position in Centered Coordinates
extern AEVector2 gAEMousePrevPosition;	// Previous Mouse Position in Centered Coordinates
extern AEVector2 gAEMouseMovement;		// Mouse Movement during one frame
extern bool		 gAEMouseInWindow;		// Specifies whether the mouse is contained insisde the window

// ---------------------------------------------------------------------------
// class definitions

// ---------------------------------------------------------------------------
// Function prototypes

bool	AEInputHandleMessage	(UINT msg, WPARAM wParam, LPARAM lParam);
bool	AEInputInit				();
void	AEInputReset			();
void	AEInputUpdate			();
void	AEInputTerminate		();

u8		AEInputKeyPressed		(u8 key);		// Use the character index and the macros (#define) above
u8		AEInputKeyTriggered		(u8 key);		// Use the character index and the macros (#define) above
u8		AEInputMousePressed		(u8 button);	// Use the maccros (#define) above
u8		AEInputMouseTriggered	(u8 button);	// Use the maccros (#define) above
f32		AEInputGetMouseWheel	();

// ---------------------------------------------------------------------------
// Gamepad Function prototypes

/*!
\brief	Returns whether the specified gamepad is connected.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
bool	AEInputGamepadConnected(u32 idx);				

/*!
\brief	Returns whether the specified button is pressed.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	button Button to check (use one of the maccros defined above). 
*/
bool	AEInputGamepadButtonPressed(u32 idx, u32 button);

/*!
\brief	Returns whether the specified gamepad is connected
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	button Button to check (use one of the maccros defined above). 
*/
bool	AEInputGamepadButtonTriggered(u32 idx, u32 button);

/*!
\brief	Returns the LEFT stick X axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadStickLeftRawX(u32 idx);

/*!
\brief	Returns the LEFT stick Y axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadStickLeftRawY(u32 idx);

/*!
\brief	Returns the RIGHT stick X axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadStickRightRawX(u32 idx);

/*!
\brief	Returns the RIGHT stick Y axis value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadStickRightRawY(u32 idx);

/*!
\brief	Returns the LEFT stick axis value in vector format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	The function implements a circular deadzone. You can set the deadzone
		values using the provided functions below. 
\remark	The returned vector's magnitude is in the range [0, 1].
*/
AEVec2	AEInputGamepadStickLeft(u32 idx);

/*!
\brief	Returns the RIGHT stick axis value in vector format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	The function implements a circular deadzone. You can set the deadzone
		values using the provided functions below.
\remark	The returned vector's magnitude is in the range [0, 1].
*/
AEVec2	AEInputGamepadStickRight(u32 idx);

/*!
\brief	Returns the LEFT trigger value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 255].
\remark You shouldn't need to use this function in general. Use
		AEInputGamepadTriggerLeft instead. 
*/
u8	AEInputGamepadTriggerLeftRaw(u32 idx);

/*!
\brief	Returns the RIGHT trigger value in raw format
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 255].
\remark You shouldn't need to use this function in general. Use
		AEInputGamepadTriggerRight instead.
*/
u8	AEInputGamepadTriggerRightRaw(u32 idx);

/*!
\brief	Returns the LEFT trigger value normalized.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 1].
\remark This function ignores any value of the trigger below a threshold.
		You can get the threshold value using AEInputGamepadGetTriggerDeadzone.
		You can set the threshold value using AEInputGamepadSetTriggerDeadzone.
*/
f32	AEInputGamepadTriggerLeft(u32 idx);

/*!
\brief	Returns the RIGHT trigger value normalized.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark The return value is in the range [0, 1].
\remark This function ignores any value of the trigger below a threshold.
		You can get the threshold value using AEInputGamepadGetTriggerDeadzone.
		You can set the threshold value using AEInputGamepadSetTriggerDeadzone.
*/
f32	AEInputGamepadTriggerRight(u32 idx);

/*!
\brief	Sets the vibration of the specified gamepad (both motors).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void	AEInputGamepadSetVibration(u32 idx, f32 valNorm);

/*!
\brief	Stops all vibration on specified gamepad.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void	AEInputGamepadStopVibration(u32 idx);

/*!
\brief	Returns the current vibration on specified gamepad.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\remark	Because there are 2 motors in the controller, this function returns
		the value of the motor with the greatest vibration speed.
*/
f32		AEInputGamepadGetVibration(u32 idx);

/*!
\brief	Sets the vibration of the specified gamepad (LEFT  motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void	AEInputGamepadSetVibrationLeft(u32 idx, f32 valNorm);

/*!
\brief	Sets the vibration of the specified gamepad (RIGHT motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
\param	valNorm Speed of vibration in the range [0, 1].
*/
void	AEInputGamepadSetVibrationRight(u32 idx, f32 valNorm);


/*!
\brief	Gets the vibration of the specified gamepad (LEFT  motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
f32		AEInputGamepadGetVibrationLeft(u32 idx);

/*!
\brief	Gets the vibration of the specified gamepad (RIGHT motor).
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
f32		AEInputGamepadGetVibrationRight(u32 idx);

/*!
\brief	Sets the deadzone value for the LEFT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void	AEInputGamepadSetLeftThumbDeadzone(s32 deadzone);

/*!
\brief	Sets the deadzone value for the RIGHT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void	AEInputGamepadSetRightThumbDeadzone(s32 deadzone);

/*!
\brief	Sets the deadzone value for both triggers.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
void	AEInpuGamepadtSetTriggerDeadzone(s32 deadzone);

/*!
\brief	Returns the deadzone value for the LEFT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadGetLeftThumbDeadzone();

/*!
\brief	Returns the deadzone value for the RIGHT thumbstick.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadGetRightThumbDeadzone();

/*!
\brief	Returns the deadzone value for the triggers.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadGetTriggerDeadzone();

/*!
\brief	Resets the deadzone value for the LEFT thumbstick to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadResettLeftThumbDeadzone();

/*!
\brief	Resets the deadzone value for the RIGHT thumbstick to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadResetRightThumbDeadzone();

/*!
\brief	Resets the deadzone value for the triggers to the default value.
\param	idx Index of the gamepad to check. Must be in the range [0,3]
*/
s32		AEInputGamepadResetTriggerDeadzone();

// ---------------------------------------------------------------------------

#endif // AE_INPUT_H

