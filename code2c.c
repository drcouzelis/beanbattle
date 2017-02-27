//bean battle code
//written by david rawson couzelis
//using ALLEGRO
//3/22/00

//code1*.c = my first codes for this game!
//code2*.c = code after i implemented the actual game play!


#include <stdio.h>      //not really used
#include <dos.h>        //maybe used for filenames
#include <time.h>       //for animation timing

#include "allegro.h"    //holds graphics/bitmap functions


#define TILE 16              //general size of tiles
#define ENTER_KEY 7181       //enter key
#define LEFT_KEY 19200       //left arrow
#define RIGHT_KEY 19712      //right arrow
#define INVENTORY_KEY 14624  //space bar
#define ESCAPE_KEY 283       //escape key
#define YES_KEY 5497         //key y
#define NO_KEY 12654         //key n

#define SECONDS 1000         //for the timed functions


typedef struct {
   int grenades;  //int is how many are carried
   int under_cover;
   int mines;
} INVENTORY;


typedef struct {
   int x;
   int y;
} COORDS;  //to hold any singular coord, such as refresh locations



typedef struct  {
   int music_on;
   int sfx_on;
} OPTIONS;  //general game options


typedef struct {
   int a_bean_is_selected;
   int a_move_is_selected;
   int inventory_is_selected;
   int options_is_selected;
   int want_to_quit;
   int turn_is_done;
} FLAGS;  //for input turns


typedef struct {
   int BMPnum;
   int x_loc;
   int y_loc;
} FRAMES;


typedef struct {
   BITMAP *goodbean[10];
   BITMAP *evilbean[10];
   BITMAP *tile1;
   BITMAP *tile2;
   BITMAP *bground3;
   BITMAP *bground4;
} GFX;  //graphics


 
/*** function headers ***/

/*** display functions ***/
void draw_screen (void);    //completely redraws the screen (background, then info bar, then objects)
void draw_info_bar (void);     //initially draws the info bar

/*** game setting functions ***/
void init_game_settings (void);                              //initializes game settings to zero
void load_a_bitmap (char *filename, BITMAP *temp); //loads a bitmap into memory
void load_a_background (char *filename, int *bgroundPTR);    //loads a level's background into memory
void create_bmps_init (void);

/*** animation functions ***/
void bounce_anim (COORDS loc);
void jump_anim (COORDS loc, int x_dir, int y_dir);

/*** input functions ***/
int input (void);                                       //starts the input routine
int input_select (FLAGS *flagsPTR, COORDS *temp);       //select which bean to move, or options or inventory
int input_bean_move (FLAGS *flagsPTR, COORDS *locPTR);  //select where to move selected bean
void inventory_options (void);                          //used to display/select item to use
void check_move (COORDS move);
void check_a_bean (int x, int y);
void animation_init (void);

/*** music functions ***/
void play_song (char *filename);     //plays 'filename.wav'
void music_question_init (void);     //asks if you want game music playing


/*** global use ***/
INVENTORY good_invent, evil_invent;  //hold good/evil items to use
OPTIONS options;                     //important until entire game is quit (to dos)
GFX gfx;                             //the bmps of the game's graphics
SAMPLE *curr_music;                  //holds current playing background music

BITMAP *screen_buffer;  //this is a big deal...  THE screen buffer
BITMAP *animation_buffer;  //for the small animations

int objects[400];     //stores where the actual characters and items are right now
int bground[400];     //stores scenery pictures, like trees and such

FRAMES jump_upFRAMES[8];
FRAMES jump_downFRAMES[8];
FRAMES bounceFRAMES[2];
int frame=0;

int turns;            //how many turns taken
int whose_turn;       //1:player one  2:player two
int x_size;           //curent size of board
int y_size;
int goodbeans_left;
int evilbeans_left;
int x_space;          //current space surrounding board
int y_space;
int game_over;        //is the game over flag
int game_won;         //is the game won flag
int level;            //current level
int quit_dos;         //quit to dos flag
int quit_title;       //quit to title flag


