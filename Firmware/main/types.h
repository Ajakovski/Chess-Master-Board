#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "config.h"

// color
typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
}rgb_t;

// Pieces
typedef enum {
    PT_NONE   = 0, PT_PAWN   = 1,
    PT_KNIGHT= 2,
    PT_BISHOP= 3,
    PT_ROOK  = 4,
    PT_QUEEN = 5,
    PT_KING  = 6
} piece_type_t;

typedef enum {
    PC_WHITE = 0,
    PC_BLACK = 1
} piece_color_t;

typedef struct {
    piece_type_t  type;
    piece_color_t color;
} piece_t;

// Chess board

/* Castling rights bitmask */
#define CASTLE_WK  (1 << 0)
#define CASTLE_WQ  (1 << 1)
#define CASTLE_BK  (1 << 2)
#define CASTLE_BQ  (1 << 3)

typedef struct {
    piece_t       pieces[64];
    uint8_t       castling;  
    int8_t        en_passant;
    piece_color_t active;    
    uint16_t      halfmove;  
    uint16_t      fullmove;  
} board_t;

/* moving right*/
typedef struct {
    int8_t       from;
    int8_t       to; 
    piece_type_t promo;
} move_t;

#define MOVE_NULL   ((move_t){SQ_NONE, SQ_NONE, PT_NONE})
#define MOVE_IS_NULL(m) ((m).from == SQ_NONE)

// detect move
typedef enum {
    MDS_IDLE,
    MDS_PIECE_LIFTED,
    MDS_CAPTURE_PROGRESS,
    MDS_AWAITING_CONFIRM 
} move_detect_state_t;

// gamephase
typedef enum {
    GP_BOOT,        
    GP_WAIT_POSITION,
    GP_SELECT_SIDE,  
    GP_PLAYING,      
    GP_GAME_OVER,    
    GP_SLEEPING,     
} game_phase_t;

typedef enum {
    SF_SIDE_WHITE = 0,    
    SF_SIDE_BLACK = 1,    
    SF_SIDE_NONE  = 2     
} sf_side_t;

typedef enum {
    GO_NONE = 0,
    GO_CHECKMATE,
    GO_STALEMATE,
    GO_TIME_WHITE,
    GO_TIME_BLACK,
    GO_DRAW_50MOVE,
    GO_DRAW_MATERIAL
} game_over_reason_t;

// button akcii
typedef enum {
    BTN_P1_SHORT,   
    BTN_P2_SHORT,   
    BTN_BOTH_HOLD,   /* Both held*/
} button_event_t;


typedef struct {
    char entries[MAX_HALF_MOVES][6]; 
    int  count;
} move_history_t;

/* MachineSTATUS*/
typedef enum {
    SF_STATUS_IDLE,
    SF_STATUS_THINKING,
    SF_STATUS_HAS_MOVE,
    SF_STATUS_ERROR
} sf_status_t;

// Game stae
typedef struct {
    /* Board and move tracking */
    board_t           board;
    move_history_t    history;
    uint64_t          sensor_occupied;   
    uint64_t          expected_occupied; 

    /* Move detection */
    move_detect_state_t mds;
    int8_t              lifted_sq; 
    int8_t              enemy_lift_sq;   
    move_t              pending_move;    

    /* Clock */
    int32_t  clock_ms[2];     /* Remaining time per player (ms)        */
    bool     clock_running;  

    
    game_phase_t     phase;
    game_over_reason_t over_reason;
    sf_side_t        sf_side;
    int              current_player;   /* 0 = white, 1 = black */

    // outputs
    sf_status_t      sf_status;
    move_t           sf_move;  

    /* LED display buffer*/
    rgb_t            leds[64];

   
    uint8_t          battery_pct;
} game_state_t;
