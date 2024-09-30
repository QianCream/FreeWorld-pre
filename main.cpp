/*
Copyright [C] 2024 Armand FreeWorld pre-alpha
All right reserved
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>
#include <queue>

void clearScreen() {
#ifdef _WIN32
	system("cls"); // Windows 系统
#else
	system("clear"); // Unix/Linux 系统
#endif
}

#define VERSION "pre" // 版本号
#define AUTHOR "Armand" // 作者
#define COPYRIGHT "Copyright [C] 2024 Armand FreeWorld " + VERSION // 版权信息

namespace FreeWorld {
	using namespace std;

	map<int, string> colours = {
		{0, "\033[48;5;12m"},
		{1, "\033[48;2;192;192;192m"},
	}; // 颜色lookup表

	int MAP_WIDTH = 20; // 地图宽度
	int MAP_HEIGHT = 256; // 地图高度

	const int PRINT_WIDTH = 20; // 打印宽度
	const int PRINT_HEIGHT = 20; // 打印高度

	int player_x = 0; // 玩家x坐标
	int player_y = 0; // 玩家y坐标

	int terrain_height = MAP_HEIGHT / 2; // 地形高度
	vector<vector<int>> game_map(MAP_HEIGHT, vector<int>(MAP_WIDTH, 0)); // 地图数据
	auto new_game_map = game_map; // 地图副本

	queue<string> debug_queue; // 调试信息队列

	int getRandomSeed(int range) {
		return rand() % range;
	} // 随机种子生成器

	void setBornPoint() {
		player_y = 20;
		player_x = MAP_HEIGHT - 1;

		while (game_map[player_x][player_y] == 1) {
			player_x--;
		}
	} // 设置出生点

	pair<vector<vector<int>>, int> terrainGenerator(int seed, int new_terrain_height) {
		vector<vector<int>> new_game_map(MAP_HEIGHT, vector<int>(MAP_WIDTH, 0));
		srand(seed);
		for (int i = MAP_WIDTH - 1; i >= 0; i--) {
			switch (getRandomSeed(2)) {
			case 0:
				new_terrain_height -= getRandomSeed(3);
				break;
			case 1:
				new_terrain_height += getRandomSeed(3);
				break;
			}
			for (int j = MAP_HEIGHT - 1; j > new_terrain_height; j--) {
				new_game_map[j][i] = 1;
			}
		}

		return make_pair(new_game_map, new_terrain_height);

	} // 地形生成器

	void printWholeMap() {
		for (int i = 0; i < MAP_HEIGHT; i++) {
			for (int j = 0; j < MAP_WIDTH; j++) {
				cout << colours[game_map[i][j]] << "  \033[0m";
			}
			cout << endl;
		}
	} // 测试：打印全部地图

	void printMap() {
		int startX = player_x - PRINT_HEIGHT / 2;
		int startY = player_y - PRINT_WIDTH / 2;
		int endX = player_x + PRINT_HEIGHT / 2;
		int endY = player_y + PRINT_WIDTH / 2;

		for (int i = startX; i < endX; i++) {
			for (int j = startY; j < endY; j++) {
				if (player_x == i && player_y == j) {
					cout << colours[game_map[i][j]] << "！\033[0m"; // 玩家位置
				}
				else {
					cout << colours[game_map[i][j]] << "  \033[0m"; // 其他地图部分
				}
			}
			cout << endl;
		}
	} // 打印以玩家为中心的10x10地图

	void attendDebug(string info) {
		debug_queue.push(info);
	} // 调试信息记录器

	void printDebug(queue<string> temp_debug_queue) {
		cout << "\n----------Debug info:-----------" << endl;
		while (!temp_debug_queue.empty()) {
			cout << temp_debug_queue.front() << endl;
			temp_debug_queue.pop();
		}
		cout << "-------------End Debug------------" << endl;
	} // 调试信息打印器

	void shut() {
		cout << "任意键继续>>>";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cin.get();
	}

	void gameStart() {
		cout << "欢迎来到FreeWorld " << VERSION << "！" << endl;
		cout << "作者：" << AUTHOR << endl;
		shut();

		clearScreen();
	} // 游戏开始

	void mainLoop() {
		for (int i = 0; i < 2; i++) {
			MAP_WIDTH += 20;
			tie(game_map, terrain_height) = terrainGenerator(11, terrain_height);
			for (int j = 0; j < MAP_HEIGHT; j++) {
				for (auto it = new_game_map[j].begin(); it != new_game_map[j].end(); ++it) {
					game_map[j].push_back(*it);
				}
			}
		}
		setBornPoint();

		while (true) {
			printMap();
			string input;
			cin >> input;
			if (input == "w") {
				player_x--;
				attendDebug("玩家向上行进一个单位。");
			}
			else if (input == "s") {
				player_x++;
				attendDebug("玩家向下行进一个单位。");
			}
			else if (input == "a") {
				player_y--;
				attendDebug("玩家向左行进一个单位。");
			}
			else if (input == "d") {
				player_y++;
				attendDebug("玩家向右行进一个单位。");
			}
			else if (input == "/debug") {
				attendDebug("显示调试信息。");
				printDebug(debug_queue);
				shut();
			}

			clearScreen();
		}
	} // 游戏主循环

}

int main(int argc, char* argv[]) {
	try {
		FreeWorld::mainLoop();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		FreeWorld::printDebug(FreeWorld::debug_queue);
		return 1;
	}
	return 0;
}
