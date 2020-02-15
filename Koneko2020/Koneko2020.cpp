// Transfer.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <WinSock2.h>
#include <map>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <vector>

#pragma warning(disable: 4996)

#define POINT_HIGH 5
#define POINT_LOW  2

int nowPoint;

class Board
{
public:
	//int board[49];
	std::string value[49];
	int evaluation[49];
	Board()
	{
		for (int i = 0; i < 49; i++)
		{
			this->evaluation[i] = 0;
			this->value[i] = "";
		}

	}
};

class Cell
{
public:
	int row;
	int col;
	Cell()
	{
		this->row = 0;
		this->col = 0;
	}
	Cell(int row, int col)
	{
		this->row = row;
		this->col = col;
	}
};

int ToArrayNum(int row, int col)
{
	return (row * 7) + col;
}

void ToRowCol(int arrayNum, int* row, int* col)
{
	*row = arrayNum / 7;
	*col = arrayNum % 7;
}

void DebugMasu(Board* board)
{
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			std::cout << board->value[ToArrayNum(i, j)] << ", ";
		}
	}
}

void PrintBoard(Board* board)
{
	std::string str = "   0  1  2  3  4  5  6";
	std::cout << str << std::endl;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (0 == j)
			{
				if (9 < board->evaluation[ToArrayNum(i, j)] || -1 == board->evaluation[ToArrayNum(i, j)])
					std::cout << i << " " << board->evaluation[ToArrayNum(i, j)];
				else
					std::cout << i << "  " << board->evaluation[ToArrayNum(i, j)];
			}
			else if (6 == j)
			{
				if (9 < board->evaluation[ToArrayNum(i, j)] || -1 == board->evaluation[ToArrayNum(i, j)])
					std::cout << " ";
				else
					std::cout << "  ";
				std::cout << board->evaluation[ToArrayNum(i, j)] << std::endl;
			}
			else
			{
				if (9 < board->evaluation[ToArrayNum(i, j)] || -1 == board->evaluation[ToArrayNum(i, j)])
					std::cout << " ";
				else
					std::cout << "  ";
				std::cout << board->evaluation[ToArrayNum(i, j)];
			}
		}
	}
}

void GetKeyValue(Board *board, const char* str, char* value, char* key, std::map<std::string, std::string>* pair)
{
	int len = std::strlen(str);
	int i = 0;
	int status = 0;

	while (i < len)
	{
		if ('\"' == str[i] && 0 == status)
		{
			int j = 0;
			i++;

			while (i < len)
			{
				if ('\"' == str[i])
				{
					i++;
					break;
				}

				key[j] = str[i];
				i++;
				j++;
			}

			key[j] = '\0';

			std::cout << key << std::endl;

			status = 2;
		}
		if ('\"' == str[i] && 1 == status)
		{
			status = 2;
		}
		if (':' == str[i] && 2 == status)
		{
			i++;
			status = 3;
		}
		if (3 == status)
		{
			int j = 0;
			while (i < len)
			{

				value[j] = str[i];
				i++;
				j++;
			}

			value[j] = '\0';

			std::cout << value << std::endl;
		}

		if (0 == (strcmp("\"ko\"", value)))
			strcpy(value, "ko");
		if (0 == (strcmp("\"ne\"", value)))
			strcpy(value, "ne");

		pair->insert(std::make_pair(key, value));

		i++;
	}
}