/*** main ***/
int main (void)
{

   int do_proceed;


   allegro_init();                                 //for all the graphics stuff
   install_keyboard();                             //for keyboard input
   set_gfx_mode(GFX_VGA, 320, 200, 0, 0);          //set screen mode
   install_sound(DIGI_AUTODETECT, MIDI_NONE, 0);   //sets the sound card
   create_bmps_init ();
   animation_init ();

   /*** title screen ***/
   //put the title screen here sometime...

   /*** let the game begin!! ***/

   //do    //this 'do' is the entire game 'do'
   //{

      music_question_init ();
      if (options.music_on)
         play_song ("level1d.wav");

      init_game_settings ();
      draw_screen ();

      do   //this 'do' is the start game 'do'
      {
         do_proceed = input ();
         //include extras move option actions in here
      }
      while (!do_proceed);
   /*
   }
   while (!game_over && !game_won);
   */

   if (game_over)
      printf ("GAME OVER!!");

   //eventually make a 'clean_up' functions
   destroy_sample (curr_music);
   readkey();

   /*** this is the end of the game ***/

}


void animation_init (void)
{
   bounceFRAMES[0].BMPnum = 0;
      bounceFRAMES[0].x_loc = 0;
      bounceFRAMES[0].y_loc = 0;
   bounceFRAMES[1].BMPnum = 1;
      bounceFRAMES[1].x_loc = 0;
      bounceFRAMES[1].y_loc = -1;

   //edit these to look like the anim test program
   jump_upFRAMES[0].BMPnum = 0;
      jump_upFRAMES[0].x_loc = 0;
      jump_upFRAMES[0].y_loc = 0;
   jump_upFRAMES[1].BMPnum = 1;
      jump_upFRAMES[1].x_loc = 0;
      jump_upFRAMES[1].y_loc = -1;
   jump_upFRAMES[2].BMPnum = 2;
      jump_upFRAMES[2].x_loc = 0;
      jump_upFRAMES[2].y_loc = -1;
   jump_upFRAMES[3].BMPnum = 3;
      jump_upFRAMES[3].x_loc = 4;
      jump_upFRAMES[3].y_loc = -9;
   jump_upFRAMES[4].BMPnum = 4;
      jump_upFRAMES[4].x_loc = 9;
      jump_upFRAMES[4].y_loc = -13;
   jump_upFRAMES[5].BMPnum = 3;
      jump_upFRAMES[5].x_loc = 13;
      jump_upFRAMES[5].y_loc = -16;
   jump_upFRAMES[6].BMPnum = 1;
      jump_upFRAMES[6].x_loc = 15;
      jump_upFRAMES[6].y_loc = -16;
   jump_upFRAMES[7].BMPnum = 0;
      jump_upFRAMES[7].x_loc = 16;
      jump_upFRAMES[7].y_loc = -16;

   jump_downFRAMES[0].BMPnum = 0;
      jump_downFRAMES[0].x_loc = 0;
      jump_downFRAMES[0].y_loc = 0;
   jump_downFRAMES[1].BMPnum = 1;
      jump_downFRAMES[1].x_loc = 0;
      jump_downFRAMES[1].y_loc = -1;
   jump_downFRAMES[2].BMPnum = 2;
      jump_downFRAMES[2].x_loc = 0;
      jump_downFRAMES[2].y_loc = -1;
   jump_downFRAMES[3].BMPnum = 3;
      jump_downFRAMES[3].x_loc = 4;
      jump_downFRAMES[3].y_loc = -2;
   jump_downFRAMES[4].BMPnum = 4;
      jump_downFRAMES[4].x_loc = 9;
      jump_downFRAMES[4].y_loc = 5;
   jump_downFRAMES[5].BMPnum = 3;
      jump_downFRAMES[5].x_loc = 13;
      jump_downFRAMES[5].y_loc = 10;
   jump_downFRAMES[6].BMPnum = 1;
      jump_downFRAMES[6].x_loc = 15;
      jump_downFRAMES[6].y_loc = 15;
   jump_downFRAMES[7].BMPnum = 0;
      jump_downFRAMES[7].x_loc = 16;
      jump_downFRAMES[7].y_loc = 16;


}


void music_question_init (void)
{
   int inkeyINT;

   printf ("MUSIC ON? ('y' or 'n')::");
   do
   {
      inkeyINT = readkey();
   }
   while (inkeyINT != YES_KEY && inkeyINT != NO_KEY);

   if (inkeyINT == YES_KEY)
      options.music_on = 1;
   if (inkeyINT == NO_KEY)
      options.music_on = 0;

}


