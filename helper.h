#include <sstream>

// State class
enum class State { kObstecal, kEmpty, kPath, kStart, kFinish, kVisited, kClose, kTem };

class Node {
public:
	int x, y;
	State state;
	bool visited;

	int parent_x;
	int parent_y;

	int G;
	int H;
	int cost;

	Node(int _x, int _y, State _state, int px, int py, int g =0, int h =0) {
		x = _x;
		y = _y;
		state = _state;
		parent_x = px;
		parent_y = py;
		visited = false;
		G = g;
		H = h;
		if (state == State::kObstecal)
		{
			cost = 255;
		}
		else
		{
			cost = 0;
		}
	}
};

vector<State> ParseLine(string line) {
	istringstream sline(line);
	int n;
	char c;
	vector<State> row;
	while (sline >> n >> c && c == ',')
	{
		if (n == 0)
		{
			row.push_back(State::kEmpty);
		}
		else {
			row.push_back(State::kObstecal);
		}
	}

	return row;
}

vector<vector<State>> ReadBoard(string path) {
	ifstream myfile(path);
	vector<vector < State >> board;
	if (myfile)
	{
		string line;
		while (getline(myfile, line))
		{
			auto row = ParseLine(line);
			board.push_back(row);
		}
	}
	return board;
}

vector<vector<Node>> CreateGrid(vector<vector<State>> board) {
	vector<vector<Node>> grid;
	for (int x = 0; x < board.size(); x++)
	{
		vector<Node> row;
		for (size_t y = 0; y < board[x].size(); y++)
		{
			auto node = Node(x, y, board[x][y], 0, 0);
			row.push_back(node);
		}
		grid.push_back(row);
	}

	return grid;
}

string CallString(State node) {
	switch (node)
	{
	case State::kObstecal:
		return "\033[33mB";
		break;
	case State::kPath:
		return "\033[32m*";
		break;
	case State::kStart:
		return "\033[31mS";
		break;
	case State::kFinish:
		return "\033[31mF";
		break;
	case State::kVisited:
		return "\033[35mB";
		break;
	default:
		return "\033[37m0";
		break;
	}
}

void PrintGrid(vector < vector<Node>> grid) {
	for (int x = 0; x < grid.size(); x++)
	{
		for (int y = 0; y < grid[x].size(); y++)
		{
			cout << CallString(grid[x][y].state);
		}
		cout << "\n";
	}
}

void PrintGridCost(vector < vector<Node>> grid) {
	for (int x = 0; x < grid.size(); x++)
	{
		for (int y = 0; y < grid[x].size(); y++)
		{
			cout << grid[x][y].cost <<" " ;
		}
		cout << "\n";
	}
}