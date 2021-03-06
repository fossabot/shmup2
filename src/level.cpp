#include "level.h"
#include "path.h"
#include <cstring>
#include "shared.h"

/*
    this level module cant be done without help of Neil Roy's Deluxe Pacman Source code help
    made me understand how to load files properly, its easier use allegro I/O funcs than stdio!!!
    https://nitehackr.github.io/games_index.html
    Thanks for the Source!
*/





void level_init_default(LEVEL* level){

    strncpy(level->magic, MAP_ID, 6);
    level->ver = MAP_VER;
    level->player_pos.x = 100;
    level->player_pos.y = 100;
    level->map_width = 75;
    level->map_height = 22;
    level->background_id = 0;
    level->valid_file = false;


    strncpy(level->mapname, "Mapa Teste", 19);

    for(unsigned int i = 0; i < 4; i++){
        level->keys[i].x = 25;
        level->keys[i].y = 25;
    }



    for(int y = 0; y < MAX_GRID_Y; y++){
        for(int x = 0; x < MAX_GRID_X;x++){
            level->bg_layer[y][x].id = 0;
            level->bg_layer[y][x].block = false;
            level->bg_layer[y][x].passable = true;
        }
    }

    for(int y = 0; y < MAX_GRID_Y; y++){
        for(int x = 0; x < MAX_GRID_X;x++){
            level->map_layer[y][x].id = 0;
            level->map_layer[y][x].block = false;
            level->map_layer[y][x].passable = false;
        }
    }

    for(int y = 0; y < MAX_GRID_Y; y++){
        for(int x = 0; x < MAX_GRID_X;x++){
            level->obj_layer[y][x].id = 0;
            level->obj_layer[y][x].block = false;
            level->obj_layer[y][x].passable = true;
        }
    }

    level->powerup_count = 0;

}

