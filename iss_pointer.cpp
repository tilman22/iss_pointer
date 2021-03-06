#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include "/home/tilman/tools/include/eigen3/Eigen/Dense"

static double to_bogenmass(double w)
{
    return w * 2 * M_PI / 360;
}

static double to_winkelmass(double w)
{
    return w * 360 / (2 * M_PI);
}

static Eigen::Vector3d ToCartesian(double lat, double lon, double r)
{
    // Siehe https://www.gutefrage.net/frage/geographische-koordinatenlaengen-und-breitengrade-in-kartesische-koordinatenxyz-umrechnen
    Eigen::Vector3d p;
    p[0] = r * std::cos(to_bogenmass(lat)) * std::cos(to_bogenmass(lon));
    p[1] = r * std::cos(to_bogenmass(lat)) * std::sin(to_bogenmass(lon));
    p[2] = r * std::sin(to_bogenmass(lat));
    return p;
}

int main(int argc, char *argv[])
{
#if 1
    if (argc < 3)
    {
        std::cout << "Anwendung: iss_pointer <Breite_von_ISS> <Laenge_von_ISS>" << std::endl;
        std::cout << "Standort ist fest auf München kodiert." << std::endl;
        return 1;
    }

    double pointer_r   = 6400;
    double pointer_lat = 48.142870;
    double pointer_lon = 11.647197;
    double iss_r   = pointer_r + 410;
    double iss_lat = std::stod(argv[1]);
    double iss_lon = std::stod(argv[2]);

    if (argc >= 4) iss_r       = std::stod(argv[3]);
    if (argc >= 5) pointer_lat = std::stod(argv[4]);
    if (argc >= 6) pointer_lon = std::stod(argv[5]);
    if (argc >= 7) pointer_r   = std::stod(argv[6]);

#elif 0
    // Ausgangssituation
    double pointer_lat = 48.142870;
    double pointer_lon = 11.647197;
    double pointer_r   = 6400;
    double iss_lat = -48.1491;
    double iss_lon = 21.4662;
    double iss_r   = pointer_r + 410;
#elif 1
    // Ausgangssituation
    double pointer_lat = 45;
    double pointer_lon = 0;
    double pointer_r   = 6400;
    double iss_lat = -45;
    double iss_lon = 0;
    double iss_r   = pointer_r + 0;
#endif

    // Kleine Drehung
    iss_lon -= pointer_lon;
    pointer_lon = 0;

    //
    Eigen::Vector3d pointer_p = ToCartesian(pointer_lat, pointer_lon, pointer_r);
    Eigen::Vector3d iss_p     = ToCartesian(iss_lat, iss_lon, iss_r);

    // 
    Eigen::Vector3d p = iss_p - pointer_p;

    // Orthonormalsystem für die Tangential-Ebene des Pointers
    // x zeigt nach Norden, y nach rechts, z senkrecht nach oben.
    Eigen::Vector3d ebene_x = Eigen::Vector3d(-pointer_p[2], 0, pointer_p[0]) / pointer_p.norm();
    Eigen::Vector3d ebene_y(0, 1, 0);
    Eigen::Vector3d ebene_z = pointer_p / pointer_p.norm();

    // std::cout << "pointer_p = " << pointer_p << ", iss_p = " << iss_p << std::endl;

    // Projektionen von p auf das Orthonormalsystem
    double x = p.dot(ebene_x);
    double y = p.dot(ebene_y);
    double z = p.dot(ebene_z);

    double winkel_neigung = to_winkelmass(std::asin(z/std::sqrt(x*x+y*y+z*z)));
    double winkel_drehung = to_winkelmass(std::acos(x/std::sqrt(x*x+y*y)));
    if (y < 0)
        winkel_drehung = 360 - winkel_drehung;   

    std::cout << std::setprecision(2) << std::fixed << std::right;
    std::cout << "Drehung: " << winkel_drehung << ", Neigung: " << winkel_neigung << " (x=" << x << ", y=" << y << ", z=" << z << ")" << std::endl;

    return 0;
}

