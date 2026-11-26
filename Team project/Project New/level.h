/*!
@file     level.h
@author   Sin Wee
@co-author Lewin Yeo
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     19/09/26
@brief    Declarations for the level system: level selection and layouts,
          spawn points, win conditions, the level timer, objective text,
          and the interactable objects (desks and printers) that fall,
          get carried and get kicked around.
*//*______________________________________________________________________*/
#pragma once

/***************LEVEL****************/
extern float killZoneX; /*!< Current x position of the advancing kill zone in level 4.
							 Game_Reset() sets it back to 0. */

/*!
@brief  Describes the exit (goal) rectangle of a level.
        Used with DrawExit() and CheckWinCondition().
        Note that DrawExit() treats x and y as the rectangle's centre,
        but CheckWinCondition() treats them as its top-left corner.
*//*_______________________________________________________________________*/
typedef struct {
	float x;
	float y;
	float w;
	float h;
	float degrees;
	float cornerRadius;
} win_condition;

/*!
@brief  Identifiers for the levels. Also used as indices into
		player_spawns and NPC_1_spawns.
*//*_______________________________________________________________________*/
typedef enum {
	main_menu,	/*!< 0: main menu. Its spawn entry is a placeholder. */
	level_1,	/*!< 1: getting into the building. */
	level_2,	/*!< 2: avoiding colleagues. */
	level_3,	/*!< 3: the manufacturing floor. */
	level_4,	/*!< 4: advancing kill zone level. */
	boss_level,	/*!< 5: the boss level (elevator climb). */
} levels;

/*! Number of entries in the level enum, and the size of the spawn arrays.
	A compile-time constant. */
enum { LEVELS = 6 };

extern float PauseingameTime;   /*!< Time accumulated before the last pause, in seconds. Added
									 back in by ingameTimer() so the timer resumes after a pause. */

extern levels level_selected;   /*!< The level currently being played. */

/*!
@brief  Runs the level that matches level_no: calls Main_Menu_Update() for
		main_menu, level_1st() to level_4th() for levels 1 to 4, and
		Elevator_Update() for boss_level.

@param  level_no  The level to run, a value from the levels enum.

@return None.
*//*_______________________________________________________________________*/
void level_selection(int level_no);

/*****************BACKGROUND DRAWING FUNCTION****************/
extern float PosX;      /*!< X position at which backgrounds are drawn. Always 0. */
extern float PosY;      /*!< Y position at which backgrounds are drawn. Always 0. */
extern float alpha;     /*!< Opacity used when drawing backgrounds (0 to 255). */

/*!
@brief  Draws a background image from its top-left corner.
		Sets the image mode to CP_POSITION_CORNER.

@param  BG      The background image to draw.
@param  PosX    X coordinate of the image's top-left corner.
@param  PosY    Y coordinate of the image's top-left corner.
@param  width   Width to draw the image at.
@param  height  Height to draw the image at.
@param  alpha   Opacity, from 0 (invisible) to 255 (opaque).

@return None.
*//*_______________________________________________________________________*/
void DrawBG(CP_Image BG, float PosX, float PosY, float width, float height, float alpha);

/*****************EXIT DRAWING FUNCTION****************/
/*!
@brief  Draws a level's exit as a medium light green rectangle.

@param  x             X coordinate of the rectangle's centre.
@param  y             Y coordinate of the rectangle's centre.
@param  w             Width of the rectangle.
@param  h             Height of the rectangle.
@param  degrees       Rotation of the rectangle, in degrees.
@param  cornerRadius  Corner rounding of the rectangle.

@return None.
*//*_______________________________________________________________________*/
void DrawExit(float x, float y, float w, float h, float degrees, float cornerRadius);

/*!
@brief  Runs level 1. Draws the platforms and background, the two
		treasures (500 points each), and two alternating kill walls, then
		draws the exit. On reaching the exit it scores the level and
		switches to the win screen.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void level_1st(void);

/*!
@brief  Runs level 2. Draws the platforms and background, the kill floor,
		the two-door pressure plate and the two treasures (500 points
		each), then draws the exit. On reaching the exit it scores the
		level and switches to the win screen.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void level_2nd(void);

/*!
@brief  Runs level 3. Draws the platforms and background, the two
		treasures (500 points each), the moving platform between the 2nd
		and 3rd buildings, the pressure plate and door, and three kill
		zones, then draws the exit. On reaching the exit it scores the
		level and switches to the win screen.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void level_3rd(void);

/*!
@brief  Runs level 4. Advances a kill zone from the left at 50 pixels per
		second (stopping at its end point), draws the platforms, then
		draws the exit. On reaching the exit it scores the level, sets
		level_selected to boss_level and switches to the elevator
		(boss) level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void level_4th(void);

/*******************Level Timer Variables************/
extern int   TimeSet;           /*!< 0 until the timer has been started for the current level, then 1. */
extern int   PauseTimeStatus;   /*!< Set to 1 when the game is paused so ingameTimer() stores the elapsed time. */
extern int   ResetTime;         /*!< Set to 1 when a level restarts so ingameTimer() clears the paused time. */
extern float WinScreenTime;     /*!< The time shown on the win screen. Updated every frame by ingameTimer(). */
extern float Seconds;           /*!< Elapsed time in the current level, in seconds. */

