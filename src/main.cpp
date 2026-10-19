#include <raylib.h>
#include <iostream>
#include <vector>
using namespace std;

class Broom
{
    public:    
    Vector2 position;
    Texture2D broom_texture;
    Rectangle frameRect2;
    bool scored=false;  
    int gap=50;
    int gapy;
    
      Broom(int i,Texture2D texture,int gapy)
    {
        broom_texture=texture;
        position.x=400*(i/2)+80;   
        this->gapy=gapy;
    }
    
    void Draw(int i,Texture2D broom_texture)
    {
        if(i%2!=0)
        {
            DrawTexturePro(broom_texture,
            (Rectangle){0,0,(float)broom_texture.width,(float)broom_texture.height},
            (Rectangle){position.x+150,(float)800.0f,(float)broom_texture.width/5,(float)(GetScreenHeight()-(gapy+gap))},
            {(float)broom_texture.width/20.0f,(float)broom_texture.height/20.0f},180.0f,WHITE);   
            frameRect2={(float)position.x+75.0f, 800.0f,(float)broom_texture.width /15.0f,(float)-(GetScreenHeight()-(gapy+150.0f)+50.0f)};    
           // DrawRectangleLines(position.x+75, 800,(int)broom_texture.width /15,(int)-(GetScreenHeight()-(gapy+150)-50),RED);
    }
    else
    {
       DrawTexturePro(broom_texture,
            (Rectangle){0,0,(float)broom_texture.width,(float)broom_texture.height},
            (Rectangle){position.x,(float)-50.0f,(float)broom_texture.width/5,(float)gapy},
            {0.0f,0.0f},0.0f,WHITE);   
            frameRect2={(float)position.x+75,(float)-50.0f,(float)broom_texture.width/15.0f,(float)gapy-50.0f};
           // DrawRectangleLines(position.x+75,(int)-50.0f,(int)broom_texture.width/15,(int)gapy-50,RED);    
    }
    }    

        bool Collide(Rectangle frameRect1)
        {
        return CheckCollisionRecs(frameRect1,frameRect2);
        }
    

};

int main()
{
  
    int score=0;
    bool game_start=false;
    bool game_over=false;
    const int screen_width=1280;
    const int screen_height=800;
    InitWindow(screen_width,screen_height,"Ghost");
    SetTargetFPS(60);   

    InitAudioDevice();
    Music bgm=LoadMusicStream("sprites/background_music.mp3");
    PlayMusicStream(bgm);
    SetMusicVolume(bgm,0.9f);
    Sound jump=LoadSound("sprites/jump_sound.mp3");
    SetSoundVolume(jump,0.1f);

    Texture2D broom_texture=LoadTexture("sprites/broom.png");
    Texture2D bg=LoadTexture("sprites/image.png");
    Texture2D ghost=LoadTexture("sprites/ghost.png");
    Vector2 position={200.0f,400.0f};
    Rectangle frameRect1={position.x-ghost.width/8,position.y-ghost.height/8,(float)ghost.width/4,(float)ghost.height/4};
    int ghost_velocity;
    
    //variables
   vector<Broom> brooms;
 
    while(WindowShouldClose()==false)
    {
      
         if((!game_start || game_over==true) && IsKeyPressed(KEY_SPACE))
    {
        game_start=true;
        game_over=false;
        position.x=screen_width/2;
        position.y=screen_height/2;
        ghost_velocity=10;
        brooms.clear();
        for(int i=0;i<10;i+=2)
        {
        int gapy=GetRandomValue(100,GetScreenHeight()-50-100);
           Broom top(i,broom_texture,gapy);
           Broom bottom(i+1,broom_texture,gapy);
           bottom.position.x=top.position.x;
           brooms.push_back(top); 
           brooms.push_back(bottom);
            //updating top and bottom brooms with the same x position
            
        }
        score=0;
        DrawText(TextFormat("Score:%d ",score),10,10,20,RED); 
    }
     BeginDrawing();
    
    if(!game_start)
    {
        DrawText("WELCOME TO THE GHOST GAME!\n PRESS SPACE TO START",320,screen_height/2,52,RED);   
             
    }
    else
    {
            if(IsKeyPressed(KEY_SPACE)) 
            {
                ghost_velocity=-20;
                PlaySound(jump);
            }
            else
            {
                ghost_velocity+=1;
                position.y+=ghost_velocity/2;
            }       
        //functions   
        DrawTexturePro(bg,{0,0,(float)bg.width,(float)bg.height},
        {0,0,1280,800},{0,0},0.0f,WHITE);
        DrawTexturePro
        (ghost, 
        (Rectangle){0,0,(float)ghost.width,(float)ghost.height},
        (Rectangle){(float)position.x,(float)position.y,(float)ghost.width,(float)ghost.height},
        {(float)ghost.width/2,(float)ghost.height/2},
        float(ghost_velocity)/2,WHITE);
        //DrawRectangleLines(position.x-ghost.width/8,position.y-ghost.height/8,(int)ghost.width/4,(int)ghost.height/4,RED);
        int newX=0;
        for(int i=0;i<10;i+=2)
        {
            if(!game_over)
            {
             UpdateMusicStream(bgm);
            brooms[i+1].position.x=brooms[i].position.x;
            if(brooms[i].position.x>newX)
                {
                    newX=brooms[i].position.x;
                }
            brooms[i].position.x-=3;
            brooms[i].frameRect2.x=brooms[i].position.x;
    
             DrawText(TextFormat("Score:%d ",score),10,10,20,RED);   
            if(i%2==0 && brooms[i].position.x<position.x && brooms[i].scored==false)
            {
                brooms[i].scored=true;
                score+=2;        
            }
            if((brooms[i].Collide(frameRect1)==true && game_over==false) || position.y+frameRect1.height/2>=GetScreenHeight())  
            {
            game_over=true;
            }  
            brooms[i].Draw(i,broom_texture);
            brooms[i+1].Draw(i+1,broom_texture);
            frameRect1.x=position.x;
            frameRect1.y=position.y;
        }              
        }
        for(int i=0;i<10;i+=2)
        {
         if(i%2==0)
            {
            for(int i=0;i<10;i+=2)
            {
           
             if(brooms[i].position.x<=0)
             {
            brooms[i].position.x=GetScreenWidth()-300;
            brooms[i+1].position.x=GetScreenWidth()-300;
            brooms[i].scored=false;
            }            
            }
            }
        }
        if(game_over==true)
        {
        DrawText(TextFormat("Score:%d ",score),10,10,20,RED);
        DrawText("GAME OVER!\n Press space to start.",GetScreenWidth()/2-100,GetScreenHeight()/2-30,35,RED);
        position.x=(screen_width/2)-200;
        position.y=screen_height/2;
        ghost_velocity=0; 
       
        }  
      
    }
      EndDrawing();
}
    UnloadMusicStream(bgm);
    UnloadSound(jump);
    CloseAudioDevice();
    UnloadTexture(broom_texture);
    UnloadTexture(ghost);
    UnloadTexture(bg);
    CloseWindow();
    return 0;
}

