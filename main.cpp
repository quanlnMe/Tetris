/*
 *        Computer Graphics Course - Shenzhen University
 *    Mid-term Assignment - Tetris implementation sample code
 * ============================================================
 *
 * - ����������ǲο����룬����Ҫ����ο���ҵ˵��������˳������ɡ�
 * - ��������OpenGL�����������������У���ο���һ��ʵ��γ�����ĵ���
 *
 * - ��ʵ�ֹ������£�
 * - 1) �������̸�͡�L���ͷ���
 * - 2) ������/��/�¼����Ʒ�����ƶ����ϼ���ת����
 *
 * - δʵ�ֹ������£�
 * - 1) ������ɷ��鲢���ϲ�ͬ����ɫ  ok
 * - 2) ������Զ������ƶ�   ok
 * - 3) ����֮�����ײ���   
 * - 4) ���̸���ÿһ�������֮���Զ�����
 * - 5) ����
 *
 */

#include "include/Angel.h"

#pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;

int starttime;			// ���Ʒ��������ƶ�ʱ��
int rotation = 0;		// ���Ƶ�ǰ�����еķ�����ת
vec2 tile[4];			// ��ʾ��ǰ�����еķ���
bool gameover = false;	// ��Ϸ�������Ʊ���
int xsize = 400;		// ���ڴ�С��������Ҫ�䶯���ڴ�С����
int ysize = 720;
int w;  //��ʾ������ʲô��״
int score;

// һ����ά�����ʾ���п��ܳ��ֵķ���ͷ���ÿ�������в�ͬ�ĵ����״
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

// ���ƴ��ڵ���ɫ����,��ͬ����ɫ��Ӧ��ͬ��vec4ֵ
vec4 orange = vec4(1.0, 0.5, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);
vec4 yellow = vec4(1.0, 1.0, 0.0, 1.0);
vec4 purple = vec4(1.0, 0.0, 1.0, 1.0);
vec4 light_blue = vec4(0.0, 1.0, 1.0, 1.0);
vec4 green = vec4(0.0, 1.0, 0.0, 0.0);
vec4 red = vec4(1.0, 0.0, 0.0, 0.0);
vec4 blue = vec4(0.0, 0.0, 1.0, 0.0);

// ��ǰ�����λ�ã������̸�����½�Ϊԭ�������ϵ��
vec2 tilepos = vec2(5, 19);

// ���������ʾ���̸��ĳλ���Ƿ񱻷�����䣬��board[x][y] = true��ʾ(x,y)�����ӱ���䡣
// �������̸�����½�Ϊԭ�������ϵ��
bool board[10][20];


// �����̸�ĳЩλ�ñ��������֮�󣬼�¼��Щλ���ϱ�������ɫ
vec4 boardcolours[1200];
vec4 boardpoints[1200]; //���̸���λ��
GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];//VBO  2��ʾ����λ�ã�3��ʾ������ɫ��4��ʾ����λ�ã�5��ʾ������ɫ

//////////////////////////////////////////////////////////////////////////
// �޸����̸���posλ�õ���ɫΪcolour�����Ҹ��¶�Ӧ��VBO