bool level_load(ALLEGRO_DISPLAY *display, LEVEL *lvl, const char *mapname, bool dialog){

   const char *filepath = nullptr;

   if(mapname){
        filepath = get_file_path("map", mapname);
        lvl->level_path = filepath;

        if(!al_filename_exists(filepath)){
           WARN("FILENAME %s not loaded!", filepath);
           return false;
       }
   }


    ALLEGRO_FILECHOOSER *openfile_diag = nullptr;
    if(dialog){

        openfile_diag = al_create_native_file_dialog(filepath, "Load MAP:", "*.cbm", ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
        al_rest(1.0);
        if(!al_show_native_file_dialog(display, openfile_diag)){
            if(openfile_diag) al_destroy_native_file_dialog(openfile_diag);
            return false;
        }
        filepath = al_get_native_file_dialog_path(openfile_diag,0);

    }

    char path_lowercase[4096] = "";

    strncpy(path_lowercase, filepath, strlen(filepath) + 1);


    strcpy(lvl->mapname, mapname);
    lvl->level_path = path_lowercase;


    ALLEGRO_FILE *fp = al_fopen(lvl->level_path.c_str(),"rb");

    al_fread(fp, lvl->magic, sizeof (char) * strlen(MAP_ID));


    lvl->ver= al_fgetc(fp);
    bool valid_header = false;

    if(lvl->magic[0] == 'C' && lvl->magic[1] == 'B' && lvl->magic[2] == 'M' && lvl->magic[3] == 'A' && lvl->magic[4] == 'P'){
        valid_header = true;
    }else {
        WARN("MAP HEADER INVALID");
        goto FINISH;
    }



    if(lvl->ver > MAP_VER || lvl->ver != MAP_VER){
       WARN("%s map version incorrect ", filepath);
       goto FINISH;
    }

     lvl->player_pos.x = (unsigned char) al_fgetc(fp);
     lvl->player_pos.y = (unsigned char) al_fgetc(fp);

     for(unsigned int i = 0;i < 4; i++){
         lvl->keys[i].x = (unsigned char) al_fgetc(fp);
         lvl->keys[i].y = (unsigned char) al_fgetc(fp);
     }



     lvl->map_width = (unsigned char) al_fgetc(fp);
     lvl->map_height = (unsigned char) al_fgetc(fp);

     if(lvl->map_width > MAX_GRID_X) lvl->map_width =  MAX_GRID_X;
     if(lvl->map_height > MAX_GRID_Y) lvl->map_width =  MAX_GRID_Y;

     lvl->background_id = (unsigned char)al_fgetc(fp);

     for(unsigned int y = 0; y < MAX_GRID_Y;y++){
         for(unsigned int x = 0; x < MAX_GRID_X;x++){
             lvl->bg_layer[y][x].id = (unsigned char) al_fgetc(fp);
             lvl->bg_layer[y][x].block = (unsigned char) al_fgetc(fp);
             lvl->bg_layer[y][x].passable = (unsigned char) al_fgetc(fp);
         }
     }

     for(unsigned int y = 0; y < MAX_GRID_Y;y++){
         for(unsigned int x = 0; x < MAX_GRID_X;x++){
             lvl->map_layer[y][x].id = (unsigned char) al_fgetc(fp);
             lvl->map_layer[y][x].block = (unsigned char) al_fgetc(fp);
             lvl->map_layer[y][x].passable = (unsigned char) al_fgetc(fp);
         }
     }


     for(unsigned int y = 0; y < MAX_GRID_Y;y++){
         for(unsigned int x = 0; x < MAX_GRID_X;x++){
             lvl->obj_layer[y][x].id = (unsigned char) al_fgetc(fp);
             lvl->obj_layer[y][x].block = (unsigned char) al_fgetc(fp);
             lvl->obj_layer[y][x].passable = (unsigned char) al_fgetc(fp);
         }
     }


     lvl->valid_file = (unsigned char)al_fgetc(fp);

     if(lvl->valid_file == valid_header)
     {
        LOG("MAP [ %s ] LOADED SUCCESS!", path_lowercase);
     }

     if(dialog){
         if(openfile_diag) al_destroy_native_file_dialog(openfile_diag);
     }

      al_fclose(fp);
      if(filepath) delete filepath;
      return true;

FINISH:
      if(dialog){
        if(openfile_diag) al_destroy_native_file_dialog(openfile_diag);
      }
        if(fp)al_fclose(fp);
        return false;




}


bool level_save(ALLEGRO_DISPLAY *display,LEVEL *lvl, char *mapname, bool dialog){
    const char * filepath = nullptr;

    if(mapname){
        filepath = get_file_path("map", mapname);
    }


    ALLEGRO_FILECHOOSER *chooser_diag = nullptr;

    if(dialog){
        chooser_diag = al_create_native_file_dialog(filepath, "Save File..", "*.*", ALLEGRO_FILECHOOSER_SAVE);

        if(!al_show_native_file_dialog(display, chooser_diag)){
            if(chooser_diag) al_destroy_native_file_dialog(chooser_diag);
            return false;
        }

        filepath = al_get_native_file_dialog_path(chooser_diag,0);

        if(!filepath){
            if(chooser_diag) al_destroy_native_file_dialog(chooser_diag);
            return false;
        }

    }



    char file_lc[4096] =  ""; // need to fit not only the filename but full path, on windows this  can be huge, so we need to use a big buffer to store the path

    strncpy(file_lc, filepath, strlen(filepath) + 1);


    ALLEGRO_FILE *fp = nullptr;

    fp = al_fopen(file_lc,"wb");

    if(fp == nullptr){
        fprintf(stderr, "level_save(): FILE: %s LINE: %d - cant load %s ", __FILE__, __LINE__, file_lc);
        if(chooser_diag) al_destroy_native_file_dialog(chooser_diag);
        return false;
    }

    al_fwrite(fp, lvl->magic, strlen(lvl->magic));

    al_fputc(fp, lvl->ver);
    al_fputc(fp, lvl->player_pos.x);
    al_fputc(fp, lvl->player_pos.y);

    for(unsigned int i = 0;i < 4; i++){
        al_fputc(fp, lvl->keys[i].x);
        al_fputc(fp, lvl->keys[i].y);
    }

    al_fputc(fp, lvl->map_width);
    al_fputc(fp, lvl->map_height);

    al_fputc(fp, lvl->background_id);

    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            al_fputc(fp, lvl->bg_layer[y][x].id);
            al_fputc(fp, lvl->bg_layer[y][x].block);
            al_fputc(fp, lvl->bg_layer[y][x].passable);
        }
    }

    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            al_fputc(fp, lvl->map_layer[y][x].id);
            al_fputc(fp, lvl->map_layer[y][x].block);
            al_fputc(fp, lvl->map_layer[y][x].passable);
        }
    }

    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            al_fputc(fp, lvl->obj_layer[y][x].id);
            al_fputc(fp, lvl->obj_layer[y][x].block);
            al_fputc(fp, lvl->obj_layer[y][x].passable);
        }
    }

    lvl->valid_file = true;

     LOG("MAP [ %s ] SAVED SUCCESS!", file_lc);

    al_fclose(fp);

    if(dialog){
        if(chooser_diag) al_destroy_native_file_dialog(chooser_diag);
    }

    //if(filepath) free(filepath);
    return true;
}


TILE *level_get_tile(TILE map[MAX_GRID_Y][MAX_GRID_X], int x, int y){
    return &(map[y][x]);
}

bool level_file_exists(const char *mapname){
    char *path = get_file_path("map", mapname);

    if(al_filename_exists(path)) return true;

    return false;
}

// copy level;
void level_map_copy(LEVEL *dest, const LEVEL *orig){

    dest->background_id = orig->background_id;

    for(int i = 0; i < 4; i++){
        dest->keys[i].x = orig[i].keys->x;
        dest->keys[i].y = orig[i].keys->y;
    }

    dest->level_path = orig->level_path;
    strncpy(dest->magic, orig->magic, 6);
    dest->map_width = orig->map_width;
    dest->map_height = orig->map_height;
    dest->player_pos = orig->player_pos;
    dest->powerup_count = orig->powerup_count;

    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            dest->bg_layer[y][x].id = orig->bg_layer[y][x].id;
            dest->bg_layer[y][x].block = orig->bg_layer[y][x].block;
            dest->bg_layer[y][x].passable = orig->bg_layer[y][x].passable;
        }
    }

    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            dest->map_layer[y][x].id = orig->map_layer[y][x].id;
            dest->map_layer[y][x].block = orig->map_layer[y][x].block;
            dest->map_layer[y][x].passable = orig->map_layer[y][x].passable;
        }
    }


    for(unsigned int y = 0; y < MAX_GRID_Y;y++){
        for(unsigned int x = 0; x < MAX_GRID_X;x++){
            dest->obj_layer[y][x].id = orig->obj_layer[y][x].id;
            dest->obj_layer[y][x].block = orig->obj_layer[y][x].block;
            dest->obj_layer[y][x].passable = orig->obj_layer[y][x].passable;
        }
    }

    return;
}
