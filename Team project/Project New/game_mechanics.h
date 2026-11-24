/*!
@file     game_mechanics.h
@author   Jun Kai (l.junkai)
@co-author  Sin Wee & Lewin Yeo
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief	Declarations for the core gameplay mechanics: player movement, wall
        cling, teleport, shield, platform collision, pressure plates, kill
        zones and moving platforms.
*//*______________________________________________________________________*/
#pragma once
#include "level.h"

/* ---------------------------------------------------------------------
   Coordinate conventions used in this module:
   - position is the player's TOP-LEFT corner. The player occupies the
	 box (position.x, position.y) to (position.x + PLAYER_SIZE,
	 position.y + PLAYER_SIZE).
   - DrawPlatform, collision_check, Pressure_Plate_Door,
	 Pressure_Plate_2Door, kill_zone and the *_alternating_movement
	 functions take the CENTRE of a rectangle.
   - sphere_aabb_intersection and CheckWinCondition take the TOP-LEFT
	 corner of a rectangle.
   --------------------------------------------------------------------- */

   /* ===== Constants ===== */
#define PLAYER_SIZE 50.f                    /*!< Width and height of the player, in pixels. */
#define SHIELD_SIZE (PLAYER_SIZE * 1.6f)    /*!< Diameter of the shield circle. */
#define SHIELD_DURATION    1.0f             /*!< Total time the shield exists, in seconds. */
#define SHIELD_PARRY_TIME  0.5f             /*!< Time at the start of the shield during which
												 projectiles are reflected, in seconds. */

// =====================================
// CLING BAR
// =====================================
extern int   is_clinging;   /*!< 1 while the player is clinging to a wall this frame. */
extern int   cling_side;    /*!< Side of the wall being clung to: 3 = wall on the player's
                                 right, 4 = wall on the player's left. */
extern float wall_timer;    /*!< Time spent clinging to the current wall, in seconds. */



/*!
@brief  Draws the wall-cling endurance bar next to the player, on the side
		away from the wall. The bar drains from top to bottom over
		WALL_CLING_TIME seconds and turns red when it is nearly empty.
		Draws nothing unless the player is clinging, and clears
		is_clinging afterwards so collision_check() must set it again
		each frame.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Draw_ClingBar(void);


// =====================================
// SHIELD
// =====================================
extern int   shieldActive;  /*!< 1 while the shield is up. */
extern float shieldTimer;   /*!< Time since the shield was summoned, in seconds. */

/*!
@brief  Draws the shield as a circle around the player. Gold during the
		first SHIELD_PARRY_TIME seconds (projectiles are reflected) and
		blue afterwards (projectiles are only blocked). Draws nothing if
		the shield is not active.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Shield_Draw(void);

/*!
@brief  Updates the shield each frame. Pressing P summons the shield if
		one is not already up. The shield disappears after
		SHIELD_DURATION seconds.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Shield_Update(void);

// =====================================
// TELEPORT
// =====================================
extern float TeleportWindowTiming;  /*!< Time since the last teleport, in seconds. Clamped to the cooldown. */
extern int   Teleport_Status;       /*!< 1 if teleport is ready (cooldown finished), 0 if cooling down. */
extern int   Teleport_count;        /*!< Teleport charges left. 3 in normal levels, 4 means unlimited
										 (boss level). */

/*!
@brief  Advances the teleport cooldown by the frame's delta time. The
        timer is clamped to the 2 second cooldown. When it reaches the
        cooldown, Teleport_Status is set back to 1 so the player can
        teleport again.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void TeleportTimer(void);
/*!
@brief  Handles the teleport ability and its HUD. Draws the cooldown bar
		above the player while cooling down, and draws the remaining
		charges (or "Unlimited Charges") at the top right of the screen.
		If the player holds D (moving right) or A (moving left) and
		presses Left Shift while a charge is available and the cooldown
		is over, the player is moved 85 pixels in that direction and
		a charge is used.

@param  None.

@return 1 if the player teleported this frame, otherwise 0.
*//*_______________________________________________________________________*/
int Teleport_Mechannic(void);