void create_bmps_init (void)
{
   int i;

   //create THE screen buffer...  whoa
   screen_buffer = create_bitmap(320, 200);
   animation_buffer = create_bitmap(3*TILE, 3*TILE);

   for (i=0; i<10; i+=1)
      gfx.goodbean[i] = create_bitmap(16, 16);
   for (i=0; i<10; i+=1)
      gfx.evilbean[i] = create_bitmap(16, 16);
   gfx.tile1 = create_bitmap(16, 16);
   gfx.tile2 = create_bitmap(16, 16);
   gfx.bground3 = create_bitmap(16, 16);
   gfx.bground4 = create_bitmap(16, 16);
}


void draw_screen (void)
{

   //must draw everything!!
   int temp, x, y;

   //first draw background and scenery
   for (y=0; y < 10; y += 1)
      for (x=0; x < 20; x += 1)
      {
         if (bground[y*20+x] == 1)
            blit(gfx.tile1, screen_buffer, 0, 0, x*16, y*16, 16, 16);
         if (bground[y*20+x] == 2)
            blit(gfx.tile2, screen_buffer, 0, 0, x*16, y*16, 16, 16);
         if (bground[y*20+x] == 3)
            blit(gfx.bground3, screen_buffer, 0, 0, x*16, y*16, 16, 16);
         if (bground[y*20+x] == 4)
            blit(gfx.bground4, screen_buffer, 0, 0, x*16, y*16, 16, 16);
      }

   //draw the entire info bar
   //location of this doesn't matter right now
   //actually, it shouldn't ever matter...
   draw_info_bar ();

   //draw objects (sprites)
   for (y=0; y < y_size; y += 1)
      for (x=0; x < x_size; x += 1)
      {
         if (objects[y*x_size+x] == 1)
            draw_sprite(screen_buffer, gfx.goodbean[0], (x_space+x)*TILE, (y_space+y)*TILE);
         if (objects[y*x_size+x] == 2)
            draw_sprite(screen_buffer, gfx.evilbean[0], (x_space+x)*TILE, (y_space+y)*TILE);
      }

   //this is the big part!!
   blit(screen_buffer, screen, 0, 0, 0, 0, 320, 200);
   //whoa...
}


void draw_info_bar ()
{

   //full screen box
   rect(screen_buffer, 0, 0, 320-1, 200-1, 113);
   rect(screen_buffer, 1, 1, 320-2, 200-2, 6);
   rect(screen_buffer, 2, 2, 320-3, 200-3, 113);

   //lower info box
   rect(screen_buffer, 0, 158+1, 320-1, 200-1, 113);  //dark brown
   rect(screen_buffer, 1, 158+2, 320-2, 200-2, 6);    //light brown
   rect(screen_buffer, 2, 158+3, 320-3, 200-3, 113);  //dark brown

   //fill the box
   floodfill(screen_buffer, 3, 158+4, 116);

}


void edit_game_options ()
{
   printf ("THERE ARE NO GAME OPTIONS YET \n");
}

void init_game_settings ()
{
   FILE *jeni;
   int temp, tempnum, x, y, i;

   //game info
   turns = 0;
   whose_turn = 0;
   x_size = 6;
   y_size = 6;
   goodbeans_left = y_size;
   evilbeans_left = y_size;
   x_space = (20-x_size)/2;
   y_space = 9-y_size-1;
   game_over = 0;
   game_won = 0;
   level = 1;
   quit_dos = 0;
   quit_title = 0;


   //the bitmaps
   clear (screen_buffer);
   clear (animation_buffer);

   for (x=0; x<10; x+=1)
      clear (gfx.goodbean[x]);
   for (x=0; x<10; x+=1)
      clear (gfx.evilbean[x]);
   clear (gfx.tile1);
   clear (gfx.tile2);
   clear (gfx.bground3);
   clear (gfx.bground4);


   //load_a_bitmap ("goodbean.txt", gfx.goodbean);
   jeni = fopen ("goodbean.txt", "r");
   for (i=0; i<10; i+=1)
      for (y=0; y<16; y+=1)
         for (x=0; x<16; x+=1)
         {
            fscanf(jeni, "%d", &tempnum);
            putpixel(gfx.goodbean[i], x, y, tempnum);
         }
   fclose(jeni);

   //load_a_bitmap ("evilbean.txt", gfx.evilbean);
   jeni = fopen ("evilbean.txt", "r");
   for (i=0; i<10; i+=1)
      for (y=0; y<16; y+=1)
         for (x=0; x<16; x+=1)
         {
            fscanf(jeni, "%d", &tempnum);
            putpixel(gfx.evilbean[i], x, y, tempnum);
         }
   fclose(jeni);

   load_a_bitmap ("grass.txt", gfx.tile1);
   load_a_bitmap ("dirt.txt", gfx.tile2);
   load_a_bitmap ("gengrass.txt", gfx.bground3);
   load_a_bitmap ("genrock.txt", gfx.bground4);

   //load where objects are (checkered effect)
   for (y=0; y < y_size; y += 1)  {
      for (x=0; x < x_size; x += 1)  {
         if (x == 0)  {
            if (y%2==0)
               objects[y*x_size+x] = 1;
         }
         else  {
            if (x == 1)  {
               if (y%2==1)
                  objects[y*x_size+x] = 1;
            }
            else  {
               if (x == x_size - 2)  {
                  if (y%2==1)
                     objects[y*x_size+x] = 2;
               }
               else  {
                  if (x == x_size-1)  {
                     if (y%2==0)
                        objects[y*x_size+x] = 2;
                  }
                  else  objects[y*x_size+x] = 0;
               }
            }
         }
      }
   }

   //the background
   load_a_background("bground1.txt", bground);
}


