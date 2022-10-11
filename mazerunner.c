#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 10
#define COL 10
#define STACK_SIZE ROW*COL
#define TRUE 1
#define FALSE 0

typedef struct {
	 int row;
	 int col;
	 int dir;

}element;

typedef struct {
	 int vert;
	 int horiz;

}offset;

offset move[8] = { {-1,0},{0,-1},{1,0},{0,1}, {-1,1 },{1,1},{-1,-1},{1,-1} };

element stack[STACK_SIZE];

//미로 변수
int maze[ROW][COL];
//지나간 길 표시할 변수
int mark[ROW][COL];
//이동 방향 저장
int direction[ROW][COL];

int i, j, row, col, nextRow, nextCol, dir, found = FALSE;
int top;

//스택에 푸시
void push(int* top, element position)
{
	(*top)++;
	stack[*top].row = position.row;
	stack[*top].col = position.col;
	stack[*top].dir = position.dir;
}

element pop(int* top)
{
	element result;
	if (*top < 0)
	{
		printf("stack is empty\n");
	}
	else 
	{
		result = stack[*top];
		(*top)--;
	}
	return result;
}

element peek(int* top)
{
	element result;
	if (*top < 0)
	{
		printf("stack is empty\n");
	}
	else
	{
		result = stack[*top];
	}
	return result;
}

// 미로 생성 함수
void init()
{
	int i, j;
	srand((unsigned)time(NULL));

	for (i = 0; i < ROW; i++)
	{
		for (j = 0; j < COL; j++)
		{
			// 입구와 출구는 0으로 만들기 
			if ((i == 1 && j == 1) || (i == ROW - 2 && j == COL - 2))
				maze[i][j] = 0;
			// 가장 겉부분은 1로 만듦
			else if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1)
				maze[i][j] = 1;
			// 나머지 부분은 0또는 1을 랜덤으로 할당받음
			else
				maze[i][j] = rand() % 2;
		}
	}

	// 생성된 미로 출력
	printf("\n\n-------maze-------\n\n");
	printf("   ");


	//printf("	");

	// COL 좌표 출력(X축)
	for (j = 0; j < COL; j++)
	{
		printf("%2d", j);
	}
	printf("\n");
	for (j = 0; j < COL; j++)
	{
		printf("--");
	}
	printf("----");
	printf("\n");


		//ROW 출력
		for (i = 0; i < ROW; i++)
		{
			// ROW 좌표 출력(y축)
			printf("%2d|", i);

			for (j = 0; j < COL; j++)
			{
				printf("%2d", maze[i][j]);
			}
			printf("\n");
		}
		

		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j <= COL; j++)
			{
				mark[i][j] = 0;
			}
		}
		

		printf("\n\n\n----maze----\n");

}

// 미로의 길을 찾아서 있으면 스택에 저장
void findPath()
{
	element position;
	mark[1][1] = 1;
	top = 0;
	stack[0].row = 1;
	stack[0].col = 1;
	stack[0].dir = 1;

	while (top>-1&&!found) // stack에 path가 남아있고 출구를 찾지 못한 상태
	{
		// 스택의 top을 pop
		position = pop(&top);
		
		row = position.row;
		col = position.col;
		dir = position.dir;

		//dir방향으로 이동 후 길이 있으면 이동, 없으면 다시 돌아옴
		while (dir < 8 && !found)
		{
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			// 출구를 찾으면 길을 찾았다고 표시
			if (nextRow == ROW - 1 && nextCol == COL - 1)
			{
				found = TRUE;
			}
			else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol]) // 다음위치가 0이면서 방문한 적이 없을 때
			{
				mark[nextRow][nextCol] = TRUE;
				direction[row][col] = dir;
				// 좌표 저장
				position.row = row;
				position.col = col;
				// 방향 저장
				position.dir = dir++;

				//스택에 push
				push(&top, position);
				row = nextRow;
				col = nextCol;
				dir = 0;
			}
			// 출구가 아니며 갈 수 없는 곳일 때(방문했거나 벽)
			else ++dir;
		}
		

		
	}
}

// 길을 찾으면 출력
void printPath()
{
	// 스택을 이용하기 위해 초기화
	if (found)
	{
		for (i = 0; i < ROW; i++)
		{
			for (j = 0; j <= COL; j++)
			{
				mark[i][j] = 0;
			}
		}
		printf("The path is \n");
		printf("row col\n");
		for (i = 0; i <= top; i++)
		{
			printf("%2d%5d\n", stack[i].row, stack[i].col);
			// 스택에 저장된 부분만 길로 취급
			mark[stack[i].row][stack[i].col] = 1;
		}
		printf("%2d%5d\n", row, col);

		printf("\n이동경로를 출력합니다\n\n");

		for (row = 0; row < ROW; row++)
		{
			for (col = 0; col < COL; col++)
			{
				if ((row == 1) && (col == 1))
				{
					printf(" S");
				}
				else if ((row == ROW - 2) && (col == COL - 2))
				{
					printf(" E");
				}
				else
				{
					// mark길이 아니면 X 혹은 공백
					if ((mark[row][col] == FALSE))
					{
						//벽인 경우에 X표시
						if (maze[row][col] == 1)
						{
							printf("%2c", 'X');

						}
						// 벽이 아니면 공백
						else
						{
							printf("%2c", ' ');
						}
					}
					else
					{
						printf(" .");


					}
				}
			}
		printf("\n");
		}
	}
	else
		printf("----have no path----\n");
}	

int main(void)
{
	init();
	findPath();
	printPath();

	return 0;
}