/* ===== Player state ===== */
extern int   isCarrying;    /*!< 1 while the player is carrying an object. */
extern int   pickingUp;     /*!< 1 during the pick-up animation. */
extern int   dropping;      /*!< 1 during the drop animation. */
extern float carry_timer;   /*!< Timer for the pick-up and drop animations, in seconds. */
extern int   onGround;      /*!< 1 if the player is standing on the floor or a platform. */
extern int   player_action; /*!< Current animation state: 1 standing, 2 jumping, 3 running,
								 4 grabbing (frame 1), 5 grabbing (frame 2), 6 carrying,
								 7 sliding. Used by Player_Animation(). */
extern int   kickedObject;  /*!< Index of the object currently being kicked, or -1 if none. */
extern CP_Vector position;  /*!< Player position, the TOP-LEFT corner of the player. */
extern CP_Vector velocity;  /*!< Player velocity, in pixels per second. */

/*!
@brief  Per-frame movement for a player who is not carrying anything.
		Handles picking up an object (E), kicking an object (F), moving
		left and right (A/D), jumping (W or Space), gravity (S falls
		faster), snapping to the ground, and keeping the player inside
		the screen horizontally. Horizontal movement is locked during
		the pick-up animation.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Character_MovementDefault(void);

/*!
@brief  Per-frame movement for a player who is carrying an object.
		Handles dropping the object (E, grounded only), moving, jumping,
		gravity, ground snapping and screen limits, and keeps the held
		object positioned above the player's head. Horizontal movement
		is locked during the drop animation.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Character_MovementCarry(void);

/*!
@brief  Draws a grey platform and resolves the player's collision with it
		by calling collision_check().

@param  x             X coordinate of the platform's centre.
@param  y             Y coordinate of the platform's centre.
@param  w             Width of the platform.
@param  h             Height of the platform.
@param  degrees       Rotation of the drawn rectangle, in degrees.
@param  cornerRadius  Corner rounding of the drawn rectangle.

@return None.
*//*_______________________________________________________________________*/
void DrawPlatform(float x, float y, float w, float h, float degrees, float cornerRadius);


/*!
@brief  Tests whether a circle touches an axis-aligned rectangle.
		Finds the point on the rectangle closest to the circle's centre
		and compares its squared distance with the squared radius.
		Touching counts as an intersection. In collision.h

@param  cx      X coordinate of the circle's centre.
@param  cy      Y coordinate of the circle's centre.
@param  radius  Radius of the circle.
@param  x       X coordinate of the rectangle's top-left corner.
@param  y       Y coordinate of the rectangle's top-left corner.
@param  w       Width of the rectangle.
@param  h       Height of the rectangle.

@return 1 if the circle and rectangle intersect, otherwise 0.
*//*_______________________________________________________________________*/
int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h);

/*!
@brief  Collision between the player and a solid rectangle. If they
		overlap, the player is pushed out along the axis of least
		overlap. Landing on top sets onGround and stops falling.
		Touching a side starts or continues a wall cling, which holds
		the player against the wall for WALL_CLING_TIME seconds and lets
		them climb with W/S. With wall climb on, being within 2 pixels
		of a side also counts as touching. In collision.h

@param  area_center_x  X coordinate of the rectangle's centre.
@param  area_center_y  Y coordinate of the rectangle's centre.
@param  area_width     Width of the rectangle.
@param  area_height    Height of the rectangle.

@return 0 if not touching, 1 if the player landed on top, 2 if the player
		hit the underside, 3 if the player touched the rectangle's left
		side, 4 if the player touched its right side.
*//*_______________________________________________________________________*/
int collision_check(float area_center_x, float area_center_y, float area_width, float area_height);



/*!
@brief  Draws the player's sprite at position, chosen by player_action
		(standing, running, jumping, grabbing, carrying), scaled to
		PLAYER_SIZE x PLAYER_SIZE. In player_animation.h

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Player_Animation(void);



// level_mechanics.h
/*!
@brief  Checks whether the player overlaps a goal rectangle.

@param  x             X coordinate of the rectangle's top-left corner.
@param  y             Y coordinate of the rectangle's top-left corner.
@param  w             Width of the rectangle.
@param  h             Height of the rectangle.
@param  degrees       Unused.
@param  cornerRadius  Unused.

@return 1 if the player overlaps the rectangle, otherwise 0.
*//*_______________________________________________________________________*/
int CheckWinCondition(float x, float y, float w, float h, float degrees, float cornerRadius);

