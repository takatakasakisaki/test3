#include <iostream>
#include <execution>
#include <algorithm>
#include <execution>
#include <string>
#include <cmath>
#include <vector>
#include <stdio.h>
#include <chrono>
#include <omp.h>
using namespace std;
//  U 実際の観測値=センサ値
//  K= (P*H) / (H*P*H+R)
//  U_hat += K*(U- H*U_hat)
//   P=( 1-K*H)*P + Q   推定値の誤差---> next P[k+1] 次回使用される
static double U_hat=0; //initial esitmated state 初期推定値 
double kalman(double U)
{
	//static double R=4.0; //noise covariance  ノイズの分散 Vk   yk=HkXk+Vk
	//static double R=8.0; //noise covariance  ノイズの分散 Vk   yk=HkXk+Vk
	static double R=16.0; //noise covariance  ノイズの分散 Vk   yk=HkXk+Vk
	static double H=1.0; // measurement map scalar
	//static double Q=2.0;//initial estimated covariance  推定値の分散 Wk.  Xk+1 = Xk + Wk
	static double Q=4.0;//initial estimated covariance  推定値の分散 Wk.  Xk+1 = Xk + Wk
	static double P=0; //initial error covariance  誤差の分散
	static double K=0; //initial kalman gain
//begin
	K= P *H /(H*P*H+R); //update kalman gain
	U_hat = U_hat + K*(U-H*U_hat); //update estimate  推定値更新　推定値と入力値の差にゲインをかけて足す
printf("P=,%f,H=,%f,R=,%f,K=,%f, U=,%f, U_hat=,%f\n", P,H,R,K, U, U_hat);
	//update error covariance
	double I = 1; //identify matrix
	P = (I - K*H)*P+Q;   //これが変化するので次回のゲインが変わる  1 == I , 単位行列

	return U_hat;

}
int main(int argc, char *argv[])
{
	auto ts0 = chrono::system_clock::now();
	auto ts1 = chrono::system_clock::now();
	auto tdiff = chrono::duration_cast<std::chrono::microseconds>(ts1 - ts0);
	cout << tdiff.count();
	double th=0;
	double r = (r/ double(RAND_MAX))*4.0;
	for(int i = 0; i < 100; i++){
		r = random();
		r = (r/ double(RAND_MAX))*4.0;
		double s = sin(th);
		th += 0.1;
		double u = s*5+r;
		if(i == 0){
			U_hat = u;
		}
		kalman(u);
	}
	return 0;

}

