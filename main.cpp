/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - 本代码仅仅是参考代码，具体要求请参考作业说明，按照顺序逐步完成。
 * - 关于配置OpenGL开发环境、编译运行，请参考第一周实验课程相关文档。
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格和‘L’型方块
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 *
 * - 未实现功能如下：
 * - 1) 随机生成方块并赋上不同的颜色  ok
 * - 2) 方块的自动向下移动   ok
 * - 3) 方块之间的碰撞检测   
 * - 4) 棋盘格中每一行填充满之后自动消除
 * - 5) 其他
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;

int starttime;			// 控制方块向下移动时间
int rotation = 0;		// 控制当前窗口中的方块旋转
vec2 tile[4];			// 表示当前窗口中的方块
bool gameover = false;	// 游戏结束控制变量
int xsize = 400;		// 窗口大小（尽量不要变动窗口大小！）
int ysize = 720;
int w;  //表示现在是什么形状
int score;

// 一个二维数组表示所有可能出现的方块和方向。每个方块有不同的点和形状
vec2 allRotationsLshape[4][4] =
							  {{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   "L"
							   {vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   //
							   {vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
							   {vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}};

vec2 allRotationsOshape[4][4] =
{
	{vec2(-1,0),vec2(-1,-1),vec2(0,-1),vec2(0,0)},// "O"
	{vec2(-1,0),vec2(-1,-1),vec2(0,-1),vec2(0,0)},
	{vec2(-1,0),vec2(-1,-1),vec2(0,-1),vec2(0,0)},
	{vec2(-1,0),vec2(-1,-1),vec2(0,-1),vec2(0,0)}
};

vec2 allRotationsZshape[2][4] =
{
	{vec2(-1,0),vec2(0,-1),vec2(1,-1),vec2(0,0)},  //"Z"
	{vec2(1,1),vec2(0,0),vec2(0,-1),vec2(1,0)}
};

vec2 allRotationsIshape[2][4] =
{
	{vec2(-2,0),vec2(-1,0),vec2(0,0),vec2(1,0)},// "I"
	{vec2(0,1),vec2(0,0),vec2(0,-1),vec2(0,-2)}

};

vec2 allRotationsSshape[2][4] =
{
	{vec2(-1,-1),vec2(0,-1),vec2(1,0),vec2(0,0)},// "S"
	{vec2(0,1),vec2(0,0),vec2(1,-1),vec2(1,0)}
};

vec2 allRotationsJshape[4][4] =
{
	{vec2(-1,0),vec2(0,0),vec2(1,0),vec2(1,-1)},//   "J"
	{vec2(0,1),vec2(0,0),vec2(0,-1),vec2(1,1)},
	{vec2(-1,1),vec2(-1,0),vec2(0,0),vec2(1,0)},
	{vec2(-1,-1),vec2(0,-1),vec2(0,1),vec2(0,0)}
};

vec2 allRotationsTshape[4][4] =
{
	{vec2(-1,0),vec2(0,0),vec2(1,0),vec2(0,-1)},//   "T"
	{vec2(0,1),vec2(0,0),vec2(0,-1),vec2(1,0)},
	{vec2(0,1),vec2(-1,0),vec2(0,0),vec2(1,0)},
	{vec2(-1,0),vec2(0,-1),vec2(0,1),vec2(0,0)}
};

// 绘制窗口的颜色变量,不同的颜色对应不同的vec4值
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);
vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
vec4 purple = vec4(1.0, 0.0, 1.0, 1.0);
vec4 light_blue = vec4(0.0, 1.0, 1.0, 1.0);
vec4 green = vec4(0.0, 1.0, 0.0, 0.0);
vec4 red = vec4(1.0, 0.0, 0.0, 0.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 0.0);

// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilepos = vec2(5, 19);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];


// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardcolours[1200];
vec4 boardpoints[1200]; //棋盘格子位置
GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];//VBO  2表示方格位置，3表示方格颜色，4表示方块位置，5表示方块颜色

//////////////////////////////////////////////////////////////////////////
// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// 当前方块移动或者旋转时，更新VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。

void newtile()
{
	// 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
	tilepos = vec2(5 , 19);
	rotation = 0;
	w = rand() % 7;  //随机产生方块
	if (w == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsLshape[0][i];
		}
	}
	else if (w == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsOshape[0][i];
		}
	}
	else if (w == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsIshape[0][i];
		}
	}
	else if (w == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsSshape[0][i];
		}
	}
	else if (w == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsZshape[0][i];
		}

	}
	else if (w == 5)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsJshape[0][i];
		}
	}
	else if (w == 6)
	{
		for (int i = 0; i < 4; i++)
		{
			tile[i] = allRotationsTshape[0][i];
		}
	}
	updatetile();

	// 给新方块赋上颜色
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++)
	{
		if (w == 0)
			newcolours[i] = orange;
		else if (w == 1)
			newcolours[i] = yellow;
		else if (w == 2)
			newcolours[i] = red;
		else if (w == 3)
			newcolours[i] = light_blue;
		else if (w == 4)
			newcolours[i] = purple;
		else if (w == 5)
			newcolours[i] = green;
		else if (w == 6)
			newcolours[i] = blue;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// 游戏和OpenGL初始化

void init()
{

	score = 0;//初始化得分为0
	// 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// 纵向线
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// 水平线
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);

			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;
		}

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// 载入着色器
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// 棋盘格顶点
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 棋盘格每个格子
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// 当前方块
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// 游戏初始化
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

