#include <array>
#include <cassert>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

void testRegionsBySlashes() {
  class Solution {
   public:
    static int regionsBySlashes(const vector<string> &grid) {
      constexpr auto MAX_GROUPS{32};

      array<unsigned char, MAX_GROUPS> groups{};
      for (auto i = 0; i < grid[0].length(); i++)
        groups[i] = static_cast<unsigned char>(i);

      int count{static_cast<int>(grid[0].length())};
      for (const auto &row : grid) {
        array<unsigned char, MAX_GROUPS * 2> groupMap{};
        array<list<unsigned char>, MAX_GROUPS * 2> groupMapHelper{};
        unsigned char group{};
        unsigned char newGroup = MAX_GROUPS;
        int mappedFromCount{};
        int mappedToCount{};
        const auto mapTo = [&](unsigned char from, unsigned char to) {
          const auto mapped = groupMap[from] - 1;
          if (mapped == -1) {
            mappedFromCount++;
            groupMap[from] = static_cast<unsigned char>(to + 1);
            if (groupMapHelper[to].empty()) mappedToCount++;
            groupMapHelper[to].push_back(from);
          } else if (mapped != to) {
            for (auto m : groupMapHelper[mapped])
              groupMap[m] = static_cast<unsigned char>(to + 1);
            if (!groupMapHelper[to].empty()) mappedToCount--;
            groupMapHelper[to].splice(groupMapHelper[to].cbegin(),
                                      groupMapHelper[mapped]);
          }
        };

        for (auto i = 0; i < row.length(); i++) {
          const unsigned char c = row[i];
          const unsigned char oldGroup = groups[i];
          if (c == '\\') {
            count++;
            groups[i] = newGroup++;
            mapTo(groups[i], group);
            mapTo(oldGroup, ++group);
          } else if (c == '/') {
            count++;
            groups[i] = newGroup++;
            mapTo(oldGroup, group);
            mapTo(groups[i], ++group);
          } else
            mapTo(oldGroup, group);
        }

        count += mappedToCount - mappedFromCount;

        for (auto i = 0; i < row.length(); i++) groups[i] = groupMap[groups[i]];
      }
      return count;
    }
  };

  cout << Solution::regionsBySlashes(
              {"\\ \\//\\\\\\/\\\\\\ //\\\\/\\\\\\ /  //\\\\\\",
               "\\\\\\/\\/    \\\\\\/\\/ \\\\\\\\/\\ ///// ",
               "/\\  / \\/   /\\\\ // \\///\\\\// /\\\\",
               "// / \\/\\\\/// // /\\/\\/\\//\\/\\\\\\/",
               "//\\///\\ \\\\/\\\\/// // \\\\\\// /\\\\ ",
               "\\////\\    \\/\\//\\\\\\// ///\\//\\ \\",
               "\\///\\\\/ \\//\\/\\\\\\\\ //\\/\\\\/ //\\\\",
               "//\\\\\\/\\ // \\////\\\\ ////\\\\\\//  ",
               "// \\  /\\\\/\\ /\\/\\//\\\\/\\/ \\/\\//\\",
               " \\\\/\\ \\\\//\\\\/ \\\\/\\/\\\\////   //",
               " / \\\\/\\ \\//\\\\\\////\\\\ /// /\\\\  ",
               "//  \\\\\\\\/\\\\/\\/\\\\ / \\/\\\\\\ \\\\\\\\\\",
               "/ /\\///  /\\\\\\ //\\/\\/\\\\//\\\\\\   ",
               "\\\\\\\\/\\/\\\\/\\\\\\\\\\//////\\/\\\\\\///\\",
               "/ \\\\\\ ///\\ \\\\ \\/\\/\\ /\\/\\\\\\\\\\//",
               "\\ \\\\// \\ /\\\\\\/////\\//////\\/\\\\/",
               " \\/\\\\\\ /\\\\\\\\\\    \\\\//\\\\\\\\//\\/\\",
               "// ///\\/\\\\  // \\\\\\/\\\\  /\\\\ /\\/",
               "/\\/ /\\\\\\////\\  \\//   \\\\\\////\\\\",
               "\\\\/ /   \\  /\\ //\\//\\/\\\\ \\//\\/\\",
               "/\\\\\\/\\\\ \\  \\\\ \\/ \\//\\\\\\/\\\\/\\/\\",
               "\\ /\\ //\\\\\\// \\\\ /\\/\\\\\\\\   \\//\\",
               "  \\\\\\\\///\\ \\/ \\//\\\\ //\\/  / \\\\",
               "\\\\/\\\\/ \\ \\/ //\\////\\\\\\ ////\\\\\\",
               "/\\/////\\/\\/\\/ /////\\ \\//////  ",
               "\\/\\/ // \\ \\ \\//\\\\\\\\\\/\\\\///\\\\/\\",
               "/\\/ / \\ //\\/\\\\ \\//\\//\\\\///\\\\  ",
               "\\//\\\\/  //\\//\\\\\\\\\\/\\ \\/\\\\//\\  ",
               "\\//  /   \\//\\\\\\\\//\\/\\// \\/\\\\\\\\",
               "//\\\\/// /\\/////\\\\ /  / \\/\\ / \\"})
       << endl;
}

void testRegionsBySlashes2() {
  class Solution {
    vector<int> parent;
    vector<int> rank;
    int count;

   public:
    int regionsBySlashes(const vector<string> &grid) {
      int n = grid.size();
      int dots = n + 1;
      parent.resize(dots * dots);
      rank.resize(dots * dots, 1);
      count = 0;

      // Initialize Union-Find structure
      for (int i = 0; i < parent.size(); ++i) {
        parent[i] = i;
      }

      // Connect boundaries to the top-left corner (0,0)
      for (int i = 0; i < dots; ++i) {
        for (int j = 0; j < dots; ++j) {
          if (i == 0 || j == 0 || i == n || j == n) {
            int cell = i * dots + j;
            unionSets(0, cell);
          }
        }
      }

      // Process each cell and connect regions based on slashes
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
          if (grid[i][j] == '\\') {
            int cell1 = i * dots + j;
            int cell2 = (i + 1) * dots + (j + 1);
            unionSets(cell1, cell2);
          } else if (grid[i][j] == '/') {
            int cell1 = (i + 1) * dots + j;
            int cell2 = i * dots + (j + 1);
            unionSets(cell1, cell2);
          }
        }
      }

      return count;
    }

   private:
    void unionSets(int a, int b) {
      int parentA = find(a);
      int parentB = find(b);
      if (parentA == parentB) {
        count++;
      } else {
        if (rank[parentA] > rank[parentB]) {
          parent[parentB] = parentA;
        } else if (rank[parentA] < rank[parentB]) {
          parent[parentA] = parentB;
        } else {
          parent[parentB] = parentA;
          rank[parentA]++;
        }
      }
    }

    int find(int a) {
      if (parent[a] == a) {
        return a;
      }
      return parent[a] = find(parent[a]);
    }
  };

  cout << Solution().regionsBySlashes({"  ", "  "}) << endl;;
}

int main() {
  testRegionsBySlashes2();
  return 0;
}
