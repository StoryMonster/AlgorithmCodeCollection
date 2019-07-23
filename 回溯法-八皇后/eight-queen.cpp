#include <iostream>
#include <cmath>

struct Node
{
    int x = -1;
    int y = -1;
};

bool isValid(Node nodes[], int lvl)
{
    for (int i = lvl; i >= 1; --i)
    {
        for (int j = i-1; j >= 0; --j)
        {
            if (nodes[i].x == nodes[j].x or nodes[i].y == nodes[j].y) { return false;}
            if (abs(nodes[i].x - nodes[j].x) == abs(nodes[i].y - nodes[j].y)) { return false;}
        }
    }
    return true;
}

void print(Node nodes[], int lvl)
{
    for (int i = 0; i <= lvl; ++i)
    {
        std::cout << "(" << nodes[i].x << ", " << nodes[i].y << ")" << std::endl;
    }
}

bool doSearch(Node nodes[], int N, int lvl)
{
    for (int i = 0; i < N; ++i)
    {
        nodes[lvl].y = lvl;
        nodes[lvl].x = i;
        bool res = isValid(nodes, lvl);
        if (res)
        {
            if (lvl < N-1)
            {
                if (doSearch(nodes, N, lvl+1)) { return true; }
            }
            else
            {
                print(nodes, lvl);
                return true;
            }
        }
    }
    return false;
}

int main()
{
    int N = 8;
    Node nodes[N] = {};
    if (not doSearch(nodes, N, 0))
    {
        std::cout << "No answer" << std::endl;
    }
}