void ParseJson(const char* str, char* element, std::map<std::string, std::string>* pair, Board *board)
{
    int len = std::strlen(str);
    int status = 0;

    int i = 0;
    int k = 0;

    int depth = 0;
    int dig = 0;

    int s = 0;

    char key[1024];
    char value[1024];

	int row = -1;
	int col = -1;
	std::string v = "";

    while (i < len)
    {
        if ('\"' == str[i] && 0 == status)
        {
            dig = depth;
            status = 1;
        }
        if ('\"' == str[i] && 1 == status)
        {
            status = 2;
        }
        if (':' == str[i] && 2 == status)
        {
            status = 3;
        }
        if ('{' == str[i])
        {
            depth++;
        }
        if ('}' == str[i])
        {
            depth--;
        }
        if ('}' == str[i] && 0 == depth && 2 < status)
        {
            int j = 0;
            while (k < i)
            {
                element[j] = str[k];
                k++;
                j++;
            }

            element[j] = '\0';


            GetKeyValue(board, element, value, key, pair);

			if (0 == strcmp("request", key))
			{
				if (0 == (strcmp("\"match request\"}", value)))
				{
					std::cout << "request" << std::endl;

					goto PRINT;
				}
			}

			if (0 == strcmp("row", key))
			{
				row = atoi(value);
			}
			if (0 == strcmp("col", key))
			{
				col = atoi(value);
			}
			if (0 == strcmp("value", key))
			{
				if(0 == (strcmp("\"ko\"", value)))
					v = "ko";
				if (0 == (strcmp("\"ne\"", value)))
					v = "ne";
			}

			/*
			if (-1 != row && -1 != col)
			{
				//nowPoint = board->evaluation[ToArrayNum(row, col)];
				board->value[ToArrayNum(row, col)] = v;
				board->evaluation[ToArrayNum(row, col)] = -1;
			}
			*/

            ParseJson(value, element, pair, board);

            i++;
        }
        if (',' == str[i] && 3 == status && dig == depth)
        {

            int j = 0;
            while (k < i)
            {
                element[j] = str[k];
                k++;
                j++;
            }

            element[j] = '\0';

            GetKeyValue(board, element, value, key, pair);

			if (0 == strcmp("request", key))
			{
				if (0 == (strcmp("\"match request\"}", value)))
				{
					std::cout << "request" << std::endl;

					goto PRINT;
				}
			}

			if (0 == strcmp("row", key))
			{
				row = atoi(value);
			}
			if (0 == strcmp("col", key))
			{
				col = atoi(value);
			}
			if (0 == strcmp("value", key))
			{
				if (0 == (strcmp("\"ko\"", value)))
					v = "ko";
				if (0 == (strcmp("\"ne\"", value)))
					v = "ne";
			}

			/*
			if (-1 != row && -1 != col)
			{
				//nowPoint = board->evaluation[ToArrayNum(row, col)];
				board->value[ToArrayNum(row, col)] = v;
				board->evaluation[ToArrayNum(row, col)] = -1;
			}
			*/

			ParseJson(value, element, pair, board);

			i++;
            status = 0;
        }

		i++;
    }

PRINT:

	std::cout << "Board--->" << std::endl;

	PrintBoard(board);
}

/*
{
    "match request": "OK",
    "size": 3,
    "board":
    {
        "1":
        {
            "row": 3,
            "col": 3
        },
        "2":
        {
            "row": 4,
            "col": 4
        },
        "3":
        {
            "row": 5,
            "col": 5
        }
    }
}
*/

void JsonEntry(const char* json, std::map<std::string, std::string> *pair, Board *board)
{
    char element[1024];

    ParseJson(json, element, pair, board);
}

int Random(int max, int seed)
{
    srand((unsigned int)time(NULL) * seed);
    Sleep(167 % max);
    int ans = rand() % max;

    return ans;
}

void Double(int N, int MIN, int MAX, std::vector<int>* array)
{
	// 乱数生成用変数
	int rnd;
	// データ検索フラグ
	int bFind;
	// ループカウンタ
	int i, j;

	// 乱数シード初期化
	srand((unsigned)time(NULL));

	for (i = 0; i < N; i++)
	{
		do
		{
			// フラグ初期化
			bFind = 0;
			// 乱数生成
			rnd = rand() % (MAX - MIN) + MIN;
			// データ中に生成した乱数と同値のデータがないか確認
			for (j = 0; j < i; j++)
			{
				// データ中に見つかった場合
				if (array->at(j) == rnd)
				{
					// フラグを立てて再度乱数を生成する
					bFind = 1;
					break;
				}
			}
		} while (bFind);
		// データに生成した乱数を保存する
		array->push_back(rnd);
	}

	// 生成した乱数を出力する
	for (int s : *array)
		std::cout << s << std::endl;
}

