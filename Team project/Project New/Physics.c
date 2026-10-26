#include "Physics.h"
#include "PlayerMechanics.h"
#include "Player's_Elements.h"

wall_climb_status = 1;

int collision_check(float area_center_x, float area_center_y, float area_width, float area_height)
{
	float dt = CP_System_GetDt();

	// Standing on the floor or a platform -> cling time refills

	float area_left = area_center_x - area_width / 2;
	float area_right = area_center_x + area_width / 2;
	float area_top = area_center_y - area_height / 2;
	float area_bottom = area_center_y + area_height / 2;

	// Player edges (position is the player's top-left corner)
	float player_left = position.x;
	float player_right = position.x + PLAYER_SIZE;
	float player_top = position.y;
	float player_bottom = position.y + PLAYER_SIZE;

	// How far the player has gone into the platform from each side
	float overlap_top = player_bottom - area_top;
	float overlap_bottom = area_bottom - player_top;
	float overlap_left = player_right - area_left;
	float overlap_right = area_right - player_left;
	int side;

	// Not touching the platform
	if (player_right <= area_left || player_left >= area_right || player_bottom < area_top || player_top >= area_bottom)
	{
		return 0;
	}


	// TOP: stop falling through, platform becomes the new ground
	if (overlap_top <= overlap_bottom && overlap_top <= overlap_left && overlap_top <= overlap_right)
	{
		position.y = area_top - PLAYER_SIZE;
		if (velocity.y > 0) velocity.y = 0.0f;
		onGround_status = 1;
		return 1;
	}
	// BOTTOM: stop phasing up through the platform
	else if (overlap_bottom <= overlap_left && overlap_bottom <= overlap_right)
	{
		position.y = area_bottom;
		if (velocity.y < 0) velocity.y = 0.0f;
		return 2;
	}
	// LEFT SIDE: stop moving right into the platform
	else if (overlap_left <= overlap_right)
	{
		position.x = area_left - PLAYER_SIZE;
		side = 3;
	}
	// RIGHT SIDE: stop moving left into the platform
	else
	{
		position.x = area_right;
		side = 4;
	}

	Wall_Climb(area_center_x, area_center_y, area_width, area_height, wall_climb_status);
	//Return Player Y position so that the Gravity is updated 
	// *Note* the Wall climb mechanic is interacting differently for some reason
	return side;
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
