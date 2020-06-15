#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <algorithm>
using namespace std;


#define R 6371
#define toRadians(x) ((x) * 3.141593f / 180.0f)


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


class Points{
public:
    vector<vector<double>> GPS;
    vector<vector<double>> distance;
    vector<int> road;
    double len = -1;
    int speed = 8;//средняя скорость дрона в км в ч
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
            for (int j = 0; j < n; ++j)
                printf("%f ", distance[i][j]);
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
            }
        }
    }

    void makeRoad(double dst, vector<int> p) {
        if (p.size() == GPS.size()) {
            dst = dst + distance[p.back()][0];
            p.push_back(0);
            if ((len < 0) || (dst < len)) {
                len = dst;
                time = len / speed;
                road = p;
            }
            /*for (int number : p)
                cout << number << " ";
            cout << "   " << dst << endl;*/
        } else {
            int tek = p.back();
            for (int i = 0; i < GPS.size(); ++i) {
                if(find(p.begin(), p.end(), i) == p.end()) {
                    double to_dst = dst + distance[tek][i];
                    vector<int> to = p;
                    to.push_back(i);
                    makeRoad(to_dst, to);
                }
            }
        }
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
    test.makeRoad(0, {0});
    test.showRoad();

    return 0;
}
