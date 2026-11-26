/*!
@file     scoring.h
@author   Sin Wee
@module   csd1401f26 Software Engineering Project 1
@section  B
@date     10/11/26
@brief    Declarations for the scoring system: per-level and cumulative
          scores, bonus awards (reflecting projectiles, killing the NPC,
          opening a gate, teleporting, finishing quickly), treasure
          collection, and saving scores per level.
*//*______________________________________________________________________*/
#pragma once

/* ===== Score variables ===== */
extern int level_score;     /*!< Points earned in the current level only. Reset whenever a level
								 starts or restarts. Added to player_score when the level is completed. */
extern int player_score;    /*!< Total score across all completed levels. */

/* ===== Award flags ===== */
/* Each flag is 0 while its award can still be given and 1 once it has been
   given, so that a bonus is only awarded once. */
extern int reflect_score;           /*!< 1 once the 100-point projectile reflect bonus has been awarded. */
extern int npc_score;               /*!< 1 once the 300-point NPC kill bonus has been awarded. */
extern int timer_bonus_awarded;     /*!< 1 once the completion time bonus has been awarded. */
extern int level_score_locked;      /*!< 1 after the level is completed. Stops any further score
										 changes for that level. */

/*!
@brief  Stores the points earned in each level, so they can be shown
		later (for example on the leaderboard). Filled in by
		save_lvl_score().
*//*_______________________________________________________________________*/
extern struct LevelScore {
	int level1_score;
	int level2_score;
	int level3_score;
	int level4_score;
} Level_Score;

/*!
@brief  Awards the one-time bonuses for the current level. Does nothing if
        level_score_locked is 1. Awards:
          - 100 points when a projectile has been reflected
            (NPC_reflected)
          - 300 points when the NPC has been killed (NPC_ded)
          - 750 points when a pressure plate has been pressed (pressed)
        Each bonus is only given once, guarded by its flag. Called every
        frame from Objective_Text() and once more on level completion.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void game_score(void);

/*!
@brief  Awards the completion time bonus based on Seconds: 1000 points
        under 25 seconds, 500 points under 35 seconds, nothing otherwise.
        Only awarded once per level, guarded by timer_bonus_awarded.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void time_score(void);

/*!
@brief  Resets all scoring: the current level's score and flags,
        player_score, every entry of the completed-level scores, and
        Level_Score. Used when starting a new game.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void score_reset(void);

/*!
@brief  Resets only the current level's score and flags: level_score,
        the reflect, NPC, gate, teleport and time bonus flags, the
        score lock, and all collected treasures. player_score is not
        changed. Call when a level starts or restarts.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void level_score_reset(void);

/*!
@brief  Finalises the score when the player completes a level. Does
        nothing if the level is already locked, so calling it more
        than once cannot add the score twice. Otherwise it awards any
        pending bonuses with game_score() and time_score(), records the
        level's score, adds level_score to player_score, and locks the
        level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void score_complete_level(void);

/*!
@brief  Awards 200 points for using a teleport. Does nothing once the
        level is locked, so teleporting after winning does not change
        the score. Called by Teleport_Activated().

@param  None.

@return None.
*//*_______________________________________________________________________*/
void score_teleport_used(void);

/*!
@brief  Saves a score into Level_Score for the current level_selected.
        Only levels 1 to 4 are saved, any other level is ignored.

@param  score  The score to save.

@return None.
*//*_______________________________________________________________________*/
void save_lvl_score(int score);

/*!
@brief  Called when the player loses a level. Unless the level is
        locked, resets the level's score and flags and saves a score of
        0 for the level.

@param  None.

@return None.
*//*_______________________________________________________________________*/
void score_abandoned_level(void);

/*!
@brief  Removes a level's earlier contribution from player_score, for
        example when a completed level is replayed. Subtracts the score
        recorded for that level (never going below 0), clears the
        record, and resets the current level's score and flags.
        Does nothing for an invalid level index.

@param  level  Index of the level to remove, from level_1 up to but not
               including LEVELS. main_menu (0) is rejected.

@return None.
*//*_______________________________________________________________________*/
void score_remove_level(int level);

/*!
@brief  Sizes of the character buffers used to format numbers as text.
*//*_______________________________________________________________________*/
enum BUFFER_SIZE {
    TIME_BUF_SIZE = 16,     /*!< Buffer size for the timer text. */
    SCORE_BUF_SIZE = 16     /*!< Buffer size for the score text. */
};

/*********Treasure Related************/
#define MAX_TREASURES 2     /*!< Maximum number of treasures per level. */

extern int Trea_score;                          /*!< Treasure score. Reset on level restart. */
extern int Trea;                                /*!< Treasure counter. Not used in the code. */
extern int treasure_collected[MAX_TREASURES];   /*!< 1 for each treasure that has been collected this level,
                                                     indexed by treasure_id. Cleared by level_score_reset(). */
/*!
@brief  Draws a treasure and checks whether the player has collected it.
        The treasure is a 20 x 20 orange square centred on (x, y).
        A collected treasure is no longer drawn and cannot be collected
        again. It does not add any points itself. The caller awards the
        points (level.c adds 500) when it returns 1.

@param  x            X coordinate of the treasure's centre.
@param  y            Y coordinate of the treasure's centre.
@param  treasure_id  Index of this treasure, from 0 to MAX_TREASURES - 1.

@return 1 on the frame the player collects the treasure, otherwise 0.
*//*_______________________________________________________________________*/
int Treasure(float x, float y, int treasure_id);

