#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <map>
#include <algorithm>

#define R 6371
#define toRadians(x) ((x) * M_PI / 180.0)
#define INF 1E18

using namespace std;

double calc_distance(double lat1, double long1, double lat2, double long2) {
    lat1 = toRadians(lat1);
    long1 = toRadians(long1);
    lat2 = toRadians(lat2);
    long2 = toRadians(long2);
    double dlong = long2 - long1;
    double dlat = lat2 - lat1;

    double ans = pow(sin(dlat / 2), 2) +
                 cos(lat1) * cos(lat2) *
                 pow(sin(dlong / 2), 2);
    ans = 2 * asin(sqrt(ans)) * R;
    return ans;
}

void showD(vector<vector<double>>  distance, vector<int> cols, vector<int> rows) {
    /*printf("D:\n");
    int n = distance.size();
    for (int i = 0; i < n; ++i) {
        printf("%d ", cols[i]);
    }
    printf("\n");
    for (int i = 0; i < n; ++i) {
        for (int j = -1; j < n; ++j){
            if (j == -1) printf("%d ", rows[i]);
            else{
                if (distance[i][j] > 1E9) printf("INFINITY ");
                else printf("%f ", distance[i][j]);
            }
        }
        printf("\n");
    }

    printf("*******************\n\n");*/
}


class Points{
public:
    vector<vector<double>> GPS;
    vector<vector<double>> distance;
    vector<int> road;
    int speed = 8;//средняя скорость дрона в км в ч
    double len = 0;
    double time = 0;//время в пути в часах

    int n;

    Points(vector<vector<double>> point) {
        GPS = point;
        n = GPS.size();
        distance.resize(n, vector<double>(n));
    }

    void showGps() {
        printf("gps:\n");
        for (int i = 0; i < n; ++i) {
            printf("%f %f\n", GPS[i][0], GPS[i][1]);
        }

        printf("*******************\n\n");
    }