int makeJson(char *json, Board *board)
{
	int sizeMax = 12;

    std::vector<int> masuRandom;
    std::vector<int> value;

    Double(sizeMax, 0, 48, &masuRandom);
	
	for (int i=0;i<sizeMax;i++)
	{
		int arg = Random(2, i);
		value.push_back(arg);
	}

    strcpy(json, "{\"match request\":\"OK\",\"size\":");
    
    char num[8];
    itoa(sizeMax, num, 10);

    strcat(json, num);

    strcat(json, "\"board\":{");

    for (int i = 0; i < sizeMax; i++)
    {
        int mas = masuRandom.at(i);

        int row = mas / 7;
        int col = mas % 7;

        int ko_ne = value.at(i);

        char str[64];

        char iStr[8];
        char rowStr[8];
        char colStr[8];

        itoa(i, iStr, 10);
        itoa(row, rowStr, 10);
        itoa(col, colStr, 10);


        strcpy(str, "\"");
        strcat(str, iStr);
        strcat(str, "\":{\"row\":");
        strcat(str, rowStr);
        strcat(str, ",\"col\":");
        strcat(str, colStr);
        strcat(str, ",\"value\":");

        if(ko_ne == 0)
            strcat(str, "\"ko\"");
        else
            strcat(str, "\"ne\"");

        if(i != sizeMax-1)
            strcat(str, "},");
        else
            strcat(str, "}");

        strcat(json, str);


		if(ko_ne == 0)
			board->value[ToArrayNum(row, col)] = "ko";
		else
			board->value[ToArrayNum(row, col)] = "ne";

		board->evaluation[ToArrayNum(row, col)] = -1;
    }

    strcat(json, "}}");

    return 0;
}