void load_a_background (char *filename, int *bgroundPTR)
{
   FILE *jeni;
   int x, y, temp;

   jeni = fopen (filename, "r");
   for (y=0; y<10; y+=1)
      for (x=0; x<20; x+=1)
      {
         fscanf(jeni, "%d", &temp);
         bgroundPTR[y*20+x] = temp;
      }
   fclose(jeni);
}


void load_a_bitmap (char *filename, BITMAP *temp)
{
   FILE *jeni;
   int i, x, y, tempnum;

   jeni = fopen (filename, "r");

   for (y=0; y<16; y+=1)
      for (x=0; x<16; x+=1)
      {
         fscanf(jeni, "%d", &tempnum);
         putpixel(temp, x, y, tempnum);
      }

   fclose(jeni);
}


int input (void)
{
   FLAGS flags;
   COORDS *beanCOORDS, *moveCOORDS;

   //initializing my flags
   flags.a_bean_is_selected = 0;
   flags.a_move_is_selected = 0;
   flags.inventory_is_selected = 0;
   flags.options_is_selected = 0;
   flags.want_to_quit = 0;
   flags.turn_is_done = 0;

   //find out whose turn it is
   if (turns%2 == 0)
      whose_turn = 1;
   if (turns%2 == 1)
      whose_turn = 2;

   do
   {
      input_select (&flags, beanCOORDS);
      moveCOORDS->x = beanCOORDS->x;
      moveCOORDS->y = beanCOORDS->y;

      if (flags.a_bean_is_selected)
         input_bean_move (&flags, moveCOORDS);

      if (flags.a_bean_is_selected && flags.a_move_is_selected)
      {
         /*
         //should probably make this a function
         if (bground[(y_space+beanCOORDS->y)*20+(x_space+beanCOORDS->x)] == 1)
            blit(gfx.tile1, screen, 0, 0, (x_space+beanCOORDS->x)*TILE, (y_space+beanCOORDS->y)*TILE, TILE, TILE);
         if (bground[(y_space+beanCOORDS->y)*20+(x_space+beanCOORDS->x)] == 2)
            blit(gfx.tile2, screen, 0, 0, (x_space+beanCOORDS->x)*TILE, (y_space+beanCOORDS->y)*TILE, TILE, TILE);
         if (whose_turn == 1)
            draw_sprite(screen, gfx.goodbean, (x_space+moveCOORDS->x)*TILE, (y_space+moveCOORDS->y)*TILE);
         if (whose_turn == 2)
            draw_sprite(screen, gfx.evilbean, (x_space+moveCOORDS->x)*TILE, (y_space+moveCOORDS->y)*TILE);
         */

         //for the animation
         jump_anim (*beanCOORDS, moveCOORDS->x - beanCOORDS->x, moveCOORDS->y - beanCOORDS->y);

         objects[beanCOORDS->y*x_size+beanCOORDS->x] = 0;
         objects[moveCOORDS->y*x_size+moveCOORDS->x] = whose_turn;
         draw_screen();

         //see if a bean needs to be deleted
         check_move (*moveCOORDS);

      }


      if (flags.inventory_is_selected)
         inventory_options ();

      if (flags.options_is_selected)
         edit_game_options ();

      //reset flags as things are done, as to not confuse the next loop

   }
   while (!flags.turn_is_done && !flags.want_to_quit && !game_over);

   turns += 1;

   if (flags.want_to_quit || game_over)
      return 1;
   else  return  0;  //do_proceed
}


