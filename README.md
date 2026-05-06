# Rogue TUI

## You don't need to read the whole manual, just get comfortable with a handful of functions. Focus on:

- [*] initscr(), endwin() — start and tear down the terminal
- [*] noecho(), cbreak() — so keypresses aren't echoed and you get them instantly without Enter
- [*] keypad(stdscr, TRUE) — enables arrow keys if you want them later
- [*] mvprintw(y, x, "...") — move cursor and print (note: y comes first, always)
- [*] getch() — blocking read of a single keypress
- [*] clear() / refresh() — clear the screen and flush your drawn frame

That's basically the whole API for what you're building. The ncurses HOWTO at tldp.org is the best beginner reference, ignore anything about panels/menus/forms for now.

## Understand the viewport vs. the world
This is the central design decision for your milestone. You have two coordinate systems:

World coordinates — where things actually are in the full map (could be 500×500)
Screen coordinates — what's currently visible in the terminal window

You'll need a camera offset (call it cam_x, cam_y) and every render loop you draw world[cam_y + screen_y][cam_x + screen_x]. When the player gets close to an edge, you shift the camera. Get COLS and LINES from ncurses to know your terminal dimensions.

## Procedural map generation — start with cellular automata
It sounds fancy but it's simple and produces great cave-like maps. The algorithm:

Fill a 2D array randomly with walls/floor based on a probability (e.g. 45% wall)
Run several "smoothing" passes: a cell becomes a wall if more than 4 of its 8 neighbours are walls, otherwise floor
Repeat 4–5 times

The result looks like natural caves and it's only ~30 lines of C. Your map is just a char ** or a flat char * array you index with map[y * width + x] — good malloc practice.

## Your first-milestone checklist

- [*] Get ncurses rendering a static @ on screen
- [*] Read h/j/k/l keypresses and move @ around
- [*] Allocate a world map (2D array on the heap) and fill it with walls (#) and floors (.)
- [*] Render only the viewport portion of the map each frame
- [*] Implement camera scrolling — camera follows the player as they approach edges
- [*] Add cellular automata generation so the map looks like a cave
- [*] Add collision detection so the player can't walk through walls
- [*] Graceful quit on q with endwin() cleanup


### Practical tips

Use a fixed map size to start (e.g. 200×200) rather than infinite generation — get movement and viewport working first, then make maps bigger or chunked later
Your game loop is just 
```c
while (!quit) { 
    handle_input();
    render(); 
    }
```

#### SIGWINCH is the signal for terminal resize; you can ignore it for now but know it exists
Compile with -lncurses e.g. gcc main.c -lncurses -o rogue