int SearchPattern_A(Board* board, Cell* now, Cell* pos1, Cell* pos2)
{
	int nowPos = 0;
	int firstPos = 0;
	int secondPos = 0;

	try {
		nowPos = ToArrayNum(now->row, now->col);
	}
	catch (std::exception & e)
	{
	}

	try {
		firstPos = ToArrayNum(pos1->row, pos1->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos1 is out of range."); // 例外送出
		return 0;
	}

	try {
		secondPos = ToArrayNum(pos2->row, pos2->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos2 is out of range."); // 例外送出
		return 0;
	}

	if (0 > nowPos || 48 < nowPos)
		return 0;
	if (0 > firstPos || 48 < firstPos)
		return 0;
	if (0 > secondPos || 48 < secondPos)
		return 0;

	if (board->value[nowPos] == "ko")
	{
		if (board->value[firstPos] == "ne")
		{
			if (board->value[secondPos] == "ko")
			{
				std::cout << "A = " << POINT_HIGH << "<== " << nowPos << " : " << firstPos << " : " << secondPos << std::endl;
				return POINT_HIGH;
			}
		}
	}

	return 0;
}

int SearchPattern_B(Board* board, Cell* now, Cell* pos1)
{
	int nowPos = 0;
	int firstPos = 0;

	try {
		nowPos = ToArrayNum(now->row, now->col);
	}
	catch (std::exception & e)
	{
	}

	try {
		firstPos = ToArrayNum(pos1->row, pos1->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos1 is out of range."); // 例外送出
		return 0;
	}

	if (0 > nowPos || 48 < nowPos)
		return 0;
	if (0 > firstPos || 48 < firstPos)
		return 0;

	if (board->value[nowPos] == "ne")
	{
		if (board->value[firstPos] == "ko")
		{
			std::cout << "B = " << POINT_LOW << "<== " << nowPos << " : " << firstPos << std::endl;
			return POINT_LOW;
		}
	}

	return 0;
}

int SearchPattern_C(Board* board, Cell* now, Cell* pos1, Cell* pos2)
{
	int nowPos = 0;
	int firstPos = 0;
	int secondPos = 0;

	try {
		nowPos = ToArrayNum(now->row, now->col);
	}
	catch (std::exception & e)
	{
	}

	try {
		firstPos = ToArrayNum(pos1->row, pos1->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos1 is out of range."); // 例外送出
		return 0;
	}

	try {
		secondPos = ToArrayNum(pos2->row, pos2->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos2 is out of range."); // 例外送出
		return 0;
	}

	if (0 > nowPos || 48 < nowPos)
		return 0;
	if (0 > firstPos || 48 < firstPos)
		return 0;
	if (0 > secondPos || 48 < secondPos)
		return 0;

	if (board->value[firstPos] == "ko")
	{
		if (board->value[nowPos] == "ne")
		{
			if (board->value[secondPos] == "ko")
			{
				std::cout << "C = " << POINT_HIGH << "<== " << nowPos << " : " << firstPos << " : " << secondPos << std::endl;
				return POINT_HIGH;
			}
		}
	}

	return 0;
}

int SearchPattern_D(Board* board, Cell* now, Cell* pos1)
{
	int nowPos = 0;
	int firstPos = 0;

	try {
		nowPos = ToArrayNum(now->row, now->col);
	}
	catch (std::exception & e)
	{
	}

	try {
		firstPos = ToArrayNum(pos1->row, pos1->col);
	}
	catch (std::exception & e)
	{
		throw std::runtime_error("pos1 is out of range."); // 例外送出
		return 0;
	}

	if (0 > nowPos || 48 < nowPos)
		return 0;
	if (0 > firstPos || 48 < firstPos)
		return 0;

	if (board->value[firstPos] == "ne")
	{
		if (board->value[nowPos] == "ko")
		{
			std::cout << "D = " << POINT_LOW << "<== " << nowPos << " : " << firstPos << std::endl;
			return POINT_LOW;
		}
	}

	return 0;
}

int SearchPointByDirection(Board* board, Cell* nowPos, std::string value, int direction)
{
	board->value[ToArrayNum(nowPos->row, nowPos->col)] = value;

	Cell pos1;
	Cell pos2;

	int max = 0;
	int point[4];

	point[0] = 0;
	point[1] = 0;
	point[2] = 0;
	point[3] = 0;

	switch (direction)
	{
		// 8
	case 8:

		// [3,2]-[2,2]-[1,2] $-[-1,0]-[-2,0]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col;
		pos2.row = nowPos->row - 2;
		pos2.col = nowPos->col;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[2,2] $-[-1,0]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [4,2]-[3,2]-[2,2] [1,0]-$-[-1,0]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col;
		pos2.row = nowPos->row - 1;
		pos2.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [4,2]-[3,2] [1,0]-$
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;

		break;
		// 9
	case 9:

		// [3,2]-[2,3]-[1,4] $-[-1,1]-[-2,2]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row - 2;
		pos2.col = nowPos->col + 2;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[2,3] $-[-1,1]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [4,1]-[3,2]-[2,3] [1,-1]-$-[-1,1]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row - 1;
		pos2.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [4,1]-[3,2] [1,-1]-$
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col - 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 6
	case 6:

		// [3,2]-[3,3]-[3,4] $-[0,1]-[0,2]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row;
		pos2.col = nowPos->col + 2;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[3,3] $-[0,1]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [3,1]-[3,2]-[3,3] [0,-1]-$-[0,1]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row;
		pos2.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [3,1]-[3,2] [0,1]-$
		pos1.row = nowPos->row;
		pos1.col = nowPos->col - 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 3
	case 3:

		// [3,2]-[4,3]-[5,4] $-[1,1]-[2,2]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row + 2;
		pos2.col = nowPos->col + 2;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[4,3] $-[1,1]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [2,1]-[3,2]-[4,3] [-1,-1]-$-[1,1]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row + 1;
		pos2.col = nowPos->col + 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [2,1]-[3,2] [-1,-1]-$
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col - 1;

		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 2
	case 2:

		// [3,2]-[4,2]-[5,2] $-[1,0]-[2,0]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col;
		pos2.row = nowPos->row + 2;
		pos2.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[4,2] $-[1,0]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [2,2]-[3,2]-[4,2] [-1,0]-$-[1,0]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col;
		pos2.row = nowPos->row + 1;
		pos2.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [2,2]-[3,2] [-1,0]-$
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 1
	case 1:

		// [3,2]-[4,1]-[5,0] $-[1,-1]-[2,-2]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row + 2;
		pos2.col = nowPos->col - 2;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[4,1] $-[1,-1]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [2,3]-[3,2]-[4,1] [-1,1]-$-[1,-1]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row + 1;
		pos2.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [2,3]-[3,2] [-1,1]-$
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col + 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 4
	case 4:

		// [3,2]-[3,1]-[3,0] $-[0,-1]-[0,-2]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row;
		pos2.col = nowPos->col - 2;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[3,1] $-[0,-1]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [3,3]-[3,2]-[3,1] [0,1]-$-[0,-1]
		pos1.row = nowPos->row;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row;
		pos2.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [3,3]-[3,2] [0,1]-$
		pos1.row = nowPos->row;
		pos1.col = nowPos->col + 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;

		// 7
	case 7:

		// [3,2]-[2,1]-[1,0] $-[-1,-1]-[-2,-2]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col - 1;
		pos2.row = nowPos->row - 2;
		pos2.col = nowPos->col - 2;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[0] = SearchPattern_A(board, nowPos, &pos1, &pos2);
		else
			point[0] = 0;
		// [3,2]-[2,1] $-[-1,-1]
		pos1.row = nowPos->row - 1;
		pos1.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[1] = SearchPattern_B(board, nowPos, &pos1);
		else
			point[1] = 0;
		// [4,3]-[3,2]-[2,1] [1,1]-$-[-1,-1]
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col + 1;
		pos2.row = nowPos->row - 1;
		pos2.col = nowPos->col - 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7) && (pos2.row > -1 && pos2.row < 7) && (pos2.col > -1 && pos2.col < 7)))
			point[2] = SearchPattern_C(board, nowPos, &pos1, &pos2);
		else
			point[2] = 0;
		// [4,3]-[3,2] [1,1]-$
		pos1.row = nowPos->row + 1;
		pos1.col = nowPos->col + 1;
		if (((pos1.row > -1 && pos1.row < 7) && (pos1.col > -1 && pos1.col < 7)))
			point[3] = SearchPattern_D(board, nowPos, &pos1);
		else
			point[3] = 0;
		break;
	}

	board->value[ToArrayNum(nowPos->row, nowPos->col)] = "";

	max = point[0];
	for (int i = 1; i < 4; i++)
	{
		if (max < point[i])
		{
			max = point[i];
		}
	}

	return max;
}

int CalcPoint(Board* board, Cell* nowPos, std::string value)
{
	int p = 0;

	int dir_8 = SearchPointByDirection(board, nowPos, value, 8);
	int dir_9 = SearchPointByDirection(board, nowPos, value, 9);
	int dir_6 = SearchPointByDirection(board, nowPos, value, 6);
	int dir_3 = SearchPointByDirection(board, nowPos, value, 3);
	int dir_2 = SearchPointByDirection(board, nowPos, value, 2);
	int dir_1 = SearchPointByDirection(board, nowPos, value, 1);
	int dir_4 = SearchPointByDirection(board, nowPos, value, 4);
	int dir_7 = SearchPointByDirection(board, nowPos, value, 7);

	/*
		std::cout << "[" << nowPos->row << "," << nowPos->col << "]" << std::endl;
		std::cout << "dir_8 = " << dir_8 << std::endl;
		std::cout << "dir_9 = " << dir_9 << std::endl;
		std::cout << "dir_6 = " << dir_6 << std::endl;
		std::cout << "dir_3 = " << dir_3 << std::endl;
		std::cout << "dir_2 = " << dir_2 << std::endl;
		std::cout << "dir_1 = " << dir_1 << std::endl;
		std::cout << "dir_4 = " << dir_4 << std::endl;
		std::cout << "dir_7 = " << dir_7 << std::endl << std::endl;
	*/

	if (dir_8 == dir_2)
		p += dir_8;
	else if (dir_8 > dir_2)
		p += dir_8;
	else
		p += dir_2;

	if (dir_9 == dir_1)
		p += dir_9;
	else if (dir_9 > dir_1)
		p += dir_9;
	else
		p += dir_1;

	if (dir_6 == dir_4)
		p += dir_6;
	else if (dir_6 > dir_4)
		p += dir_6;
	else
		p += dir_4;

	if (dir_3 == dir_7)
		p += dir_3;
	else if (dir_3 > dir_7)
		p += dir_3;
	else
		p += dir_7;

	return p;
}

void makeJsonMyPoint(Board* board, int row, int col, std::string value, int point, char* cpu_sashite)
{
	// Json書きだし
	char rowStr[8];
	char colStr[8];
	char pointStr[8];

	itoa(row, rowStr, 10);
	itoa(col, colStr, 10);
	itoa(point, pointStr, 10);

	strcpy(cpu_sashite, "{\"response_my\":{\"row\":");
	strcat(cpu_sashite, rowStr);
	strcat(cpu_sashite, ",\"col\":");
	strcat(cpu_sashite, colStr);
	strcat(cpu_sashite, ",\"value\":");

	if ("ko" == value)
		strcat(cpu_sashite, "\"ko\"");
	else
		strcat(cpu_sashite, "\"ne\"");

	strcat(cpu_sashite, ",\"point\":");
	strcat(cpu_sashite, pointStr);

	strcat(cpu_sashite, "}}");

	//nowPoint = board->evaluation[ToArrayNum(row, col)];

	//board->value[ToArrayNum(row, col)] = value;
	//board->evaluation[ToArrayNum(row, col)] = -1;
}

void GetPoint(Board* board, int *row, int *col, std::string value, int *point)
{
	int max = 0;
	int p1 = 0;
	int p2 = 0;

	Board* b1 = new Board();
	Board* b2 = new Board();

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			b1->value[ToArrayNum(i, j)] = board->value[ToArrayNum(i, j)];
			b2->value[ToArrayNum(i, j)] = board->value[ToArrayNum(i, j)];

			b1->evaluation[ToArrayNum(i, j)] = board->evaluation[ToArrayNum(i, j)];
			b2->evaluation[ToArrayNum(i, j)] = board->evaluation[ToArrayNum(i, j)];
		}
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (b1->value[ToArrayNum(i, j)] == "")
			{
				Cell* nowPos = new Cell(i, j);
				int num = ToArrayNum(i, j);

				int p = CalcPoint(b1, nowPos, "ko");

				if (-1 != b1->evaluation[ToArrayNum(i, j)])
					b1->evaluation[ToArrayNum(i, j)] = p;

				b1->value[ToArrayNum(i, j)] = "";

				delete(nowPos);
			}
			else
			{
				//b1->evaluation[ToArrayNum(i, j)] = -1;
			}

			if (b2->value[ToArrayNum(i, j)] == "")
			{
				Cell* nowPos = new Cell(i, j);
				int num = ToArrayNum(i, j);

				int p = CalcPoint(b2, nowPos, "ne");

				if (-1 != b2->evaluation[ToArrayNum(i, j)])
					b2->evaluation[ToArrayNum(i, j)] = p;

				b2->value[ToArrayNum(i, j)] = "";

				delete(nowPos);
			}
			else
			{
				//b2->evaluation[ToArrayNum(i, j)] = -1;
			}

			std::cout << "[" << i << "," << j << "]" << " : " << b1->evaluation[ToArrayNum(i, j)] << " : " << b2->evaluation[ToArrayNum(i, j)] << std::endl;
		}
	}

	int dec_row1 = 0;
	int dec_col1 = 0;
	int dec_row2 = 0;
	int dec_col2 = 0;

	std::cout << "********************************************************" << std::endl;
	std::cout << value << std::endl;
	if ("ko" == value)
	{
		p1 = b1->evaluation[ToArrayNum(*row, *col)];
		dec_row1 = *row;
		dec_col1 = *col;
		goto DECISION;
	}
	if ("ne" == value)
	{
		p2 = b2->evaluation[ToArrayNum(*row, *col)];
		dec_row2 = *row;
		dec_col2 = *col;
		goto DECISION;
	}


