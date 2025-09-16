#include <iostream>
#include <vector>

using namespace std;

/*
非常朴素的思路，不再详细叙述， 20250916 */
class Solution{
public:
    int mathPathSum(vector<vector<int>>& grid){
        int m = grid.size();
        int n = grid[0].size();
        //dp[i][j]的含义是到点(i,j)的最小数字总和
        vector<vector<int>> dp(m,vector<int>(n,-1));

        int prefix = 0;
        //初始化第一行
        for(int j=0;j<n;j++){
            prefix += grid[0][j];
            dp[0][j] = prefix;
        }

        //初始化第一列
        prefix = 0;
        for(int i=0;i<m;i++){
            prefix += grid[i][0];
            dp[i][0] = prefix;
        }

        //遍历顺序
        for(int i=1;i<m;i++){
            for(int j=1;j<n;j++){
                dp[i][j] = min(dp[i-1][j],dp[i][j-1]) + grid[i][j];
            }
        }

        print(dp);

        return dp[m-1][n-1];
    }

    void print(vector<vector<int>>& dp){
        int m = dp.size();
        int n = dp[0].size();
        for(int i=0;i<m;i++){
            for(int j=0;j<n;j++){
                std::cout << dp[i][j] << " ";
            }

            std::cout << std::endl;
        }
    }

};

int main(){
    vector<vector<int>> test({{1,3,1},{1,5,1},{4,2,1}});
    Solution solution;

    int res = solution.mathPathSum(test);
    std::cout << "The ans : " << res << std::endl;
    return 0;
}