/*!
@brief  Moves the player to the spawn point of the given level.

@param  arr             Array of spawn points, indexed by level.
@param  level_selected  Index of the level whose spawn point is used.

@return None.
*//*_______________________________________________________________________*/
void Player_Starting_Pos(spawn arr[], int level_selected);


// =====================================
// PRESSURE PLATES AND HAZARDS
// =====================================
extern int unlocked_gate_score; /*!< Flag so the gate-unlock score is only awarded once per level. */
extern int pressed;             /*!< 1 if a pressure plate was pressed by the player or an object
									 on the last call to a pressure plate function. */


/*!
@brief  Draws a pressure plate and a door. The plate counts as pressed
        when the player or any object overlaps it. While pressed, the
        plate is drawn medium green and the door disappears. While not
        pressed, the plate is light green and the door is drawn solid
        and collides with the player. Updates the global pressed.

@param  pres_x       X coordinate of the plate's centre.
@param  pres_y       Y coordinate of the plate's centre.
@param  pres_width   Width of the plate.
@param  pres_height  Height of the plate.
@param  door_x       X coordinate of the door's centre.
@param  door_y       Y coordinate of the door's centre.
@param  door_width   Width of the door.
@param  door_height  Height of the door.

@return None.
*//*_______________________________________________________________________*/
void Pressure_Plate_Door(float pres_x, float pres_y, float pres_width, float pres_height,
					     float door_x, float door_y, float door_width, float door_height);

/*!
@brief  Like Pressure_Plate_Door(), but with two doors that swap. door1
		is solid while the plate is not pressed, and door2 is solid
		while it is pressed. Updates the global pressed.

@param  pres_x, pres_y          Centre of the plate.
@param  pres_width, pres_height Size of the plate.
@param  door1_x, door1_y        Centre of door 1 (solid when not pressed).
@param  door1_width, door1_height Size of door 1.
@param  door2_x, door2_y        Centre of door 2 (solid when pressed).
@param  door2_width, door2_height Size of door 2.

@return None.
*//*_______________________________________________________________________*/
void Pressure_Plate_2Door(float pres_x, float pres_y, float pres_width, float pres_height,
	float door1_x, float door1_y, float door1_width, float door1_height,
	float door2_x, float door2_y, float door2_width, float door2_height);

/*!
@brief  Draws a red kill zone. If the player overlaps it, the game
		switches to the lose screen.

@param  x  X coordinate of the zone's centre.
@param  y  Y coordinate of the zone's centre.
@param  w  Width of the zone.
@param  h  Height of the zone.

@return None.
*//*_______________________________________________________________________*/
void kill_zone(float x, float y, float w, float h);

// =====================================
// MOVING PLATFORMS AND KILL ZONES
// =====================================
extern float platform_velocity; /*!< Horizontal speed of the moving platform, in pixels per second.
                                     Flips between -100 and 100 at the boundaries. */
extern float platform_move;     /*!< Current horizontal offset of the moving platform from its start x. */

/*!
@brief  Draws a platform that slides left and right at 100 pixels per
		second between two limits, reversing at each one. The platform
		is drawn and collided with through DrawPlatform().

@param  x               X coordinate of the platform's starting centre.
@param  y               Y coordinate of the platform's centre.
@param  w               Width of the platform.
@param  h               Height of the platform.
@param  degrees         Rotation of the drawn rectangle, in degrees.
@param  cornerRadius    Corner rounding of the drawn rectangle.
@param  left_boundary   Leftmost x the platform's centre can reach.
@param  right_boundary  Rightmost x the platform's centre can reach.

@return None.
*//*_______________________________________________________________________*/
void platform_alternating_movement(float x, float y, float w, float h,
	float degrees, float cornerRadius,
	float left_boundary, float right_boundary);

/*!
@brief  Draws a kill zone that slides left and right at 100 pixels per
		second between two limits, reversing at each one. The zone is
		drawn and collision-checked through kill_zone().

@param  x               X coordinate of the zone's starting centre.
@param  y               Y coordinate of the zone's centre.
@param  w               Width of the zone.
@param  h               Height of the zone.
@param  left_boundary   Leftmost x the zone's centre can reach.
@param  right_boundary  Rightmost x the zone's centre can reach.

@return None.
*//*_______________________________________________________________________*/
void kill_zone_alternating_movement(float x, float y, float w, float h, float left_boundary, float right_boundary);
// ------------------------------------------------