DECISION:

	if (p1 > p2)
	{
		*row = dec_row1;
		*col = dec_col1;
		*point = p1;

		board->evaluation[ToArrayNum(dec_row1, dec_col1)] = p1;

		std::cout << "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB" << std::endl;
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
	}
	else
	{
		*row = dec_row2;
		*col = dec_col2;
		*point = p2;

		board->evaluation[ToArrayNum(dec_row2, dec_col2)] = p2;

		std::cout << "BBBB" << std::endl;
		std::cout << p1 << std::endl;
		std::cout << p2 << std::endl;
	}

	delete(b1);
	delete(b2);
}



void SearchMovePoint(Board* board, char* cpu_sashite, int flag)
{
	int p = 0;

	int row = 0;
	int col = 0;
	std::string value = "";

	Board* b1 = new Board();
	Board* b2 = new Board();

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			b1->value[ToArrayNum(i, j)] = board->value[ToArrayNum(i, j)];
			b2->value[ToArrayNum(i, j)] = board->value[ToArrayNum(i, j)];

			b1->evaluation[ToArrayNum(i, j)] = board->evaluation[ToArrayNum(i, j)];
			b2->evaluation[ToArrayNum(i, j)] = board->evaluation[ToArrayNum(i, j)];
		}
	}

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (b1->value[ToArrayNum(i, j)] == "")
			{
				Cell* nowPos = new Cell(i, j);
				int num = ToArrayNum(i, j);

				p = CalcPoint(b1, nowPos, "ko");

				if (-1 != b1->evaluation[ToArrayNum(i, j)])
					b1->evaluation[ToArrayNum(i, j)] = p;

				b1->value[ToArrayNum(i, j)] = "";

				delete(nowPos);
			}
			else
			{
				//b1->evaluation[ToArrayNum(i, j)] = -1;
			}

			if (b2->value[ToArrayNum(i, j)] == "")
			{
				Cell* nowPos = new Cell(i, j);
				int num = ToArrayNum(i, j);

				p = CalcPoint(b2, nowPos, "ne");

				if (-1 != b2->evaluation[ToArrayNum(i, j)])
					b2->evaluation[ToArrayNum(i, j)] = p;

				b2->value[ToArrayNum(i, j)] = "";

				delete(nowPos);
			}
			else
			{
				//b2->evaluation[ToArrayNum(i, j)] = -1;
			}

			std::cout << "[" << i << "," << j << "]" << " : " << b1->evaluation[ToArrayNum(i, j)] << " : " << b2->evaluation[ToArrayNum(i, j)] << std::endl;
		}
	}

	int b1_targetMax = 0;
	int b1_targetNum = 0;

	int b2_targetMax = 0;
	int b2_targetNum = 0;

	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (b1->evaluation[ToArrayNum(i, j)] > b1_targetMax&& b1->evaluation[ToArrayNum(i, j)] > -1)
			{
				b1_targetNum = ToArrayNum(i, j);
				b1_targetMax = b1->evaluation[ToArrayNum(i, j)];
			}

			if (b2->evaluation[ToArrayNum(i, j)] > b2_targetMax&& b2->evaluation[ToArrayNum(i, j)] > -1)
			{
				b2_targetNum = ToArrayNum(i, j);
				b2_targetMax = b2->evaluation[ToArrayNum(i, j)];
			}
		}
	}

	if (b1_targetMax > b2_targetMax)
	{
		ToRowCol(b1_targetNum, &row, &col);
		value = "ko";
	}
	else
	{
		ToRowCol(b2_targetNum, &row, &col);
		value = "ne";
	}

	// Json書きだし
	char rowStr[8];
	char colStr[8];
	char pointStr[8];

	itoa(row, rowStr, 10);
	itoa(col, colStr, 10);
	if ("ko" == value)
	{
		itoa(b1->evaluation[ToArrayNum(row, col)], pointStr, 10);
		//GetPoint(b1, &row, &col, value, &p);
		nowPoint = p;
	}
	else
	{
		itoa(b2->evaluation[ToArrayNum(row, col)], pointStr, 10);
		//GetPoint(b2, &row, &col, value, &p);
		nowPoint = p;
	}

	strcpy(cpu_sashite, "{\"response_op\":{\"row\":");
	strcat(cpu_sashite, rowStr);
	strcat(cpu_sashite, ",\"col\":");
	strcat(cpu_sashite, colStr);
	strcat(cpu_sashite, ",\"value\":");

	if ("ko" == value)
		strcat(cpu_sashite, "\"ko\"");
	else
		strcat(cpu_sashite, "\"ne\"");

	strcat(cpu_sashite, ",\"point\":");
	strcat(cpu_sashite, pointStr);

	strcat(cpu_sashite, "}}");

	if (flag == 1)
	{
		board->value[ToArrayNum(row, col)] = value;
		board->evaluation[ToArrayNum(row, col)] = -1;
	}

	PrintBoard(b1);
	PrintBoard(b2);

	delete(b1);
	delete(b2);
}