void changecellcolour(vec2 pos, vec4 colour)
{
	// ÿ�������Ǹ������Σ��������������Σ��ܹ�6�����㣬�����ض���λ�ø����ʵ�����ɫ
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// ����ƫ���������ʵ���λ�ø�����ɫ
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////
// ��ǰ�����ƶ�������תʱ������VBO

void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// ÿ����������ĸ�����
	for (int i = 0; i < 4; i++)
	{
		// ������ӵ�����ֵ
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// ÿ�����Ӱ������������Σ�������6����������
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	glBindVertexArray(0);
}

//////////////////////////////////////////////////////////////////////////
// ���õ�ǰ����Ϊ��һ���������ֵķ��顣����Ϸ��ʼ��ʱ�����������һ����ʼ�ķ��飬
// ����Ϸ������ʱ���жϣ�û���㹻�Ŀռ��������µķ��顣

void newtile()
{
	// ���·���������̸���������м�λ�ò�����Ĭ�ϵ���ת����
	tilepos = vec2(5 , 19);
	rotation = 0;
	w = rand() % 7;  //�����������
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

	// ���·��鸳����ɫ
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
// ��Ϸ��OpenGL��ʼ��

void init()
{

	score = 0;//��ʼ���÷�Ϊ0
	// ��ʼ�����̸񣬰���64���������꣨�ܹ�32���ߣ�������ÿ������һ����ɫֵ
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// ������
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);

	}

	// ˮƽ��
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// �������߸��ɰ�ɫ
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// ��ʼ�����̸񣬲���û�б����ĸ������óɺ�ɫ
	
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// ��ÿ�����ӣ���ʼ��6�����㣬��ʾ���������Σ�����һ�������θ���
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

	// �����̸�����λ�õ�����������Ϊfalse��û�б���䣩
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// ������ɫ��
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	glGenVertexArrays(3, &vaoIDs[0]);

	// ���̸񶥵�
	glBindVertexArray(vaoIDs[0]);
	glGenBuffers(2, vboIDs);

	// ���̸񶥵�λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸񶥵���ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ���̸�ÿ������
	glBindVertexArray(vaoIDs[1]);
	glGenBuffers(2, &vboIDs[2]);

	// ���̸�ÿ�����Ӷ���λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ���̸�ÿ�����Ӷ�����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	// ��ǰ����
	glBindVertexArray(vaoIDs[2]);
	glGenBuffers(2, &vboIDs[4]);

	// ��ǰ���鶥��λ��
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// ��ǰ���鶥����ɫ
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	glBindVertexArray(0);
	glClearColor(0, 0, 0, 0);

	// ��Ϸ��ʼ��
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

//////////////////////////////////////////////////////////////////////////
// �����cellposλ�õĸ����Ƿ��������Ƿ������̸�ı߽緶Χ�ڡ�

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
			if (board[i][19] == true)//����������Ͻ磬��˵����Ϸ����
			{
				gameover = true;
				break;
			}
		}
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////
// �����������㹻�ռ���������ת��ǰ����
//ÿ�����鶼���Ų�ͬ����ת����
void rotate()
{
	// ����õ���һ����ת����
	int nextrotation;

	// ��鵱ǰ��ת֮���λ�õ���Ч��,���ڲ�ͬ�ķ�����в�ͬ�ı任
	if (w == 0) {
		nextrotation = (rotation+1) % 4 ;
		if (checkvalid((allRotationsLshape[nextrotation][0]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][1]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][2]) + tilepos)
			&& checkvalid((allRotationsLshape[nextrotation][3]) + tilepos))
		{
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
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
			// ������ת������ǰ��������Ϊ��ת֮��ķ���
			rotation = nextrotation;
			for (int i = 0; i < 4; i++)
				tile[i] = allRotationsTshape[rotation][i];


		}
	}
	updatetile();
}


//���Ѿ������еĺ�����ʩ��ʹ���յķ����½���
//��row���ϵ���ȫ�������ƶ������ƽ���ɫȫ������copy������ɫVBO����Ϊ���̷���λ���ǲ��ܸı�ģ����������һ�и�λ��ɫ
void updateAfterclear(int row)
{
	cout << "Update!\n" <<row<< endl;
	int i;
	//vec4* newboardpoints = new vec4[60 * (19 - row)];
	for (i = row+1; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			board[j][i - 1] = board[j][i];//�����̷����Ƿ�full��״̬ҲҪ�ı䣬״̬���ơ�
			for(int l=0;l<6;l++)
				boardcolours[6 * (10 * (i - 1) + j)+l] = boardcolours[6 * (10 * i + j)+l];
			vec4 colour = boardcolours[6 * (10 * (i - 1) + j)];
			vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };
			glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

			// ����ƫ���������ʵ���λ�ø�����ɫ
			int offset = 6 * sizeof(vec4) * (int)(10 * (i-1) + j);
			glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);  //��row���������ɫȫ��������
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
		}
	i = i - 1;  //i=19;
	for (int j = 0; j < 10; j++)
	{
		board[j][i] = false;
		boardcolours[6 * (10 * i + j)] = black;  //���һ��ȫ������Ϊ��ɫ
		boardcolours[6 * (10 * i + j)+1] = black;
		boardcolours[6 * (10 * i + j)+2] = black;
		boardcolours[6 * (10 * i + j)+3] = black;
		boardcolours[6 * (10 * i + j)+4] = black;
		boardcolours[6 * (10 * i + j)+5] = black;
		vec4 colour = black;                    //changecellcolour();
		vec4 newcolours[6] = { colour, colour, colour, colour, colour, colour };
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

		// ����ƫ���������ʵ���λ�ø�����ɫ
		int offset = 6 * sizeof(vec4) * (int)(10 * i + j);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}

	
}