void check_move (COORDS move)
{
   //look at where you just moved, and...
   //see if a surrounding bean is of the other player's type
   //if yes, check THAT bean's surrounding beans
   if (objects[(move.y-1)*x_size+(move.x)] == whose_turn%2+1)
      check_a_bean (move.x, move.y-1);
   if (objects[(move.y)*x_size+(move.x+1)] == whose_turn%2+1)
      check_a_bean (move.x+1, move.y);
   if (objects[(move.y+1)*x_size+(move.x)] == whose_turn%2+1)
      check_a_bean (move.x, move.y+1);
   if (objects[(move.y)*x_size+(move.x-1)] == whose_turn%2+1)
      check_a_bean (move.x-1, move.y);
}


void check_a_bean (int x, int y)
{
   int count = 0;

   //check surrounding squares
   if (objects[(y-1)*x_size+(x)] == whose_turn)
      count += 1;
   if (objects[(y)*x_size+(x+1)] == whose_turn)
      count += 1;
   if (objects[(y+1)*x_size+(x)] == whose_turn)
      count += 1;
   if (objects[(y)*x_size+(x-1)] == whose_turn)
      count += 1;

   if (count > 2)
   {
      /*
      //refresh screen
      if (bground[(y_space+y)*20+(x_space+x)] == 1)
         blit(gfx.tile1, screen, 0, 0, (x_space+x)*TILE, (y_space+y)*TILE, TILE, TILE);
      if (bground[(y_space+y)*20+(x_space+x)] == 2)
         blit(gfx.tile2, screen, 0, 0, (x_space+x)*TILE, (y_space+y)*TILE, TILE, TILE);
      */

      //refresh 'objects'
      objects[y*x_size+x] = 0;
      if (whose_turn == 1)
         evilbeans_left -= 1;
      if (whose_turn == 2)
         goodbeans_left -= 1;
      //refresh screen  :)
      draw_screen();
   }

}


int input_select (FLAGS *flagsPTR, COORDS *temp)
{
   COORDS bean[x_size];
   int temp_key, count=0, remaining_beans;
   int x_temp, y_temp, x=0, y=0;
   double start_time;


   if (whose_turn == 1)
      remaining_beans = goodbeans_left;
   if (whose_turn == 2)
      remaining_beans = evilbeans_left;


   for (y=0; y<y_size; y += 1)
      for (x=0; x<x_size; x += 1)
         if (objects[y*x_size+x] == whose_turn)
         {
            bean[count].x = x;
            bean[count].y = y;
            count += 1;
         }

   //retreat!  (game over)
   if (remaining_beans < 3)
   {
      game_over = 1;
      return;
   }

   count = 0;
   //reset animation frame
   frame = 0;

   do
   {
      do
      {
         temp_key = 0;
         start_time = clock ();
         //start animation
         //send bounce the original x and y values, or COORDS
         bounce_anim (bean[count]);
         //draw the box
         rect (screen, (x_space+bean[count].x)*TILE, (y_space+bean[count].y)*TILE, (x_space+bean[count].x)*TILE+(TILE-1), (y_space+bean[count].y)*16+(TILE-1), 17);

         do
         {
            if (keypressed ())
               temp_key = readkey();
         }
         //how long to wait
         while (temp_key == 0 && (clock() - start_time < .02*SECONDS));
      }
      while (temp_key == 0);

      x_temp = bean[count].x;
      y_temp = bean[count].y;

      if (temp_key == LEFT_KEY)
         count = (count-1+remaining_beans)%remaining_beans;
      if (temp_key == RIGHT_KEY)
         count = (count+1)%remaining_beans;

      draw_screen();

      /*
      if (bground[(y_space+y_temp)*20+(x_space+x_temp)] == 1)
         blit(gfx.tile1, screen, 0, 0, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE, TILE, TILE);
      if (bground[(y_space+y_temp)*20+(x_space+x_temp)] == 2)
         blit(gfx.tile2, screen, 0, 0, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE, TILE, TILE);
      if (objects[y_temp*x_size+x_temp] == 1)
         draw_sprite(screen, gfx.goodbean, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE);
      if (objects[y_temp*x_size+x_temp] == 2)
         draw_sprite(screen, gfx.evilbean, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE);
      */
   }
   while (temp_key != ENTER_KEY && temp_key != ESCAPE_KEY && temp_key != INVENTORY_KEY);

   if (temp_key == ENTER_KEY)
      flagsPTR->a_bean_is_selected = 1;
   if (temp_key == ESCAPE_KEY)
      //flagsPTR->options_is_selected = 1;
      flagsPTR->want_to_quit = 1;
   if (temp_key == INVENTORY_KEY)
      flagsPTR->inventory_is_selected = 1;

   temp->x = bean[count].x;
   temp->y = bean[count].y;

}


