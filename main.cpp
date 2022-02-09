//

// Created by ay on 12/21/21.
//

#include <Magick++.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <fstream>
#include <csignal>

#define MAX_CHAR_FOR_GETWORKPACEI3 6000
#define NBCIRCLE 9

using json = nlohmann::json;


using namespace std;
using namespace Magick;

auto position_circle(int circle, int nbcircle, int width, int height, int circle_start_at = 0) {
    int x = width / 2;
    int y = height / 2;
    int radius = width / 6;
    int angle = (circle * 360 / -nbcircle) + circle_start_at;
    int x_pos = x + radius * cos(angle * M_PI / 180);
    int y_pos = y + radius * sin(angle * M_PI / 180);
    return make_pair(x_pos, y_pos);
}

int main(int argc,char **argv) {
    InitializeMagick(*argv);

    Image image;
    Image imageB2;
    Image imageB2Save;
    image.strokeAntiAlias(false);
    imageB2.strokeAntiAlias(false);
    FILE* resultOfI3GetWork;


    // Define Font
    imageB2.font("/usr/share/fonts/TTF/DejaVuMathTeXGyre.ttf");
    imageB2.fontPointsize(60);
    imageB2.strokeColor("black");
    imageB2.strokeWidth(1);
    image.font("/usr/share/fonts/gnu-free/FreeMonoBoldOblique.otf");
    image.fontPointsize(60);
    image.strokeColor("black");
    image.strokeWidth(1);

    // Open File
    try {
        // Workspace {{{1
        image.read( "/home/ay/backgroundChangerCpp/AY1920.png" );
        imageB2.read( "/home/ay/backgroundChangerCpp/B2.png" );
        // copy imageB2 to a save
        imageB2Save = imageB2;
        // set 10 imageB2 around the center of image1
        char i3_workpace_output[MAX_CHAR_FOR_GETWORKPACEI3];
        json jsondata;
        int i;
        string hdmi;

        resultOfI3GetWork = popen("i3-msg -t get_workspaces", "r") ;
        jsondata = json::parse(fgets(i3_workpace_output, MAX_CHAR_FOR_GETWORKPACEI3, resultOfI3GetWork));

        for (auto &e : jsondata) {
            i = e["num"];
            hdmi = e["output"];
            if(i==10){
                continue;
            }
            i = i%10;
            if(hdmi=="HDMI-1"){
                imageB2.type( GrayscaleType ); 
            }
            imageB2.fillColor(e["focused"] ? "white" : "black");
            auto pos = position_circle(i, NBCIRCLE, image.columns(), image.rows(), -50);//54-(36*4));
            imageB2.annotate(to_string(i), Geometry(0, 0, 0, 0), CenterGravity);  // 58, 38 Dépendent de la font
            image.composite(imageB2, pos.first-imageB2.size().width()/2, pos.second-imageB2.size().height()/2, OverCompositeOp);
            imageB2 = imageB2Save;
        }
        // }}}
        // Time {{{1
        // Get time
        time_t rawtime;
        image.fillColor("white");
        // image.annotate("R", Geometry(0, 100, 0, 0), CenterGravity);


        // Met l'image en background
        image.write( "/tmp/logo.png" );
        system("feh --bg-fill /tmp/logo.png");

    } catch( Exception &error_ ){
        cout << "Caught exception: " << error_.what() << endl;
        return 1;
    }
    return 0;
}

//     cout << e["num"] << endl;
// }
// for (int i = 0; i < nbcircle; i++) {

// image.pixelColor(i,j,Color("#ffffff"));
// image.crop( Geometry(100,100, 100, 100) );
// cout << image.size().width() << endl;
// image.composite(imageB2,0,0,CopyCompositeOp);
