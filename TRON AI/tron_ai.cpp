#include <bits/stdc++.h>
using namespace std;
#define DB(format,...) fprintf(stderr,format, ##__VA_ARGS__)
char mapa[21][31];
int N;
int P;
int R = 0;
int depth = 3;

struct point
{
    int y, x;
};
bool operator==(point a, point b)
{
    return a.x==b.x&&a.y==b.y;
}
struct head
{
    char player;
    point pos;
};
head p;
vector<head> enemies;
int addX[]= {1,-1,0,0};
int addY[]= {0,0,1,-1};
bool inline isLegit(point po)
{
    return !(po.y<0||po.y>=20||po.x<0||po.x>=30);
}
bool Blocked(point po)
{
    for(int i = 0;i<4;i++)
    {
        point poi = {po.y+addY[i], po.x+addX[i]};
        if(isLegit(poi) && mapa[poi.y][poi.x]=='E')
        {
            return false;
        }
    }
    return true;
}
int Voronoi()
{
    char viz[21][31];
    queue<point> cue;
    memset(viz, 'E', sizeof(viz[0][0])*21*31);
    int counter = 0;
    if(depth%2)
    {
        viz[p.pos.y][p.pos.x] = p.player;  
        for(int i = 0;i<enemies.size();i++)
        {
            cue.push(enemies[i].pos);
            viz[enemies[i].pos.y][enemies[i].pos.x]=enemies[i].player;
        }

    }
    else
    {
        for(int i = 0;i<enemies.size();i++)
        {
            cue.push(enemies[i].pos);
            viz[enemies[i].pos.y][enemies[i].pos.x]=enemies[i].player;
        }
        viz[p.pos.y][p.pos.x] = p.player;  
    }

    cue.push(p.pos);
    while(cue.size())
    {
        for(int i = 0;i<4;i++)
        {
            point vec = {cue.front().y+addY[i],cue.front().x+addX[i]};
            if(isLegit(vec)&&mapa[vec.y][vec.x]=='E' && viz[vec.y][vec.x]=='E')
            {
                viz[vec.y][vec.x] = viz[cue.front().y][cue.front().x];
                cue.push(vec);
                if(viz[vec.y][vec.x] == p.player) counter++;
                else counter--;
            }
        }
        cue.pop();
    }
    return counter*10;
}
void deletePlayer(char c)
{
    for(int i = 0; i<=20; i++)
    {
        for(int j = 0; j<=30; j++)
        {
            if(mapa[i][j]==c)
                mapa[i][j]='E';
        }
    }
}
int gameState(int step)
{
    if(step%2!=depth%2)
    {
        if(Blocked(p.pos))
        {
            return -4;
        }
        int lost = 0;
        for(int i = 0 ;i<enemies.size();i++)
        {
            if(Blocked(enemies[i].pos))
            {
                lost++;
            }
        }
        return lost;
    }
    else
    {
        int lost = 0;
        for(int i = 0 ;i<enemies.size();i++)
        {
            if(Blocked(enemies[i].pos))
            {
                lost++;
            }
        }
        if(Blocked(p.pos))
        {
            return -4;
        }
        return lost;
    }

    return 0;
}
int mini(int steps, int alpha, int beta, int enemyDepth)
{
    int state = gameState(steps);
    if(state!=0 && depth!=steps){
        if(state<0) return state*100000;
        else return 1000*state;
    }
    if(steps==0)
    {
        return Voronoi();
    }
    if(steps%2!=depth%2)
    {
        int minimum = INT_MAX;
        head cp = enemies[enemyDepth];
        for(int i = 0; i<4; i++)
        {
            point ve = {enemies[enemyDepth].pos.y+addY[i],enemies[enemyDepth].pos.x+addX[i]};
            if(isLegit(ve) && mapa[ve.y][ve.x]=='E')
            {
                //setting stuff up
                mapa[ve.y][ve.x]=enemies[enemyDepth].player;
                enemies[enemyDepth].pos=ve;
                //recurse
                int value = (enemyDepth==0)?(mini(steps-1,alpha, beta, 0)):(mini(steps, alpha, beta, enemyDepth-1));
                //updating mins and beta
                minimum = min(minimum, value);
                beta = min(beta, minimum);
                // reset changed values
                mapa[ve.y][ve.x]='E';
                enemies[enemyDepth]=cp;
                //pruning
                if(beta <= alpha)
                    break;
            }
        }
        return minimum;
    }
    else
    {
        int maximum = -INT_MAX;
        int open = 0;
        head cp=p;
        for(int i = 0; i<4; i++)
        {
            point ve = {p.pos.y+addY[i],p.pos.x+addX[i]};
            if(isLegit(ve) && mapa[ve.y][ve.x]=='E')
            {
                mapa[ve.y][ve.x]=p.player;
                p.pos = ve;
                int value = mini(steps-1,alpha, beta,enemies.size()-1);
                if(maximum<value)
                {
                    maximum = value;
                    if(steps==depth)
                        R = i;
                }
                open++;
                alpha = max(alpha, maximum);
                mapa[ve.y][ve.x]='E';
                p=cp;
                if(beta<=alpha)
                    break;

            }
        }

        return maximum;
    }
}
int main()
{

    memset(mapa, 'E', sizeof(mapa[0][0])*21*31);
    bool oneDied = false;
    while (1)
    {
        enemies.clear();
        cin >> N >> P;
        for (int i = 0; i < N; i++)
        {
            int X0;
            int Y0;
            int X1;
            int Y1;
            cin >> X0 >> Y0 >> X1 >> Y1;
            
            char c = '0'+i;
            
            if(X0!=-1) mapa[Y1][X1] = c, mapa[Y0][X0]=c;
            else if(!oneDied) deletePlayer('0'+i),depth=3, oneDied= true;
            
            if(i==P) p = {c,Y1,X1} ;
            else if(X0!=-1) enemies.push_back({c,Y1,X1});
        }
        mini(depth,-INT_MAX, INT_MAX,0);
        switch(enemies.size())
        {
            case 1:
            depth = 7;
            break;
            case 2:
            depth = 4;
            break;
            case 3:
            depth = 3;
            break;
        }

        switch(R)
        {
        case 0:
            cout<<"RIGHT"<<endl;
            break;
        case 1:
            cout<<"LEFT"<<endl;
            break;
        case 2:
            cout<<"DOWN"<<endl;
            break;
        case 3:
            cout<<"UP"<<endl;
            break;
        }

    }
}
