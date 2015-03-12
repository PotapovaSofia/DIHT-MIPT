#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <set>
#include <map>
#include <queue>
#include <memory.h>
#include <cmath>

using namespace std;

#define se second
#define fi first
#define mp make_pair
#define pb push_back

typedef pair <int, int> pii;

const int dx[4] = {-1, 0, 0, 1};
const int dy[4] = {0, 1, -1, 0};

const int maxn = 1005;
const int inf = 1e9;

queue <pii> order;
pii start;
bool passable[maxn][maxn]; //проходимый
int n, m;
int d[maxn][maxn]; //расстояние
pii fr[maxn][maxn]; //откуда пришел
vector <pii> edges[maxn][maxn];

void solve() {
    order.push(start);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            d[i][j] = inf;
    d[start.fi][start.se] = 0;

    while(!order.empty()) {
        pii v = order.front();
        order.pop();
        for (int j = 0; j < edges[v.fi][v.se].size(); j++) {
            pii u = edges[v.fi][v.se][j];
            int newd = d[v.fi][v.se] + 1;
            if (d[u.fi][u.se] > newd) {
                d[u.fi][u.se] = newd;
                order.push(u);
                fr[u.fi][u.se] = v;
            }
        }
    }
}

int main() {

    scanf("%d%d", &n, &m);

    scanf("%d%d", &start.fi, &start.se);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            scanf("%d", &passable[i][j]);
            passable[i][j] = 1 - passable[i][j];
        }
    int jumps_cnt;
    scanf("%d", &jumps_cnt);
    for (int jump = 0; jump < jumps_cnt; jump++) {
        pii fr, to;
        scanf("%d%d%d%d", &fr.fi, &fr.se, &to.fi, &to.se);
        edges[fr.fi][fr.se].pb(to);
    }
    for (int i = 0; i <= n + 1; i++)
        passable[i][0] = passable[i][m + 1] = false;
    for (int j = 0; j <= m + 1; j++)
        passable[0][j] = passable[n + 1][j] = false;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (passable[i][j])
                for (int move = 0; move <= 3; move++) {
                    int newx = i + dx[move];
                    int newy = j + dy[move];
                    if (passable[newx][newy])
                        edges[i][j].pb(mp(newx, newy));
                }
    solve();
    int exits_cnt;
    scanf("%d", &exits_cnt);
    int answer = inf;
    pii best;
    for (int j = 0; j < exits_cnt; j++) {
        pii ex;
        scanf("%d%d", &ex.fi, &ex.se);
        if (d[ex.fi][ex.se] < answer) {
            answer = d[ex.fi][ex.se];
            best = ex;
        }
    }

    if (answer == inf) {
        printf("Impossible");
        exit(0);
    }

    vector <pii> list;
    while(best != start) {
        list.pb(best);
        pii f = fr[best.fi][best.se];
        best = f;
    }
    list.pb(start);
    printf("%d\n", list.size());
    for (int j = list.size() - 1; j >= 0; j--)
        printf("%d %d\n", list[j].fi, list[j].se);
    return 0;
}