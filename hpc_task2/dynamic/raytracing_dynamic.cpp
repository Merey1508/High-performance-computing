#include "minirt/minirt.h"

#include <thread>
#include <vector>
#include <cmath>
#include <iostream>
#include <queue>
#include <chrono>

#include <mutex>
#include <cstdio>
#include <string>
#include <numeric>
#include <cmath>

using namespace minirt;
using namespace std;
using namespace std::chrono;
using hrc = high_resolution_clock;

void initScene(Scene &scene) {
    Color red {1, 0.2, 0.2};
    Color blue {0.2, 0.2, 1};
    Color green {0.2, 1, 0.2};
    Color white {0.8, 0.8, 0.8};
    Color yellow {1, 1, 0.2};

    Material metallicRed {red, white, 50};
    Material mirrorBlack {Color {0.0}, Color {0.9}, 1000};
    Material matteWhite {Color {0.7}, Color {0.3}, 1};
    Material metallicYellow {yellow, white, 250};
    Material greenishGreen {green, 0.5, 0.5};

    Material transparentGreen {green, 0.8, 0.2};
    transparentGreen.makeTransparent(1.0, 1.03);
    Material transparentBlue {blue, 0.4, 0.6};
    transparentBlue.makeTransparent(0.9, 0.7);

    scene.addSphere(Sphere {{0, -2, 7}, 1, transparentBlue});
    scene.addSphere(Sphere {{-3, 2, 11}, 2, metallicRed});
    scene.addSphere(Sphere {{0, 2, 8}, 1, mirrorBlack});
    scene.addSphere(Sphere {{1.5, -0.5, 7}, 1, transparentGreen});
    scene.addSphere(Sphere {{-2, -1, 6}, 0.7, metallicYellow});
    scene.addSphere(Sphere {{2.2, 0.5, 9}, 1.2, matteWhite});
    scene.addSphere(Sphere {{4, -1, 10}, 0.7, metallicRed});

    scene.addLight(PointLight {{-15, 0, -15}, white});
    scene.addLight(PointLight {{1, 1, 0}, blue});
    scene.addLight(PointLight {{0, -10, 6}, red});

    scene.setBackground({0.05, 0.05, 0.08});
    scene.setAmbient({0.1, 0.1, 0.1});
    scene.setRecursionLimit(20);

    scene.setCamera(Camera {{0, 0, -20}, {0, 0, 0}});
}

// This is a thread function for C++ threads.
// TODO: modify this function for a thread to be able to compute some specified part of the image.
// For example, now ranges of pixels by X and Y always start from 0.
// Think what additional arguments may be required to compute a range not starting from 0.


void threadFunc(queue<int> &data, Scene &scene, ViewPlane &viewPlane, Image &image, int sizeX, int block_size, int numOfSamples,mutex &mut) {
	while (true) {
		double elem;
		{
            // mut.lock();
			lock_guard<mutex> lock(mut);
			if (data.empty()) {
				return;
			}
			elem = data.front();
			data.pop();
		}
        
        int startY = elem*block_size;
        int endY = (elem+1)*block_size;
        // lock_guard<mutex> lock(mut);
		for(int x = 0; x < sizeX; x++)
        for(int y = startY; y < endY; y++) {
            const auto color = viewPlane.computePixel(scene, x, y, numOfSamples);
            {
                // mut.unlock();
                // lock_guard<mutex> unlock(mut);
                image.set(x, y, color);
            }
        }  
        
	}
}

int main(int argc, char **argv) {
    // Number of threads to use is the first parameter now.
    // The other parameters are the same as in the sequential app.
    int numberOfThreads = (argc > 1 ? std::stoi(argv[1]) : 1);
    int viewPlaneResolutionX = (argc > 2 ? std::stoi(argv[2]) : 600);
    int viewPlaneResolutionY = (argc > 3 ? std::stoi(argv[3]) : 600);
    int numOfSamples = (argc > 4 ? std::stoi(argv[4]) : 1);    
    std::string sceneFile = (argc > 5 ? argv[5] : "");

    Scene scene;
    if (sceneFile.empty()) {
        initScene(scene);
    } else {
        scene.loadFromFile(sceneFile);
    }

    const double backgroundSizeX = 4;
    const double backgroundSizeY = 4;
    const double backgroundDistance = 15;

    const double viewPlaneDistance = 5;
    const double viewPlaneSizeX = backgroundSizeX * viewPlaneDistance / backgroundDistance;
    const double viewPlaneSizeY = backgroundSizeY * viewPlaneDistance / backgroundDistance;

    ViewPlane viewPlane {viewPlaneResolutionX, viewPlaneResolutionY,
                         viewPlaneSizeX, viewPlaneSizeY, viewPlaneDistance};

    Image image(viewPlaneResolutionX, viewPlaneResolutionY); // computed image


    queue<int> data;
    for (int i = 0; i < viewPlaneResolutionY; i++) {
		data.push(i);
	}

    
    vector<thread> threads;
    
    int block_size = 1;
    
    mutex mut;

    auto ts = hrc::now();

    // TODO: make each thread to compute different part of the image.
    // To do this first decide how the image should be partitioned,
    // then compute a parition and pass this information to each thread.
    for (int i = 0; i < numberOfThreads; i++) {
        thread thr(threadFunc, ref(data), ref(scene), ref(viewPlane), ref(image), 
            viewPlaneResolutionX, block_size, numOfSamples, ref(mut));
        threads.push_back(move(thr));           
    }
    
    for (auto &thread: threads) {
        thread.join();
    }
        
    auto te = hrc::now();
    
    double time = duration<double>(te - ts).count();
    
    cout << "Time = " << time << endl;

    image.saveJPEG("raytracing_" + to_string(numberOfThreads) + ".jpg");

    return 0;
}