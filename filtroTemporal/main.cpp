#include <iostream>
#include <opencv.hpp>

using namespace std;
using namespace cv;
//variables
int capa;
vector<Mat> piramide,filtrado,pasabajo1,pasabajo2;
//header
bool fltrEspacial(Mat &src);
void fltrTemporal(Mat &src, Mat &dst, int nivel);
int main()
{
    Mat img,filtro,imgOut;

    cout<< "Ingrese el numero de capas: ";
    cin>> capa;

    VideoCapture video(0);
    namedWindow("Resultado",WINDOW_AUTOSIZE);

    while(1){
        video >> img;
        img.convertTo(img,CV_32FC3,1.0/255.0f);
        fltrEspacial(img);

        pasabajo1=piramide;
        pasabajo2=piramide;
        filtrado = piramide;
       hconcat(img,piramide[0],imgOut);


        imshow("Resultado",imgOut);
        for(int i=0;i<capa;i++)
        {

            fltrTemporal(piramide[i],filtrado[i],i);

        }

        if(waitKey(30)>=0) break;
    }
}

bool fltrEspacial(Mat &src)
{
    Mat imgDwn,imgUp,actual,imgLaplace;

    actual = src;
    piramide.clear();
    for(int i=0; i < capa; i++)
    {
        pyrDown(actual,imgDwn);
        pyrUp(imgDwn,imgUp,actual.size());

        subtract(actual,imgUp,imgLaplace);
        piramide.push_back(imgLaplace);
        actual = imgDwn;
    }


    return true;
}

void fltrTemporal(Mat &src, Mat &dst, int nivel)
{
    Mat temp1,temp2,temp3;
    temp3 = src;
    float fh = 0.4;
    float fl= 0.05;
    temp1 = (1-fh)*pasabajo1[nivel] + fh*temp3;
    temp2 = (1-fl)*pasabajo2[nivel] + fl*temp3;
    pasabajo1[nivel] = temp1;
    pasabajo2[nivel] = temp2;
    dst = pasabajo1[nivel]-pasabajo2[nivel];
}
