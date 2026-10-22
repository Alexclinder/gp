#include "cprocessing.h"
#include "game.h"

#define PLAYER_SIZE    100.0f
#define MOVE_SPEED     300.0f
#define JUMP_SPEED     650.0f
#define GRAVITY        1500.0f
#define MAX_FALL_SPEED 900.0f
float collision_radius = 50.0f;

int onGround = 0;
CP_Vector position;
CP_Vector velocity;


int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// check if the click is within 
	// x bound of the area(check the x-axis radius from the centre of the rectangle(x coordinate - (width/2) && x coordinate + (width/2)))
	// y bound of the area(check the y-axis radius from the centre of the rectangle(y coordinate - (length/2) && x coordinate + (length/2)))
	if ((click_x >= area_center_x - area_width / 2 && click_x <= area_center_x + area_width / 2) &&
		(click_y >= area_center_y - area_height / 2 && click_y <= area_center_y + area_height / 2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	// check if the click is within 
	// x bound of the area(check the x-axis radius from the centre of the circle(x coordinate - (diameter/2) && x coordinate + (diameter/2)))
	// y bound of the area(check the y-axis radius from the centre of the circle(y coordinate - (diameter/2) && y coordinate + (diameter/2)))
	if ((click_x >= circle_center_x - diameter / 2 && click_x <= circle_center_x + diameter / 2) &&
		(click_y >= circle_center_y - diameter / 2 && click_y <= circle_center_y + diameter / 2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


float Character_MovementDefault_x(void)
{
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- Horizontal: works on the ground AND in mid-air --- */
	velocity.x = 0.0f;
	if (CP_Input_KeyDown(KEY_A)) velocity.x -= MOVE_SPEED;
	if (CP_Input_KeyDown(KEY_D)) velocity.x += MOVE_SPEED;

	/* --- Jump: only when standing on something --- */
	if ((CP_Input_KeyTriggered(KEY_W)) && onGround) {
		velocity.y = -JUMP_SPEED;
		onGround = 0;
	}

	/* --- Gravity: always applied, S makes you fall faster --- */
	velocity.y += GRAVITY * dt;
	if (CP_Input_KeyDown(KEY_S) && !onGround) {
		velocity.y += GRAVITY * dt;
	}
	if (velocity.y > MAX_FALL_SPEED) velocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (position.y + PLAYER_SIZE >= groundY) {
		position.y = groundY - PLAYER_SIZE;
		velocity.y = 0.0f;
		onGround = 1;
	}
	else {
		onGround = 0;
	}

	/* --- Keep player inside the screen horizontally --- */
	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x + PLAYER_SIZE > screenW) position.x = screenW - PLAYER_SIZE;

	/* --- Draw --- */
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(background, 0, 0, screenW, groundY, 255);
	CP_Image_Draw(Zilong, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);

	return position.x;
}

float Character_MovementDefault_y(void)
{
	float dt = CP_System_GetDt();
	float groundY = (float)CP_System_GetWindowHeight();
	float screenW = (float)CP_System_GetWindowWidth();

	/* --- Horizontal: works on the ground AND in mid-air --- */
	velocity.x = 0.0f;
	if (CP_Input_KeyDown(KEY_A)) velocity.x -= MOVE_SPEED;
	if (CP_Input_KeyDown(KEY_D)) velocity.x += MOVE_SPEED;

	/* --- Jump: only when standing on something --- */
	if ((CP_Input_KeyTriggered(KEY_W)) && onGround) {
		velocity.y = -JUMP_SPEED;
		onGround = 0;
	}

	/* --- Gravity: always applied, S makes you fall faster --- */
	velocity.y += GRAVITY * dt;
	if (CP_Input_KeyDown(KEY_S) && !onGround) {
		velocity.y += GRAVITY * dt;
	}
	if (velocity.y > MAX_FALL_SPEED) velocity.y = MAX_FALL_SPEED;

	/* --- Move --- */
	position = CP_Vector_Add(position, CP_Vector_Scale(velocity, dt));

	/* --- Ground check: snap to ground and stop falling --- */
	if (position.y + PLAYER_SIZE >= groundY) {
		position.y = groundY - PLAYER_SIZE;
		velocity.y = 0.0f;
		onGround = 1;
	}
	else {
		onGround = 0;
	}

	/* --- Keep player inside the screen horizontally --- */
	if (position.x < 0.0f) position.x = 0.0f;
	if (position.x + PLAYER_SIZE > screenW) position.x = screenW - PLAYER_SIZE;

	/* --- Draw --- */
	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_ImageMode(CP_POSITION_CORNER);
	CP_Image_Draw(background, 0, 0, screenW, groundY, 255);
	CP_Image_Draw(Zilong, position.x, position.y, PLAYER_SIZE, PLAYER_SIZE, 255);

	return position.y;
}


void Character_MovementCarry(void)
{

}

//Checks if the Player is When the Win Condition Trigger Box
int Win_con(float area_corner_x, float area_corner_y, float area_width, float area_height, float Player_PosX, float Player_PosY, float Player_Size) {
	if (
		(Player_PosX <= area_corner_x + area_width) //Checks if Player's Left X-Coodinates is Less the Win condition Box Right X Coor
		&& (Player_PosY <= area_corner_y + area_height) //Checks if Player's Top Y-Coodinates is above the Win condition Bottom Y Coor
		&& (Player_PosX + Player_Size >= area_corner_x) //Checks if Player's Right X-Coodinates is Greater than the Win condition Left X Coor
		&& (Player_PosY + Player_Size >= area_corner_y) //Checks if Player's Bottom Y-Coodinates is within the Win condition
		)
	{

		return 1;
	}
	else
		return 0;
}

int sphere_aabb_intersection(float cx, float cy, float radius, float x, float y, float w, float h)
{
	// Find the closest point on the platform to the sphere centre
	float closestX = cx;
	float closestY = cy;

	if (closestX < x)
		closestX = x;
	else if (closestX > x + w)
		closestX = x + w;

	if (closestY < y)
		closestY = y;
	else if (closestY > y + h)
		closestY = y + h;

	// Vector from closest point to sphere centre
	CP_Vector offset = CP_Vector_Subtract(
		CP_Vector_Set(cx, cy),
		CP_Vector_Set(closestX, closestY)
	);

	// Compare squared distances
	float distanceSquared =
		offset.x * offset.x + offset.y * offset.y;

	float radiusSquared = radius * radius;

	// Touching counts as intersection
	if (distanceSquared <= radiusSquared) {
		return 1;
	}
	return 0;
}

// Placed the collision detection within draw Platform
void DrawPlatform(float x, float y, float w, float h,
	float degrees, float cornerRadius)
{
	// Draw the platform
	CP_Settings_RectMode(CP_POSITION_CENTER);

	CP_Graphics_DrawRectAdvanced(x + w / 2.0f, y + h / 2.0f, w, h, degrees, cornerRadius);

	float player_center = PLAYER_SIZE / 2.0f;

	// Update the player sprite center
	float cx = position.x + player_center;
	float cy = position.y + player_center;

	// Checking the y of sphere
	float current_y = cy + collision_radius;

	// Check whether the sphere intersects the platform
	int intersect = sphere_aabb_intersection(cx, cy, player_center, x, y, w, h);

	// If player is above the platform 
	if (intersect && current_y >= y)
	{
		// Update position y based on rect_y coords, radius
		position.y = y - player_center * 2;
		velocity.y = 0.0f;
		onGround = 1;
	}
}

// Used to determine the Character's starting position (*Note not in use yet)
void Character_Starting_Pos(float Character_x, float Character_y) {
	position = (CP_Vector){ Character_x, Character_y };
	velocity = (CP_Vector){ 0,0 };
}