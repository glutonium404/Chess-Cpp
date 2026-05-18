# Chess-Cpp

A graphical chess implementation in modern C++ using SFML, with full interactive play on a local board and core rule handling (legal move generation, check/checkmate/stalemate, castling, en passant, promotion, threefold repetition, and 50-move rule).

---

## Table of Contents

- [Overview](#overview)
- [Tech Stack](#tech-stack)
- [Project Structure](#project-structure)
- [Build and Run](#build-and-run)
- [How the Application Runs (Sequence)](#how-the-application-runs-sequence)
- [Core Data Model](#core-data-model)
- [Move Generation and Rule Enforcement](#move-generation-and-rule-enforcement)
- [Special Rules](#special-rules)
- [Game State and Draw Detection](#game-state-and-draw-detection)
- [Rendering and UI Flow](#rendering-and-ui-flow)
- [Limitations / Current Scope](#limitations--current-scope)
- [Contributor Guide](#contributor-guide)

---

## Overview

This repository contains a complete playable chess board application with:

- 8×8 board rendering and sprite-based pieces
- Mouse-based piece selection and move execution
- Turn management (White/Black alternation)
- Legal move filtering under check/pin constraints
- Check highlighting on kings
- Endgame detection:
  - Checkmate
  - Stalemate
  - Threefold repetition
  - 50-move rule
- Promotion selection overlay

The code is structured around a central `Board` class that coordinates state, user interaction, move application, legality updates, and rendering.

---

## Tech Stack

- **Language:** C++17
- **Graphics/UI:** SFML (`graphics`, `window`, `system`)
- **Build tool:** GNU Make (`Makefile`)

---

## Project Structure

```text
.
├── main.cpp
├── Makefile
├── assets/
│   ├── images/               # Piece PNG textures (white/black)
│   └── roboto.ttf            # Font used for game-over overlay
└── includes/
    ├── Board/
    │   ├── Board.hpp         # Central game state + orchestration API
    │   ├── Board.cpp         # Event handling, move execution, game-state updates
    │   └── initialise.cpp    # Board/piece initialization, factory helpers
    ├── Coordinate/
    │   ├── Coordinate.hpp
    │   └── Coordinate.cpp
    ├── Square/
    │   ├── Square.hpp
    │   └── Square.cpp
    ├── Piece/
    │   ├── Piece.hpp         # Abstract base piece + common movement logic
    │   └── Piece.cpp
    └── Pieces/
        ├── King/
        ├── Queen/
        ├── Rook/
        ├── Bishop/
        ├── Knight/
        └── Pawn/
```

---

## Build and Run

### Prerequisites

1. C++ compiler with C++17 support (`g++`)
2. SFML development libraries/headers installed (graphics/window/system)

### Build

From repository root:

```bash
make
```

This compiles all `.cpp` files under `includes/` and links `chess_game`.

### Run

```bash
./chess_game
```

### Clean

```bash
make clean
```

### Notes

- Current Makefile links with:
  - `-lsfml-graphics -lsfml-window -lsfml-system`
- Several source files include `<my_utils.hpp>`; ensure your environment provides it (or remove/replace if not required in your setup).

---

## How the Application Runs (Sequence)

### 1) Program entry (`main.cpp`)

1. Create `sf::RenderWindow`.
2. Construct `Board board(450, window)`.
3. Enter main loop:
   - Poll window events
   - Forward each event to `board.handle_event(event)`
   - Clear window
   - `board.draw()`
   - Display frame

### 2) Board construction (`Board::Board`)

Initialization order:

1. Reserve piece containers.
2. Compute square size from requested board width.
3. Set board bounds centered in window.
4. Build 8×8 `Square` grid with alternating colors.
5. Create all pieces (including kings at end of vectors).
6. Place pieces into squares.
7. Prepare promotion selection squares.
8. Initialize Zobrist lookup tables.
9. Compute initial legal moves.

### 3) Runtime input handling (`Board::handle_event` / `Board::handle_click`)

On left click inside board:

1. Convert mouse pixel to board coordinate.
2. If promotion overlay is active, process promotion choice first.
3. Otherwise branch by clicked square type:
   - Empty & non-highlighted → clear selection/highlights
   - Highlighted legal target → execute move
   - Own piece → select and highlight legal moves
   - Opponent piece (not legal target) → clear selection
4. After actions, maintain check highlights.

### 4) After a legal move

Core sequence (non-promotion path):

1. Move is applied (`make_move`)
2. Turn toggles
3. Zobrist hash recalculated
4. Derived state reset (`reset_variables`)
5. Pinned pieces recomputed (`update_pinned_pieces`)
6. Legal moves recomputed (`update_legal_moves`)
7. Game end/draw checked (`check_game_state`)

Promotion path defers turn switch + recalculations until promotion piece is selected.

---

## Core Data Model

### `Coordinate`

- 1-based indexing (`row`, `col` in `[1..8]`)
- Arithmetic helpers (`+`, `-`, scalar multiply, step direction)
- Bounds checking and index conversion (`to_index`) for Zobrist hashing

### `Square`

Represents one board cell:

- Visual shape + base type (light/dark)
- Optional occupying `piece`
- Control flags:
  - `is_controlled_by_white`
  - `is_controlled_by_black`
- Highlight states used for legal move UI and check UI

### `Piece` (abstract base)

Holds common piece data and behavior:

- Render sprite/texture and board position
- `legal_moves` cache
- `attacked_by` list (which enemy pieces currently attack this piece)
- Pin metadata (`is_pinned`, `pinned_dir`)
- Move state (`has_moved`)
- Generic directional move logic for sliding pieces
- Check-response filtering logic (`get_check_elimination_moves`)

### `Board`

Owns global game state:

- 8×8 squares matrix
- Piece lists for each side (`w_pieces`, `b_pieces`)
- Turn (`current_turn`)
- Selection and highlighted coordinates
- Castling rights bitmask
- En passant target info
- Half-move counter
- Zobrist hash tables + repetition map
- Promotion overlay state
- Game state enum-like int (`ongoing`, `white wins`, `black wins`, `draw`)

---

## Move Generation and Rule Enforcement

## High-level strategy

The engine tracks:

- Controlled squares (per side)
- Current attackers on each king (`king->attacked_by`)
- Piece pin constraints

Because legal moves depend on king-check status, move updates run in two batches (`Board::update_legal_moves`):

1. Opponent pieces first (to populate attacks on current player’s king)
2. Current player pieces next (using up-to-date check context)

## Base piece logic (`Piece::set_legal_moves`)

For sliding pieces (queen, rook, bishop) and partially reused by others:

1. Iterate movement directions
2. Mark controlled squares
3. Stop traversal at blockers
4. Add legal move if square is empty or enemy occupied
5. Enforce pin direction restrictions
6. If king is in single-check, reduce moves to those that block/capture attacker
7. If double-check, non-king pieces receive no legal moves

## Piece-specific behavior

### King

- Evaluates 8 adjacent squares
- Rejects squares controlled by enemy side
- Additional castling checks:
  - king has not moved
  - king not currently in check
  - path squares empty
  - transit squares not enemy-controlled
  - rook exists, same color, and has not moved

### Queen / Rook / Bishop

- Use directional vectors and common sliding logic from base class

### Knight

- Evaluates 8 L-shaped targets
- If pinned, contributes control but cannot legally move
- Under single-check, filtered to valid check-eliminating moves

### Pawn

Implements:

- Forward single move (if empty)
- Initial double move (if both squares empty and `!has_moved`)
- Diagonal captures
- En passant capture
- Pin-aware restrictions:
  - Horizontal/diagonal pin can block forward movement
  - Vertical pin blocks diagonal capture
  - Diagonal pin allows only capture along pin direction

---

## Special Rules

### Castling

- Rights tracked in `Board::castling_right` bitmask (`WK`, `WQ`, `BK`, `BQ`)
- Rights are removed when:
  - king moves
  - relevant rook moves
  - relevant rook is captured before moving
- King move by two files triggers rook relocation in `make_move`

### En Passant

- Stored as `(en_passant_row, en_passant_file)`
- Set only after qualifying pawn double-step
- Cleared on subsequent move updates
- Capture logic removes adjacent pawn from original square

### Promotion

- Triggered when pawn reaches back rank (row 1 or 8)
- Board enters promotion-selection mode with overlay + 4 choices:
  - Queen, Knight, Rook, Bishop
- After selection:
  - Promoted piece inserted into side piece list
  - Pawn marked dead and replaced on promotion square
  - Turn/hash/legal-state update resumes

---

## Game State and Draw Detection

`Board::check_game_state` handles final outcomes:

1. Draw if:
   - Same Zobrist hash occurs 3+ times (`repetition_list[hash] >= 3`)
   - Half-move clock reaches 100 plies (50-move rule)
2. If side has no legal moves:
   - If king attacked → checkmate (opponent wins)
   - Else → stalemate draw

Outcome values:

- `0`: ongoing
- `1`: white won
- `2`: black won
- `3`: draw

---

## Rendering and UI Flow

Each frame (`Board::draw`):

1. Draw all squares (each square draws its living piece if present)
2. If promotion mode active:
   - Draw dimmer overlay
   - Draw promotion option squares and piece sprites
3. If game over:
   - Build message (`WHITE WON!`, `BLACK WON!`, `DRAW!`)
   - Draw dimmer + centered text

Highlight color semantics:

- Light/dark highlight: legal destination
- Red highlight: capture / en passant target
- Check highlight: king in check

---

## Limitations / Current Scope

- No AI engine (human-vs-human local play)
- No PGN/FEN import-export
- No move history UI panel
- No undo/redo
- No explicit test suite in repository currently

---

## Contributor Guide

When extending gameplay logic:

1. Keep `Board` as the source of truth for turn/state transitions.
2. Preserve update sequence after each move:
   - reset derived variables → recompute pins → recompute legal moves → check game state.
3. Ensure both **control squares** and **legal moves** remain correct; many rules depend on both.
4. For new rules/features, update both:
   - Piece-specific movement logic
   - Board-level state transitions (hash, castling/en-passant/half-move/repetition)
5. Keep king vectors at end of `w_pieces` / `b_pieces` unless refactoring `Board::get_king`.

For rendering changes, keep board coordinate assumptions 1-based and aligned with `Coordinate` utilities.

---

If you want, I can also add a developer-focused architecture diagram section (class relationship + move pipeline diagram) in a follow-up update.