void clearrow(int row)
{
	for (int i = 0; i < 10; i++)
	{
		cout << "��" << row << "��" << "is full" << endl;
		changecellcolour(vec2(i, row), black);  //�������ǽ���ɫ�ָ���ԭ���ĳ�ʼ����ɫ
		board[i][row] = false; //ͬʱ������״̬�ָ���û�б����״̬
	}
	score += 10;  //ÿ����һ�м�10��
	updateAfterclear(row);  //�½�
}

//////////////////////////////////////////////////////////////////////////
// ������̸���row����û�б��������ͨ��board�������ж�

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
// ���õ�ǰ���飬���Ҹ������̸��Ӧλ�ö������ɫVBO

void settile()
{
	// ÿ������
	for (int i = 0; i < 4; i++)
	{
		// ��ȡ���������̸��ϵ�����
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		// �����Ӷ�Ӧ�����̸��ϵ�λ������Ϊ���
		board[x][y] = true;
		// ������Ӧλ�õ���ɫ�޸�
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
// ����λ��(x,y)���ƶ����顣��Ч���ƶ�ֵΪ(-1,0)��(1,0)��(0,-1)���ֱ��Ӧ����
// �����º������ƶ�������ƶ��ɹ�������ֵΪtrue����֮Ϊfalse��

bool movetile(vec2 direction)
{
	for (int i = 0; i < 20; i++)
	{
		checkfullrow(i);   //�����һ����û��full�����full������
	}
	// �����ƶ�֮��ķ����λ������
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// ����ƶ�֮�����Ч��
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// ��Ч���ƶ��÷���
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

//////////////////////////////////////////////////////////////////////////
// ����������Ϸ

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
// ��Ϸ��Ⱦ����

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	glBindVertexArray(vaoIDs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 1200); // �������̸� (10*20*2 = 400 ��������)

	glBindVertexArray(vaoIDs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 24);	 // ���Ƶ�ǰ���� (8 ��������)

	glBindVertexArray(vaoIDs[0]);
	glDrawArrays(GL_LINES, 0, 64);		 // �������̸����


	glutSwapBuffers();
}

//////////////////////////////////////////////////////////////////////////
// �ڴ��ڱ������ʱ�򣬿������̸�Ĵ�С��ʹ֮���̶ֹ��ı�����

void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

void idleFunction(int id)   //�Զ��½������¶������ص�����
{
	if (!gameover) {
		if (!movetile(vec2(0, -1)))
		{
			settile();
			newtile();
		}

		glutTimerFunc(500, idleFunction, 1);//ʹ���ܹ�ѭ�����ܹ������������ж��ܽ����Զ��½�
	}
	else
	{
		cout << "You have got " << score << endl;
		cout << "gameover" << endl;
	}
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧ�¼��е����ⰴ����Ӧ

void special(int key, int x, int y)
{
	if(!gameover)
	{
		switch(key)
		{
			case GLUT_KEY_UP:	// ���ϰ�����ת����
				rotate();
				break;
			case GLUT_KEY_DOWN: // ���°����ƶ�����
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
			case GLUT_KEY_LEFT:  // ���󰴼��ƶ�����
				movetile(vec2(-1, 0));
				break;
			case GLUT_KEY_RIGHT: // ���Ұ����ƶ�����
				movetile(vec2(1, 0));
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// ������Ӧʱ���е���ͨ������Ӧ

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC�� �� 'q' ���˳���Ϸ
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' ��������Ϸ
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
	glutTimerFunc(500,idleFunction,1);  //�Զ��½���ÿ0.5���½�һ��
	glutIdleFunc(idle);
	//glutIdleFunc(idleFunction);
	glutMainLoop();
	
	return 0;
}