//////////////////////////////////////////////////////////////////////////
// 检查在cellpos位置的格子是否被填充或者是否在棋盘格的边界范围内。

bool checkvalid(vec2 cellpos)
{
	int x = cellpos.x;
	int y = cellpos.y;
	if((cellpos.x >=0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y < 20)&&!board[x][y])
		return true;
	else
	{
		for (int i = 0; i < 10; i++)
		{
			if (board[i][19] == true)//如果超出了上界，则说明游戏结束
			{
				gameover = true;
				break;
			}
		}
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// 在棋盘上有足够空间的情况下旋转当前方块
//每个方块都有着不同的旋转方向
void rotate()
{
	// 计算得到下一个旋转方向
	int nextrotation;

	// 检查当前旋转之后的位置的有效性,对于不同的方块进行不同的变换
	if (w == 0) {
		nextrotation = (rotation+1) % 4 ;
		if (checkvalid((allRotationsLshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsLshape[rotation][i];


		}
	}
	else if (w == 1) {
		nextrotation = (rotation+1)%4;
		if (checkvalid((allRotationsOshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsOshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsOshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsOshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsOshape[rotation][i];


		}
	}
	else if (w == 2) {
		nextrotation = (rotation+1) % 2 ;
		if (checkvalid((allRotationsIshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsIshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsIshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsIshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsIshape[rotation][i];


		}
	}
	else if (w == 3) {
		nextrotation = (rotation+1) % 2 ;
		if (checkvalid((allRotationsSshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsSshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsSshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsSshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsSshape[rotation][i];


		}
	}
	else if (w == 4) {
		nextrotation = (rotation+1) % 2 ;
		if (checkvalid((allRotationsZshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsZshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsZshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsZshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsZshape[rotation][i];


		}
	}
	else if (w == 5) {
		nextrotation = (rotation+1) % 4 ;
		if (checkvalid((allRotationsJshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsJshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsJshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsJshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsJshape[rotation][i];


		}
	}
	else if (w == 6) {
		cout << w << endl;
		nextrotation = (rotation+1) % 4;
		if (checkvalid((allRotationsTshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsTshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsTshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsTshape[nextrotation][3]) + tilepos))
		{
			// 更新旋转，将当前方块设置为旋转之后的方块
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsTshape[rotation][i];


		}
	}
	updatetile();
}


//对已经消除列的后续措施，使悬空的方块下降；
//在row以上的行全部向下移动就类似将颜色全部下移copy下来颜色VBO（因为棋盘方格位置是不能改变的），并且最后一行复位黑色
void updateAfterclear(int row)
{
	cout << "Update!\n" <<row<< endl;
	int i;
	//vec4* newboardpoints = new vec4[60 * (19 - row)];
	for (i = row+1; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			board[j][i - 1] = board[j][i];//将棋盘方格是否full的状态也要改变，状态复制。
			for(int l=0;l<6;l++)
				boardcolours[6 * (10 * (i - 1) + j)+l] = boardcolours[6 * (10 * i + j)+l];
			vec4 colour = boardcolours[6 * (10 * (i - 1) + j)];
			vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

			// 计算偏移量，在适当的位置赋上颜色
			int offset = 6 * sizeof(vec4) * (int)(10 * (i-1) + j);
			glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);  //将row行上面的颜色全部往下移
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
		}
	i = i - 1;  //i=19;
	for (int j = 0; j < 10; j++)
	{
		board[j][i] = false;
		boardcolours[6 * (10 * i + j)] = black;  //最后一行全部设置为黑色
		boardcolours[6 * (10 * i + j)+1] = black;
		boardcolours[6 * (10 * i + j)+2] = black;
		boardcolours[6 * (10 * i + j)+3] = black;
		boardcolours[6 * (10 * i + j)+4] = black;
		boardcolours[6 * (10 * i + j)+5] = black;
		vec4 colour = black;                    //changecellcolour();
		vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

		// 计算偏移量，在适当的位置赋上颜色
		int offset = 6 * sizeof(vec4) * (int)(10 * i + j);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}

	
}

void clearrow(int row)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "第" << row << "行" << "is full" << endl;
		changecellcolour(vec2(i, row), black);  //消除就是将颜色恢复到原来的初始化颜色
		board[i][row] = false; //同时将方格状态恢复到没有被填的状态
	}
	score += 10;  //每消除一行加10分
	updateAfterclear(row);  //下降
}

//////////////////////////////////////////////////////////////////////////
// 检查棋盘格在row行有没有被填充满，通过board数组来判断

void checkfullrow(int row)
{
	int i;
	for (i = 0; i < 10; i++)
	{
		if (!board[i][row]) break;
	}
	if (i == 10) clearrow(row);
}




//////////////////////////////////////////////////////////////////////////
// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO

void settile()
{
	// 每个格子
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		if(w==0)
			changecellcolour(vec2(x,y), orange); 
		if (w == 1)
			changecellcolour(vec2(x, y), yellow);
		if (w == 2)
			changecellcolour(vec2(x, y), red);
		if (w == 3)
			changecellcolour(vec2(x, y), light_blue);
		if (w == 4)
			changecellcolour(vec2(x, y), purple);
		if (w == 5)
			changecellcolour(vec2(x, y), green);
		if (w == 6)
			changecellcolour(vec2(x, y), blue);
	}
}

//////////////////////////////////////////////////////////////////////////
// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)，分别对应于向
// 左，向下和向右移动。如果移动成功，返回值为true，反之为false。

bool movetile(vec2 direction)
{
	for (int i = 0; i < 20; i++)
	{
		checkfullrow(i);   //检测哪一列有没有full，如果full就消除
	}
	// 计算移动之后的方块的位置坐标
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// 检查移动之后的有效性
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// 有效：移动该方块
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// 重新启动游戏

void restart()
{
	cout << "restart" << endl;
	gameover = false;
	//main();
	glewInit();
	init();
	
//	glutTimerFunc(500, idleFunction, 1);
//	glutIdleFunc(idle);
}

//////////////////////////////////////////////////////////////////////////
// 游戏渲染部分

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

void idleFunction(int id)   //自动下降，更新动画，回调函数
{
	if (!gameover) {
		if (!movetile(vec2(0, -1)))
		{
			settile();
			newtile();
		}

		glutTimerFunc(500, idleFunction, 1);//使其能够循环，能够在整个过程中都能进行自动下降
	}
	else
	{
		cout << "You have got " << score << endl;
		cout << "gameover" << endl;
	}
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应事件中的特殊按键响应

void special(int key, int x, int y)
{
	if(!gameover)
	{
		switch(key)
		{
			case GLUT_KEY_UP:	// 向上按键旋转方块
				rotate();
				break;
			case GLUT_KEY_DOWN: // 向下按键移动方块
				if (!movetile(vec2(0, -1)))
				{
					for (int i = 0; i < 20; i++)
					{
						checkfullrow(i);
					}
					settile();
					newtile();
				}
				break;
			case GLUT_KEY_LEFT:  // 向左按键移动方块
				movetile(vec2(-1, 0));
				break;
			case GLUT_KEY_RIGHT: // 向右按键移动方块
				movetile(vec2(1, 0));
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 键盘响应时间中的普通按键响应

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC键 和 'q' 键退出游戏
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' 键重启游戏
			cout << "restart" << endl;
			restart();
			break;
	}
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////

void idle(void)
{
	glutPostRedisplay();
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 178);
	glutCreateWindow("Mid-Term-Skeleton-Code");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(500,idleFunction,1);  //自动下降，每0.5秒下降一格
	glutIdleFunc(idle);
	//glutIdleFunc(idleFunction);
	glutMainLoop();
	
	return 0;
}