int main()
{
    /*
    const char *json = "{\"match request\": \"OK\", \"size\": 3, \"board\": {\"1\": {\"row\": 3, \"col\": 3}, \"2\": {\"row\": 4, \"col\": 4}, \"3\": {\"row\": 5, \"col\": 5}}}";

    char element[1024];

    std::map<std::string, std::string> p;

    ParseJson(json, element, &p);
    */

	Board* board = new Board();

    WSADATA wsaData;
    SOCKET sock0;
    struct sockaddr_in addr;
    struct sockaddr_in client;
    int len;
    SOCKET sock;
    int n;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        return 1;
    }

    sock0 = socket(AF_INET, SOCK_STREAM, 0);
    if (sock0 == INVALID_SOCKET) {
        printf("socket : %d\n", WSAGetLastError());
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(54949);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(sock0, (struct sockaddr*) & addr, sizeof(addr)) != 0) {
        printf("bind : %d\n", WSAGetLastError());
        return 1;
    }

    if (listen(sock0, 5) != 0) {
        printf("listen : %d\n", WSAGetLastError());
        return 1;
    }

    len = sizeof(client);
    sock = accept(sock0, (struct sockaddr*) & client, &len);
    if (sock == INVALID_SOCKET) {    
        printf("accept : %d\n", WSAGetLastError());
        return 0;        
    }

    int cnt = 1;

    while (1) {

		int myPoint = 0;

        std::map<std::string, std::string> p;

        char buffer[1024];
        char recvJson[1024];

        memset(buffer, 0, sizeof(1024));

        int fileNameLength = recv(sock, buffer, 1024, 0);

        int i;
        for (i = 0; i < fileNameLength; i++)
            recvJson[i] = buffer[i];

        recvJson[++i] = '\0';

        JsonEntry(recvJson, &p, board);

		char response[1024];

		int responseLen;

        if(cnt == 1)
        {
            char sendJson[4096];
            makeJson(sendJson, board);

            std::cout << sendJson << std::endl;

            responseLen = std::strlen(sendJson);
            send(sock, sendJson, responseLen, 0);

			SearchMovePoint(board, response, 0);
        }
		/*
        else if (cnt == 4)
        {
            send(sock, "disconnect", 10, 0);
        }
		*/
		else
		{
			SearchMovePoint(board, response, 0);

			std::cout << "shironeko" << std::endl;
			PrintBoard(board);

			try
			{
				std::string rowS = p.at("row");
				std::string colS = p.at("col");
				std::string valS = p.at("value");

				std::cout << "CcccccCcccc = " << valS << std::endl;

				int r = atoi(rowS.c_str());
				int c = atoi(colS.c_str());
				int p = 0;

				GetPoint(board, &r, &c, valS, &p);

				myPoint = board->evaluation[ToArrayNum(r, c)];

				myPoint = p;

				board->value[ToArrayNum(r, c)] = valS;

				std::cout << "kuroneko" << std::endl;
				PrintBoard(board);
				DebugMasu(board);

				//board->evaluation[ToArrayNum(r, c)] = -1;


			}
			catch (std::exception & e)
			{
				std::cout << e.what() << std::endl;
			}

			memset(response, 0, sizeof(1024));

			std::string rowS = p.at("row");
			std::string colS = p.at("col");
			std::string valS = p.at("value");

			

			board->value[ToArrayNum(atoi(rowS.c_str()), atoi(colS.c_str()))] = valS;

			//myPoint = board->evaluation[ToArrayNum(atoi(rowS.c_str()), atoi(colS.c_str()))];

			board->evaluation[ToArrayNum(atoi(rowS.c_str()), atoi(colS.c_str()))] = -100;

			//std::cout << "rowS = " << rowS << " : " << "colS = " << colS << " : " << "valS = " << valS << std::endl;

			makeJsonMyPoint(board, atoi(rowS.c_str()), atoi(colS.c_str()), valS, myPoint, response);

			responseLen = std::strlen(response);

			//std::cout << response << std::endl;

			send(sock, response, responseLen, 0);
		}

		std::cout << "Second" << std::endl;

        memset(buffer, 0, sizeof(1024));

        fileNameLength = recv(sock, buffer, 1024, 0);

        i;
        for (i = 0; i < fileNameLength; i++)
            recvJson[i] = buffer[i];

        recvJson[++i] = '\0';

		if (cnt != 1)
		{
			//JsonEntry(recvJson, &p, board);

			SearchMovePoint(board, response, 1);

			std::cout << response << std::endl;

			responseLen = std::strlen(response);

			send(sock, response, responseLen, 0);
		}
		else
		{
			send(sock, "OK", 2, 0);
		}

TO_TWO:
		
		cnt++;
    } 

	delete(board);

    closesocket(sock);

    WSACleanup();

    return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

/*

    std::cout << "Hello World!\n";

    HANDLE hFind;
    WIN32_FIND_DATA win32fd;

    TCHAR buf[1024] = "D:\\vs2019\\Transfer\\Debug\\こねこ.txt";

    hFind = FindFirstFile(buf, &win32fd);

    if (hFind == INVALID_HANDLE_VALUE) {
        return 1;
    }

    do {
        if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            printf("%s (DIR)\n", win32fd.cFileName);
        }
        else {
            printf("%s\n", win32fd.cFileName);
        }
    } while (FindNextFile(hFind, &win32fd));

    FindClose(hFind);

*/

// default, sente, goteの評価値を変える

/*

Arequest:match request
Bresponse:ok,size:12:

Arequest:{row:3,col:3,value:ko}
Bresponse:{row:3,col:3,value:ko,point:5}

Awaiting:ok
Bresponse:{row:4,col:4,value:ko,point:5}


*/