    void showDistance() {
        printf("distance:\n");
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j){
                if (distance[i][j] > 1E9) printf("INFINITY ");
                else printf("%f ", distance[i][j]);
            }
            printf("\n");
        }

        printf("*******************\n\n");
    }

    void makeDistance() {
        for(int i = 0; i < n; ++i) {
            for(int j = i+1; j < n; ++j) {
                double dst = calc_distance(GPS[i][0], GPS[i][1], GPS[j][0], GPS[j][1]);
                distance[i][j] = dst;
                distance[j][i] = dst;
                distance[i][i] = INF;
                distance[j][j] = INF;
            }
        }
    }

    void makeRoad() {
        vector<vector<double>> d = distance;
        vector<int> cols;
        vector<int> rows;
        vector<int> marshrut;
        vector<vector<int>> left_points;
        map<int, int> paths;

        for(int i = 0; i < d.size(); ++i) {
            cols.push_back(i);
            rows.push_back(i);
        }

        while (cols.size() > 1){
            int m = d.size();
            vector<double> min_col(m);
            vector<double> min_row(m);

            for(int i = 0; i < m; ++i) {
                min_row[i] = *min_element(d[i].begin(), d[i].end());
                for (int j = 0; j < m; ++j){
                    d[i][j] -= min_row[i];
                }
            }
            //showD(d, cols, rows);

            for(int i = 0; i < m; ++i) {
                double min_t = INF;
                for (int j = 0; j < m; ++j){
                    if (d[j][i] < min_t) min_t = d[j][i];
                }
                min_col[i] = min_t;
                for (int j = 0; j < m; ++j){
                    d[j][i] -= min_col[i];
                }
            }
            //showD(d, cols, rows);

            int ch = 0;
            double max_t = 0;
            int max_t_ch = 0;
            for(int i = 0; i < m; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (d[i][j] == 0) {
                        double min_col_t = INF;
                        double min_row_t = INF;
                        for (int k = 0; k < m; ++k) {
                            if (k != j) {
                                if (d[i][k] < min_row_t) min_row_t = d[i][k];
                            }
                        }
                        for (int k = 0; k < m; ++k) {
                            if (k != i) {
                                if (d[k][j] < min_col_t) min_col_t = d[k][j];
                            }
                        }
                        double sum_t = min_col_t + min_row_t;
                        if (sum_t >= max_t) {
                            max_t = sum_t;
                            max_t_ch = ch;
                        }
                        ++ch;
                    }
                }
            }
            //cout << max_t_ch << " " << max_t << endl;
            ch = 0;
            bool br = false;
            for(int i = 0; i < m; ++i) {
                for (int j = 0; j < m; ++j) {
                    if (d[i][j] == 0) {
                        if (ch == max_t_ch) {
                            if (marshrut.size() == 0){
                                marshrut.push_back(rows[i]);
                                marshrut.push_back(cols[j]);
                            } else {
                                if (cols[j] == marshrut.front()) {
                                    marshrut.insert(marshrut.begin(), rows[i]);
                                }
                                else if (rows[i] == marshrut.back()) {
                                    marshrut.push_back(cols[j]);
                                }
                                else {
                                    left_points.push_back({rows[i], cols[j]});
                                }
                                bool is_left = true;
                                while (is_left){
                                    is_left = false;
                                    //cout << endl << left_points.size() << endl;
                                    for(int l = 0; l < left_points.size(); ++l) {
                                        if (left_points[l][1] == marshrut.front()) {
                                            marshrut.insert(marshrut.begin(), left_points[l][0]);
                                            left_points.erase(left_points.begin() + l);
                                            is_left = true;
                                            break;
                                        }
                                        else if (left_points[l][0] == marshrut.back()) {
                                            marshrut.push_back(left_points[l][1]);
                                            left_points.erase(left_points.begin() + l);
                                            is_left = true;
                                            break;
                                        }
                                    }
                                }
                            }
                            //cout << rows[i] << " " << cols[j] << endl;

                            paths[rows[i]] = cols[j];
                            /*for (pair<int, int> pathh : paths) {
                                cout << "{" << pathh.first << " ; " << pathh.second << "}" << endl;
                            }*/

                            int del_c = -1;
                            int del_r = -1;
                            for (int q = 0; q < m; ++q){
                                if (cols[q] == rows[i]) {
                                    del_c = q;
                                }
                            }
                            for (int q = 0; q < m; ++q){
                                if (rows[q] == cols[j]) {
                                    del_r = q;
                                }
                            }
                            if (del_r >= 0 and del_c >= 0) d[del_r][del_c] = INF;


                            bool bre = true;
                            while (bre) {
                                //cout << "CYCLEE" << endl;
                                map<int, int> paths_copy = paths;
                                for (pair<int, int> pathh : paths) {
                                    bre = false;
                                    for (pair<int, int> pathc : paths_copy) {
                                        //cout << "[" << pathh.first << " ; " << pathc.second << "]" << endl;
                                        if (pathh.first == pathc.second) {
                                            //cout << "CYCLEE" << endl;
                                            paths[pathc.first] = pathh.second;

                                            int del_c = -1;
                                            int del_r = -1;
                                            for (int q = 0; q < m; ++q){
                                                if (cols[q] == pathc.first) {
                                                    del_c = q;
                                                }
                                            }
                                            for (int q = 0; q < m; ++q){
                                                if (rows[q] == pathh.second) {
                                                    del_r = q;
                                                }
                                            }
                                            if (del_r >= 0 and del_c >= 0) d[del_r][del_c] = INF;

                                            del_c = -1;
                                            del_r = -1;
                                            for (int q = 0; q < m; ++q){
                                                if (cols[q] == pathh.second) {
                                                    del_c = q;
                                                }
                                            }
                                            for (int q = 0; q < m; ++q){
                                                if (rows[q] == pathc.first) {
                                                    del_r = q;
                                                }
                                            }
                                            if (del_r >= 0 and del_c >= 0) d[del_r][del_c] = INF;

                                            bre = true;
                                            break;
                                        }
                                    }
                                    if (bre) break;
                                }
                            }

                            d.erase(d.begin() + i);
                            cols.erase(cols.begin() + j);
                            rows.erase(rows.begin() + i);
                            for (int k = 0; k < m-1; ++k) {
                                d[k].erase(d[k].begin() + j);
                            }
                            br = true;
                            break;
                        }
                        ++ch;
                    }
                }
                if (br) break;
            }

            //showD(d, cols, rows);
            /*for(int i = 0; i < marshrut.size(); ++i)
                cout << marshrut[i] << " ";
            cout << endl << endl << endl << endl;*/
            //getchar();
        }
        //cout << "while end" << endl << cols[0] << " " << rows[0] << endl;

        /*for(int i = 0; i < marshrut.size(); ++i)
            cout << marshrut[i] << " ";*/

        //vector<int> mm = marshrut;

        if (cols[0] == marshrut.front()) {
            //reverse( marshrut.begin(), marshrut.end() );
            marshrut.insert(marshrut.begin(), rows[0]);
        }
        else if (rows[0] == marshrut.back()) {
            marshrut.push_back(cols[0]);
        }



        /* for(int i = 0; i < marshrut.size(); ++i) {}
             cout << marshrut[i] << " ";
         cout << endl;
         cout << "YEAHHH";*/


        if (marshrut[0] != 0) {
            int nul;
            for(int i = 0; i < marshrut.size(); ++i) {
                if (marshrut[i] == 0) {
                    nul = i;
                    break;
                }
            }
            for(int i = nul; i < marshrut.size(); ++i) {
                road.push_back(marshrut[i]);
            }
            for(int i = 1; i < nul; ++i) {
                road.push_back(marshrut[i]);
            }
            road.push_back(0);
        } else {
            road = marshrut;
        }



        for(int i = 0; i < road.size() - 1; ++i) {
            len += distance[road[i]][road[i+1]];
        }
        time = len / speed;
    }

    void showRoad(){
        printf("time:\n");
        cout << time << endl;
        printf("*******************\n\n");
        printf("route:\n");
        for (int number : road)
            cout << number << " ";
        printf("\n*******************\n\n");

    }
};


int main()
{
    vector<vector<double>> gps = {{59.458953, 30.316287},
                                  {59.515355, 30.368472},
                                  {59.518670, 30.474215},
                                  {59.437800, 30.488635},
                                  {59.426781, 30.423747}};

    Points test(gps);
    test.showGps();
    test.makeDistance();
    test.showDistance();
    test.makeRoad();
    test.showRoad();

    return 0;
}