/*!
@brief  Stores the completion time of each level, shown on the leaderboard.
*//*_______________________________________________________________________*/
extern struct LevelTimer {
	float level1_time;  /*!< Time spent in level 1, in seconds. */
	float level2_time;  /*!< Time spent in level 2, in seconds. */
	float level3_time;  /*!< Time spent in level 3, in seconds. */
	float level4_time;  /*!< Time spent in level 4 (and the boss level), in seconds. */
}Level_Timer;

/*!
@brief  Saves a time into Level_Timer for the current level_selected.
		All levels from levels 1 to 3 is saved using a switch case that 
		checks the current level_selected value stored level4/boss-level is 
		the default as we are assuming that the bos level will always be the 
		last level.
		Once the level_selected value is checked it saves the time tracked taken 
		on the cuurent level, until the player has reached the end of the level.

@param  time  The time to save, in seconds.

@return None.
*//*_______________________________________________________________________*/
void SaveTime(float time);

/*!
@brief  Updates and draws the in-game timer. On the first call it records
		the start time. After that it computes the elapsed time, saves it
		with SaveTime(), draws it at the top centre of the screen, and
		draws the star rating. 
		The first star always shows. 
		The second shows under 35 seconds (90 in the boss level)
		and the third under 25 seconds (70 in the boss level). 
		It also handles the pause and reset flags (PauseTimeStatus, ResetTime).
		Where these flags indicate if the player has done either of these actions:
		-Pause the game and entered into the mainmenu
		-Entered a new level or restart the current level

@param  None.

@return The elapsed time in seconds.
*//*_______________________________________________________________________*/
float ingameTimer(void);

/*!
@brief  Draws the objective text for the current level at the top-left of
		the screen, followed by the current score.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Objective_Text(void);



/*****************Spawn Locations for Player********************/

/*!
@brief  A spawn point, stored as a position.
		For the player, pos is the top-left corner of the player.
*//*_______________________________________________________________________*/
typedef struct{
	CP_Vector pos;
} spawn;

extern spawn player_spawns[6]; /*!< Player spawn point for each level, indexed by the levels enum.
                                     Entry 0 (main menu) is a placeholder. */

/*****************Spawn Locations for NPCs********************/
extern spawn NPC_1_spawns[6]; /*!< NPC spawn point for each level, indexed by the levels enum.
                                     Entry 0 (main menu) is a placeholder. */


/*********************Object Related*****************/
/* Each object is one row of 5 floats: { x, y, w, h, type }.
   The "start" arrays are const and hold the layout. The "ingame" arrays are
   copies that carrying, dropping, kicking and falling change. Objects_Reset()
   copies the start array into the ingame one. */

extern const float first_level_objects[2][5];       /*!< Starting layout of level 1's objects (const). */
extern float first_level_objects_ingame[2][5];      /*!< Level 1's objects as they are during play. */

extern const float second_level_objects[2][5];      /*!< Starting layout of level 2's objects (const). */
extern float second_level_objects_ingame[2][5];     /*!< Level 2's objects as they are during play. */

extern const float third_level_objects[2][5];       /*!< Starting layout of level 3's objects (const). */
extern float third_level_objects_ingame[2][5];      /*!< Level 3's objects as they are during play. */


/*! Column indices of an object row: { x, y, w, h, type }.
	x and y are the object's top-left corner. */
enum { OBJ_X, OBJ_Y, OBJ_W, OBJ_H, OBJ_TYPE };

/*! Column indices of a platform row: { x, y, w, h, degrees, cornerRadius }.
	x and y are the platform's centre. Only the first four columns have names. */
enum { PLAT_X, PLAT_Y, PLAT_W, PLAT_H };

/*! Row indices of level 1's objects, in the same order as first_level_objects. */
enum { DESK, PRINTER };

extern float (*current_objects)[5]; /*!< Points to the in-game object array of the level being played,
										 or NULL if the level has no objects. */
extern int   current_obj_count;     /*!< Number of objects in current_objects. */
extern int   heldObject;            /*!< Index of the object being carried, or -1 if none. */

/*!
@brief  Copies the current level's starting object layout into its
		in-game array and points current_objects at it. Clears the held
		and kicked objects. Levels without objects leave current_objects
		as NULL with a count of 0. Call when a level starts or restarts.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Objects_Reset(void);

/*!
@brief  Checks whether the player is touching an object. The player is
		treated as a circle of radius PLAYER_SIZE / 2 around their centre.
		The held object is skipped.

@param  None.

@return Index of the first object touched, or -1 if none.
*//*_______________________________________________________________________*/
int Object_Check(void);

/*!
@brief  Updates and draws all objects. Every object that is not being
		carried falls at 400 pixels per second until it lands on a
		platform or the bottom of the screen. Each object is drawn with
		a highlight behind it that only shows when the player is touching
		it and can pick it up.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void Objects_Draw(void);

/*!
@brief  Checks a circle against the level's platforms, the other objects
		and the bottom of the screen. Used to test where a thrown or
		moved object would collide.

@param  cx      X coordinate of the circle's centre.
@param  cy      Y coordinate of the circle's centre.
@param  radius  Radius of the circle.
@param  self    Index of the object to ignore, so it does not collide with
				itself. Pass -1 to ignore none.

@return 0 if nothing is hit, 1 if a platform is hit, 2 if another object
		is hit, 3 if the bottom of the screen is hit.
*//*_______________________________________________________________________*/
int Object_HitCheck(float cx, float cy, float radius, int self);