void bounce_anim (COORDS loc)
{

   //copy surroundings for the animation to write over
   blit(screen_buffer, animation_buffer, (x_space+loc.x-1)*TILE, (y_space+loc.y-1)*TILE, 0, 0, 3*TILE, 3*TILE);
   if (bground[(y_space+loc.y)*20+(x_space+loc.x)] == 1)
      blit(gfx.tile1, animation_buffer, 0, 0, 1*TILE, 1*TILE, TILE, TILE);
   if (bground[(y_space+loc.y)*20+(x_space+loc.x)] == 2)
      blit(gfx.tile2, animation_buffer, 0, 0, 1*TILE, 1*TILE, TILE, TILE);

   //do the animation
   if (whose_turn == 1)
      draw_sprite(animation_buffer, gfx.goodbean[bounceFRAMES[frame].BMPnum], (1*TILE)+bounceFRAMES[frame].x_loc, (1*TILE)+bounceFRAMES[frame].y_loc);
   if (whose_turn == 2)
      draw_sprite(animation_buffer, gfx.evilbean[bounceFRAMES[frame].BMPnum], (1*TILE)+bounceFRAMES[frame].x_loc, (1*TILE)+bounceFRAMES[frame].y_loc);

   //blit to the screen
   blit(animation_buffer, screen, 0, 0, (x_space+loc.x-1)*TILE, (y_space+loc.y-1)*TILE, 3*TILE, 3*TILE);

   //change the frame number, between 0 and 1
   frame = (frame+1)%2;

}


void jump_anim (COORDS loc, int x_dir, int y_dir)
{
   double start_time;

   frame = 1;

   //for test purposes
   //x_dir = 1;
   //y_dir = 1;

   //the animation loop
   do
   {
      start_time = clock();

      //copy surroundings for the animation to write over
      blit(screen_buffer, animation_buffer, (x_space+loc.x-1)*TILE, (y_space+loc.y-1)*TILE, 0, 0, 3*TILE, 3*TILE);
      if (bground[(y_space+loc.y)*20+(x_space+loc.x)] == 1)
         blit(gfx.tile1, animation_buffer, 0, 0, 1*TILE, 1*TILE, TILE, TILE);
      if (bground[(y_space+loc.y)*20+(x_space+loc.x)] == 2)
         blit(gfx.tile2, animation_buffer, 0, 0, 1*TILE, 1*TILE, TILE, TILE);

      //do the animation
      if (y_dir < 0)
      {
         if (whose_turn == 1)
            draw_sprite(animation_buffer, gfx.goodbean[jump_upFRAMES[frame].BMPnum], (1*TILE)+(jump_upFRAMES[frame].x_loc*x_dir), (1*TILE)+(jump_upFRAMES[frame].y_loc));
         if (whose_turn == 2)
            draw_sprite(animation_buffer, gfx.evilbean[jump_upFRAMES[frame].BMPnum], (1*TILE)+(jump_upFRAMES[frame].x_loc*x_dir), (1*TILE)+(jump_upFRAMES[frame].y_loc));
      }
      if (y_dir > 0)
      {
         if (whose_turn == 1)
            draw_sprite(animation_buffer, gfx.goodbean[jump_downFRAMES[frame].BMPnum], (1*TILE)+(jump_downFRAMES[frame].x_loc*x_dir), (1*TILE)+(jump_downFRAMES[frame].y_loc));
         if (whose_turn == 2)
            draw_sprite(animation_buffer, gfx.evilbean[jump_downFRAMES[frame].BMPnum], (1*TILE)+(jump_downFRAMES[frame].x_loc*x_dir), (1*TILE)+(jump_downFRAMES[frame].y_loc));
      }

      //blit to the screen
      blit(animation_buffer, screen, 0, 0, (x_space+loc.x-1)*TILE, (y_space+loc.y-1)*TILE, 3*TILE, 3*TILE);

      //change the frame number, between 1 and 8
      //this is good only for a singly bounce
      frame = (frame+1);

      //wait a little bit
      while (clock() - start_time < .007*SECONDS)
         ;

   }
   while (frame != 8);

}


int input_bean_move (FLAGS *flagsPTR, COORDS *locPTR)
{

   COORDS space[4];
   COORDS for_anim_start;
   int temp_key;
   int x_temp, y_temp;
   int count = 0, spacesNUM;
   double start_time;

   //the COORDS for...
   for_anim_start.x = locPTR->x;
   for_anim_start.y = locPTR->y;

   //should probably make this a functions
   if (bground[(y_space+locPTR->y-1)*20+(x_space+locPTR->x-1)] == whose_turn  &&
   objects[(locPTR->y-1)*y_size+(locPTR->x-1)] != whose_turn)
   {
      space[count].x = locPTR->x-1;
      space[count].y = locPTR->y-1;
      count += 1;
   }
   if (bground[(y_space+locPTR->y-1)*20+(x_space+locPTR->x+1)] == whose_turn  &&
   objects[(locPTR->y-1)*y_size+(locPTR->x+1)] != whose_turn)
   {
      space[count].x = locPTR->x+1;
      space[count].y = locPTR->y-1;
      count += 1;
   }
   if (bground[(y_space+locPTR->y+1)*20+(x_space+locPTR->x+1)] == whose_turn  &&
   objects[(locPTR->y+1)*y_size+(locPTR->x+1)] != whose_turn)
   {
      space[count].x = locPTR->x+1;
      space[count].y = locPTR->y+1;
      count += 1;
   }
   if (bground[(y_space+locPTR->y+1)*20+(x_space+locPTR->x-1)] == whose_turn  &&
   objects[(locPTR->y+1)*y_size+(locPTR->x-1)] != whose_turn)
   {
      space[count].x = locPTR->x-1;
      space[count].y = locPTR->y+1;
      count += 1;
   }

   spacesNUM = count;

   //if you can't move anywhere
   if (count == 0)
   {
      flagsPTR->a_bean_is_selected = 0;
      return 0;
   }

   count = 0;

   do
   {
      do
      {
         temp_key = 0;
         start_time = clock ();
         //start animation
         //send bounce the original x and y values, or COORDS
         bounce_anim (for_anim_start);
         //draw the box
         rect (screen, (x_space+space[count].x)*TILE, (y_space+space[count].y)*TILE, (x_space+space[count].x)*TILE+(TILE-1), (y_space+space[count].y)*TILE+(TILE-1), 17);

         do
         {
            if (keypressed ())
               temp_key = readkey();
         }
         //how long to wait
         while (temp_key == 0 && (clock() - start_time < .02*SECONDS));
      }
      while (temp_key == 0);


      x_temp = space[count].x;
      y_temp = space[count].y;

      //should probably make this a function
      if (temp_key == LEFT_KEY)
         count = (count-1+spacesNUM)%spacesNUM;
      if (temp_key == RIGHT_KEY)
         count = (count+1)%spacesNUM;

      draw_screen();

      /*
      if (bground[(y_space+y_temp)*20+(x_space+x_temp)] == 1)
         blit(gfx.tile1, screen, 0, 0, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE, TILE, TILE);
      if (bground[(y_space+y_temp)*20+(x_space+x_temp)] == 2)
         blit(gfx.tile2, screen, 0, 0, (x_space+x_temp)*TILE, (y_space+y_temp)*TILE, TILE, TILE);
      */
   }
   while (temp_key != ENTER_KEY && temp_key != ESCAPE_KEY);

   if (temp_key == ENTER_KEY)
   {
      flagsPTR->a_move_is_selected = 1;
      flagsPTR->turn_is_done = 1;
      locPTR->x = space[count].x;
      locPTR->y = space[count].y;

      return 1;
   }
   if (temp_key == ESCAPE_KEY)
   {
      flagsPTR->a_bean_is_selected = 0;
      return 0;
   }

}


void inventory_options ()
{
   printf ("THERE IS NOTHING IN YOUR INVENTORY \n");
}


void play_song (char *filename)
{

   /* read in the WAV file */
   curr_music = load_sample(filename);

   /* start up the sample */
   play_sample(curr_music, 255, 128, 1000, TRUE);

}







